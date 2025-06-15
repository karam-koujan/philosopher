/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:16:40 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/15 15:00:15 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_func(void *data)
{
	t_philo	*philo_data;

	philo_data = data;
	if (philo_data->data->num_philos == 1)
		return (single_philo(philo_data), NULL);
	if (philo_data->id % 2 == 0)
		usleep_wrapper(500 / 1000, philo_data->data);
	while (!is_dead(philo_data->data))
	{
		if (take_fork(philo_data) == -1)
			return (NULL);
		if (eat(philo_data) == -1)
			return (NULL);
		if (pthread_mutex_lock(&philo_data->num_meals_lock) != 0)
			return (NULL);
		if (philo_data->num_meals_eaten == philo_data->data->eat_num)
			return (pthread_mutex_unlock(&philo_data->num_meals_lock), \
			NULL);
		if (pthread_mutex_unlock(&philo_data->num_meals_lock) != 0)
			return (NULL);
		if (sleeping(philo_data) == -1)
			return (NULL);
	}
	return (NULL);
}

void	*monitoring(void *data)
{
	t_data	*philo_data;
	int		i;

	philo_data = data;
	i = -1;
	while (stop_eating(philo_data) == 0)
	{
		while (++i < philo_data->num_philos)
		{
			if (get_last_meal_time(philo_data, i) >= philo_data->time_to_die)
			{
				if (pthread_mutex_lock(&philo_data->death_lock) != 0)
					return (NULL);
				philo_data->philo_died = 1;
				if (pthread_mutex_unlock(&philo_data->death_lock) != 0)
					return (NULL);
				print_message(philo_data->philosophers[i], 4);
				return (NULL);
			}
		}
		i = -1;
	}
	return (NULL);
}

int	is_dead(t_data *data)
{
	int	dead;

	dead = 0;
	if (pthread_mutex_lock(&data->death_lock) != 0)
		return (-1);
	dead = data->philo_died;
	if (pthread_mutex_unlock(&data->death_lock) != 0)
		return (-1);
	return (dead);
}

long	get_last_meal_time(t_data *philo_data, int i)
{
	long	eat_time;

	if (pthread_mutex_lock(&philo_data->philosophers[i]->eat_time_lock) != 0)
		return (-1);
	eat_time = philo_data->philosophers[i]->eat_time;
	if (pthread_mutex_unlock(&philo_data->philosophers[i]->eat_time_lock) != 0)
		return (-1);
	return (get_timestamp(philo_data->start_time) - eat_time);
}

void	single_philo(t_philo *data)
{
	if (take_fork(data) == -1)
		return (destroy_data_mutex(data->data));
}
