/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalatzas <aalatzas@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 01:53:42 by aalatzas          #+#    #+#             */
/*   Updated: 2024/06/18 03:51:18 by aalatzas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	minus;

	i = 0;
	minus = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		minus = minus * -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= 48 && *str <= 57)
	{
		i = i * 10 + *str - 48;
		str++;
	}
	return (i * minus);
}

void	is_all_int(int argc, char **argv)
{
	int	x;
	int	y;

	x = 1;
	y = 0;
	while (x <= argc - 1)
	{
		while (argv[x][y] != '\0')
		{
			if (argv[x][y] < '0' || argv[x][y] > '9')
			{
				printf(\
				RED"ERROR: Only digits allower als aqument! %s\n"R, CRY);
				exit(EXIT_FAILURE);
			}
			y++;
		}
		y = 0;
		x++;
	}
}

unsigned long long int	get_time(void)
{
	struct timeval			tv;
	unsigned long long int	ms;
	unsigned long long int	sec;
	unsigned long long int	usec;

	gettimeofday(&tv, NULL);
	sec = (unsigned long long int)tv.tv_sec;
	usec = (unsigned long long int)tv.tv_usec;
	ms = sec * 1000ULL + usec / 1000ULL;
	return (ms);
}

void	*ft_calloc(size_t count, size_t size)
{
	size_t		i;
	char		*str;

	i = 0;
	if (size != 0 && count > SIZE_MAX / size)
		return (NULL);
	str = (char *)malloc(count * size);
	if (str == NULL)
		return (NULL);
	while (i < count * size)
	{
		str[i] = 0;
		i++;
	}
	return (str);
}

void	ft_sleep(long long ms)
{
	long long	start;

	start = (long long)get_time();
	while ((long long)(get_time() - start) < (ms / 1000))
		usleep(300);
}
