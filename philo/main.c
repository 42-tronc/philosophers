/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:32:09 by croy              #+#    #+#             */
/*   Updated: 2023/03/16 17:10:03 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
Need to check for the args
	number_of_philosophers
	time_to_die
	time_to_eat
	time_to_sleep
	number_of_times_each_philosopher_must_eat

	- number_of_philosophers: The number of philosophers and also the number of forks.
	- time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die
	milliseconds since the beginning of their last meal or the beginning of the simulation, they die.
	- time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
	During that time, they will need to hold two forks.
	- time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
	- number_of_times_each_philosopher_must_eat (optional argument): If all
	philosophers have eaten at least number_of_times_each_philosopher_must_eat
	times, the simulation stops. If not specified, the simulation stops when a
	philosopher dies.

Allowed functions:
	memset, printf, malloc, free, write,
	usleep, gettimeofday,
	pthread_create, pthread_detach, pthread_join, pthread_mutex_init,
	pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock
*/

/*
Need to check:

number_of_philosophers: has to be 1+
time_to_die: has to be 1+
*/



/**
 * @brief Create the philosophers' routine (eat, sleep, think)
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
		sleep(2); // a mettre en usleep a terme

	// print_status(*philo, 0);
/*
	// EAT
	pthread_mutex_lock(&philo->fork_mutex);
	print_status(, philo, 0);
	pthread_mutex_lock(&philo->fork_mutex);
	print_status(time, philo, 0);

	pthread_mutex_unlock(&philo->fork_mutex);
	pthread_mutex_unlock(&philo->fork_mutex);
	// SLEEP
	print_status(time, philo, 2);

	// THINK
	print_status(time, philo, 3); */

	// printf("Philo %ld, meal eaten= %ld, last meal= %ld, start time = %ld\n", philo->id, philo->meal_eaten, philo->last_meal, philo->data->time.tv_sec);
	printf("Philo %ld, meal eaten=%ld, last meal=%ld.%03ld, start time=%ld.%03ld\n",
		philo->id,
		philo->meal_eaten,
		philo->last_meal.tv_sec,
		philo->last_meal.tv_usec / 1000,
		philo->data->time.tv_sec,
		philo->data->time.tv_usec / 1000);

	return (NULL);
}

void	create_philos(t_data data)
{
	int i;
	// create array of philosopher structs
	t_philo philos[data.nb_philo];

	// initialize mutex for print
	pthread_mutex_init(&data.print, NULL);

	// create threads and fork mutexes for each philo
	pthread_t threads[data.nb_philo];
	i = 0;
	while (i < data.nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].meal_eaten = 0;
		philos[i].last_meal = data.time;
		philos[i].data = &data;
		pthread_mutex_init(&philos[i].fork_mutex, NULL);
		pthread_create(&threads[i], NULL, philo_routine, (void *)&philos[i]);
		i++;
	}

	// wait for threads to finish and destroy mutexes
	i = 0;
	while (i < data.nb_philo)
	{
		pthread_join(threads[i], NULL);
		pthread_mutex_destroy(&philos[i].fork_mutex);
		i++;
	}
	pthread_mutex_destroy(&data.print);
}

int	get_time(t_data data)
{
	struct timeval now;
	long milliseconds;

	gettimeofday(&now, NULL);
	milliseconds = (now.tv_sec - data->time.tv_sec) * 1000 +
		(now.tv_usec - data->time.tv_usec) / 1000;

	printf("\nget_time(): now.tv_sec=%ld, data->time.tv_sec=%ld\n", now.tv_sec, data->time.tv_sec);
	printf("get_time(): now.tv_usec=%ld, data->time.tv_usec=%ld\n", now.tv_usec, data->time.tv_usec);
	printf("get_time(): milliseconds=%ld\n", milliseconds);

	return (milliseconds);
}

void	print_status(t_philo philo, int status_code)
{
	char	*status[5];
	long	elapsed_time;

	pthread_mutex_lock(&philo.data->print);
	elapsed_time = get_time(philo.data);
	status[0] = "has taken a fork";
	status[1] = "is eating";
	status[2] = "is sleeping";
	status[3] = "is thinking";
	status[4] = "died";
	printf("%ld: %ld %s", elapsed_time, philo.id, status[status_code]);
	pthread_mutex_unlock(&philo.data->print);

	// if (status_code == 4)
	// 	// need to free and exit here; maybe with a while (1 to var) in the main ?
	// 	return;
}

int	check_args(char **av, t_data *data)
{
	data->nb_philo = ft_atoi(av[1]);
	data->death_time = ft_atoi(av[2]);
	data->eat_time = ft_atoi(av[3]);
	data->sleep_time = ft_atoi(av[4]);
	if (av[5])
		data->eat_count = ft_atoi(av[5]);

	// to be deleted
	else
		printf("\e[33mnothing in eat count\n\e[0m");

	printf("Nb philo = '%ld'\n", data->nb_philo);
	printf("Death time = '%ld'\n", data->death_time);
	printf("Eat time = '%ld'\n", data->eat_time);
	printf("Sleep time = '%ld'\n", data->sleep_time);
	printf("Eat count = '%ld'\n", data->eat_count);
	// up to here

	if (data->nb_philo <= 0 || data->death_time <= 0 || data->eat_time <= 0
		|| data->sleep_time <= 0 || (av[5] && data->eat_count <= 0))
		return (printf("\e[31mError: Arguments must be greater than 0\n"), 1);
	return (0);
}

int	main(int ac, char **av)
{
	struct timeval start;
	t_data	data;

	if (ac < 5 || ac > 6)
		return (printf("Expected: ./philo <nb of philosophers> <time to die> "\
		"<time to eat> <time to sleep> (philosophers eat count wanted)\n"), 1);

	if (check_args(av, &data))
		return (1);

	gettimeofday(&start, NULL);
	data.time = start;

	// // Wait for 3 seconds
	// sleep(3);

	// Get elapsed time in milliseconds
	get_time(&data);

	return (0);
}
