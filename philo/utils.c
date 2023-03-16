/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: croy <croy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 18:46:09 by croy              #+#    #+#             */
/*   Updated: 2023/03/16 16:29:36 by croy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief converts a char to an int
 *
 * @param str string to convert
 * @return int converted
 */
int	ft_atoi(const char *str)
{
	long	result;
	int		sign;
	size_t	i;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (result != ((result * 10 + ((str[i] - 48) * sign)) / 10))
			return ((sign + 1) / 2 / -1);
		result = ((result * 10) + ((str[i] - 48) * sign));
		i++;
	}
	return ((int) result);
}
