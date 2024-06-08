/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/08 08:17:22 by aalatzas         ###   ########.fr       */
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
		pthread_mutex_unlock(&ph->env->lock_right_fork[ph->index - 1]);
	}
	else if (ph->index % 2 == 0)
	{
		pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index + 1]);
		pthread_mutex_unlock(&ph->env->lock_right_fork[ph->index]);
	}
	else
	{
		pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index]);
		pthread_mutex_unlock(&ph->env->lock_right_fork[ph->index + 1]);
	}
	pthread_mutex_unlock(&ph->env->free_fork);
}

void try_take_fork(t_philo *ph)
{
	// printf("TRY Take ! ph->index[%d]\n", ph->index);
	pthread_mutex_lock(&ph->env->lock_fork);
	if ((ph->index) == ph->env->philo_nbr && ph->env->philo_nbr % 2 != 0)
	{
		pthread_mutex_lock(&ph->env->lock_left_fork[ph->index % ph->env->philo_nbr]);
		pthread_mutex_lock(&ph->env->lock_right_fork[ph->index - 1]);
	}
	else if (ph->index % 2 == 0)
	{
		pthread_mutex_lock(&ph->env->lock_left_fork[ph->index + 1]);
		pthread_mutex_lock(&ph->env->lock_right_fork[ph->index]);
	}
	else
	{
		pthread_mutex_lock(&ph->env->lock_left_fork[ph->index]);
		pthread_mutex_lock(&ph->env->lock_right_fork[ph->index + 1]);
	}
	printf("%llu %d has taken a fork\n", get_time(), ph->index);
	pthread_mutex_unlock(&ph->env->lock_fork);
}

void *check_philo_death(void *arg)
{
	t_env	*env;

	env = (t_env *)arg;
	int		i;

	i = 1;
	// ft_sleep(6000);
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
		if ((((get_time() - env->ph[i].last_eat_time) >= (unsigned long long)env->ph[i].time_to_die) && env->ph[i].last_eat_time != 0) || env->philo_nbr <= 1)
		{
			printf("%llu %d died\n", get_time(), i);
			exit(0);
		}
		i++;
		ft_sleep(1000);
	}
	return NULL;
}

void *schedule_action(void *arg)
{
	t_philo *ph = (t_philo *)arg;
	if (ph->index % 2 == 0)
		ft_sleep(ph->time_to_eat / 2);
	while (true)
	{
		if (ph->env->death != true && ph->env->philo_nbr > 1)
			try_take_fork(ph);
		pthread_mutex_lock(&ph->env->lock_last_time_eat);
		ph->last_eat_time = get_time();
		pthread_mutex_unlock(&ph->env->lock_last_time_eat);
		if (ph->env->death != true && ph->env->philo_nbr > 1)
			printf("%llu %d is eating\n", get_time(), ph->index);
		ft_sleep(ph->time_to_eat);
		give_fork_back(ph);
		if (ph->env->min_meals > 0)
		{
			ph->philo_meals++;
			if (ph->philo_meals >= ph->env->min_meals)
			{
				ph->last_eat_time = 0;
				give_fork_back(ph);
				printf("%llu %d is sleeping\n", get_time(), ph->index);
				break ;
			}
		}
		if (ph->env->death != true)
			printf("%llu %d is sleeping\n", get_time(), ph->index);
		ft_sleep(ph->time_to_sleep);
		if (ph->env->death != true)
		{
			printf("%llu %d is thinking\n", get_time(), ph->index);
			// ft_sleep((ph->time_to_eat + ph->time_to_sleep) - (ph->time_to_die / 2));
		}
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

	int i = 1;
	pthread_mutex_init(&env->lock_fork, NULL);
	pthread_mutex_init(&env->free_fork, NULL);
	pthread_mutex_init(&env->lock_last_time_eat, NULL);
	env->lock_left_fork = malloc(env->philo_nbr * sizeof(pthread_mutex_t));
	env->lock_right_fork = malloc(env->philo_nbr * sizeof(pthread_mutex_t));
	philo_thread = calloc(env->philo_nbr, sizeof(pthread_t));
	if (pthread_create(&death_thread, NULL, check_philo_death, (void *)env) != 0)
		return(result = -1, result);
	while (i <= env->philo_nbr)
	{
		pthread_mutex_init(&env->lock_left_fork[i], NULL);
		pthread_mutex_init(&env->lock_right_fork[i], NULL);
		env->ph[i].last_eat_time = get_time();
		if (pthread_create(&philo_thread[i], NULL, schedule_action, (void *)&env->ph[i]) != 0)
		{
			result = env->ph[i].index;
			break;
		}
		i++;
	}
	pthread_mutex_init(&env->lock_left_fork[i], NULL);
	pthread_mutex_init(&env->lock_right_fork[i], NULL);
	pthread_join(death_thread, NULL);
	join_threads(env, result, philo_thread);
	terminate(env, i);
	return result;
}