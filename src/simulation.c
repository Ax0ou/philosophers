/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalvard <aalvarad@student.42lausanne.ch    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:00:00 by aalvarad          #+#    #+#             */
/*   Updated: 2026/10/05 23:15:28 by aalvard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	join_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
}

int	start_simulation(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	table->start_time = get_time_ms();
	while (i < table->nb_philo)
	{
		philo = &table->philos[i];
		philo->last_meal = table->start_time;
		if (pthread_create(&philo->thread, NULL, philo_routine, philo) != 0)
			return (1);
		i++;
	}
	monitor(table);
	join_threads(table);
	return (0);
}
