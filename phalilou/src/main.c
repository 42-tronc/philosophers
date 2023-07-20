/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 15:56:50 by croy              #+#    #+#             */
/*   Updated: 2023/07/20 17:07:04 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>



int	main(int ac, char **av)
{
	t_data	data;

	if (ac < 5 || ac > 6)
		return (printf("Expected: ./philo <nb of philosophers> <time to die> "\
		"<time to eat> <time to sleep> [meal count required]\n"), 1);
	if (init_data(&data, av) || init_philo(&data))
		return (1);

}

void	oldkkmain(int ac, char **av)
{
	t_philo	*philos;

	if (ac < 5 || ac > 6)
		return (printf("Expected: ./philo <nb of philosophers> <time to die> "\
		"<time to eat> <time to sleep> (philosophers eat count wanted)\n"), 1);
	if (check_args(av, &data))
		return (1);
	gettimeofday(&data.start_time, NULL); // get start time of simulation
	// create_philos(&data);
	philos = init_data(&data);
	if (!philos)
		return (1);
	launch_simulation(&data, philos);

	// Get elapsed time in milliseconds
	// printf("Runtime: %ldms\n", get_time(data.start_time));

	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		status;

	if (init_struct(&data, argc, argv) || init_philo(&data))
		return (1);
	status = 0;
	if (start_philos(&data))
		status = 1;
	wait_end(&data);
	destroy_mutex_i(&data, data.nb_philo - 1);
	return (status);
}
