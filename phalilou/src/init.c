/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:56:14 by croy              #+#    #+#             */
/*   Updated: 2023/07/31 14:09:50 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// ignore the variable set but not used warning
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "../header/philo.h"

int	init_data(t_data *data, char **av)
{
	data->all_alive = 1;
	data->nb_philo = ft_atoi(av[1]);
	data->still_hungry = data->nb_philo;
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		data->meal_limit = ft_atoi(av[5]);
	else
		data->meal_limit = 0;

	// debug msg
	// printf("Nb philo = '%ld'\n", data->nb_philo);
	// printf("Death time = '%ld'\n", data->time_to_die);
	// printf("Eat time = '%ld'\n", data->time_to_eat);
	// printf("Sleep time = '%ld'\n", data->time_to_sleep);
	// printf("Meal limit = '%ld'\n", data->meal_limit);
	// up to here

	if (data->nb_philo <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || (av[5] && data->meal_limit <= 0))
		return (printf("\e[31mError: Arguments must be greater than 0\n"), 1);
	return (0);
}

void	destroy_mutexes(t_data *data, int i)
{
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->alive_mutex);
	while (i >= 0)
		pthread_mutex_destroy(&data->fork_mutexes[i--]);
	// free(data->fork_mutexes);
}

int	init_mutexes(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&data->alive_mutex, NULL))
		return (pthread_mutex_destroy(&data->print_mutex), EXIT_FAILURE);
	data->fork_mutexes = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->fork_mutexes[i], NULL))
			return (destroy_mutexes(data, i - 1), EXIT_FAILURE);
		i++;
	}
	return (0);
}

int	init_philo(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philos)
		return (EXIT_FAILURE);
	data->forks = malloc(sizeof(int) * data->nb_philo);
	if (!data->forks)
		return (free(data->philos), EXIT_FAILURE);

	// Create the mutexes
	if (init_mutexes(data))
		return (free(data->philos), free(data->forks), EXIT_FAILURE);
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals = 0;
		gettimeofday(&data->philos[i].last_meal, NULL);
		data->philos[i].data = data;
		i++;
	}
	return (0);
}
