/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariverg <mariverg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:16:21 by mariverg          #+#    #+#             */
/*   Updated: 2024/11/07 12:14:24 by mariverg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_rules	*rules;

	rules = malloc(sizeof(t_rules));
	if (ac != 5 && ac != 6)
		error_msg(USAGE);
	if ((!ft_init_rules(rules, av)))
		error_msg("threats could not be created");
	if ((!launcher(rules)))
		error_msg("something went wrong");
	exit(0);
}

bool	launcher(t_rules *rules)
{
	int		i;
	t_philo	*phi;

	i = 0;
	phi = rules->philo;
	rules->first_timestamp = timestamp();
	while (i < rules->nb_philo)
	{
		if (pthread_create(&(phi[i].thread_id), NULL, philo_thread, &(phi[i])))
			error_msg("something went wrong creating the threads");
		phi[i].time_last_eat = timestamp();
		i++;
	}
	death_seeker(rules, rules->philo);
	finisher(rules);
	return (true);
}

void	error_msg(char *str)
{
	printf(RED"Error"RESET": %s", str);
	exit (EXIT_FAILURE);
}
