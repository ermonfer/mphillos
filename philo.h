/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:30:06 by fmontero          #+#    #+#             */
/*   Updated: 2025/08/01 13:38:01 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// System libraries
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <stdint.h>
# include <sys/time.h>

// Macros
# define ERR_ARGS 			-1
# define ERR_MUTEX			-2
# define ERR_INIT_PHILO		-3
# define ERR_LAUNCH_PHILO	-4
# define ERR_TIME			-5
# define HAS_FINISHED		-1
# define NOT_STARTED_PHILO	-2
# define PHILO_DIED			1

// Structs
typedef struct s_args
{
	int					n_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					meals_required;
}	t_args;

typedef struct s_shared
{
	t_args				args;
	pthread_mutex_t		lock_start;
	long				start_time;
	pthread_mutex_t		lock_print;
	char				philo_died;
}	t_shared;

typedef struct s_philo
{
	int				id;
	pthread_t		tid;
	pthread_mutex_t	fork;
	pthread_mutex_t	*next_fork;
	pthread_mutex_t	lock_deadline;
	long			deadline;
	int				meals_eaten;
	int				has_finished;
	t_shared		*shared;
}	t_philo;

typedef struct s_supervisor
{
	pthread_t	tid;
	int			n_philos_finished;
	t_philo		*philos;
	t_shared	shared;
}	t_supervisor;

// Functions

// init_simulation.c
int		ft_init_simulation(t_supervisor *sv);
// philo_routine.c
void	*ft_philo_routine(void *args);
// utilities.c
void	ft_cleanup_mutex(t_supervisor *sv, int n);
void	ft_declare_death(t_philo *philo);
int		ft_wait_start_time(long start_time);
long	ft_get_time_ms(void);
void	ft_mutex_store_l(long *rd, long*wr, pthread_mutex_t *lock);
// main.c
int		ft_wr_ret(char *msg, int rvalue);
#endif