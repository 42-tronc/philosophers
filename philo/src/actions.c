/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/12 18:44:36 by croy              #+#    #+#             */
/*   Updated: 2023/08/12 18:44:58 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

int	is_alive(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->data_mutex);
	if (philo->data->all_alive == 0 || philo->data->hungry_philos == 0)
	{
		pthread_mutex_unlock(&philo->data->data_mutex);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->data_mutex);
	if (get_time_ms() - philo->last_meal >= philo->data->time_to_die)
	{
		pthread_mutex_lock(&philo->data->data_mutex);
		philo->data->all_alive = 0;
		pthread_mutex_unlock(&philo->data->data_mutex);
		print_status(*philo, S_DIED);
		return (0);
	}
	return (1);
}

int	philo_thinking(t_philo *philo)
{
	print_status(*philo, S_THINKING);
	return (0);
}

int	philo_take_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->first_fork]);
	if (philo->data->forks[philo->first_fork] == 0)
	{
		philo->data->forks[philo->first_fork] = philo->id;
		philo->forks_taken++;
		print_status(*philo, S_FORK);
	}
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->first_fork]);
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->second_fork]);
	if (philo->data->forks[philo->second_fork] == 0)
	{
		philo->data->forks[philo->second_fork] = philo->id;
		philo->forks_taken++;
		print_status(*philo, S_FORK);
	}
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->second_fork]);
	if (philo->forks_taken == 2)
		return (philo->forks_taken = 0, 2);
	return (1);
}

int	philo_eating(t_philo *philo)
{
	// Eating
	print_status(*philo, S_EATING);
	philo->last_meal = get_time_ms();
	usleep(philo->data->time_to_eat * 1000);
	philo->meals++;

	// Release the 1st fork
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->first_fork]);
	philo->data->forks[philo->first_fork] = 0;
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->first_fork]);

	// Release the 2nd fork
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->second_fork]);
	philo->data->forks[philo->second_fork] = 0;
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->second_fork]);
	return (1);
}

int	philo_sleeping(t_philo *philo)
{
	print_status(*philo, S_SLEEPING);
	usleep(philo->data->time_to_sleep * 1000);
	return (1);
}
