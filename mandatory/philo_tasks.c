/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/16 01:27:37 by aalatzas         ###   ########.fr       */
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
	if (death == false)
	{
		if (env->s_t == 0)
			env->s_t = get_time();
		time = get_time() - env->s_t;
		if (state == E_TAKE_FORK)
			printf(Y"%llu %d %s%s\n"R, time, index, msg, TAKE_FORK);
		else if (state == E_EAT)
			printf(GREEN"%llu %d %s%s\n"R, time, index, msg, EAT);
		else if (state == E_SLEEP)
			printf(MAGENTA"%llu %d %s%s\n"R, time, index, msg, SLEEP);
		else if (state == E_THINKING)
			printf(BLUE"%llu %d %s%s\n"R, time, index, msg, THINKING);
		else if (state == E_DIE)
			printf(RED"%llu %d %s%s\n"R, time, index, msg, DIE);
	}
	pthread_mutex_unlock(&env->print_lock);
	return (time);
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
int last_eat_time_check(t_philo *ph)
{
	pthread_mutex_lock(&ph->lock_last_time_eat);
	if (ph->last_eat_time == 0)
	{
		pthread_mutex_unlock(&ph->lock_last_time_eat);
		return (1);
	}
	pthread_mutex_unlock(&ph->lock_last_time_eat);
	return (0);
}
unsigned long long last_time(t_philo *ph)
{
	unsigned long long last_time;

	pthread_mutex_lock(&ph->lock_last_time_eat);
	last_time = ph->last_eat_time;
	pthread_mutex_unlock(&ph->lock_last_time_eat);
	return (last_time);
}


// int	philo_is_death(t_env *env, int i)
// {

// 	return (1);
// }

// int check_min_meals(t_philo *ph)
// {
// 	int meals_buffer;
// 	pthread_mutex_lock(&ph->lock_philo_meals);
// 	meals_buffer = ph->philo_meals;
// 	pthread_mutex_unlock(&ph->lock_philo_meals);
// 	return (meals_buffer);
// }

void	*check_philo_death(void *arg)
{
	t_env	*env;
	int		i;

	env = (t_env *)arg;
	i = 1;
	usleep(5000);
	while (death_check(env) != true)
	{
		if (i > env->philo_nbr)
			i = 1;
		if (env->philo_nbr == 1)
		{
			print_msg(env, env->ph[i].index,"died", E_DIE);
			pthread_mutex_lock(&env->death_lock);
			env->death = true;
			pthread_mutex_unlock(&env->death_lock);
			break ;
		}
		// printf("get_time: %llu\n", get_time());
		// printf("last_time: %llu\n", last_time(&env->ph[i]));
		if ((((get_time() - last_time(&env->ph[i])) >= \
		(unsigned long long)env->ph[i].time_to_die) \
		&& last_eat_time_check(&env->ph[i]) != 0))
		{
			print_msg(env, env->ph[i].index,"died", E_DIE);
			pthread_mutex_lock(&env->death_lock);
			env->death = true;
			pthread_mutex_unlock(&env->death_lock);
			return (0);
		}
		i++;
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

	ph = (t_philo *)arg;
	if (ph->index % 2 == 0)
		ft_sleep(ph->time_to_eat / 2);
	while (death_check(ph->env) != true)
	{
		// schedule_action2(ph);
		try_take_fork(ph);
		if (ph->env->philo_nbr > 1)
		{
			pthread_mutex_lock(&ph->lock_last_time_eat);
			ph->last_eat_time = print_msg(ph->env, ph->index,"is eating", E_EAT);
			pthread_mutex_unlock(&ph->lock_last_time_eat);
		}
		ft_sleep(ph->time_to_eat);
		give_fork_back(ph);
		// printf("philo_meals: %d\n", ph->philo_meals);
		// printf("min_meals: %d\n", ph->env->min_meals);
		if (ph->env->min_meals != - 1 && ph->env->philo_nbr > 1)
		{
			if (ph->philo_meals >= ph->env->min_meals)
				break ;
			pthread_mutex_lock(&ph->lock_philo_meals);
			ph->philo_meals++;
			pthread_mutex_unlock(&ph->lock_philo_meals);
			pthread_mutex_lock(&ph->lock_last_time_eat);
			ph->last_eat_time = 0;
			pthread_mutex_unlock(&ph->lock_last_time_eat);
			give_fork_back(ph);
			print_msg(ph->env, ph->index,"is sleeping", E_SLEEP);
		}
		sleep_or_think(ph);
	}
	return (NULL);
}

// void	schedule_action2(t_philo *ph)
// {
// 	try_take_fork(ph);
// 	if (ph->env->philo_nbr > 1)
// 	{
// 		pthread_mutex_lock(&ph->lock_last_time_eat);
// 		ph->last_eat_time = print_msg(ph->env, ph->index,"is eating", E_EAT);
// 		pthread_mutex_unlock(&ph->lock_last_time_eat);
// 	}
// 	ft_sleep(ph->time_to_eat);
// 	give_fork_back(ph);
// }
