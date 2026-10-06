/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalvard <aalvarad@student.42lausanne.ch    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:00:00 by aalvarad          #+#    #+#             */
/*   Updated: 2026/10/06 00:23:54 by aalvard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, MSG_FORK);
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, MSG_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, MSG_FORK);
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, MSG_FORK);
	}
}

static void	eat(t_philo *philo)
{
	take_forks(philo);
	print_status(philo, MSG_EAT);
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_lock);
	ft_usleep(philo->table->time_to_eat, philo->table);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->nb_philo == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, MSG_FORK);
		ft_usleep(philo->table->time_to_die, philo->table);
		pthread_mutex_unlock(philo->left_fork);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(10, philo->table);
	while (!is_stopped(philo->table))
	{
		eat(philo);
		print_status(philo, MSG_SLEEP);
		ft_usleep(philo->table->time_to_sleep, philo->table);
		print_status(philo, MSG_THINK);
	}
	return (NULL);
}
