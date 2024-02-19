/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/02/16 18:06:41 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_think(void *env)
{
	t_env	e;

	e = *(t_env *)env;
	return (NULL);
}

void	*philo_eat(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
		static int y;
		printf("y: %d\n", y);
		y++;

	return (NULL);
}

void	*philo_sleep(void *env)
{
	t_env	e;

	e = *(t_env *)env;
	return (NULL);
}

void	*philo_die(void *env)
{
	t_env	e;

	e = *(t_env *)env;
	return (NULL);
}

void	*schedule_action(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	pthread_mutex_init(&ph->ph_mutex_print_eat, NULL);
	pthread_mutex_lock(&ph->ph_mutex);
	while (ph->time_to_die != 0)
	{
		if (ph->times_philo_eat <= 0 && ph->argv_5 != NULL)
			break;
		pthread_mutex_lock(&ph->ph_mutex_print_eat);
		philo_eat(&ph);
		pthread_mutex_unlock(&ph->ph_mutex_print_eat);
		if (ph->argv_5 != 0)
			ph->times_philo_eat--;
	}
	pthread_mutex_destroy(&ph->ph_mutex_print_eat);
	pthread_mutex_unlock(&ph->ph_mutex);
	return (NULL);
}

int	greate_thread(t_env *env)
{
	pthread_t	t[env->philo_nbr];
	pthread_mutex_init(&env->ph->ph_mutex, NULL);
	env->i = 0;
	while (env->i < env->philo_nbr)
	{
		if (pthread_create (&t[env->i], NULL, schedule_action, (void *)&env->ph[env->i]) != 0)
			return (printf("ERROR Thread: %d\n", env->ph[env->i].index), env->ph->index);
		env->i++;
	}
	env->x = 0;
	while (env->x < env->philo_nbr)
	{
		if(pthread_join(t[env->x], NULL) != 0)
			return(printf("ERROR Join Thread: %d\n", env->ph[env->i].index), env->ph->index);
		env->x++;
	}
	pthread_mutex_destroy(&env->ph->ph_mutex);
	return (0);
}
