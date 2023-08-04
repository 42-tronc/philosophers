/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maplepy <maplepy@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 09:54:48 by maplepy           #+#    #+#             */
/*   Updated: 2023/07/31 10:38:25 by maplepy          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

long	get_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (EXIT_FAILURE); // put an error msg
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	print_status(t_philo philo, int status_code)
{
	char	*status[5];
	long	timestamp;

	// pthread_mutex_lock(&philo.philo_mutex);
	timestamp = get_time_ms() - philo.data->start_time_ms;
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

void	*philo_routine(t_philo *philo)
{
	// while (1)
	// {
	// 	philo_eat(philo);
	// 	philo_sleep(philo);
	// 	philo_think(philo);
	// }
	printf("philo %ld will do its routine\n", philo->id);
	return (NULL);
}

static void	debug_change_data(t_data *data)
{
	// usleep(800);
	pthread_mutex_lock(&data->data_mutex);
	data->all_alive = 0;
	printf("faking a death\n");
	pthread_mutex_unlock(&data->data_mutex);
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
	return (EXIT_SUCCESS);
}