/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:19:41 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/19 21:41:04 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_init_philo(t_philo *philo, t_shared *sh, int i, t_philo *all);
static int	ft_init_philos(t_supervisor *sv);
static int	ft_launch_philos(t_supervisor *sv);
static void	*ft_sv_watch(t_supervisor *sv);

int	ft_init_simulation(t_supervisor *sv)
{
	sv->n_philos_finished = 0;
	if (pthread_mutex_init(&sv->shared.lock_print, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&sv->shared.lock_start, NULL) != 0)
	{
		pthread_mutex_destroy(&sv->shared.lock_print);
		return (-1);
	}
	if (ft_init_philos(sv) != 0)
		return (-1);
	pthread_mutex_lock(&sv->shared.lock_start);
	if (ft_launch_philos(sv) == -1)
	{
		ft_cleanup_mutex(sv, sv->shared.n_philos);
		return (-1);
	}
	sv->shared.start_time = ft_get_time_ms() + 60 + sv->shared.n_philos * 2;
	pthread_mutex_unlock(&sv->shared.lock_start);
	ft_wait_start_time(sv->shared.start_time);
	ft_sv_watch(sv);
	return (0);
}

static int	ft_init_philos(t_supervisor *sv)
{
	int	i;

	sv->philos = malloc(sizeof(t_philo) * sv->shared.n_philos);
	if (sv->philos == NULL)
		return (1);
	i = -1;
	while (++i < sv->shared.n_philos)
	{
		if (ft_init_philo(&sv->philos[i], &sv->shared, i, sv->philos) != 0)
		{
			ft_cleanup_mutex(sv, i);
			return (1);
		}
	}
	return (0);
}

static int	ft_init_philo(t_philo *philo, t_shared *shared, int i, t_philo *all)
{
	if (pthread_mutex_init(&philo->fork, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&philo->lock_meal, NULL) != 0)
	{
		pthread_mutex_destroy(&philo->fork);
		return (1);
	}
	philo->id = i + 1;
	philo->shared = shared;
	philo->meals_eaten = 0;
	philo->next_fork = &all[(i + 1) % shared->n_philos].fork;
	philo->meal_deadline = 0;
	philo->has_finished = 0;
	return (0);
}

static int	ft_launch_philos(t_supervisor *sv)
{
	int	i;

	i = -1;
	while (++i < sv->shared.n_philos)
	{
		if (pthread_create(&sv->philos[i].tid, NULL,
				ft_philo_routine, &sv->philos[i]) != 0)
			break ;
	}
	if (i < sv->shared.n_philos)
	{
		sv->shared.start_time = -1;
		pthread_mutex_unlock(&sv->shared.lock_start);
		while (--i > -1)
			pthread_join(sv->philos[i].tid, NULL);
		return (-1);
	}
	return (0);
}

static void	*ft_sv_watch(t_supervisor *sv)
{
	int		i;
	long	deadline;

	while (sv->n_philos_finished != sv->shared.n_philos)
	{
		i = -1;
		while (++i < sv->shared.n_philos)
		{
			if (sv->philos[i].has_finished == 1)
				continue ;
			pthread_mutex_lock(&sv->philos[i].lock_meal);
			deadline = sv->philos[i].meal_deadline;
			pthread_mutex_unlock(&sv->philos[i].lock_meal);
			if (deadline < 0)
			{
				if (deadline == -1)
					sv->n_philos_finished += ++sv->philos[i].has_finished;
				continue ;
			}
			if (ft_get_time_ms() > deadline)
				ft_declare_death(&sv->philos[i]);
		}
	}
}
