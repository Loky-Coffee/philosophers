/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_philo_die.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 04:43:41 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/18 05:38:11 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_meals(t_philo *ph)
{
	int	result;

	pthread_mutex_lock(&ph->lock_philo_meals);
	if (ph->env->min_meals != -1 && ph->env->philo_nbr > 1)
	{
		if (ph->philo_meals >= ph->env->min_meals)
			result = 1;
		else
			result = 2;
	}
	else
		result = 0;
	pthread_mutex_unlock(&ph->lock_philo_meals);
	return (result);
}

int	death_check(t_env *env)
{
	bool	death;

	pthread_mutex_lock (&env->death_lock);
	death = env->death;
	pthread_mutex_unlock (&env->death_lock);
	return (death);
}

void	one_philo_die(t_env *env, int i)
{
	pthread_mutex_lock (&env->death_lock);
	env->death = true;
	pthread_mutex_unlock (&env->death_lock);
	ft_sleep (env->ph[i].time_to_die * 1000);
	print_msg (env, env->ph[i].index, "died", E_DIE);
}

void	check_philo_death(t_env *env)
{
	int					i;
	unsigned long long	last_time;

	i = 1;
	while (death_check(env) != true && check_meals(&env->ph[i]) != 1)
	{
		if (env->philo_nbr == 1)
			one_philo_die(env, i);
		while (i < env->philo_nbr)
		{
			pthread_mutex_lock(&env->ph[i].lock_last_time_eat);
			last_time = env->ph[i].last_eat_time;
			pthread_mutex_unlock(&env->ph[i].lock_last_time_eat);
			if (get_time() - last_time >= env->ph[i].time_to_die)
			{
				pthread_mutex_lock(&env->death_lock);
				env->death = true;
				pthread_mutex_unlock(&env->death_lock);
				print_msg(env, env->ph[i].index, "died", E_DIE);
				break ;
			}
			i++;
		}
		i = 1;
	}
}
