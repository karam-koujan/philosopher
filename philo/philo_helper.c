/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 10:05:50 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/15 14:53:28 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strtoint(const char *str, int sign)
{
	int	result;
	int	max_div_10;
	int	max_mod_10;
	int	min_div_10;
	int	min_mod_10;

	max_mod_10 = INT_MAX % 10;
	min_mod_10 = INT_MIN % 10;
	max_div_10 = INT_MAX / 10;
	min_div_10 = INT_MIN / 10;
	result = 0;
	while (*str && *str >= '0' && *str <= '9')
	{
		if (sign == 1 && (result > max_div_10 || (result == max_div_10 \
		&& *str - '0' > max_mod_10)))
			return (-1);
		if (sign == -1 && (result < min_div_10 || (result == min_div_10 && \
		(*str - '0') > -(min_mod_10))))
			return (0);
		result = result * 10 + (sign * (*str - '0'));
		str++;
	}
	return (result);
}

int	ft_atoi(const char *str)
{
	int	sign;

	sign = 1;
	while (*str && (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\v'
			|| *str == '\f' || *str == '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	return (ft_strtoint(str, sign));
}

int	is_number(char *nbr)
{
	int	i;

	i = -1;
	while (nbr[++i])
	{
		if (!(nbr[i] >= '0' && nbr[i] <= '9'))
			return (0);
	}
	return (1);
}
