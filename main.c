/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:01:04 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/20 18:46:51 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_parse_args(int argc, char *argv[], t_shared *args);
static int	ft_str_to_valid_arg(char *str);

int	main(int argc, char *argv[])
{
	t_supervisor	sv;
	int				i;

	if (ft_parse_args(argc, argv, &sv.shared) != 0)
		return (ERR_ARGS);
	ft_init_simulation(&sv);
	i = -1;
	while (++i < sv.shared.n_philos)
		pthread_join(sv.philos[i].tid, NULL);
	ft_cleanup_mutex(&sv, sv.shared.n_philos);
	return (0);
}

static int	ft_parse_args(int argc, char *argv[], t_shared *args)
{
	int	**args_arr;
	int	i;

	if (!(argc == 5 || argc == 6))
		return (write_return("Incorrect number of arguments\n", ERR_ARGS));
	args_arr = (int *[5]){&args->n_philos, &args->time_to_die,
		&args->time_to_eat, &args->time_to_sleep, &args->meals_required};
	i = -1;
	while (++i < 4)
	{
		*args_arr[i] = ft_str_to_valid_arg(argv[i + 1]);
		if (*args_arr[i] < 0)
			return (write_return("Incorrect format of argument\n", ERR_ARGS));
	}
	*args_arr[i] = -1;
	if (argc != 6)
		return (0);
	*args_arr[i] = ft_str_to_valid_arg(argv[i + 1]);
	if (*args_arr[i] < 0)
		return (write_return("Incorrect format of argument\n", ERR_ARGS));
	return (0);
}

static int	ft_str_to_valid_arg(char *str)
{
	int	nmb;
	int	len;

	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '+')
		str++;
	if (*str == '\0')
		return (-1);
	while (*str == '0')
		str++;
	len = 1;
	nmb = 0;
	while (str[len] >= '0' && str[len] <= '9')
	{
		if (len > 6)
			return (-1);
		nmb = nmb * 10 + (str[len++] - '0');
	}
	if (str[len] != '\0')
		return (-1);
	return (nmb);
}

int	write_return(char *msg, int rvalue)
{
	size_t	i;

	i = -1;
	while (msg[++i] != '\0')
		;
	write(2, msg, i);
	return (rvalue);
}
