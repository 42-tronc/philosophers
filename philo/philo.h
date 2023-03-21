/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:40:42 by croy              #+#    #+#             */
/*   Updated: 2023/03/21 14:43:00 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	int				all_alive;
	long			nb_philo;
	long			death_time;
	long			eat_time;
	long			sleep_time;
	long			meal_limit;
	struct timeval	time;
	pthread_mutex_t	print;
	pthread_mutex_t	*fork_mutexes;
}	t_data;

typedef struct s_philo
{
	long			id;
	long			eaten;
	long			last_meal;
	t_data			*data;
}	t_philo;

int	ft_atoi(const char *str);

#endif
