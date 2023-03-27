/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:14:43 by croy              #+#    #+#             */
/*   Updated: 2023/03/27 14:22:11 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eat(t_philo *philo)
{
	// printf("Philo id '%ld' will lock mutex %ld and %ld\n", philo->id, philo->id - 1, (philo->id) % philo->data->nb_philo);
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->id - 1]);
	if (!philo->data->all_alive)
	{
		// printf("\n%ld sees %d now\n", philo->id, philo->data->all_alive);
		pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id - 1]);
		return;
	}
	print_status(*philo, 1);
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);
	if (!philo->data->all_alive)
	{
		// printf("\n%ld sees %d now again\n", philo->id, philo->data->all_alive);
		pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);
		return;
	}
	print_status(*philo, 1);

	print_status(*philo, 2);
	gettimeofday(&philo->last_meal, NULL); // maybe move this at the end of the meal ?
	usleep(philo->data->time_to_eat * 1000);
	philo->eaten++;
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id - 1]);
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);
}

/**
 * @brief Create the philosophers' routine (think, eat, sleep)
 * It will lock each fork, eat then unlock those. Sleep for a timed duration
 * and think for the remaining time until it can eat again.
 *
 * @param arg void* for each philosopher structure
 * @return void* returns NULL but needed to use pthread_create
 */
void	*philo_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo*) arg;
	if (philo->id % 2 == 0)
		usleep(30);
	while (philo->data->all_alive)
	{
		print_status(*philo, 0);

		if (philo->data->all_alive)
			philo_eat(philo);

		if (philo->data->all_alive)
		{
			print_status(*philo, 3);
			usleep(philo->data->time_to_sleep * 1000);
		}

		// Debug
		// printf("Philo %ld, meal eaten=%ld, last meal=%ld\n",
		// 	philo->id,
		// 	philo->eaten,
		// 	get_time(philo->last_meal));

		if (philo->data->meal_limit > 0 && philo->eaten >= philo->data->meal_limit)
			// return (printf("%ld am full\n", philo->id), philo->data->need_to_eat--, NULL);
			return (philo->data->need_to_eat--, NULL);
	}
	return (NULL);
}

/**
 * @brief checks if a philo has died
 *
 * @param data t_data struct to get nb of philo
 * @param philos t_philo array to check for each philo death
 * @return int 1 if it needs to check again (so no one died yet), 0 if one dies
 */
int	check_death(t_data *data, t_philo *philos)
{
	long	id;
	long	since_meal;

	while (data->all_alive)
	{
		id = 0;
		while(id < data->nb_philo)
		{
			if (data->need_to_eat == 0)
				return (0);
			since_meal = get_time(philos[id].last_meal); // maybe refact this line
			if (since_meal > data->time_to_die)
			{
				if (!(data->meal_limit > 0 && philos[id].eaten >= data->meal_limit))
				{
					// printf("%ld be dead, he waited %ldms\n", id + 1, since_meal);
					print_status(philos[id], 4);
					data->all_alive = 0;
					return (1);
				}
				// else
					// printf("%ld has eaten enough, hes not dead\n", id + 1);
			}
			id++;
		}
		usleep(1000); // maybe put less than 1ms or calculate ideal time
	}
	return (0);
}

/**
 * @brief create each philo, init its vars, mutexes and start the simulation
 *
 * @param data t_data struct, to pass it to each philo
 */
void	create_philos(t_data *data)
{
	int				i;
	t_philo			philos[data->nb_philo];
	pthread_mutex_t	fork_mutex[data->nb_philo];

	// initialize fork mutexes
	i = 0;
	while (i < data->nb_philo)
		pthread_mutex_init(&fork_mutex[i++], NULL);
	data->fork_mutexes = fork_mutex;

	// initialize print mutex
	pthread_mutex_init(&data->print, NULL);

	// create threads and fork mutexes for each philo
	pthread_t threads[data->nb_philo];
	i = 0;
	while (i < data->nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].eaten = 0;
		gettimeofday(&philos[i].last_meal, NULL);
		philos[i].data = data; // Pass a pointer to the t_data structure
		pthread_create(&threads[i], NULL, philo_routine, (void *)&philos[i]);
		i++;
	}

	check_death(data, philos);
	// wait for threads to finish and destroy mutexes
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(threads[i], NULL);
		pthread_mutex_destroy(&data->fork_mutexes[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print);
}
