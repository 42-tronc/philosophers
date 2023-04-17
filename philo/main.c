/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:32:09 by croy              #+#    #+#             */
/*   Updated: 2023/04/17 14:49:55 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
Allowed functions:
	memset, printf, malloc, free, write,
	usleep, gettimeofday,
	pthread_create, pthread_detach, pthread_join,
	pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock
*/

/**
 * @brief Get the time between now and arg passed as time
 *
 * @param time struct timeval where (now - time) will be calculated
 * @return long milliseconds from now since time passed
 */
long	get_time(struct timeval time)
{
	struct timeval	now;
	long			milliseconds;

	gettimeofday(&now, NULL);
	milliseconds = (now.tv_sec - time.tv_sec) * 1000 +
		(now.tv_usec - time.tv_usec) / 1000;
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
	status[4] = "\e[31;1mdied 💀💀💀\e[0m";
	// if (philo.data->all_alive)
		printf("%ldms:\tphilo %ld %s\n", timestamp, philo.id, status[status_code]);
	pthread_mutex_unlock(&philo.data->print);
}

/**
 * @brief checks if args passed are correct (unsigned int > 0)
 *
 * @param av array of args (typically argv)
 * @param data struct t_data to add each arg to
 * @return int 1 if error, 0 if all checks passed
 */
static int	check_args(char **av, t_data *data)
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
	// printf("Runtime: %ldms\n", get_time(data.start_time));

	return (0);
}
