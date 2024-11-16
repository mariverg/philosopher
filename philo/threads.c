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


// Corrección de philo_eats
static void philo_eats(t_philo *phi)
{
    t_rules *rules;

    rules = phi->rules;
    pthread_mutex_lock(&(rules->fork[phi->left_fork_id]));
    print_moment(rules, phi->philo_id, "has taken left  fork");
    pthread_mutex_lock(&(rules->fork[phi->right_fork_id]));
    print_moment(rules, phi->philo_id, "has taken right fork");
    
    pthread_mutex_lock(&(rules->eating));
    print_moment(rules, phi->philo_id, "is eating");
    phi->time_last_eat = timestamp();
    pthread_mutex_unlock(&(rules->eating)); //usamos mute para eating en lugar de meal_count_mutex

    wait_time(rules, rules->time_to_eat);
    
    pthread_mutex_lock(&(rules->meal_count_mutex));
    phi->times_ate++;
    if (phi->times_ate == rules->nb_eat)
        rules->philo_feed++;
    if (rules->philo_feed == rules->nb_philo)
        rules->all_ate = true;
    pthread_mutex_unlock(&(rules->meal_count_mutex));
    
    pthread_mutex_unlock(&(rules->fork[phi->left_fork_id]));
    pthread_mutex_unlock(&(rules->fork[phi->right_fork_id]));
}

// Corrección de philo_thread
void *philo_thread(void *philosopher)
{
    t_philo *phi = (t_philo *)philosopher;
    t_rules *rules = phi->rules;

    pthread_mutex_lock(&(rules->eating));
    phi->time_last_eat = timestamp();
    pthread_mutex_unlock(&(rules->eating));

    if (phi->philo_id % 2)
        usleep(500); // Pequeño delay para filósofos impares

    while (!rules->died && !rules->all_ate)
    {
        philo_eats(phi);
        
        if (rules->died || rules->all_ate)
            break;

        print_moment(rules, phi->philo_id, "is sleeping");
        wait_time(rules, rules->time_sleep);

        print_moment(rules, phi->philo_id, "is thinking");
        // Pequeña pausa para evitar sobrecarga de CPU
        usleep(100);
    }
    return NULL;
}

// Corrección del death_seeker
void death_seeker(t_rules *rules, t_philo *philos)
{
    int i;
    long long current_time;

    while (!rules->died && !rules->all_ate)
    {
        i = 0;
        while (i < rules->nb_philo && !rules->died)
        {
            current_time = timestamp();
            pthread_mutex_lock(&(rules->eating));
            if (time_taken(philos[i].time_last_eat, current_time) > rules->time_death)
            {
                print_moment(rules, philos[i].philo_id + 1, "died");
                rules->died = true;
                pthread_mutex_unlock(&(rules->eating));
                return;
            }
            pthread_mutex_unlock(&(rules->eating));
            i++;
            usleep(100); // Pequeña pausa para no sobrecargar CPU
        }
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
