/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:59:47 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/24 20:05:24 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	stop_philos(t_data *data)
{
	pthread_mutex_lock(&data->death_lock);
	data->philo_died = 2;
	pthread_mutex_unlock(&data->death_lock);
}

int	init_threads(t_monitor *monitor, int *num_philos)
{
	int				i;
	t_data			*data;
	int				status;

	status = 0;
	data = monitor->data;
	i = -1;
	while (++i < data->num_philos)
	{
		if (pthread_create(&monitor->philosophers[i]->thread, NULL, \
				&philo_func, monitor->philosophers[i]) != 0)
		{
			stop_philos(data);
			*num_philos = i;
			return (1);
		}
	}
	if (!status && \
		pthread_create(&data->monitor, NULL, &monitoring, monitor) != 0)
	{
		stop_philos(data);
		status = 1;
	}
	return (status);
}
