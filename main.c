/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:19:41 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/13 19:52:51 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_args			cfg;
	t_supervisor	sv;

	ft_parse_args(argc, argv, &cfg);
	sv.cfg = &cfg;
	pthread_create(&sv.id, NULL, ft_init_supervisor, &sv); // Crear estructura con un compound literal para argumentos?
	
}

void	*routine(void	*args);

long	ft_get_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int		ft_init_philo(t_philo *philo, t_args *cfg, int i, t_philo *all)
{
	philo->id = i + 1;
	philo->cfg = cfg;
	philo->meals_eaten = 0;
	if (pthread_mutex_init(&philo->fork, NULL) != 0)
		return (1);	
	if (pthread_mutex_init(&philo->lock_meal, NULL) != 0)
	{
		pthread_mutex_destroy(&philo->fork);
		return (1);
	}
	philo->next_fork = &all[(i + 1) % cfg->n_philos].fork;
	philo->last_meal_time = 0;
	philo->has_finished = 0;
	return (0);
}

int	ft_init_supervisor(t_supervisor *sv)
{
	int		i;

	sv->n_philos_finished = 0;
	sv->philos = malloc(sizeof(t_philo) * cfg->n_philos);
	if (!sv->philos)
		exit (1);
	i = -1;
	while (++i < cfg->n_philos)
	{
		if (ft_init_philo(&sv->philos[i], cfg, i, sv->philos) != 0)
		{
			while (--i > -1)
			{
				pthread_mutex_destroy(&sv->philos[i].fork);
				pthread_mutex_destroy(&sv->philos[i].lock_meal);
			}
		}
	}
	sv->start_time = ft_get_time_ms() + 60 + cfg->n_philos * 2;
	i = -1;
	while (++i < cfg->n_philos)
	{
		if (pthread_create(&sv->philos[i].thread, NULL,
				&routine, &sv->philos[i]) != 0)
		{
			free(sv->philos);
			return (2);
		}
	}
	return (0);
}
