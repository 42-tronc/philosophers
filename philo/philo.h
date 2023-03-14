/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:40:42 by croy              #+#    #+#             */
/*   Updated: 2023/03/14 12:37:58 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stddef.h>

typedef struct s_data
{
	unsigned	nb_philo;
	unsigned	death_time;
	unsigned	eat_time;
	unsigned	sleep_time;
	unsigned	eat_count;
}	t_data;


#endif
