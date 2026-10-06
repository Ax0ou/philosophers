/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalvard <aalvarad@student.42lausanne.ch    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:00:00 by aalvarad          #+#    #+#             */
/*   Updated: 2026/10/05 02:31:46 by aalvard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *msg)
{
	t_table	*table;
	long	elapsed;

	table = philo->table;
	pthread_mutex_lock(&table->print_lock);
	if (!is_stopped(table))
	{
		elapsed = get_time_ms() - table->start_time;
		printf("%ld %d %s\n", elapsed, philo->id, msg);
	}
	pthread_mutex_unlock(&table->print_lock);
}
