/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalvard <aalvarad@student.42lausanne.ch    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:00:00 by aalvarad          #+#    #+#             */
/*   Updated: 2026/10/06 15:51:13 by aalvard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	all_ate_enough(t_table *table)
{
	int		i;
	int		done;
	t_philo	*philo;

	if (table->must_eat == -1)
		return (0);
	i = 0;
	done = 0;
	while (i < table->nb_philo)
	{
		philo = &table->philos[i];
		pthread_mutex_lock(&philo->meal_lock);
		if (philo->meals_eaten >= table->must_eat)
			done = done + 1;
		pthread_mutex_unlock(&philo->meal_lock);
		i = i + 1;
	}
	if (done == table->nb_philo)
	{
		set_stop(table);
		return (1);
	}
	return (0);
}

static int	check_death(t_table *table)
{
	int		i;
	long	elapsed;
	t_philo	*philo;

	i = 0;
	while (i < table->nb_philo)
	{
		philo = &table->philos[i];
		pthread_mutex_lock(&philo->meal_lock);
		elapsed = get_time_ms() - philo->last_meal;
		pthread_mutex_unlock(&philo->meal_lock);
		if (elapsed > philo->table->time_to_die)
		{
			print_status(philo, MSG_DIED);
			set_stop(table);
			return (1);
		}
		i++;
	}
	return (0);
}

void	monitor(t_table *table)
{
	while (!is_stopped(table))
	{
		if (check_death(table))
			return ;
		if (all_ate_enough(table))
			return ;
		usleep(1000);
	}
}
