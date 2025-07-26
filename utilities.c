/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 13:56:35 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/26 16:06:43 by fmontero         ###   ########.fr       */
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
		pthread_mutex_destroy(&sv->philos[n].lock_deadline);
	}
	free(sv->philos);
}

long	ft_get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_declare_death(t_philo *philo)
{
	long	timeofdeath;

	pthread_mutex_lock(&philo->lock_deadline);
	timeofdeath = philo->deadline - philo->shared->start_time;
	pthread_mutex_unlock(&philo->lock_deadline);
	pthread_mutex_lock(&philo->shared->lock_print);
	if (philo->shared->philo_died == 0)
	{
		printf("%ld %d died\n", timeofdeath, philo->id);
		philo->shared->philo_died = 1;
	}
	pthread_mutex_unlock(&philo->shared->lock_print);
}

int	ft_wait_start_time(long start_time)
{
	long	now;

	if (start_time == -1)
		return (ERR_LAUNCH_PHILO);
	while (1)
	{
		now = ft_get_time_ms();
		if (now >= start_time)
			break ;
		if (start_time - now > 2)
			usleep(500);
		else
			usleep(50);
	}
	return (0);
}

void	ft_mutex_store_l(long *rd, long*wr, pthread_mutex_t *lock)
{
	pthread_mutex_lock(lock);
	*wr = *rd;
	pthread_mutex_unlock(lock);
}
