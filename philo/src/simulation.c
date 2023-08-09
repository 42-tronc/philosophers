/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 09:54:48 by maplepy           #+#    #+#             */
/*   Updated: 2023/08/09 14:26:59 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

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

	// pthread_mutex_lock(&philo.philo_mutex);
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
	// pthread_mutex_unlock(&philo.philo_mutex);
	status[S_THINKING] = "is thinking";
	status[S_FORK] = "has taken a fork";
	status[S_EATING] = "is eating";
	status[S_SLEEPING] = "is sleeping";
	status[S_DIED] = "\e[31;1mdied 💀💀💀\e[0m";
	pthread_mutex_lock(&philo.data->print_mutex);
	printf("%ldms:\tphilo %ld %s\n", timestamp, philo.id, status[status_code]);
	pthread_mutex_unlock(&philo.data->print_mutex);
}

void	philo_thinking(t_philo *philo)
{
	print_status(*philo, S_THINKING);
}

void	philo_eating(t_philo *philo)
{
	print_status(*philo, S_FORK);
	print_status(*philo, S_FORK);
	print_status(*philo, S_EATING);
	usleep(philo->data->time_to_eat * 1000);
}

void	philo_sleeping(t_philo *philo)
{
	print_status(*philo, S_SLEEPING);
	usleep(philo->data->time_to_sleep * 1000);
}

int	do_if_alive(t_philo *philo, void (*fn)(t_philo *philo))
{
	int	alive;

	pthread_mutex_lock(&philo->data->data_mutex);
	if (philo->data->error)
	{
		pthread_mutex_unlock(&philo->data->data_mutex);
		return (-1);
	}
	alive = philo->data->all_alive;
	pthread_mutex_unlock(&philo->data->data_mutex);
	if (alive)
		fn(philo);
	return (alive);
}

void	*philo_routine(t_philo *philo)
{
	int	alive;

	alive = 1;
	while (alive)
	{
		alive = do_if_alive(philo, &philo_thinking);
		if (alive == -1) // maybe useless
			break ; // maybe useless
		alive = do_if_alive(philo, &philo_eating);
		if (alive == -1) // maybe useless
			break ; // maybe useless
		alive = do_if_alive(philo, &philo_sleeping);
	}
	return (NULL);
}

static void	debug_change_data(t_data *data)
{
	usleep(1000000);
	pthread_mutex_lock(&data->data_mutex);
	// data->all_alive = 0;
	data->error = 1;
	pthread_mutex_unlock(&data->data_mutex);
	printf("faking a death\n");
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
	// debug_change_data(data);
	close_threads(data);
	if (data->error)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
