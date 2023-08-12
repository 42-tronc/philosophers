/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:56:14 by croy              #+#    #+#             */
/*   Updated: 2023/08/12 12:54:26 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

// ignore the variable set but not used warning
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#include "../header/philo.h"

int	init_data(t_data *data, char **av)
{
	data->error = 0;
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

int	init_mutexes(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (print_error(E_MUTEX, "init_mutexes"), EXIT_FAILURE);
	if (pthread_mutex_init(&data->data_mutex, NULL))
		return (print_error(E_MUTEX, "init_mutexes"), \
		pthread_mutex_destroy(&data->print_mutex), EXIT_FAILURE);
	data->fork_mutexes = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->fork_mutexes)
		return (print_error(E_MALLOC, "init_mutexes"), \
		destroy_mutexes(data, -1), EXIT_FAILURE);
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->fork_mutexes[i], NULL))
			return (print_error(E_MUTEX, "init_mutexes"), destroy_mutexes(data, i - 1), EXIT_FAILURE);
		i++;
	}
	return (0);
}

static void	get_forks_id(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		philo->type = EVEN;
		philo->first_fork = philo->id - 1;
		philo->second_fork = philo->id % philo->data->nb_philo;
	}
	else
	{
		philo->type = ODD;
		philo->first_fork = philo->id % philo->data->nb_philo;
		philo->second_fork = philo->id - 1;
	}
	// printf("Philo '%ld' being %s will lock mutex %ld and %ld\n", philo->id, philo->type == ODD ? "odd" : "even", philo->first_fork, philo->second_fork);
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
	i = 0;
	while (i < data->nb_philo)
		data->forks[i++] = 0;
	// Create the mutexes
	if (init_mutexes(data))
		return (free(data->philos), free(data->forks), EXIT_FAILURE);
	i = 0;
	while (i < data->nb_philo)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals = 0;
		// gettimeofday(&data->philos[i].last_meal, NULL);
		data->philos[i].last_meal = get_time_ms();
		data->philos[i].data = data;
		get_forks_id(&data->philos[i]);
		// pthread_mutex_init(&data->philos[i].philo_mutex, NULL);
		i++;
	}
	return (0);
}