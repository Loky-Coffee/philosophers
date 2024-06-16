/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_fork.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 23:02:57 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/16 20:51:13 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	give_fork_back(t_philo *ph)
{
	pthread_mutex_lock(&ph->env->free_fork);
	if (ph->index == 1)
	{
		pthread_mutex_unlock(&ph->env->l_fork[ph->env->philo_nbr]);
		pthread_mutex_unlock(&ph->env->l_fork[ph->index]);
	}
	else if (ph && ph->env)
	{
		pthread_mutex_unlock(&ph->env->l_fork[ph->index - 1]);
		pthread_mutex_unlock(&ph->env->l_fork[ph->index]);
	}
	pthread_mutex_unlock(&ph->env->free_fork);
}

void	try_take_fork(t_philo *ph)
{
	pthread_mutex_lock(&ph->env->lock_fork);
	if (death_check(ph->env) != true)
	{
		if (ph->env->philo_nbr == 1)
		{
			pthread_mutex_lock (&ph->env->l_fork[ph->index]);
			print_msg (ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
			ft_sleep (ph->time_to_die * 1200);
		}
		else if (ph->index == 1)
		{
			pthread_mutex_lock (&ph->env->l_fork[ph->index]);
			print_msg (ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
			pthread_mutex_lock (&ph->env->l_fork[ph->env->philo_nbr]);
			print_msg (ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
		}
		else
		{
			pthread_mutex_lock (&ph->env->l_fork[ph->index]);
			print_msg (ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
			pthread_mutex_lock (&ph->env->l_fork[ph->index - 1]);
			print_msg (ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
		}
	}
	pthread_mutex_unlock(&ph->env->lock_fork);
}

// void give_fork_back(t_philo *ph)
// {
//     pthread_mutex_lock(&ph->env->free_fork);
//     if (ph->index == ph->env->philo_nbr)
//     {
//         pthread_mutex_unlock(&ph->env->l_fork[ph->index]);
//         pthread_mutex_unlock(&ph->env->l_fork[1]);
//     }
//     else
//     {
//         pthread_mutex_unlock(&ph->env->l_fork[ph->index]);
//         pthread_mutex_unlock(&ph->env->l_fork[ph->index + 1]);
//     }
//     pthread_mutex_unlock(&ph->env->free_fork);
// }

// void try_take_fork(t_philo *ph)
// {
//     if (death_check(ph->env) != true)
//     {
//         if (ph->env->philo_nbr == 1)
//         {
//             pthread_mutex_lock(&ph->env->l_fork[ph->index]);
//             print_msg(ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
//             ft_sleep(ph->time_to_die * 1200);
//         }
//         else
//         {
//             if (ph->index == ph->env->philo_nbr)
//             {
//                 pthread_mutex_lock(&ph->env->l_fork[1]);
//                 print_msg(ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
//                 pthread_mutex_lock(&ph->env->l_fork[ph->index]);
//                 print_msg(ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
//             }
//             else
//             {
//                 pthread_mutex_lock(&ph->env->l_fork[ph->index]);
//                 print_msg(ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
//                 pthread_mutex_lock(&ph->env->l_fork[ph->index + 1]);
//                 print_msg(ph->env, ph->index, "has taken a fork", E_TAKE_FORK);
//             }
//         }
//     }
// }
