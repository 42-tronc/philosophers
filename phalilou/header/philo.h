/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:40:42 by croy              #+#    #+#             */
/*   Updated: 2023/07/31 11:05:35 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_data
{
	long			all_alive;
	long			nb_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			meal_limit;
	long			still_hungry;
	struct timeval	start_time;
	pthread_mutex_t	*forks_mutexes;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	alive_mutex;
	t_philo			*philos;
}					t_data;

typedef struct s_philo
{
	long			id;
	long			meals;
	struct timeval	last_meal;
	pthread_t		thread;
	t_data			*data;
}					t_philo;

//	===== @functions =====
// init.c
int		init_data(t_data *data, char **av);
int		init_philo(t_data *data);

#endif
