/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalvarad <aalvarad@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 14:00:00 by aalvarad          #+#    #+#             */
/*   Updated: 2026/09/21 14:00:00 by aalvarad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Le main ne fait que l'orchestration : chaque etape est une fonction
** qui renvoie 0 si tout va bien, autre chose sinon.
*/
int	main(int argc, char **argv)
{
	t_table	table;

	if (argc != 5 && argc != 6)
		return (printf(USAGE), 1);
	if (parse_args(argc, argv, &table) != 0)
		return (printf("Error: invalid arguments\n"), 1);
	if (init_table(&table) != 0)
		return (printf("Error: initialization failed\n"), 1);
	start_simulation(&table);
	destroy_table(&table);
	return (0);
}
