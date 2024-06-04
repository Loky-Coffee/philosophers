/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/03 22:01:43 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/04 19:18:15 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_values(char **argv, t_env *env)
{
	int i;

	i = 0;
	while (i < env->philo_nbr)
	{
		env->ph[i].time_to_die = (ft_atoi(argv[2]) * 100);
		env->ph[i].time_to_eat = (ft_atoi(argv[3]) * 100);
		env->ph[i].time_to_sleep = (ft_atoi(argv[4]) * 100);
		env->ph[i].philo_meals = 1;
		env->ph[i].index = i;
		env->ph[i].env = env;
		i++;
	}
	if (argv[5] != NULL)
		env->min_meals = ft_atoi(argv[5]);
}

int	main(int argc, char **argv)
{
	t_env	env;
	// t_philo *philo_array;
	if (argc < 5 || argc > 6)
		return (printf(RED"ERROR: Wrong number of aqument!%s\n"RESET, CRY), 1);
	env = (t_env){0};
	env.philo_nbr = ft_atoi(argv[1]);
	if (argv[5] != NULL)
		env.min_meals = ft_atoi(argv[5]);
	if (argc >= 5 && argc <= 6)
		is_all_int (argc, argv);
	env.ph = malloc(env.philo_nbr * sizeof(t_philo));
	if (env.ph == NULL)
		return (printf("ERROR: Memory allocation failed!\n"), 1);
	init_values(argv, &env);
	init_threads(&env);
	free(env.ph);
	return (0);
}
