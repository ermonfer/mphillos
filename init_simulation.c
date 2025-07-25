/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_simulation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:19:41 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/24 18:15:04 by fmontero         ###   ########.fr       */
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
		ft_wr_ret("Mutex init error\n", ERR_MUTEX);
	if (pthread_mutex_init(&sv->shared.lock_start, NULL) != 0)
	{
		pthread_mutex_destroy(&sv->shared.lock_print);
		ft_wr_ret("Mutex init error\n", ERR_MUTEX);
	}
	if (ft_init_philos(sv) != 0)
		return (ERR_INIT_PHILO);
	pthread_mutex_lock(&sv->shared.lock_start);
	if (ft_launch_philos(sv) != 0)
	{
		ft_cleanup_mutex(sv, sv->shared.args.n_philos);
		return (ERR_LAUNCH_PHILO);
	}
	sv->shared.start_time = ft_get_time_ms()
		+ 60 + sv->shared.args.n_philos * 2;
	pthread_mutex_unlock(&sv->shared.lock_start);
	ft_wait_start_time(sv->shared.start_time);
	ft_sv_watch(sv);
	return (0);
}

static int	ft_init_philos(t_supervisor *sv)
{
	int	i;

	sv->philos = malloc(sizeof(t_philo) * sv->shared.args.n_philos);
	if (sv->philos == NULL)
		ft_wr_ret("Malloc error\n", ERR_INIT_PHILO);
	i = -1;
	while (++i < sv->shared.args.n_philos)
	{
		if (ft_init_philo(&sv->philos[i], &sv->shared, i, sv->philos) != 0)
		{
			ft_cleanup_mutex(sv, i);
			ft_wr_ret("Mutex init error\n", ERR_MUTEX);
		}
	}
	return (0);
}

static int	ft_init_philo(t_philo *philo, t_shared *shared, int i, t_philo *all)
{
	if (pthread_mutex_init(&philo->fork, NULL) != 0)
		return (ERR_MUTEX);
	if (pthread_mutex_init(&philo->lock_deadline, NULL) != 0)
	{
		pthread_mutex_destroy(&philo->fork);
		return (ERR_MUTEX);
	}
	philo->id = i + 1;
	philo->shared = shared;
	philo->meals_eaten = 0;
	philo->next_fork = &all[(i + 1) % shared->args.n_philos].fork;
	philo->deadline = 0;
	philo->has_finished = 0;
	return (0);
}

static int	ft_launch_philos(t_supervisor *sv)
{
	int	i;

	i = -1;
	while (++i < sv->shared.args.n_philos)
	{
		if (pthread_create(&sv->philos[i].tid, NULL,
				ft_philo_routine, &sv->philos[i]) != 0)
			break ;
	}
	if (i < sv->shared.args.n_philos)
	{
		sv->shared.start_time = -1;
		pthread_mutex_unlock(&sv->shared.lock_start);
		while (--i > -1)
			pthread_join(sv->philos[i].tid, NULL);
		ft_wr_ret("Pthread create error\n", ERR_LAUNCH_PHILO);
	}
	return (0);
}

static void	*ft_sv_watch(t_supervisor *sv)
{
	int		i;
	long	deadline;

	while (sv->n_philos_finished != sv->shared.args.n_philos)
	{
		i = -1;
		while (++i < sv->shared.args.n_philos)
		{
			if (sv->philos[i].has_finished == 1)
				continue ;
			ft_mutex_store_l(&sv->philos[i].deadline,
				&deadline, &sv->philos[i].lock_deadline);
			if (deadline < 0)
			{
				if (deadline == HAS_FINISHED)
					sv->n_philos_finished += ++sv->philos[i].has_finished;
				continue ;
			}
			if (ft_get_time_ms() > deadline)
				ft_declare_death(&sv->philos[i]);
		}
	}
	return (NULL);
}
