/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/07 02:47:03 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// pthread_mutex_t shared_mutex = PTHREAD_MUTEX_INITIALIZER;

// void	*philo_think(void *env)
// {
// 	t_env	e;

// 	e = *(t_env *)env;
// 	return (NULL);
// }

void	*philo_eat(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
		static int y;
		printf("y: %d\n", y);
		y++;

	return (NULL);
}

// void	*philo_sleep(void *env)
// {
// 	t_env	e;

// 	e = *(t_env *)env;
// 	return (NULL);
// }

// void	*philo_die(void *env)
// {
// 	t_env	e;

// 	e = *(t_env *)env;
// 	return (NULL);
// }
int can_eat(t_philo *ph)
{
	int right_fork;

	right_fork = 0;
	// printf("ph->env->philo_nbr [%d]\n", ph->env->philo_nbr);
	// right_fork = (ph->index + 1) % ph->env->philo_nbr;
	// pthread_mutex_lock(&ph->ph_mutex_left_fork);
	printf("philo [%d] is eating\n", ph->index);
	// if (pthread_mutex_lock(&ph->env->ph[right_fork].ph_mutex_left_fork) == 0)
	// 	return 1;
	// pthread_mutex_unlock(&ph->ph_mutex_left_fork);
	return 0;
}

void *schedule_action(void *arg)
{
	t_philo *ph = (t_philo *)arg;
	int i = 0;
	if (ph->index % 2 == 0)
		usleep(50000);
	while (true)
	{
		// pthread_mutex_lock(&ph->env->mutex);
		if ((ph->index + 1) == ph->env->philo_nbr)
		{
			pthread_mutex_lock(&ph->env->lock_left_fork[ph->index - 1]);
			pthread_mutex_lock(&ph->env->lock_right_fork[0]);
		}
		else if (ph->index % 2 == 0)
		{
			pthread_mutex_lock(&ph->env->lock_left_fork[(ph->index + 1) % ph->env->philo_nbr]);
			pthread_mutex_lock(&ph->env->lock_right_fork[ph->index]);
		}
		else
		{
			// pthread_mutex_lock(&ph->env->lock_right_fork[ph->index]);
			pthread_mutex_lock(&ph->env->lock_left_fork[ph->index]);
			pthread_mutex_lock(&ph->env->lock_right_fork[(ph->index + 1) % ph->env->philo_nbr]);
		}
		// pthread_mutex_unlock(&ph->env->mutex);
		printf("%llu %d has taken a fork\n", get_time(), ph->index + 1);
		// printf("ph->index[%d]    ph->env->philo_nbr[%d]\n",ph->index + 1, ph->env->philo_nbr);
		if (ph->env->min_meals > 0)
		{
			printf("%llu %d is eating\n", get_time(), ph->index + 1);
			usleep(ph->time_to_eat * 10);
			if (ph->philo_meals == ph->env->min_meals)
			{
				if ((ph->index + 1) == ph->env->philo_nbr)
				{
					pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index - 1]);
					pthread_mutex_unlock(&ph->env->lock_right_fork[0]);
				}
				else if (ph->index % 2 == 0)
				{
					pthread_mutex_unlock(&ph->env->lock_left_fork[(ph->index + 1) % ph->env->philo_nbr]);
					pthread_mutex_unlock(&ph->env->lock_right_fork[ph->index]);
				}
				else
				{
					pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index]);
					pthread_mutex_unlock(&ph->env->lock_right_fork[(ph->index + 1) % ph->env->philo_nbr]);
				}
				// pthread_mutex_unlock(&ph->env->lock_right_fork[ph->index]);
				break;
			}
			ph->philo_meals++;
		}
		if ((ph->index + 1) == ph->env->philo_nbr)
		{
			pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index - 1]);
			pthread_mutex_unlock(&ph->env->lock_right_fork[0]);
		}
		else if (ph->index % 2 == 0)
		{
			pthread_mutex_unlock(&ph->env->lock_left_fork[(ph->index + 1) % ph->env->philo_nbr]);
			pthread_mutex_unlock(&ph->env->lock_right_fork[ph->index]);
		}
		else
		{
			pthread_mutex_unlock(&ph->env->lock_left_fork[ph->index]);
			pthread_mutex_unlock(&ph->env->lock_right_fork[(ph->index + 1) % ph->env->philo_nbr]);
		}
		// pthread_mutex_unlock(&ph->env->lock_right_fork[ph->index]);
		printf("%llu %d is sleeping\n", get_time(), ph->index + 1);
		usleep(ph->time_to_sleep * 10);
		printf("%llu %d is thinking\n", get_time(), ph->index + 1);
		// if (ph->env->philo_nbr % 2 == 0)
		// 	usleep((ph->time_to_die - (ph->time_to_eat + ph->time_to_sleep)) * 10);
		// else
		// 	usleep((ph->time_to_die - (ph->time_to_eat + ph->time_to_sleep)) * 5);
		i++;
	}
	return NULL;
}

int join_threads(t_env *env, int result, pthread_t	*t)
{
	int i = 0;

	if (result == 0)
	{
		i = 0;
		while (i < env->philo_nbr)
		{
			if(pthread_join(t[i], NULL) != 0)
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
	pthread_t	t[200];
	int			result = 0;

	int i = 0;
	pthread_mutex_init(&env->mutex, NULL);
	env->lock_left_fork = malloc(env->philo_nbr * sizeof(pthread_mutex_t));
	env->lock_right_fork = malloc(env->philo_nbr * sizeof(pthread_mutex_t));
	while (i < env->philo_nbr)
	{
		pthread_mutex_init(&env->lock_left_fork[i], NULL);
		pthread_mutex_init(&env->lock_right_fork[i], NULL);
		if (pthread_create(&t[i], NULL, schedule_action, (void *)&env->ph[i]) != 0)
		{
			result = env->ph[i].index;
			break;
		}
		i++;
	}
	pthread_mutex_init(&env->lock_left_fork[i], NULL);
	pthread_mutex_init(&env->lock_right_fork[i], NULL);
	join_threads(env, result, t);
	pthread_mutex_destroy(&env->mutex);
	while (i != -1)
	{
		pthread_mutex_destroy(&env->lock_left_fork[i]);
		pthread_mutex_destroy(&env->lock_right_fork[i]);
		i--;
	}
	free(env->lock_left_fork);
	// free(env->lock_right_fork);
	return result;
}
