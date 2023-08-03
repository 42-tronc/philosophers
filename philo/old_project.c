
/**
 * @brief Get the time between now and arg passed as time
 *
 * @param time struct timeval where (now - time) will be calculated
 * @return long milliseconds from now since time passed
 */
long	get_time(struct timeval time)
{
	struct timeval	now;
	long			milliseconds;

	gettimeofday(&now, NULL);
	milliseconds = (now.tv_sec - time.tv_sec) * 1000 +
		(now.tv_usec - time.tv_usec) / 1000;
	return (milliseconds);
}

/**
 * @brief  printf status with timestamp since simu start, philo's id and action
 *
 * @param philo takes a t_philo struct to get the id of philo
 * @param status_code action that will be displayed like thinking, eating etc.
 */
void	print_status(t_philo philo, int status_code)
{
	char	*status[5];
	long	timestamp;

	pthread_mutex_lock(&philo.data->print);
	timestamp = get_time(philo.data->start_time);
	status[0] = "is thinking";
	status[1] = "has taken a fork";
	status[2] = "is eating";
	status[3] = "is sleeping";
	status[4] = "\e[31;1mdied 💀💀💀\e[0m";
	// if (philo.data->all_alive)
		printf("%ldms:\tphilo %ld %s\n", timestamp, philo.id, status[status_code]);
	pthread_mutex_unlock(&philo.data->print);
}

/**
 * @brief launch the simulation by creating threads for each philo
 *
 * @param data t_data struct
 * @param philos t_philo struct array containing each philo
 */
void	launch_simulation(t_data *data, t_philo *philos)
{
	int				i;
	pthread_t		*threads;

	threads = malloc(sizeof(pthread_t) * data->nb_philo);
	// create threads for each philo
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_create(&threads[i], NULL, philo_routine, (void *)&philos[i]);
		i++;
	}

	check_death(data, philos);
	// wait for threads to finish and destroy mutexes
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(threads[i], NULL);
		// pthread_mutex_destroy(&data->fork_mutexes[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print);
	free(threads);
	free(philos);
	free(data->fork_mutexes);
}

void	philo_eat(t_philo *philo)
{
	int first_fork, second_fork;

	// Determine the order of acquiring the locks based on the fork index
	if (philo->id % 2 == 0)
	{
		first_fork = philo->id - 1;
		second_fork = philo->id % philo->data->nb_philo;
	}
	else
	{
		first_fork = philo->id % philo->data->nb_philo;
		second_fork = philo->id - 1;
	}
	printf("Philo id '%ld' will lock mutex %d and %d\n", philo->id, first_fork, second_fork);

	// Acquire the locks in the strict order
	pthread_mutex_lock(&philo->data->fork_mutexes[first_fork]);
	if (!philo->data->all_alive)
	{
		printf("\n%ld sees all alive=%d now\n", philo->id, philo->data->all_alive);
		pthread_mutex_unlock(&philo->data->fork_mutexes[first_fork]);
		return;
	}
	print_status(*philo, 1);

	pthread_mutex_lock(&philo->data->fork_mutexes[second_fork]);
	print_status(*philo, 1);

	// Do the eating
	print_status(*philo, 2);
	gettimeofday(&philo->last_meal, NULL);
	usleep(philo->data->time_to_eat * 1000);
	philo->eaten++;

	// Release the locks in the reverse order
	pthread_mutex_unlock(&philo->data->fork_mutexes[second_fork]);
	pthread_mutex_unlock(&philo->data->fork_mutexes[first_fork]);
}

/* void	philo_eat(t_philo *philo)
{
	int	first_fork, second_fork;

	// Determine the order of acquiring the locks based on the fork index
	if (philo->id % 2 == 0)
	{
		first_fork = philo->id - 1;
		second_fork = philo->id % philo->data->nb_philo;
	}
	else
	{
		first_fork = philo->id % philo->data->nb_philo;
		second_fork = philo->id - 1;
	}

	printf("Philo id '%ld' will lock mutex %ld and %ld\n", philo->id, philo->id - 1, (philo->id) % philo->data->nb_philo);
	printf("\n%ld sees %d\n", philo->id, philo->data->all_alive);
	pthread_mutex_lock(&philo->data->fork_mutexes[first_fork]);
	if (!philo->data->all_alive)
	{
		// printf("\n%ld sees %d now\n", philo->id, philo->data->all_alive);
		pthread_mutex_unlock(&philo->data->fork_mutexes[first_fork]);
		return;
	}
	print_status(*philo, 1);

	printf("\n%ld sees %d now\n", philo->id, philo->data->all_alive);
	pthread_mutex_lock(&philo->data->fork_mutexes[second_fork]);
	if (!philo->data->all_alive)
	{
		// printf("\n%ld sees %d now again\n", philo->id, philo->data->all_alive);
		pthread_mutex_unlock(&philo->data->fork_mutexes[second_fork]);
		return;
	}
	print_status(*philo, 1);

	print_status(*philo, 2);
	gettimeofday(&philo->last_meal, NULL); // maybe move this at the end of the meal ?
	usleep(philo->data->time_to_eat * 1000);
	philo->eaten++;
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id - 1]);
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);
} */

