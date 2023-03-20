/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:32:09 by croy              #+#    #+#             */
/*   Updated: 2023/03/20 16:32:07 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(struct timeval time);
void	print_status(t_philo philo, int status_code);

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
	// THINK
	print_status(*philo, 0);

	// EAT
	if (philo->id % 2 == 0)
		sleep(2); // a mettre en usleep a terme

	// printf("Philo id '%ld' will lock mutex %ld and %ld\n", philo->id, philo->id - 1, (philo->id) % philo->data->nb_philo);
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->id - 1]);
	print_status(*philo, 1);
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);
	print_status(*philo, 1);

	print_status(*philo, 2);
	usleep(philo->data->eat_time * 1000);
	philo->meal_eaten++;
	philo->last_meal = get_time(philo->data->time); // takes timestamp since start N ms
	// printf("Last meal of %ld: %ld\n", philo->id, philo->last_meal);
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id - 1]);
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);

/*	Exemple de mutex
	pthread_mutex_lock(&philo->data->fork_mutexes[0]);
	printf("\nLocking mutex %d \n", 0);
	printf("Bonjour ici philo %ld\n", philo->id);
	// sleep(2);
	printf("Unlocking mutex %d \n", 0);
	pthread_mutex_unlock(&philo->data->fork_mutexes[0]);
*/

	// SLEEP
	print_status(*philo, 3);
	usleep(philo->data->sleep_time * 1000);

	printf("Philo %ld, meal eaten=%ld, last meal=%ld\n",
		philo->id,
		philo->meal_eaten,
		philo->last_meal);
	return (NULL);
}

/**
 * @brief create each philo, init its vars, mutexes and start the simulation
 *
 * @param data t_data struct, to pass it to each philo
 */
void	create_philos(t_data data)
{
	int				i;
	t_philo			philos[data.nb_philo];
	pthread_mutex_t	fork_mutex[data.nb_philo];

	// initialize fork mutexes
	i = 0;
	while (i < data.nb_philo)
		pthread_mutex_init(&fork_mutex[i++], NULL);
	data.fork_mutexes = fork_mutex;

	// initialize print mutex
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

/**
 * @brief Get the time between now and arg passed as time
 *
 * @param time struct timeval where (now - time) will be calculated
 * @return long milliseconds from now since time passed
 */
long	get_time(struct timeval time)
{
	struct timeval now;
	long milliseconds;

	gettimeofday(&now, NULL);
	milliseconds = (now.tv_sec - time.tv_sec) * 1000 +
		(now.tv_usec - time.tv_usec) / 1000;

	// printf("\nget_time(): rnow.tv_sec=%ld, rnow.tv_usec=%ld\n", now.tv_sec, now.tv_usec);
	// printf("get_time(): time.tv_sec=%ld, time.tv_usec=%ld\n", time.tv_sec, time.tv_usec);
	// printf("get_time(): milliseconds=%ld\n", milliseconds);
	return (milliseconds);
}

/**
 * @brief  printf status with timestamp since simu start, philo's id and action
 *
 * @param philo takes a t_philo struct to get the id of philo
 * @param status_code action that will be displayed like thinking, eating etc.
 */
void	print_status(t_philo philo, int status_code)
{
	char	*status[5];
	long	timestamp;

	pthread_mutex_lock(&philo.data->print);
	timestamp = get_time(philo.data->time);
	status[0] = "is thinking";
	status[1] = "has taken a fork";
	status[2] = "is eating";
	status[3] = "is sleeping";
	status[4] = "died";
	printf("%ld: %ld %s\n", timestamp, philo.id, status[status_code]);
	pthread_mutex_unlock(&philo.data->print);

	// if (status_code == 4)
	// 	// need to free and exit here; maybe with a while (1 to var) in the main ?
	// 	return;
}

/**
 * @brief checks if args passed are correct (unsigned int > 0)
 *
 * @param av array of args (typically argv)
 * @param data struct t_data to add each arg to
 * @return int 1 if error, 0 if all checks passed
 */
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
	// struct timeval start_time;
	t_data	data;

	if (ac < 5 || ac > 6)
		return (printf("Expected: ./philo <nb of philosophers> <time to die> "\
		"<time to eat> <time to sleep> (philosophers eat count wanted)\n"), 1);

	if (check_args(av, &data))
		return (1);
	gettimeofday(&data.time, NULL);
	create_philos(data);

	// Get elapsed time in milliseconds
	printf("Runtime: %dms", get_time(data.time));

	return (0);
}
