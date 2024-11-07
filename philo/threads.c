/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariverg <mariverg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:16:18 by mariverg          #+#    #+#             */
/*   Updated: 2024/11/07 12:03:41 by mariverg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_eats_even(t_philo *phi)
{
	t_rules	*rules;

	rules = phi->rules;
	pthread_mutex_lock(&(rules->fork[phi->left_fork_id]));
	print_moment(rules, phi->philo_id, "has taken left fork");
	pthread_mutex_lock(&(rules->fork[phi->right_fork_id]));
	print_moment(rules, phi->philo_id, "has taken right fork");
	print_moment(rules, phi->philo_id, "is eating");
	phi->time_last_eat = timestamp();
	pthread_mutex_lock(&(rules->meal_count_mutex));
	phi->times_ate++;
	if (phi->times_ate == rules->nb_eat)
		rules->philo_feed++;
	if (rules->philo_feed == rules->nb_philo)
		rules->all_ate = true;
	pthread_mutex_unlock(&(rules->meal_count_mutex));
	wait_time(rules, rules->time_to_eat * 1000);
	pthread_mutex_unlock(&(rules->fork[phi->left_fork_id]));
	pthread_mutex_unlock(&(rules->fork[phi->right_fork_id]));
}

static void	philo_eats_odd(t_philo *phi)
{
	t_rules	*rules;

	rules = phi->rules;
	pthread_mutex_lock(&(rules->fork[phi->right_fork_id]));
	print_moment(rules, phi->philo_id, "has taken right fork");
	pthread_mutex_lock(&(rules->fork[phi->left_fork_id]));
	print_moment(rules, phi->philo_id, "has taken left fork");
	print_moment(rules, phi->philo_id, "is eating");
	phi->time_last_eat = timestamp();
	pthread_mutex_lock(&(rules->meal_count_mutex));
	phi->times_ate++;
	if (phi->times_ate == rules->nb_eat)
		rules->philo_feed++;
	if (rules->philo_feed == rules->nb_philo)
		rules->all_ate = true;
	pthread_mutex_unlock(&(rules->meal_count_mutex));
	wait_time(rules, rules->time_to_eat * 1000);
	pthread_mutex_unlock(&(rules->fork[phi->left_fork_id]));
	pthread_mutex_unlock(&(rules->fork[phi->right_fork_id]));
}

void	*philo_thread(void *philosopher)
{
	t_philo			*phi;
	t_rules			*rules;
	unsigned long	fruit;

	phi = (t_philo *)philosopher;
	rules = phi->rules;
	if (phi->philo_id % 2 == 0)
		usleep(200);
	fruit = fruit_generator(timestamp(), phi->philo_id);
	while (1)
	{
		if (rules->died || rules->all_ate)
			break ;
		print_moment(rules, phi->philo_id, "is thinking");
		usleep(my_rand(fruit) % 1000);
		if (phi->philo_id % 2 == 0)
			philo_eats_even(phi);
		else if (phi->philo_id != 0)
			philo_eats_odd(phi);
		if (rules->died || rules->all_ate)
			break ;
		print_moment(rules, phi->philo_id, "is sleeping");
		wait_time(rules, rules->time_sleep);
	}
	return (NULL);
}

void	death_seeker(t_rules *rules, t_philo *p)
{
	int		i;

	while (!rules->all_ate)
	{
		i = -1;
		while (++i < rules->nb_philo && !rules->died)
		{
			pthread_mutex_lock(&(rules->eating));
			if (time_taken(p[i].time_last_eat, timestamp()) > rules->time_death)
			{
				print_moment(rules, i, "died");
				rules->died = true;
			}
			pthread_mutex_unlock(&(rules->eating));
			usleep(100);
		}
		if (rules->died)
			break ;
		usleep(100);
	}
}

void	finisher(t_rules *rules)
{
	int	i;

	i = -1;
	while (++i < rules->nb_philo)
		pthread_join(rules->philo[i].thread_id, NULL);
	i = -1;
	while (++i < rules->nb_philo)
		pthread_mutex_destroy(&rules->fork[i]);
	pthread_mutex_destroy(&rules->meal_count_mutex);
	pthread_mutex_destroy(&rules->write);
	pthread_mutex_destroy(&rules->eating);
}
