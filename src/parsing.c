/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalvard <aalvarad@student.42lausanne.ch    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:00:00 by aalvarad          #+#    #+#             */
/*   Updated: 2026/10/04 00:19:34 by aalvard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	ft_atol_strict(char *str)
{
	long	result;
	int		i;

	i = 0;
	result = 0;
	if (!str || !str[0])
		return (-1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		result = result * 10 + (str[i] - '0');
		if (result > INT_MAX)
			return (-1);
		i++;
	}
	return (result);
}

static int	set_value(char *arg, long *dest)
{
	long	value;

	value = ft_atol_strict(arg);
	if (value <= 0)
		return (1);
	*dest = value;
	return (0);
}

int	parse_args(int argc, char **argv, t_table *table)
{
	if (set_value(argv[1], &table->nb_philo))
		return (1);
	if (set_value(argv[2], &table->time_to_die))
		return (1);
	if (set_value(argv[3], &table->time_to_eat))
		return (1);
	if (set_value(argv[4], &table->time_to_sleep))
		return (1);
	if (argc == 6)
	{
		if (set_value(argv[5], &table->must_eat))
			return (1);
	}
	else
		table->must_eat = -1;
	return (0);
}
