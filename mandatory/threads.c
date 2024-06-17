/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 23:03:23 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/17 01:00:58 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	join_threads(t_env *env, pthread_t	*philo_thread)
{
	int	i;

	i = 1;
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
	int	i;

	i = 1;
	pthread_mutex_init(&env->lock_fork, NULL);
	pthread_mutex_init(&env->free_fork, NULL);
	pthread_mutex_init(&env->death_lock, NULL);
	pthread_mutex_init(&env->print_lock, NULL);
	pthread_mutex_init(&env->lock_s_t, NULL);
	pthread_mutex_init(&env->start_sim_lock, NULL);
	while (i <= env->philo_nbr)
	{
		pthread_mutex_init(&env->ph[i].lock_last_time_eat, NULL);
		pthread_mutex_init(&env->ph[i].lock_philo_meals, NULL);
		pthread_mutex_init(&env->l_fork[i], NULL);
		i++;
	}
}

int	init_threads(t_env *env)
{
	pthread_t	*philo_thread;
	int			i;

	i = 1;
	env->l_fork = ft_calloc(env->philo_nbr, sizeof(pthread_mutex_t));
	philo_thread = ft_calloc(env->philo_nbr, sizeof(pthread_t));
	if (env->l_fork == NULL || philo_thread == NULL)
		return (1);
	init_mutex(env);
	while (i <= env->philo_nbr)
	{
		if (pthread_create(&philo_thread[i], NULL, schedule_action, (void *)&env->ph[i]) != 0)
			return (1);
		i++;
	}
	if (env->s_t == 0)
		env->s_t = get_time();
	i = 1;
	while (i <= env->philo_nbr)
	{
		pthread_mutex_lock(&env->ph[i].lock_last_time_eat);
		env->ph[i].last_eat_time = env->s_t;
		pthread_mutex_unlock(&env->ph[i].lock_last_time_eat);
		i++;
	}
	pthread_mutex_lock(&env->start_sim_lock);
	env->start_sim = true;
	pthread_mutex_unlock(&env->start_sim_lock);
	check_philo_death(env);
	if (join_threads(env, philo_thread) == 1)
		return (1);
	terminate(env, --i);
	return (0);
}
