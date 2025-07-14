/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:19:41 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/14 19:42:11 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_sim_ctxt			sim;
	t_philo			*philos;
	t_supervisor	sv;

	ft_parse_args(argc, argv, &sim);
	
}

void	*ft_philo_routine(void *args);
void	*ft_supervisor_routine(void *args);

long	ft_get_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int		ft_init_philo(t_philo *philo, t_sim_ctxt *sim, int i, t_philo *all)
{
	if (pthread_mutex_init(&philo->fork, NULL) != 0)
		return (1);	
	if (pthread_mutex_init(&philo->lock_meal, NULL) != 0)
	{
		pthread_mutex_destroy(&philo->fork);
		return (1);
	}
	philo->id = i + 1;
	philo->sim = sim;
	philo->meals_eaten = 0;
	philo->next_fork = &all[(i + 1) % sim->n_philos].fork;
	philo->last_meal_time = 0;
	philo->has_finished = 0;
	return (0);
}

int	ft_init_philos(t_philo **philos, t_sim_ctxt *sim)
{
	int	i;

	*philos = malloc(sizeof( t_philo) * sim->n_philos);
	if (*philos == NULL)
		return (1);
	i = -1;
	while (++i < sim->n_philos)
	{
		if (ft_init_philo(&philos[i], sim, i, philos) != 0)
		{
			while (--i > -1)
			{
				pthread_mutex_destroy(&(*philos)[i].fork);
				pthread_mutex_destroy(&(*philos)[i].lock_meal);
			}
			return (1);
		}
	}
	return (0);
}

int	ft_launch_philos(t_philo *philos, int n_philos)
{
	int i;

	i = -1;
	while (++i < n_philos)
	{
		if (pthread_create(&philos[i].tid, NULL,
				ft_philo_routine, &philos[i]) != 0)
			return (i);
	}
}

int	ft_launch_supervisor(t_supervisor *sv, t_philo *philos, t_sim_ctxt *sim)
{
	sv->n_philos_finished = 0;
	sv->sim = sim;
	sv->philos = philos;
	if (pthread_create(&sv->tid, NULL,
			ft_supervisor_routine, sv) != 0)
		return (1);
	return (0);
}

void	*ft_supervisor_routine(void *arg)
{
	t_supervisor	*sv;

	sv = (t_supervisor *)arg;
	sv->start_time = ft_get_time_ms() + 60 + sim->n_philos * 2;
	i = -1;
	while (++i < sim->n_philos)
	{
		if (pthread_create(&sv->philos[i].tid, NULL,
				ft_philo_routine, &sv->philos[i]) != 0)
		{
			free(sv->philos);
			return (2);
		}
	}
}

void	ft_cleanup_simulation(t_philo *philos, t_supervisor *mon, int n)
{
	int	i;

	i = -1;
	while (++i < n)
	{
		pthread_mutex_destroy(&philos[i].fork);
		pthread_mutex_destroy(&philos[i].lock_meal);
	}
	free(philos);
}

int	ft_init_supervisor(t_supervisor *sv)
{
	int		i;

	sv->n_philos_finished = 0;
	sv->philos = malloc(sizeof(t_philo) * sim->n_philos);
	if (!sv->philos)
		exit (1);
	//////
	i = -1;
	while (++i < sim->n_philos)
	{
		if (ft_init_philo(&sv->philos[i], sim, i, sv->philos) != 0)
		{
			while (--i > -1)
			{
				pthread_mutex_destroy(&sv->philos[i].fork);
				pthread_mutex_destroy(&sv->philos[i].lock_meal);
			}
		}
	}
	/////////
	sv->start_time = ft_get_time_ms() + 60 + sim->n_philos * 2;
	i = -1;
	while (++i < sim->n_philos)
	{
		if (pthread_create(&sv->philos[i].tid, NULL,
				ft_philo_routine, &sv->philos[i]) != 0)
		{
			free(sv->philos);
			return (2);
		}
	}
	return (0);
}
