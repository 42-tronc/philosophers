/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:40:42 by croy              #+#    #+#             */
/*   Updated: 2023/07/19 19:21:23 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>
// # include <stddef.h>

typedef struct s_data
{
	int				all_alive;
	int				need_to_eat;
	long			nb_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			meal_limit;
	struct timeval	start_time;
	pthread_mutex_t	print;
	pthread_mutex_t	*fork_mutexes;
}	t_data;

typedef struct s_philo
{
	long			id;
	long			eaten;
	struct timeval	last_meal;
	t_data			*data;
}	t_philo;

//	===== @functions =====
// main.c
long	get_time(struct timeval time);
void	print_status(t_philo philo, int status_code);

// simulation.c
void	philo_eat(t_philo *philo);
void	*philo_routine(void *arg);
int	check_death(t_data *data, t_philo *philos);
void	create_philos(t_data *data);

// utils.c
int	ft_atoi(const char *str);

#endif
