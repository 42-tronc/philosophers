/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:40:42 by croy              #+#    #+#             */
/*   Updated: 2023/07/31 11:36:24 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_data	t_data;
typedef struct s_philo	t_philo;

typedef enum e_state
{
	S_THINKING,
	S_FORK,
	S_EATING,
	S_SLEEPING,
	S_DIED,
}	t_state;

typedef enum e_error
{
	E_MALLOC,
	E_MUTEX,
	E_THREAD,
	E_TIME,
}	t_error;


struct s_data
{
	int				error;
	int				all_alive;
	int				*forks;
	long			nb_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			meal_limit;
	long			still_hungry;
	// struct timeval	start_time;
	long			start_time_ms;
	pthread_mutex_t	*fork_mutexes;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	data_mutex;
	t_philo			*philos;
};

struct s_philo
{
	long			id;
	long			meals;
	struct timeval	last_meal;
	pthread_t		thread;
	pthread_mutex_t	philo_mutex;
	t_data			*data;
};

//	===== @functions =====
// free.c
void	free_data(t_data *data);
void	destroy_mutexes(t_data *data, int i);
void	close_threads(t_data *data);

// init.c
int	init_data(t_data *data, char **av);
int	init_mutexes(t_data *data);
int	init_philo(t_data *data);

// main.c
void	print_error(int code, char *source);

// simulation.c
void	*philo_routine(t_philo *philo);
int	launch_simulation(t_data *data);

// utils.c
int	ft_atoi(const char *str);

#endif