/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 14:32:09 by croy              #+#    #+#             */
/*   Updated: 2023/03/13 19:12:43 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

/*
Need to check for the args
	number_of_philosophers
	time_to_die
	time_to_eat
	time_to_sleep

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
	usleep, gettimeofday, pthread_create,
	pthread_detach, pthread_join, pthread_mutex_init,
	pthread_mutex_destroy, pthread_mutex_lock,
	pthread_mutex_unlock
*/

/*
Need to check:

number_of_philosophers: has to be 1+
time_to_die: has to be 1+
*/
int	check_args(char **args)
{

	return 0;
}

int	main(int argc, char const *argv[])
{
	if (argc == 5)
	{
		if (!check_args(argv))
			return 1;
	}
	return 0;
}
