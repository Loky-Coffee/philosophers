/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 23:03:23 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/15 22:40:19 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	join_threads(t_env *env, pthread_t	*philo_thread)
{
	int	i;

	i = 0;
	while (i <= env->philo_nbr)
	{
		if (pthread_join(philo_thread[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

void	init_mutex(t_env *env)
{
	pthread_mutex_init(&env->lock_fork, NULL);
	pthread_mutex_init(&env->free_fork, NULL);
	pthread_mutex_init(&env->death_lock, NULL);
}

int	init_threads(t_env *env)
{
	pthread_t	*philo_thread;
	pthread_t	die_thread;
	int			i;

	i = 1;
	init_mutex(env);
	env->l_fork = ft_calloc(env->philo_nbr, sizeof(pthread_mutex_t));
	philo_thread = ft_calloc(env->philo_nbr, sizeof(pthread_t));
	if (pthread_create(&die_thread, NULL, check_philo_death, (void *)env) != 0)
		return (1);
	while (i <= env->philo_nbr)
	{
		pthread_mutex_init(&env->ph[i].lock_last_time_eat, NULL);
		pthread_mutex_init(&env->l_fork[i], NULL);
		if (pthread_create(&philo_thread[i], NULL, \
		schedule_action, (void *)&env->ph[i]) != 0)
			return (1);
		pthread_mutex_lock(&env->ph[i].lock_last_time_eat);
		env->ph[i].last_eat_time = get_time();
		pthread_mutex_unlock(&env->ph[i].lock_last_time_eat);
		i++;
	}
	pthread_join(die_thread, NULL);
	if (join_threads(env, philo_thread) == 1)
		return (1);
	terminate(env, i);
	return (0);
}