/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/17 01:23:43 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long long print_msg(t_env *env, int index, char *msg,t_state state)
{
	bool death;
	unsigned long long time;

	time = 0;
	pthread_mutex_lock(&env->death_lock);
	death = env->death;
	pthread_mutex_unlock(&env->death_lock);
	pthread_mutex_lock(&env->print_lock);
	if (death_check(env) != true)
	{
		time = get_time();
		if (state == E_TAKE_FORK)
			printf(Y"%llu %d %s%s\n"R, time - env->s_t, index, msg, TAKE_FORK);
		else if (state == E_EAT)
			printf(GREEN"%llu %d %s%s\n"R, time - env->s_t, index, msg, EAT);
		else if (state == E_SLEEP)
			printf(MAGENTA"%llu %d %s%s\n"R, time - env->s_t, index, msg, SLEEP);
		else if (state == E_THINKING)
			printf(BLUE"%llu %d %s%s\n"R, time - env->s_t, index, msg, THINKING);
	}
	else if (state == E_DIE && death == true)
		printf(RED"%llu %d %s%s\n"R, get_time() - env->s_t, index, msg, DIE);
	pthread_mutex_unlock(&env->print_lock);
	return (time);
}

int check_meals(t_philo *ph)
{
	int result;
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

void	schedule_action2(t_philo *ph);

int death_check(t_env *env)
{
	pthread_mutex_lock(&env->death_lock);
	if (env->death == true)
	{
		pthread_mutex_unlock(&env->death_lock);
		return (1);
	}
	pthread_mutex_unlock(&env->death_lock);
	return (0);
}

void one_philo_die(t_env *env, int i)
{
	ft_sleep(env->ph[i].time_to_die * 1000);
	print_msg(env, env->ph[i].index,"died", E_DIE);
	pthread_mutex_lock(&env->death_lock);
	env->death = true;
	pthread_mutex_unlock(&env->death_lock);
}

void	*check_philo_death(void *arg)
{
	t_env	*env;
	int		i;
	unsigned long long last_time;
	unsigned long long curr_time;

	env = (t_env *)arg;
	i = 1;
	while (death_check(env) != true)
	{
		if (check_meals(&env->ph[i]) == 1)
			break ;
		pthread_mutex_lock(&env->ph[i].lock_last_time_eat);
		// last_time = get_last_eat_time(&env->ph[i]);
		last_time = env->ph[i].last_eat_time;
		curr_time = get_time();
		if (env->philo_nbr == 1)
			return (one_philo_die(env, i), NULL);
		// printf("last_time: %llu\n", last_time);
		// printf("curr_time: %llu\n", curr_time);
		// printf("curr_time - last_time: %llu\n", curr_time - last_time);
		if (curr_time - last_time >= (unsigned long long)env->ph[i].time_to_die)
		{
			pthread_mutex_lock(&env->death_lock);
			env->death = true;
			pthread_mutex_unlock(&env->death_lock);
			print_msg(env, env->ph[i].index, "died", E_DIE);
			pthread_mutex_unlock(&env->ph[i].lock_last_time_eat);
			return (0);
		}
		pthread_mutex_unlock(&env->ph[i].lock_last_time_eat);
		i++;
		if (i > env->philo_nbr)
			i = 1;
		usleep(100);
	}
	return (NULL);
}



void	sleep_or_think(t_philo *ph)
{
	if (death_check(ph->env) != true && ph->env->philo_nbr > 1)
		print_msg(ph->env, ph->index,"is sleeping", E_SLEEP);
	ft_sleep(ph->time_to_sleep);
	if (death_check(ph->env) != true && ph->env->philo_nbr > 1)
		print_msg(ph->env, ph->index,"is thinking", E_THINKING);
}

void	*schedule_action(void *arg)
{
	t_philo	*ph;
	// unsigned long long eat_time;

	ph = (t_philo *)arg;
	pthread_mutex_lock(&ph->env->start_sim_lock);
	while (ph->env->start_sim == false)
	{
		if (ph->env->start_sim == true)
			break ;
		pthread_mutex_unlock(&ph->env->start_sim_lock);
		pthread_mutex_lock(&ph->lock_last_time_eat);
		ph->last_eat_time = get_time();
		pthread_mutex_unlock(&ph->lock_last_time_eat);
		usleep(50);
		pthread_mutex_lock(&ph->env->start_sim_lock);
	}
	pthread_mutex_unlock(&ph->env->start_sim_lock);
	if (ph->index % 2 == 0)
		ft_sleep(ph->time_to_eat / 10);
	while (death_check(ph->env) != true)
	{
		try_take_fork(ph);
		pthread_mutex_lock(&ph->lock_last_time_eat);
		ph->last_eat_time = print_msg(ph->env, ph->index, "is eating", E_EAT);
		pthread_mutex_unlock(&ph->lock_last_time_eat);
		ft_sleep(ph->time_to_eat);
		give_fork_back(ph);
		if (ph->env->min_meals != -1 && ph->env->philo_nbr > 1)
		{
			pthread_mutex_lock(&ph->lock_philo_meals);
			ph->philo_meals++;
			pthread_mutex_unlock(&ph->lock_philo_meals);
			if (ph->philo_meals >= ph->env->min_meals)
				break ;
		}
		sleep_or_think(ph);
		// if (death_check(ph->env) == true)
		// 	break ;
	}
	return (NULL);
}
