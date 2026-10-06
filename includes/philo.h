/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalvard <aalvarad@student.42lausanne.ch    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:00:00 by aalvarad          #+#    #+#             */
/*   Updated: 2026/10/05 00:55:50 by aalvard          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>

# define USAGE "Usage: ./philo nb_philo die eat sleep [must_eat]\n"

# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DIED "died"

/*
** t_philo : UN philosophe = UN thread.
** left_fork / right_fork : pointeurs vers des mutex du tableau table->forks.
**   Deux voisins pointent vers le MEME mutex : c'est la fourchette partagee.
** meal_lock : protege last_meal et meals_eaten, car le monitor les lit
**   pendant que le thread du philo les ecrit (sinon : data race).
** table : pointeur vers les donnees communes (pas de variable globale).
*/
typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_lock;
	struct s_table	*table;
}	t_philo;

typedef struct s_table
{
	long			nb_philo;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			must_eat;
	long			start_time;
	int				stop;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	stop_lock;
	t_philo			*philos;
}	t_table;

/* parsing.c */
int		parse_args(int argc, char **argv, t_table *table);

/* init.c */
int		init_table(t_table *table);

/* simulation.c */
int		start_simulation(t_table *table);

/* routine.c */
void	*philo_routine(void *arg);

/* monitor.c */
void	monitor(t_table *table);

/* print.c */
void	print_status(t_philo *philo, char *msg);

/* time.c */
long	get_time_ms(void);
void	ft_usleep(long ms, t_table *table);

/* stop.c */
int		is_stopped(t_table *table);
void	set_stop(t_table *table);

/* cleanup.c */
void	destroy_table(t_table *table);

#endif
