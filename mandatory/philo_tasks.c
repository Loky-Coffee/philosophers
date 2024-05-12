/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/05/12 10:47:49 by aalatzas         ###   ########.fr       */
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
	pthread_mutex_lock(&ph->ph_mutex_left_fork);
	printf("philo [%d] is eating\n", ph->index);
	// if (pthread_mutex_lock(&ph->env->ph[right_fork].ph_mutex_left_fork) == 0)
	// 	return 1;
	pthread_mutex_unlock(&ph->ph_mutex_left_fork);
	return 0;
}

void *schedule_action(void *arg)
{
	t_philo *ph = (t_philo *)arg;
	while (ph->env->run && ph->times_philo_eat > 0)
	{
		int i = 0;
		if (ph->times_philo_eat <= 0 && ph->argv_5 != NULL)
			break;
		pthread_mutex_lock(&ph->env->mutex);
		while (i < 1000)
		{
			i++;
		}
		if (ph->argv_5 != 0)
			ph->times_philo_eat--;
		printf("philo [%d] is eating\n", i);
		printf("philo [%d] is eating\n", ph->index);
		pthread_mutex_unlock(&ph->env->mutex);
		if (ph->argv_5 != 0)
			ph->times_philo_eat--;
	}
	return NULL;
}

// void *schedule_action(void *arg)
// {
// 	t_philo *ph = (t_philo *)arg;
// 	while (ph->time_to_die != 0)
// 	{
// 		int i = 0;
// 		if (ph->times_philo_eat <= 0 && ph->argv_5 != NULL)
// 			break ;
// 		pthread_mutex_lock(&ph->ph_mutex_left_fork);
// 		// pthread_mutex_lock(&ph->ph_mutex_right_fork);
// 		// if (can_eat(ph))
// 		// {
// 		while (i < 1000)
// 		{
// 			i++;
// 		}
// 			if (ph->argv_5 != 0)
// 				ph->times_philo_eat--;
// 			printf("philo [%d] is eating\n", i);


// 			// usleep(ph->time_to_eat * 1000);
// 		// }
// 		// pthread_mutex_unlock(&ph->ph_mutex_right_fork);
// 		pthread_mutex_unlock(&ph->ph_mutex_left_fork);
// 		// usleep(ph->time_to_sleep * 1000);
// 	}
// 	return NULL;
// }

int join_threads(t_env *env, int result, pthread_t	*t)
{
	if (result == 0)
	{
		env->x = 0;
		while (env->x < env->i)
		{
			if(pthread_join(t[env->x], NULL) != 0)
			{
				printf("ERROR Join Thread: %d\n", env->ph[env->x].index);
				result = env->ph[env->x].index;
				break;
			}
			env->x++;
		}
	}
	return (result);
}

int init_threads(t_env *env)
{
	pthread_t	t[env->philo_nbr];
	int			result = 0;

	env->i = 0;
	pthread_mutex_init(&env->ph->env->mutex, NULL);
	while (env->i < env->philo_nbr)
	{
		if (pthread_create(&t[env->i], NULL, schedule_action, (void *)&env->ph[env->i]) != 0)
		{
			printf("ERROR Thread: %d\n", env->ph[env->i].index);
			result = env->ph[env->i].index;
			break;
		}
		env->i++;
	}
	env->i = 0;
	pthread_mutex_destroy(&env->ph->env->mutex);
	join_threads(env, result, t);
	return result;
}
