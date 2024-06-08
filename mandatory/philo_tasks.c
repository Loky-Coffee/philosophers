/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/08 22:42:19 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_sleep(unsigned long long time)
{
	unsigned long long int	start;

	start = get_time();
	while (get_time() - start < time / 1000)
		usleep(100);
}

void	give_fork_back(t_philo *ph)
{
	pthread_mutex_lock(&ph->env->free_fork);
	// printf("ph->index[%d]\n", ph->index);
	if ((ph->index) == ph->env->philo_nbr && ph->env->philo_nbr % 2 != 0)
	{
		pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index % ph->env->philo_nbr]);
		pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index - 1]);
	}
	else if (ph->index % 2 == 0)
	{
		pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index + 1]);
		pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index]);
	}
	else
	{
		pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index]);
		pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index + 1]);
	}
	pthread_mutex_unlock(&ph->env->free_fork);
}

void try_take_fork(t_philo *ph)
{
	pthread_mutex_lock(&ph->env->lock_fork);
	if (ph->env->philo_nbr == 1 && ph->env->death != true)
	{
		pthread_mutex_lock(&ph->env->lock_left_fork[ph->index]);
		printf(YELLOW"%llu %d has taken a fork %s\n"RESET, get_time() - ph->env->start_time, ph->index, TAKE_FORK);
	}
	else if (ph->index == ph->env->philo_nbr - 1 && ph->env->philo_nbr % 2 != 0 && ph->index != 0 && ph->env->death != true)
	{
		pthread_mutex_lock(&ph->env->lock_left_fork[ph->index]);
		printf(YELLOW"%llu %d has taken a fork %s\n"RESET, get_time() - ph->env->start_time, ph->index, TAKE_FORK);
		pthread_mutex_lock(&ph->env->lock_left_fork[ph->index - 1]);
		printf(YELLOW"%llu %d has taken a fork %s\n"RESET, get_time() - ph->env->start_time, ph->index, TAKE_FORK);
	}
	else if (ph->index % 2 == 0 && ph->env->death != true)
	{
		pthread_mutex_lock(&ph->env->lock_left_fork[ph->index + 1]);
		printf(YELLOW"%llu %d has taken a fork %s\n"RESET, get_time() - ph->env->start_time, ph->index, TAKE_FORK);
		pthread_mutex_lock(&ph->env->lock_left_fork[ph->index]);
		printf(YELLOW"%llu %d has taken a fork %s\n"RESET, get_time() - ph->env->start_time, ph->index, TAKE_FORK);
	}
	else if(ph->env->death != true)
	{
		pthread_mutex_lock(&ph->env->lock_left_fork[ph->index]);
		printf(YELLOW"%llu %d has taken a fork %s\n"RESET, get_time() - ph->env->start_time, ph->index, TAKE_FORK);
		pthread_mutex_lock(&ph->env->lock_left_fork[ph->index + 1]);
		printf(YELLOW"%llu %d has taken a fork %s\n"RESET, get_time() - ph->env->start_time, ph->index, TAKE_FORK);
	}
	pthread_mutex_unlock(&ph->env->lock_fork);
}

void *check_philo_death(void *arg)
{
	t_env	*env;

	env = (t_env *)arg;
	int		i;

	i = 1;
	while (true)
	{
		if (env->philo_nbr && i >= env->philo_nbr)
		{
			i = 1;
			while (env->min_meals > 0 && env->ph[i].philo_meals >= env->min_meals)
			{
				if(i >= env->philo_nbr)
					exit(0);
				i++;
			}
			i = 1;
		}
		if (env->philo_nbr == 1)
		{
			ft_sleep(env->ph[i].time_to_die * 1000);
			printf(RED"%llu %d died %s\n"RESET, get_time() - env->start_time, i, DIE);
			exit(0);
		}
		if ((((get_time() - env->ph[i].last_eat_time) >= (unsigned long long)env->ph[i].time_to_die) && env->ph[i].last_eat_time != 0))
		{
			env->death = true;
			printf(RED"%llu %d died %s\n"RESET, get_time() - env->start_time, i, DIE);
			exit(0);
		}
		i++;
		// ft_sleep(1000);
	}
	return NULL;
}

void *schedule_action(void *arg)
{
	t_philo *ph = (t_philo *)arg;
	if (ph->index % 2 == 0)
		ft_sleep(ph->time_to_eat / 2);
	while (ph->env->death != true)
	{
		if (ph->env->death != true)
			try_take_fork(ph);
		pthread_mutex_lock(&ph->env->lock_last_time_eat);
		ph->last_eat_time = get_time();
		pthread_mutex_unlock(&ph->env->lock_last_time_eat);
		if (ph->env->death != true && ph->env->philo_nbr > 1)
			printf(GREEN"%llu %d is eating %s\n"RESET, get_time() - ph->env->start_time, ph->index, EAT);
		ft_sleep(ph->time_to_eat);
		give_fork_back(ph);
		if (ph->env->min_meals > 0 && ph->env->philo_nbr > 1)
		{
			ph->philo_meals++;
			if (ph->philo_meals >= ph->env->min_meals && ph->env->death != true)
			{
				ph->last_eat_time = 0;
				give_fork_back(ph);
				printf(MAGENTA"%llu %d is sleeping %s\n"RESET, get_time() - ph->env->start_time, ph->index, SLEEP);
				break ;
			}
		}
		if (ph->env->death != true  && ph->env->philo_nbr > 1)
			printf(MAGENTA"%llu %d is sleeping %s\n"RESET, get_time() - ph->env->start_time, ph->index, SLEEP);
		ft_sleep(ph->time_to_sleep);
		if (ph->env->death != true  && ph->env->philo_nbr > 1)
			printf(BLUE"%llu %d is thinking %s\n"RESET, get_time() - ph->env->start_time, ph->index, THINKING);
	}
	return NULL;
}

int join_threads(t_env *env, int result, pthread_t	*philo_thread)
{
	int i;

	i = 1;
	if (result == 0)
	{
		i = 0;
		while (i <= env->philo_nbr)
		{
			if(pthread_join(philo_thread[i], NULL) != 0)
			{
				result = env->ph[i].index;
				break;
			}
			i++;
		}
	}
	return (result);
}

int init_threads(t_env *env)
{
	pthread_t	*philo_thread;
	pthread_t	death_thread;
	int			result = 0;
	int 		i;

	i = 1;
	pthread_mutex_init(&env->lock_fork, NULL);
	pthread_mutex_init(&env->free_fork, NULL);
	pthread_mutex_init(&env->lock_last_time_eat, NULL);
	env->lock_left_fork = calloc(env->philo_nbr, sizeof(pthread_mutex_t));
	philo_thread = calloc(env->philo_nbr, sizeof(pthread_t));
	if (pthread_create(&death_thread, NULL, check_philo_death, (void *)env) != 0)
		return(result = -1, result);
	while (i <= env->philo_nbr)
	{
		pthread_mutex_init(&env->lock_left_fork[i], NULL);
		env->ph[i].last_eat_time = get_time();
		if (pthread_create(&philo_thread[i], NULL, schedule_action, (void *)&env->ph[i]) != 0)
		{
			result = env->ph[i].index;
			break;
		}
		i++;
	}
	pthread_join(death_thread, NULL);
	join_threads(env, result, philo_thread);
	terminate(env, i);
	return result;
}
