/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:40:42 by croy              #+#    #+#             */
/*   Updated: 2023/08/13 17:04:59 by croy             ###   ########lyon.fr   */
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
	long			hungry_philos;
	long			start_time_ms;
	pthread_mutex_t	*fork_mutexes;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	data_mutex;
	t_philo			*philos;
};

struct s_philo
{
	int				forks_taken;
	long			id;
	int				hungry;
	long			meals;
	long			last_meal;
	long			first_fork;
	long			second_fork;
	pthread_t		thread;
	t_data			*data;
};

//	===== @functions =====
// actions.c
int		is_alive(t_philo *philo);
int		philo_thinking(t_philo *philo);
int		philo_take_forks(t_philo *philo);
int		philo_eating(t_philo *philo);
int		philo_sleeping(t_philo *philo);

// free.c
void	free_data(t_data *data);
void	destroy_mutexes(t_data *data, int i);
void	close_threads(t_data *data);

// init.c
int		init_data(t_data *data, char **av);
int		init_philo(t_data *data);

// main.c
void	print_error(int code, char *source);

// simulation.c
long	get_time_ms(void);
void	print_status(t_philo philo, int status_code);
int		launch_simulation(t_data *data);

// utils.c
int		ft_atoi(const char *str);

#endif
