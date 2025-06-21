/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 14:12:21 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/21 17:30:05 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eat(t_philo *philo_data)
{
	if (pthread_mutex_lock(&philo_data->eat_time_lock) != 0)
		return (-1);
	philo_data->eat_time = get_timestamp(philo_data->data->start_time);
	if (pthread_mutex_unlock(&philo_data->eat_time_lock) != 0)
		return (-1);
	if (print_message(philo_data, 1) == -1)
		return (-1);
	if (usleep_wrapper(philo_data->data->time_to_eat, \
		philo_data->data) == -1)
		return (pthread_mutex_unlock(get_fork(philo_data, 1)), \
		pthread_mutex_unlock(get_fork(philo_data, 0)), -1);
	if (pthread_mutex_unlock(get_fork(philo_data, 1)) != 0)
		return (-1);
	if (pthread_mutex_unlock(get_fork(philo_data, 0)) != 0)
		return (-1);
	if (pthread_mutex_lock(&philo_data->num_meals_lock) != 0)
		return (-1);
	philo_data->num_meals_eaten++;
	if (philo_data->num_meals_eaten == philo_data->data->eat_num)
		return (pthread_mutex_unlock(&philo_data->num_meals_lock), -1);
	if (pthread_mutex_unlock(&philo_data->num_meals_lock) != 0)
		return (-1);
	return (0);
}

int	sleeping(t_philo *philo_data)
{
	if (print_message(philo_data, 2) == -1)
		return (-1);
	if (usleep_wrapper(philo_data->data->time_to_sleep, \
		philo_data->data) == -1)
		return (-1);
	if (think(philo_data) == -1)
		return (-1);
	return (0);
}

int	think(t_philo *philo_data)
{
	if (print_message(philo_data, 3) == -1)
		return (-1);
	return (0);
}

int	take_fork(t_philo *philo_data)
{
	if (pthread_mutex_lock(get_fork(philo_data, 0)) != 0)
		return (-1);
	print_message(philo_data, 0);
	if (philo_data->data->num_philos == 1)
		return (pthread_mutex_unlock(get_fork(philo_data, 0)), 0);
	if (pthread_mutex_lock(get_fork(philo_data, 1)) != 0)
		return (-1);
	print_message(philo_data, 0);
	return (0);
}

pthread_mutex_t	*get_fork(t_philo *philo_data, int fork_pos)
{
	pthread_mutex_t	*forks;
	int				idx;
	int				philo_num;

	philo_num = philo_data->data->num_philos;
	forks = philo_data->data->forks;
	if (fork_pos == 0)
		return (&forks[philo_data->id - 1]);
	idx = philo_data->id - 1;
	if (idx + 1 < philo_num)
		return (&forks[idx + 1]);
	return (&forks[(idx + 1) % philo_num]);
}
