/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 21:02:14 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/23 19:32:38 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void		ft_grab_forks(t_philo *philo);
static int		ft_eating(t_philo *philo);

void	*ft_philo_routine(void *args)
{
	t_philo		*self;
	long		timestamp;

	self = args;
	pthread_mutex_lock(&self->shared->lock_start);
	pthread_mutex_unlock(&self->shared->lock_start);
	if (ft_wait_start_time(self->shared->start_time) == -1)
		return (NULL);
	self->deadline = ft_get_time_ms() + self->shared->args.time_to_die;
	while (1)
	{
		timestamp = ft_get_time_ms() - self->shared->start_time;
		pthread_mutex_lock(&self->shared->lock_print);
		printf("%ld %d is thinking", timestamp, self->id);
		pthread_mutex_unlock(&self->shared->lock_print);
		ft_grab_forks(self);
		if (ft_eating(self) != 0)
			return (NULL);
		timestamp = ft_get_time_ms() - self->shared->start_time;
		pthread_mutex_lock(&self->shared->lock_print);
		printf("%ld %d is sleeping", timestamp, self->id);
		pthread_mutex_unlock(&self->shared->lock_print);
		usleep(self->shared->args.time_to_sleep);
	}
}

static void	ft_grab_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(philo->next_fork);
	}
	else
	{
		pthread_mutex_lock(philo->next_fork);
		pthread_mutex_lock(&philo->fork);
	}
}

static int	ft_eating(t_philo *philo)
{
	long	now;

	now = ft_get_time_ms();
	if (philo->deadline < now)
	{
		ft_declare_death(philo);
		return (PHILO_DIED);
	}
	ft_mutex_store_l(&(long){EATING}, &philo->deadline, &philo->lock_meal);
	pthread_mutex_lock(&philo->shared->lock_print);
	printf("%ld %d is eating", now, philo->id);
	pthread_mutex_unlock(&philo->shared->lock_print);
	usleep(philo->shared->args.time_to_eat);
	if (++philo->meals_eaten == philo->shared->args.meals_required)
	{
		ft_mutex_store_l(&(long){PHILO_MEALS_DONE},
			&philo->deadline, &philo->lock_meal);
		return (0);
	}
	now = ft_get_time_ms() + philo->shared->args.time_to_die;
	ft_mutex_store_l(&now, &philo->deadline, &philo->lock_meal);
	return (0);
}
