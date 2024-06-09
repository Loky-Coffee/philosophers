/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 01:03:30 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/09 04:07:22 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

# define CRY 		"\U0001F622"
# define TAKE_FORK 	"\U0001F531"
# define EAT		"\U0001F60B"
# define SLEEP		"🛏️"
# define THINKING	"\U0001F914"
# define DIE		"\U0001F480"

// # define CRY 		""
// # define TAKE_FORK 	""
// # define EAT		""
// # define SLEEP		""
// # define THINKING	""
// # define DIE		""

//terminal collors
# define RED		"\033[91m"
# define GREEN		"\033[92m"
# define Y			"\033[93m"
# define BLUE		"\033[94m"
# define MAGENTA	"\033[95m"
# define CYAN		"\033[96m"
# define R			"\033[0m"

// # define RED		""
// # define GREEN		""
// # define Y			""
// # define BLUE		""
// # define MAGENTA	""
// # define CYAN		""
// # define R			""

typedef struct s_x	t_env;

typedef struct s_philo
{
	unsigned long long int	time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						philo_meals;
	unsigned long long int	last_eat_time;
	char					*argv_5;
	int						index;
	t_env					*env;
}	t_philo;

typedef struct s_x
{
	bool					death;
	int						min_meals;
	int						philo_nbr;
	unsigned long			sec;
	unsigned long			usec;
	int						run;
	t_philo					*ph;
	unsigned long long int	s_t;
	pthread_mutex_t			lock_fork;
	pthread_mutex_t			free_fork;
	pthread_mutex_t			lock_last_time_eat;
	pthread_mutex_t			*l_fork;
}	t_env;

//threads.c
int						join_threads(t_env *env, pthread_t	*philo_thread);
int						init_threads(t_env *env);

//handel_fork.c
void					give_fork_back(t_philo *ph);
void					try_take_fork(t_philo *ph);

//philo_tasks.c
void					ft_sleep(unsigned long long time);
void					*check_philo_death(void *arg);
void					*schedule_action(void *arg);

//utils.c
int						ft_atoi(const char *str);
void					is_all_int(int argc, char **argv);
unsigned long long int	get_time(void);
void					terminate(t_env *env, int i);
void					ft_sleep(unsigned long long time);

#endif