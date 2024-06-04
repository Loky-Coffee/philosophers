/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/04 19:31:19 by aalatzas         ###   ########.fr       */
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
	int i = 0;
	// printf("-------------------------\n");
	while (true)
	{
		pthread_mutex_lock(&ph->env->mutex);
		// printf("philo[%i] ph->env->min_meals [%d] ph->philo_meals [%d]\n", ph->index, ph->env->min_meals, ph->philo_meals);
		printf("philo [%d] is eating\n", ph->index);
		if (ph->env->min_meals > 0)
		{
			if (ph->philo_meals == ph->env->min_meals)
			{
				printf("Philo ->>[%i] #####break###\n", ph->index);
				pthread_mutex_unlock(&ph->env->mutex);
				break;
			}
			ph->philo_meals++;
		}
		pthread_mutex_unlock(&ph->env->mutex);
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
		while (i < 200)
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
	while (i < env->philo_nbr)
	{
		if (pthread_create(&t[i], NULL, schedule_action, (void *)&env->ph[i]) != 0)
		{
			result = env->ph[i].index;
			break;
		}
		i++;
	}
	join_threads(env, result, t);
	pthread_mutex_destroy(&env->mutex);
	return result;
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