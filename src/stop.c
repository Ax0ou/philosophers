/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalvard <aalvarad@student.42lausanne.ch    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:00:00 by aalvarad          #+#    #+#             */
/*   Updated: 2026/10/05 02:21:29 by aalvard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_stopped(t_table *table)
{
	int	result;

	pthread_mutex_lock(&table->stop_lock);
	result = table->stop;
	pthread_mutex_unlock(&table->stop_lock);
	return (result);
}

void	set_stop(t_table *table)
{
	pthread_mutex_lock(&table->stop_lock);
	table->stop = 1;
	pthread_mutex_unlock(&table->stop_lock);
}
