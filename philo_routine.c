/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 21:02:14 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/26 19:18:13 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_thinking(t_philo *philo);
static int	ft_eating(t_philo *philo);
static int	ft_report_action(t_philo *philo, char *action);
static void	ft_select_forks(t_philo *ph,
				pthread_mutex_t **fst, pthread_mutex_t **snd);

void	*ft_philo_routine(void *args)
{
	t_philo		*self;

	self = args;
	pthread_mutex_lock(&self->shared->lock_start);
	pthread_mutex_unlock(&self->shared->lock_start);
	if (ft_wait_start_time(self->shared->start_time) != 0)
		return (NULL);
	ft_mutex_store_l(&(long){self->shared->start_time
		+ self->shared->args.time_to_die}, &self->deadline,
		&self->lock_deadline);
	while (1)
	{
		if (ft_thinking(self) != 0)
			return (NULL);
		if (ft_eating(self) != 0)
		{
			pthread_mutex_unlock(&self->fork);
			pthread_mutex_unlock(self->next_fork);
			return (NULL);
		}
		pthread_mutex_unlock(&self->fork);
		pthread_mutex_unlock(self->next_fork);
		if (ft_report_action(self, "is sleeping") != 0)
			return (NULL);
		usleep(self->shared->args.time_to_sleep * 1000);
	}
}

static int	ft_thinking(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->id % 2 == 1)
		usleep(1000);
	if (ft_report_action(philo, "is thinking") != 0)
		return (PHILO_DIED);
	ft_select_forks(philo, &first, &second);
	pthread_mutex_lock(first);
	if (ft_report_action(philo, "has taken a fork") != 0)
	{
		pthread_mutex_unlock(first);
		return (PHILO_DIED);
	}
	pthread_mutex_lock(second);
	if (ft_report_action(philo, "has taken a fork") != 0)
	{
		pthread_mutex_unlock(first);
		pthread_mutex_unlock(second);
		return (PHILO_DIED);
	}
	return (0);
}

static int	ft_eating(t_philo *philo)
{
	long	deadline;

	ft_mutex_store_l(&philo->deadline, &deadline, &philo->lock_deadline);
	if (ft_get_time_ms() > deadline)
	{
		ft_declare_death(philo);
		ft_mutex_store_l(&(long){HAS_FINISHED}, &philo->deadline,
			&philo->lock_deadline);
		return (HAS_FINISHED);
	}
	ft_mutex_store_l(&(long){ft_get_time_ms()
		+ philo->shared->args.time_to_die},
		&philo->deadline, &philo->lock_deadline);
	if (ft_report_action(philo, "is eating") != 0)
		return (HAS_FINISHED);
	usleep(philo->shared->args.time_to_eat * 1000);
	if (++philo->meals_eaten == philo->shared->args.meals_required)
	{
		ft_mutex_store_l(&(long){HAS_FINISHED}, &philo->deadline,
			&philo->lock_deadline);
		return (HAS_FINISHED);
	}
	return (0);
}

static int	ft_report_action(t_philo *philo, char *action)
{
	long	timestamp;

	timestamp = ft_get_time_ms() - philo->shared->start_time;
	pthread_mutex_lock(&philo->shared->lock_print);
	if (philo->shared->philo_died == 1)
	{
		pthread_mutex_unlock(&philo->shared->lock_print);
		ft_mutex_store_l(&(long){HAS_FINISHED}, &philo->deadline,
			&philo->lock_deadline);
		return (PHILO_DIED);
	}
	printf("%ld %d %s\n", timestamp, philo->id, action);
	pthread_mutex_unlock(&philo->shared->lock_print);
	return (0);
}

static void	ft_select_forks(t_philo *ph,
	pthread_mutex_t **fst, pthread_mutex_t **snd)
{
	if (ph->id % 2 == 0)
	{
		*fst = &ph->fork;
		*snd = ph->next_fork;
	}
	else
	{
		*fst = ph->next_fork;
		*snd = &ph->fork;
	}
}
