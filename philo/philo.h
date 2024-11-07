/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariverg <mariverg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:17:35 by mariverg          #+#    #+#             */
/*   Updated: 2024/11/07 12:32:02 by mariverg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include "philo.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

# define USAGE "Usage: number_of_philosophers, time_to_die, \
time_to_eat, time_to_sleep, [number_of_times_each_philosopher_must_eat]"

# define RED "\x1B[31m"
# define RESET "\x1B[0m"

struct	s_rules;

typedef struct s_philo
{
	int				philo_id;
	int				left_fork_id;
	int				right_fork_id;
	int				times_ate;
	long long		time_last_eat;
	struct s_rules	*rules;
	pthread_t		thread_id;
}	t_philo;

typedef struct s_rules
{
	int					nb_philo;
	int					time_death;
	int					time_to_eat;
	int					time_sleep;
	int					nb_eat;
	long long			first_timestamp;
	int					philo_feed;
	bool				all_ate;
	bool				died;
	pthread_mutex_t		meal_count_mutex;
	pthread_mutex_t		eating;
	pthread_mutex_t		write;
	pthread_mutex_t		fork[250];
	t_philo				philo[250];
}	t_rules;

//main functions
int				main(int ac, char **av);
bool			ft_init_rules(t_rules *rules, char **av);
bool			launcher(t_rules *rules);

//threads
void			*philo_thread(void *philosopher);
void			death_seeker(t_rules *rules, t_philo *p);
void			finisher(t_rules *rules);

//utils & errors
int				ft_atoi(char *n);
void			print_moment(t_rules *rules, int id, char *action);
long long		timestamp(void);
void			wait_time(t_rules *rules, int time);
long long		time_taken(long long past, long long present);
unsigned int	fruit_generator(unsigned long time, int id);
unsigned long	my_rand(unsigned long fruit);
void			error_msg(char *str);

#endif