/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:32:09 by croy              #+#    #+#             */
/*   Updated: 2023/03/24 00:04:35 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(struct timeval time);
void	print_status(t_philo philo, int status_code);

/*
Allowed functions:
	memset, printf, malloc, free, write,
	usleep, gettimeofday,
	pthread_create, pthread_detach, pthread_join,
	pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock
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
	while (philo->data->all_alive)
	{
	// THINK
	print_status(*philo, 0);

	// EAT
	if (philo->id % 2 == 0)
		usleep(30); // a mettre en usleep a terme

	// printf("Philo id '%ld' will lock mutex %ld and %ld\n", philo->id, philo->id - 1, (philo->id) % philo->data->nb_philo);
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->id - 1]);
	print_status(*philo, 1);
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);
	print_status(*philo, 1);

	print_status(*philo, 2);
	gettimeofday(&philo->last_meal, NULL); // maybe move this at the end of the meal ?
	usleep(philo->data->time_to_eat * 1000);
	philo->eaten++;
	// either take from start of prog: Xms
	// or take from last meal and
	// philo->last_meal = get_time(philo->data->time); // takes timestamp since start N ms
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id - 1]);
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);

	// maybe stop here in case someone dies?
	// SLEEP
	print_status(*philo, 3);
	usleep(philo->data->time_to_sleep * 1000);

	// Debug
	// printf("Philo %ld, meal eaten=%ld, last meal=%ld\n",
	// 	philo->id,
	// 	philo->eaten,
	// 	get_time(philo->last_meal));

	if (philo->data->meal_limit > 0 && philo->eaten >= philo->data->meal_limit)
		return (philo->data->need_to_eat--, NULL);
		// return (printf("%ld am full\n", philo->id), philo->data->need_to_eat--, NULL);
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
	int		id;
	long	since_meal;

	while (data->all_alive)
	{
		id = 0;
		while(id < data->nb_philo)
		{
			if (data->need_to_eat == 0)
				return (0);
			since_meal = get_time(philos[id].last_meal);
			if (since_meal > data->time_to_die)
			{
				printf("%d be dead, he waited %ldms\n", id + 1, since_meal);
				print_status(philos[id], 4);
				data->all_alive = 0;
				return (1);
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
		// philos[i].last_meal = 0;
		gettimeofday(&philos[i].last_meal, NULL);
		// philos[i].data = &data;
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
	timestamp = get_time(philo.data->start_time);
	status[0] = "is thinking";
	status[1] = "has taken a fork";
	status[2] = "is eating";
	status[3] = "is sleeping";
	status[4] = "died";
	(void) status;
	(void) status_code;
	(void) timestamp;
	printf("%ldms: philo %ld %s\n", timestamp, philo.id, status[status_code]);
	pthread_mutex_unlock(&philo.data->print);
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
	data->all_alive = 1;
	data->nb_philo = ft_atoi(av[1]);
	data->need_to_eat = data->nb_philo;
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		data->meal_limit = ft_atoi(av[5]);
	else
		data->meal_limit = 0;

	// debug msg
	printf("Nb philo = '%ld'\n", data->nb_philo);
	printf("Death time = '%ld'\n", data->time_to_die);
	printf("Eat time = '%ld'\n", data->time_to_eat);
	printf("Sleep time = '%ld'\n", data->time_to_sleep);
	printf("Meal limit = '%ld'\n", data->meal_limit);
	// up to here

	if (data->nb_philo <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0 || (av[5] && data->meal_limit <= 0))
		return (printf("\e[31mError: Arguments must be greater than 0\n"), 1);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac < 5 || ac > 6)
		return (printf("Expected: ./philo <nb of philosophers> <time to die> "\
		"<time to eat> <time to sleep> (philosophers eat count wanted)\n"), 1);

	if (check_args(av, &data))
		return (1);
	gettimeofday(&data.start_time, NULL);
	create_philos(&data);

	// Get elapsed time in milliseconds
	printf("Runtime: %ldms\n\n", get_time(data.start_time));

	return (0);
}
