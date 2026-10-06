/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalvard <aalvarad@student.42lausanne.ch    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:00:00 by aalvarad          #+#    #+#             */
/*   Updated: 2026/10/05 02:02:20 by aalvard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutexes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nb_philo)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (1);
		if (pthread_mutex_init(&table->philos[i].meal_lock, NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&table->print_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->stop_lock, NULL) != 0)
		return (1);
	return (0);
}

static void	init_philos(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < table->nb_philo)
	{
		philo = &table->philos[i];
		philo->id = i + 1;
		philo->meals_eaten = 0;
		philo->last_meal = 0;
		philo->table = table;
		philo->left_fork = &table->forks[i];
		philo->right_fork = &table->forks[(i + 1) % table->nb_philo];
		i++;
	}
}

int	init_table(t_table *table)
{
	table->stop = 0;
	table->start_time = 0;
	table->philos = malloc(sizeof(t_philo) * table->nb_philo);
	if (!table->philos)
		return (1);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!table->forks)
	{
		free(table->philos);
		return (1);
	}
	if (init_mutexes(table))
		return (1);
	init_philos(table);
	return (0);
}
