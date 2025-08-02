/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 13:14:32 by fmontero          #+#    #+#             */
/*   Updated: 2025/08/02 18:16:05 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_parse_args(int argc, char *argv[], t_args *args);
static int	ft_str_to_valid_arg(char *str);

int	main(int argc, char *argv[])
{
	t_supervisor	sv;
	int				i;

	if (ft_parse_args(argc, argv, &sv.shared.args) != 0)
		return (ERR_ARGS);
	if (sv.shared.args.meals_required == 0)
		return (0);
	if (sv.shared.args.n_philos == 1)
	{
		write(1, "0 1 has taken a fork\n", 21);
		usleep(sv.shared.args.time_to_die * 1000);
		printf("%d 1 died\n", sv.shared.args.time_to_die);
		return (0);
	}
	ft_init_simulation(&sv);
	i = -1;
	while (++i < sv.shared.args.n_philos)
		pthread_join(sv.philos[i].tid, NULL);
	ft_cleanup_mutex(&sv, sv.shared.args.n_philos);
	return (0);
}

static int	ft_parse_args(int argc, char *argv[], t_args *args)
{
	int	**args_arr;
	int	i;

	if (!(argc == 5 || argc == 6))
		return (ft_wr_ret("Incorrect number of arguments\n", ERR_ARGS));
	args_arr = (int *[5]){&args->n_philos, &args->time_to_die,
		&args->time_to_eat, &args->time_to_sleep, &args->meals_required};
	i = -1;
	while (++i < 4)
	{
		*args_arr[i] = ft_str_to_valid_arg(argv[i + 1]);
		if (*args_arr[i] < 0)
			return (ft_wr_ret("Incorrect format of argument\n", ERR_ARGS));
	}
	if (argc != 6)
		*args_arr[i] = -1;
	else
	{
		*args_arr[i] = ft_str_to_valid_arg(argv[i + 1]);
		if (*args_arr[i] < 0)
			return (ft_wr_ret("Incorrect format of argument\n", ERR_ARGS));
	}
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
		return (ERR_ARGS);
	while (*str == '0')
		str++;
	len = 0;
	nmb = 0;
	while (str[len] >= '0' && str[len] <= '9')
	{
		if (len > 5)
			return (ERR_ARGS);
		nmb = nmb * 10 + (str[len++] - '0');
	}
	if (str[len] != '\0')
		return (ERR_ARGS);
	return (nmb);
}

int	ft_wr_ret(char *msg, int rvalue)
{
	size_t	i;

	i = -1;
	while (msg[++i] != '\0')
		;
	write(2, msg, i);
	return (rvalue);
}
