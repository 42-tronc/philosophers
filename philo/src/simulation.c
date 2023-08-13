/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 09:54:48 by croy             #+#    #+#             */
/*   Updated: 2023/08/12 18:47:26 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"

long	get_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		print_error(E_TIME, "get_time_ms");
		return (-1);
	}
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	print_status(t_philo philo, int status_code)
{
	char	*status[5];
	long	time_now;
	long	timestamp;

	time_now = get_time_ms();
	pthread_mutex_lock(&philo.data->data_mutex);
	if (philo.data->error || time_now == -1)
	{
		philo.data->error = 1;
		pthread_mutex_unlock(&philo.data->data_mutex);
		return ;
	}
	timestamp = time_now - philo.data->start_time_ms;
	pthread_mutex_unlock(&philo.data->data_mutex);
	status[S_THINKING] = "is thinking";
	status[S_FORK] = "has taken a fork";
	status[S_EATING] = "is eating";
	status[S_SLEEPING] = "is sleeping";
	status[S_DIED] = "\e[31;1mdied 💀💀💀\e[0m";
	pthread_mutex_lock(&philo.data->print_mutex);
	printf("%ldms:\tphilo %ld %s\n", timestamp, philo.id, status[status_code]);
	pthread_mutex_unlock(&philo.data->print_mutex);
}

int	do_if_alive(t_philo *philo, int (*fn)(t_philo *philo))
{
	int	alive;
	int	meals_eaten;
	int	meals_limit;

	pthread_mutex_lock(&philo->data->data_mutex);
	if (philo->data->error)
	{
		pthread_mutex_unlock(&philo->data->data_mutex);
		return (-1);
	}
	// alive = philo->data->all_alive;
	meals_eaten = philo->meals;
	meals_limit = philo->data->meal_limit;
	pthread_mutex_unlock(&philo->data->data_mutex);
	alive = is_alive(philo);
	if (alive && (meals_limit && meals_eaten < meals_limit))
		return (fn(philo));
	else if (alive && meals_limit && meals_eaten >= meals_limit)
	{
		printf("philo %ld has eaten %ld/%ld\n", philo->id, philo->meals, philo->data->meal_limit);
		pthread_mutex_lock(&philo->data->data_mutex);
		philo->data->still_hungry--;
		pthread_mutex_unlock(&philo->data->data_mutex);
		alive = 0;
	}
	return (alive);
}

void	*philo_routine(t_philo *philo)
{
	int	alive;

	alive = 1;
	if (philo->id % 2 == 0) // make even philos wait a bit before starting
		usleep(philo->data->time_to_eat * 0.8 * 1000);
	while (alive)
	{
		alive = do_if_alive(philo, &philo_thinking);
		if (alive == -1) // maybe useless
			break ; // maybe useless
		while (do_if_alive(philo, &philo_take_forks) == 1)
			usleep(1000); // sleep 1ms if forks arent available

		alive = do_if_alive(philo, &philo_eating);
		if (alive == -1) // maybe useless
			break ; // maybe useless
		alive = do_if_alive(philo, &philo_sleeping);
	}
	return (NULL);
}

int	launch_simulation(t_data *data)
{
	int	i;

	i = 0;
	data->start_time_ms = get_time_ms();
	while (i < data->nb_philo)
	{
		if (pthread_create(&data->philos[i].thread, NULL, \
			(void *)philo_routine, &data->philos[i]))
			return (destroy_mutexes(data, i), EXIT_FAILURE);
		i++;
	}
	close_threads(data);
	if (data->error)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
