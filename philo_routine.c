/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 21:02:14 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/19 21:20:20 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void		ft_grab_forks(t_philo *philo);
static void		ft_eating(t_philo *philo);

void	*ft_philo_routine(void *args)
{
	t_philo		*self;
	long		now;

	self = args;
	pthread_mutex_lock(&self->shared->lock_start);
	pthread_mutex_unlock(&self->shared->lock_start);
	if (ft_wait_start_time(self->shared->start_time) == -1)
		return (NULL);
	while (1)
	{
		now = ft_get_time_ms();
		pthread_mutex_lock(&self->shared->lock_print);
		printf("%ld %d is thinking", now, self->id);
		pthread_mutex_unlock(&self->shared->lock_print);
		ft_grab_forks(self);
		ft_eating(self);
		now = ft_get_time_ms();
		pthread_mutex_lock(&self->shared->lock_print);
		printf("%ld %d is sleeping", now, self->id);
		pthread_mutex_unlock(&self->shared->lock_print);
		usleep(self->shared->time_to_sleep);
	}
}

static void	ft_grab_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->fork);
		pthread_mutex_lock(&philo->next_fork);
	}
	else
	{
		pthread_mutex_lock(&philo->next_fork);
		pthread_mutex_lock(&philo->fork);
	}
}

static void	ft_eating(t_philo *philo)
{
	long	now;

	now = ft_get_time_ms();
	if (philo->meal_deadline < now)
	{
		ft_declare_death(philo);
		return (NULL);
	}
	pthread_mutex_lock(&philo->lock_meal);
	philo->meal_deadline == -2;
	pthread_mutex_unlock(&philo->lock_meal);
	pthread_mutex_lock(&philo->shared->lock_print);
	printf("%ld %d is eating", now, philo->id);
	pthread_mutex_unlock(&philo->shared->lock_print);
	usleep(philo->shared->time_to_eat);
	if (++philo->meals_eaten == philo->shared->meals_required)
	{
		philo->meal_deadline == -2;
		return (NULL);
	}
	now = ft_get_time_ms();
	pthread_mutex_lock(&philo->lock_meal);
	philo->meal_deadline == now + philo->shared->time_to_die;
	pthread_mutex_unlock(&philo->lock_meal);
}
