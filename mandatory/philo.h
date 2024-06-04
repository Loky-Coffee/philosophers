/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 01:03:30 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/04 19:18:11 by aalatzas         ###   ########.fr       */
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

# define CRY "\U0001F622"

//terminal collors
# define RED "\033[91m"
# define GREEN "\033[92m"
# define YELLOW "\033[93m"
# define BLUE "\033[94m"
# define MAGENTA "\033[95m"
# define CYAN "\033[96m"
# define RESET "\033[0m"

typedef struct s_x t_env;

typedef struct s_philo
{
	int						time_to_die;
	int						time_to_eat;
	int						time_to_sleep;
	int						philo_meals;
	char					*argv_5;
	int						index;
	pthread_mutex_t			ph_mutex_left_fork;
	pthread_mutex_t			ph_mutex_right_fork;
	t_env					*env;
}	t_philo;

typedef struct s_x
{
	int						i;
	int						x;
	int						y;
	bool					death;
	int						min_meals;
	int						philo_nbr;
	pthread_mutex_t			mutex;
	unsigned long			sec;
	unsigned long			usec;
	int						run;
	t_philo					*ph;
}	t_env;


int							ft_atoi(const char *str);
void						is_all_int(int argc, char **argv);
int 						init_threads(t_env *env);
unsigned long long			get_time();

#endif
