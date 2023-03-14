/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:40:42 by croy              #+#    #+#             */
/*   Updated: 2023/03/14 12:50:22 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_data
{
	long	nb_philo;
	long	death_time;
	long	eat_time;
	long	sleep_time;
	long	eat_count;
}	t_data;

int	ft_atoi(const char *str);

#endif
