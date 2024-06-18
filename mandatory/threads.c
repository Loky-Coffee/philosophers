/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 23:03:23 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/18 04:21:34 by aalatzas         ###   ########.fr       */
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
	while (i <= env->philo_nbr)
	{
		pthread_mutex_init(&env->l_fork[i], NULL);
		pthread_mutex_init(&env->ph[i].lock_last_time_eat, NULL);
		pthread_mutex_init(&env->ph[i].lock_philo_meals, NULL);
		i++;
	}
	pthread_mutex_init(&env->death_lock, NULL);
	pthread_mutex_init(&env->print_lock, NULL);
	pthread_mutex_init(&env->start_sim_lock, NULL);
}

void	set_first_time(t_env *env)
{
	int	i;

	i = 1;
	if (env->s_t == 0)
		env->s_t = get_time();
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
}

void	sync_threads(t_philo *ph)
{
	pthread_mutex_lock(&ph->env->start_sim_lock);
	while (ph->env->start_sim == false && ph->env->philo_nbr > 1)
	{
		if (ph->env->start_sim == true)
			break ;
		pthread_mutex_unlock(&ph->env->start_sim_lock);
		pthread_mutex_lock(&ph->lock_last_time_eat);
		ph->last_eat_time = get_time();
		pthread_mutex_unlock(&ph->lock_last_time_eat);
		usleep(1000);
		pthread_mutex_lock(&ph->env->start_sim_lock);
	}
	pthread_mutex_unlock(&ph->env->start_sim_lock);
	if (ph->index % 2 == 0 && ph->env->philo_nbr > 1)
		ft_sleep(ph->time_to_eat / 2);
}

int	init_threads(t_env *env)
{
	pthread_t	*philo_thread;
	int			i;

	i = 1;
	env->l_fork = ft_calloc(env->philo_nbr + 1, sizeof(pthread_mutex_t));
	philo_thread = ft_calloc(env->philo_nbr + 1, sizeof(pthread_t));
	if (env->l_fork == NULL || philo_thread == NULL)
		return (1);
	init_mutex(env);
	while (i <= env->philo_nbr)
	{
		if (pthread_create(&philo_thread[i], NULL, \
		schedule_action, (void *)&env->ph[i]) != 0)
			return (1);
		i++;
	}
	i = 1;
	set_first_time(env);
	check_philo_death(env);
	if (join_threads(env, philo_thread) == 1)
		return (1);
	terminate(env, --i);
	free(philo_thread);
	return (0);
}
