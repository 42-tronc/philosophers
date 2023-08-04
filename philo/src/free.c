/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 11:34:22 by croy              #+#    #+#             */
/*   Updated: 2023/07/31 14:11:12 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philo.h"

void	free_data(t_data *data)
{
	free(data->philos);
	free(data->forks);
}

void	destroy_mutexes(t_data *data, int i)
{
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->data_mutex);
	while (i >= 0)
		pthread_mutex_destroy(&data->fork_mutexes[i--]);
	free(data->fork_mutexes);
}

void	close_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_join(data->philos[i].thread, NULL))
			print_error(E_THREAD, "pthread_join");
		i++;
	}
}
