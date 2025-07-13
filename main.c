/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:19:41 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/13 13:17:20 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_args			cfg;
	t_supervisor	sv;

	ft_parse_args(argc, argv, &cfg);
}

void	rountine(void);

long get_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

static void	init_philo(t_philo *philo, t_args *cfg, int i, t_philo *all)
{
	philo->id = i + 1;
	philo->cfg = cfg;
	philo->meals_eaten = 0;
	philo->has_finished = 0;
	pthread_mutex_init(&philo->fork, NULL);
	pthread_mutex_init(&philo->lock_meal, NULL);
	philo->next_fork = &all[(i + 1) % cfg->n_philos].fork;
	philo->last_meal_time = 0;
}

int	init_supervisor(t_args *cfg, t_supervisor *sv, void routine())
{
	int	i;

	sv->cfg = cfg;
	sv->n_philos_finished = 0;
	sv->philos = malloc(sizeof(t_philo) * cfg->n_philos);
	if (!sv->philos)
		exit (1);
	i = -1;
	while (++i < cfg->n_philos)
		init_philo(&sv->philos[i], cfg, i, sv->philos);
	sv->start_time = get_time_ms() + 60 + cfg->n_philos * 2;
	i = -1;
	while (++i < cfg->n_philos)
	{
		if (pthread_create(&sv->philos[i].thread, NULL,
				&routine, &sv->philos[i]))
		{
			free(sv->philos);
			return (2);
		}
	}
	return (0);
}
