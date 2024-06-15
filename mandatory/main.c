/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:01:43 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/15 19:52:44 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	terminate(t_env *env, int i)
{
	while (i != -1)
	{
		pthread_mutex_destroy(&env->l_fork[i]);
		pthread_mutex_destroy(&env->ph[i].lock_last_time_eat);
		i--;
	}
	free(env->l_fork);
	pthread_mutex_destroy(&env->lock_fork);
	pthread_mutex_destroy(&env->free_fork);
	pthread_mutex_destroy(&env->death_lock);
}

int	init_values(char **argv, t_env *env)
{
	int	i;

	i = 1;
	env->philo_nbr = ft_atoi(argv[1]);
	env->ph = calloc(env->philo_nbr + 1, sizeof(t_philo));
	if (env->ph == NULL)
		return (printf("ERROR: Memory allocation failed!\n"), 1);
	while (i <= env->philo_nbr)
	{
		env->ph[i].time_to_die = (ft_atoi(argv[2]));
		env->ph[i].time_to_eat = (ft_atoi(argv[3]) * 1000);
		env->ph[i].time_to_sleep = (ft_atoi(argv[4]) * 1000);
		env->ph[i].philo_meals = 0;
		env->ph[i].index = i;
		env->ph[i].env = env;
		i++;
	}
	if (argv[5] != NULL)
		env->min_meals = ft_atoi(argv[5]);
	env->death = false;
	env->s_t = get_time();
	return (0);
}

int	main(int argc, char **argv)
{
	t_env	env;

	env = (t_env){0};
	if (argc < 5 || argc > 6)
		return (printf(RED"ERROR: Wrong number of aqument!%s\n"R, CRY), 1);
	if (argv[5] && argv[5][0] == '0')
		return (printf \
		(RED"ERROR: The philosophers must be allowed to eat at least once!%s\n" \
		R, CRY), 1);
	if (argc >= 5 && argc <= 6)
		is_all_int (argc, argv);
	if (init_values(argv, &env) == 1)
		return (1);
	if (init_threads(&env) == 1)
		return (1);
	free(env.ph);
	return (0);
}
