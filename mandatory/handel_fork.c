/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_fork.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 23:02:57 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/18 04:03:16 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	give_fork_back(t_philo *ph)
{
	pthread_mutex_unlock(&ph->env->l_fork[ph->index]);
	pthread_mutex_unlock(&ph->env->l_fork \
	[(ph->index % ph->env->philo_nbr) + 1]);
}

void	try_take_fork(t_philo *ph)
{
	if (ph->env->philo_nbr == 1)
	{
		print_msg(ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
		ft_sleep(ph->time_to_die * 1200);
	}
	else
	{
		if (ph->index % 2 == 0)
		{
			pthread_mutex_lock(&ph->env->l_fork[ph->index]);
			print_msg(ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
			pthread_mutex_lock(&ph->env->l_fork \
			[(ph->index % ph->env->philo_nbr) + 1]);
			print_msg(ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
		}
		else
		{
			pthread_mutex_lock(&ph->env->l_fork[ph->index]);
			print_msg(ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
			pthread_mutex_lock(&ph->env->l_fork \
			[(ph->index % ph->env->philo_nbr) + 1]);
			print_msg(ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
		}
	}
}
