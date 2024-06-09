/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/09 16:39:29 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	schedule_action2(t_philo *ph);

void	philo_is_death(t_env *env, int i)
{
	if (env->philo_nbr == 1)
	{
		ft_sleep(env->ph[i].time_to_die * 1000);
		printf(RED"%llu %d died %s\n"R, get_time() - env->s_t, i, DIE);
		exit(0);
	}
	usleep(100);
	if ((((get_time() - env->ph[i].last_eat_time) >= \
	(unsigned long long)env->ph[i].time_to_die) \
	&& env->ph[i].last_eat_time != 0))
	{
		env->death = true;
		usleep(100);
		printf(RED"%llu %d died %s\n"R, get_time() - env->s_t, i, DIE);
		exit(0);
	}
}

void	*check_philo_death(void *arg)
{
	t_env	*env;
	int		i;

	env = (t_env *)arg;
	i = 1;
	usleep(5000);
	while (true)
	{
		if (env->philo_nbr && i >= env->philo_nbr)
		{
			i = 1;
			while (env->min_meals > 0 && \
			env->ph[i].philo_meals >= env->min_meals)
			{
				if (i >= env->philo_nbr)
					exit(0);
				i++;
			}
			i = 1;
		}
		philo_is_death(env, i);
		i++;
	}
	return (NULL);
}

void	sleep_or_think(t_philo *ph)
{
	if (ph->env->death != true && ph->env->philo_nbr > 1)
		printf(MAGENTA"%llu %d is sleeping %s\n"R, \
		get_time() - ph->env->s_t, ph->index, SLEEP);
	ft_sleep(ph->time_to_sleep);
	if (ph->env->death != true && ph->env->philo_nbr > 1)
		printf(BLUE"%llu %d is thinking %s\n"R, \
		get_time() - ph->env->s_t, ph->index, THINKING);
}

void	*schedule_action(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	if (ph->index % 2 == 0)
		ft_sleep(ph->time_to_eat / 2);
	while (ph->env->death != true)
	{
		schedule_action2(ph);
		if (ph->env->min_meals > 0 && ph->env->philo_nbr > 1)
		{
			ph->philo_meals++;
			if (ph->philo_meals >= ph->env->min_meals && ph->env->death != true)
			{
				ph->last_eat_time = 0;
				give_fork_back(ph);
				printf(MAGENTA"%llu %d is sleeping %s\n"R, \
				get_time() - ph->env->s_t, ph->index, SLEEP);
				break ;
			}
		}
		sleep_or_think(ph);
	}
	return (NULL);
}

void	schedule_action2(t_philo *ph)
{
	if (ph->env->death != true)
		try_take_fork(ph);
	pthread_mutex_lock(&ph->env->lock_last_time_eat);
	ph->last_eat_time = get_time();
	pthread_mutex_unlock(&ph->env->lock_last_time_eat);
	if (ph->env->death != true && ph->env->philo_nbr > 1)
		printf(GREEN"%llu %d is eating %s\n"R, \
		get_time() - ph->env->s_t, ph->index, EAT);
	ft_sleep(ph->time_to_eat);
	give_fork_back(ph);
}
