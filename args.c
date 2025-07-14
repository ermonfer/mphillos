/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:01:04 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/14 17:52:26 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_parse_args(int argc, char *argv[], t_args *args)
{
	int **args_arr;
	int	i;

	if (!(argc == 5 || argc == 6))
		return (1);
	args_arr = (int *[5]){&args->n_philos, &args->time_to_die, &args->time_to_eat,
			&args->time_to_sleep, &args->meals_required};	
	i = 0;
	while (i < 4)	
	{
		*args_arr[i] = ft_str_to_valid_arg(argv[i + 1]);
		if (*args_arr[i] < 0)
			exit (1);
		i++;
	}
	*args_arr[i] = -1;
	if (argc != 6)
		return (0);
	*args_arr[i] = ft_str_to_valid_arg(argv[i + 1]);
	if (*args_arr[i] < 0)
		exit (1);
	args->simulation_ok = 0;
	return (0);
}

int ft_str_to_valid_arg(char *str)
{
	int nmb;
	int len;

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
