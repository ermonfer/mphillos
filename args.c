/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmontero <fmontero@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:01:04 by fmontero          #+#    #+#             */
/*   Updated: 2025/07/12 12:49:28 by fmontero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_check_args(int argc, char *argv[])
{
	if (!(argc == 4 || argc == 5))
		return (1);
	while (*argv != NULL)
		ft_str_to_valid_arg(*argv);
	return ;
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
