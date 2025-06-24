/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:12:51 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/18 16:32:33 by kkoujan          ###   ########.fr       */
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

void	free_philosophers(t_philo **philos)
{
	int	i;

	i = -1;
	if (!philos)
		return ;
	while (philos[++i])
		free(philos[i]);
	free(philos);
}

int	print_message(t_philo *philo_data, int type)
{
	if (pthread_mutex_lock(&(philo_data->data->print_lock)) != 0)
		return (-1);
	if (pthread_mutex_lock(&(philo_data->data->death_lock)) != 0)
		return (-1);
	if (type == 0 && !philo_data->data->philo_died)
		printf("%ld	%i has taken a fork\n", \
			get_timestamp(philo_data->data->start_time), philo_data->id);
	else if (type == 1 && !philo_data->data->philo_died)
		printf("%ld	%i is eating\n", \
		get_timestamp(philo_data->data->start_time), philo_data->id);
	else if (type == 2 && !philo_data->data->philo_died)
		printf("%ld	%i is sleeping\n", \
		get_timestamp(philo_data->data->start_time), philo_data->id);
	else if (type == 3 && !philo_data->data->philo_died)
		printf("%ld	%i is thinking\n", \
		get_timestamp(philo_data->data->start_time), philo_data->id);
	else if (type == 4 && philo_data->data->philo_died == 1)
		printf("%ld	%i died\n", \
		get_timestamp(philo_data->data->start_time), philo_data->id);
	if (pthread_mutex_unlock(&(philo_data->data->death_lock)) != 0)
		return (-1);
	if (pthread_mutex_unlock(&(philo_data->data->print_lock)) != 0)
		return (-1);
	return (0);
}

void	destroy_mutex_arr(pthread_mutex_t *arr, int i)
{
	while (i >= 0)
	{
		pthread_mutex_destroy(&arr[i]);
		i--;
	}
}

void	clean_up(t_monitor *monitor)
{
	t_data	*data;

	data = monitor->data;
	destroy_data_mutex(monitor);
	free_philosophers(monitor->philosophers);
	monitor->philosophers = NULL;
	free(data->forks);
	data->forks = NULL;
	free(data);
	free(monitor);
}
