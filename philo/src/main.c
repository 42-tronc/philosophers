/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:56:50 by croy              #+#    #+#             */
/*   Updated: 2023/08/12 13:44:55 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_error(int code, char *source)
{
	char	*error[4];

	error[E_MALLOC] = "Malloc failed to allocate a memory space";
	error[E_MUTEX] = "Mutex failed to initialize";
	error[E_THREAD] = "Thread failed to initialize";
	error[E_TIME] = "Time failed to initialize";
	printf("\e[1;31mError: \e[0m");
	printf("%s in \e[3;34m%s\e[0m\n", error[code], source);
}

int	main(int ac, char **av)
{
	int		exit_status;
	t_data	data;

	if (ac < 5 || ac > 6)
		return (printf("Expected: ./philo <nb of philosophers> <time to die> "\
		"<time to eat> <time to sleep> [meal count required]\n"), 1);
	if (init_data(&data, av) || init_philo(&data))
		return (EXIT_FAILURE);
	exit_status = EXIT_SUCCESS;
	if (launch_simulation(&data))
		exit_status = EXIT_FAILURE;

	destroy_mutexes(&data, data.nb_philo - 1);
	free_data(&data);
	return (exit_status);
}

/* TODO

- protect all mallocs
- protect all mutexes
- protect all threads
- protect overflows

*/
