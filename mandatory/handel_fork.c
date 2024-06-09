/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_fork.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 23:02:57 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/09 16:56:31 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	give_fork_back(t_philo *ph)
{
	pthread_mutex_lock(&ph->env->free_fork);
	if (ph->index == 1)
	{
		pthread_mutex_unlock(&ph->env->l_fork[ph->index]);
		pthread_mutex_unlock(&ph->env->l_fork[ph->env->philo_nbr]);
	}
	else
	{
		pthread_mutex_unlock(&ph->env->l_fork[ph->index]);
		pthread_mutex_unlock(&ph->env->l_fork[ph->index - 1]);
	}
	pthread_mutex_unlock(&ph->env->free_fork);
}

void	try_take_fork2(t_philo *ph)
{
	pthread_mutex_lock(&ph->env->l_fork[ph->index]);
	printf(Y"%llu %d has taken a fork %s\n"R, \
	get_time() - ph->env->s_t, ph->index, TAKE_FORK);
	pthread_mutex_lock(&ph->env->l_fork[ph->index - 1]);
	printf(Y"%llu %d has taken a fork %s\n"R, \
	get_time() - ph->env->s_t, ph->index, TAKE_FORK);
}

void	try_take_fork(t_philo *ph)
{
	pthread_mutex_lock(&ph->env->lock_fork);
	if (ph->env->philo_nbr == 1 && ph->env->death != true)
	{
		pthread_mutex_lock(&ph->env->l_fork[ph->index]);
		printf(Y"%llu %d has taken a fork %s\n"R, \
		get_time() - ph->env->s_t, ph->index, TAKE_FORK);
		ft_sleep(ph->time_to_die * 1000);
	}
	else if (ph->index == 1 && ph->env->death != true)
	{
		pthread_mutex_lock(&ph->env->l_fork[ph->index]);
		printf(Y"%llu %d has taken a fork %s\n"R, \
		get_time() - ph->env->s_t, ph->index, TAKE_FORK);
		pthread_mutex_lock(&ph->env->l_fork[ph->env->philo_nbr]);
		printf(Y"%llu %d has taken a fork %s\n"R, \
		get_time() - ph->env->s_t, ph->index, TAKE_FORK);
	}
	else if (ph->env->death != true)
	{
		try_take_fork2(ph);
	}
	pthread_mutex_unlock(&ph->env->lock_fork);
}
