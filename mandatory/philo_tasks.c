/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/18 05:35:35 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long long	print_msg(t_env *env, int index, char *msg, t_state state)
{
	unsigned long long	time;

	time = 0;
	pthread_mutex_lock(&env->print_lock);
	if (death_check(env) != true)
	{
		time = get_time();
		if (state == E_TAKE_FORK)
			printf(Y"%llu %d %s%s\n"R, time - env->s_t, index, msg, TAKE_FORK);
		else if (state == E_EAT)
			printf(GREEN"%llu %d %s%s\n"R, time - env->s_t, index, msg, EAT);
		else if (state == E_SLEEP)
			printf(MAGENTA"%llu %d %s%s\n"R, \
			time - env->s_t, index, msg, SLEEP);
		else if (state == E_THINKING)
			printf(BLUE"%llu %d %s%s\n"R, \
			time - env->s_t, index, msg, THINKING);
	}
	else if (state == E_DIE)
		printf(RED"%llu %d %s%s\n"R, get_time() - env->s_t, index, msg, DIE);
	pthread_mutex_unlock(&env->print_lock);
	return (time);
}

void	sleep_or_think(t_philo *ph)
{
	print_msg (ph->env, ph->index, "is sleeping", E_SLEEP);
	ft_sleep (ph->time_to_sleep);
	print_msg (ph->env, ph->index, "is thinking", E_THINKING);
	usleep (50);
}

int	check_5te_aqument(t_philo *ph)
{
	if (ph->env->min_meals != -1 && ph->env->philo_nbr > 1)
	{
		pthread_mutex_lock(&ph->lock_philo_meals);
		ph->philo_meals++;
		pthread_mutex_unlock(&ph->lock_philo_meals);
		if (ph->philo_meals >= ph->env->min_meals)
			return (true);
	}
	return (false);
}

void	*schedule_action(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	sync_threads(ph);
	if (ph->env->philo_nbr == 1)
	{
		ft_sleep(ph->time_to_die / 2);
		try_take_fork(ph);
	}
	while (death_check(ph->env) != true)
	{
		try_take_fork(ph);
		pthread_mutex_lock(&ph->lock_last_time_eat);
		ph->last_eat_time = print_msg(ph->env, ph->index, "is eating", E_EAT);
		pthread_mutex_unlock(&ph->lock_last_time_eat);
		ft_sleep(ph->time_to_eat);
		give_fork_back(ph);
		if (check_5te_aqument(ph) == true)
			break ;
		sleep_or_think(ph);
	}
	return (NULL);
}
