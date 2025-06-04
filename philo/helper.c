/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:12:51 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/04 07:41:34 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

void	free_arr(void	**arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
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

int	print_message(t_philo *philo_data, int type)
{
	if (pthread_mutex_lock(&(philo_data->data->print_lock)) != 0)
		return (-1);
	if (type == 0)
		printf("%ld	%i has taken a fork\n", \
		philo_data->eat_time, philo_data->id);
	else if (type == 1)
		printf("%ld	%i is eating\n", \
		get_timestamp(philo_data->data->start_time), philo_data->id);
	else if (type == 2)
		printf("%ld	%i is sleeping\n", \
		get_timestamp(philo_data->data->start_time), philo_data->id);
	else if (type == 3)
		printf("%ld	%i is thinking\n", \
		get_timestamp(philo_data->data->start_time), philo_data->id);
	else if (type == 4)
		printf("%ld	%i died\n", \
		get_timestamp(philo_data->data->start_time), philo_data->id);
	if (pthread_mutex_unlock(&(philo_data->data->print_lock)) != 0)
		return (-1);
	return (0);
}
