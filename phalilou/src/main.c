/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:56:50 by croy              #+#    #+#             */
/*   Updated: 2023/07/31 14:11:46 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error(int code, char *source)
{
	char	*error[2];

	error[E_MALLOC] = "Malloc failed to allocate a memory space";
	error[E_MUTEX] = "Mutex failed to initialize";
	printf("\e[1;31mError: \e[0m");
	printf("%s in \e[3;34m%s\e[0m\n", error[code], source);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (ac < 5 || ac > 6)
		return (printf("Expected: ./philo <nb of philosophers> <time to die> "\
		"<time to eat> <time to sleep> [meal count required]\n"), 1);
	if (init_data(&data, av) || init_philo(&data))
		return (1);
	// gettimeofday(&data.start_time, NULL); // get start time of simulation
	// // launch_simulation(&data, philos);
	free_mutexes(&data);
	free_data(&data);
	return (0);
}
