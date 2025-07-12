/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:30:06 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/12 22:25:26 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// System libraries
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdint.h>
# include <sys/time.h>

// Structs

typedef struct s_args {
	int		n_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		meals_required;
}	t_args;

typedef struct s_philo
{
	int             id;
	pthread_t       thread;
	pthread_mutex_t fork;
	pthread_mutex_t *next_fork;
	pthread_mutex_t lock_meal;
	long            last_meal_time;
	int             meals_eaten;
	t_args			*cfg;
} t_philo;

typedef struct s_arbitrator
{
	long	start_time;
	int		n_philos_finished;
	t_args	*cfg;
	t_philo	*philos;
} t_arbitrator;

int ft_parse_args(int argc, char *argv[], t_args *args);
#endif