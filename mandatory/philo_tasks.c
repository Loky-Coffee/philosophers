/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 02:04:21 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/15 23:59:33 by aalatzas         ###   ########.fr       */
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
		time = get_time() - env->s_t;
		if (state == E_TAKE_FORK)
			printf(Y"%llu %d %s %s\n"R, time, index, msg, TAKE_FORK);
		else if (state == E_EAT)
			printf(GREEN"%llu %d %s %s\n"R, time, index, msg, EAT);
		else if (state == E_SLEEP)
			printf(MAGENTA"%llu %d %s %s\n"R, time, index, msg, SLEEP);
		else if (state == E_THINKING)
			printf(BLUE"%llu %d %s %s\n"R, time, index, msg, THINKING);
		else if (state == E_DIE)
			printf(RED"%llu %d %s %s\n"R, time, index, msg, DIE);
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


int	philo_is_death(t_env *env, int i)
{
	usleep(100);
	printf("###############");
	if (env->philo_nbr == 1)
	{
		// ft_sleep(env->ph[i].time_to_die * 1000);
		printf(RED"%llu %d died %s\n"R, get_time() - env->s_t, i, DIE);
		exit(0);
	}
	if ((((get_time() - last_time(&env->ph[i])) >= \
	(unsigned long long)env->ph[i].time_to_die) \
	&& last_eat_time_check(&env->ph[i]) != 0))
	{
		print_msg(env, env->ph[i].index,"died", E_DIE);
		pthread_mutex_lock(&env->death_lock);
		env->death = true;
		pthread_mutex_unlock(&env->death_lock);
		// printf(RED"%llu %d died %s\n"R, get_time() - env->s_t, i, DIE);
		// printf(RED"%llu %d died %s\n"R, get_time() - env->s_t, i, DIE);
		return (0);
	}
	return (1);
}

int check_min_meals(t_philo *ph)
{
	int meals_buffer;
	pthread_mutex_lock(&ph->lock_philo_meals);
	meals_buffer = ph->philo_meals;
	pthread_mutex_unlock(&ph->lock_philo_meals);
	return (meals_buffer);
}

void	*check_philo_death(void *arg)
{
	t_env	*env;
	int		i;

	env = (t_env *)arg;
	i = 1;
	usleep(5000);
	while (death_check(env) != true && env->min_meals > 0)
	{
		if (i > env->philo_nbr)
			i = 1;
		if (env->philo_nbr && i >= env->philo_nbr)
		{
			i = 1;
			while (env->min_meals > 0 && \
			check_min_meals(&env->ph[i]) >= env->min_meals)
			{
				if (i >= env->philo_nbr)
					exit(0);
				i++;
			}
			i = 1;
		}
		printf("###############");
		if(philo_is_death(env, i) == 0)
			return (NULL);
		i++;
	}
	return (NULL);
}

void	sleep_or_think(t_philo *ph)
{
	if (death_check(ph->env) != true && ph->env->philo_nbr > 1)
		print_msg(ph->env, ph->index,"is sleeping", E_SLEEP);
		// printf(MAGENTA"%llu %d is sleeping %s\n"R, \
		// get_time() - ph->env->s_t, ph->index, SLEEP);
	ft_sleep(ph->time_to_sleep);
	if (death_check(ph->env) != true && ph->env->philo_nbr > 1)
		print_msg(ph->env, ph->index,"is thinking", E_THINKING);
		// printf(BLUE"%llu %d is thinking %s\n"R, \
		// get_time() - ph->env->s_t, ph->index, THINKING);
}

void	*schedule_action(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	if (ph->index % 2 == 0)
		ft_sleep(ph->time_to_eat / 2);
	while (death_check(ph->env) != true)
	{
		schedule_action2(ph);
		if (ph->env->min_meals > 0 && ph->env->philo_nbr > 1)
		{
			pthread_mutex_lock(&ph->lock_philo_meals);
			ph->philo_meals++;
			pthread_mutex_unlock(&ph->lock_philo_meals);
			if (ph->philo_meals >= ph->env->min_meals && death_check(ph->env) != true)
			{
				pthread_mutex_lock(&ph->lock_last_time_eat);
				ph->last_eat_time = 0;
				pthread_mutex_unlock(&ph->lock_last_time_eat);
				give_fork_back(ph);
				print_msg(ph->env, ph->index,"is sleeping", E_SLEEP);
				// printf(MAGENTA"%llu %d is sleeping %s\n"R, \
				// get_time() - ph->env->s_t, ph->index, SLEEP);
				break ;
			}
		}
		sleep_or_think(ph);
	}
	return (NULL);
}

void	schedule_action2(t_philo *ph)
{
	if (death_check(ph->env) != true)
		try_take_fork(ph);
	// ph->last_eat_time = get_time();
	if (death_check(ph->env) != true && ph->env->philo_nbr > 1)
	{
		pthread_mutex_lock(&ph->lock_last_time_eat);
		ph->last_eat_time = print_msg(ph->env, ph->index,"is eating", E_EAT);
		pthread_mutex_unlock(&ph->lock_last_time_eat);
	}
		// printf(GREEN"%llu %d is eating %s\n"R, get_time() - ph->env->s_t, ph->index, EAT);
	ft_sleep(ph->time_to_eat);
	give_fork_back(ph);
}