/* void	philo_eat(t_philo *philo)
{
	// printf("Philo id '%ld' will lock mutex %ld and %ld\n", philo->id, philo->id - 1, (philo->id) % philo->data->nb_philo);
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->id - 1]);
	if (!philo->data->all_alive)
	{
		// printf("\n%ld sees %d now\n", philo->id, philo->data->all_alive);
		pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id - 1]);
		return;
	}
	print_status(*philo, 1);
	pthread_mutex_lock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);
	if (!philo->data->all_alive)
	{
		// printf("\n%ld sees %d now again\n", philo->id, philo->data->all_alive);
		pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);
		return;
	}
	print_status(*philo, 1);

	print_status(*philo, 2);
	gettimeofday(&philo->last_meal, NULL); // maybe move this at the end of the meal ?
	usleep(philo->data->time_to_eat * 1000);
	philo->eaten++;
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id - 1]);
	pthread_mutex_unlock(&philo->data->fork_mutexes[philo->id % philo->data->nb_philo]);
} */

/**
 * @brief Create the philosophers' routine (think, eat, sleep)
 * It will lock each fork, eat then unlock those. Sleep for a timed duration
 * and think for the remaining time until it can eat again.
 *
 * @param arg void* for each philosopher structure
 * @return void* returns NULL but needed to use pthread_create
 */
void	*philo_routine(void *arg)
{
	t_philo *philo;

	philo = (t_philo*) arg;
	if (philo->id % 2 == 0)
		usleep(30);
	while (philo->data->all_alive)
	{
		if (philo->data->all_alive)
			print_status(*philo, 0);

		if (philo->data->all_alive)
			philo_eat(philo);

		if (philo->data->all_alive)
		{
			print_status(*philo, 3);
			usleep(philo->data->time_to_sleep * 1000);
		}

		// Debug
		// printf("Philo %ld, meal eaten=%ld, last meal=%ld\n",
		// 	philo->id,
		// 	philo->eaten,
		// 	get_time(philo->last_meal));

		if (philo->data->meal_limit > 0 && philo->eaten >= philo->data->meal_limit)
			// return (printf("%ld am full\n", philo->id), philo->data->need_to_eat--, NULL);
			return (philo->data->need_to_eat--, NULL);
	}
	return (NULL);
}

/**
 * @brief checks if a philo has died
 *
 * @param data t_data struct to get nb of philo
 * @param philos t_philo array to check for each philo death
 * @return int 1 if it needs to check again (so no one died yet), 0 if one dies
 */
int	check_death(t_data *data, t_philo *philos)
{
	long	id;
	long	since_meal;

	while (data->all_alive)
	{
		id = 0;
		while(id < data->nb_philo)
		{
			if (data->need_to_eat == 0)
				return (0);
			since_meal = get_time(philos[id].last_meal); // maybe refact this line
			if (since_meal >= data->time_to_die)
			{
				if (!(data->meal_limit > 0 && philos[id].eaten >= data->meal_limit))
				{
					printf("\e[31;1m%ld be dead, he waited %ldms\e[0m\n", id + 1, since_meal);
					print_status(philos[id], 4);
					// pthread_mutex_unlock(&data->fork_mutexes[id]);
					// pthread_mutex_unlock(&data->fork_mutexes[id + 1 % data->nb_philo]);
					data->all_alive = 0;
					return (1);
				}
				// else
					// printf("%ld has eaten enough, hes not dead\n", id + 1);
			}
			id++;
		}
		// usleep(10); // maybe put less than 1ms or calculate ideal time
	}
	return (0);
}



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
t_philo	*init_data(t_data *data);
void	launch_simulation(t_data *data, t_philo *philos);

// simulation.c
void	philo_eat(t_philo *philo);
void	*philo_routine(void *arg);
int	check_death(t_data *data, t_philo *philos);

// utils.c
int	ft_atoi(const char *str);

#endif
