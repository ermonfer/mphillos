/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:19:41 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/12 22:36:32 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_args			cfg;
	t_arbitrator	arb;
	t_philo			*philos;

	ft_parse_args(argc, argv, &cfg);
	philos = malloc(cfg.n_philos * sizeof(t_philo));
	ft_init_arb();
}

void	ft_init_arb(t_arbitrator* arb, t_args *cfg, t_philo *philos)
{
	struct timeval tv;
	
	arb->cfg = cfg;
	arb->philos = philos;
	arb->n_philos_finished = 0;

}