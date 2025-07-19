/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 21:25:19 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/19 21:30:29 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_cleanup_mutex(t_supervisor *sv, int n)
{
	pthread_mutex_destroy(&sv->shared.lock_start);
	pthread_mutex_destroy(&sv->shared.lock_print);
	while (--n > -1)
	{
		pthread_mutex_destroy(&sv->philos[n].fork);
		pthread_mutex_destroy(&sv->philos[n].lock_meal);
	}
	free(sv->philos);
}

void	ft_declare_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->shared->lock_print);
	printf("%ld %d died\n", philo->meal_deadline);
	philo->shared->philo_died = 1;
	pthread_mutex_unlock(&philo->shared->lock_print);
}

int	ft_wait_start_time(long start_time)
{
	long	now;

	if (start_time == -1)
		return (-1);
	while (1)
	{
		now = get_time_ms();
		if (now >= start_time)
			break ;
		if (start_time - now > 2)
			usleep(500);
		else
			usleep(50);
	}
	return (0);
}

long	ft_get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
