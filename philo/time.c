/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:10:06 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/04 14:05:11 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./philo.h"

long	get_timestamp(long start_time)
{
	long	time;

	time = gettimeofday_wrapper();
	if (time == -1)
		return (-1);
	return (time - start_time);
}

int	get_passed_time(t_time *prev, t_time *curr)
{
	long	time;

	time = ((curr->tv_sec - prev->tv_sec) * 1000000) + \
		(curr->tv_usec - prev->tv_usec);
	return (time);
}

int	usleep_wrapper(long duration, t_data *data)
{
	t_time		curr;
	t_time		prev;
	long		rest;
	int			is_dead;

	if (gettimeofday(&prev, NULL) == -1)
		return (-1);
	rest = 0;
	duration = duration * 1000;
	if (pthread_mutex_lock(&data->death_lock) != 0)
		return (-1);
	is_dead = data->philo_died;
	if (pthread_mutex_unlock(&data->death_lock) != 0)
		return (-1);
	while (rest <= duration && !is_dead)
	{
		if (gettimeofday(&curr, NULL) == -1)
			return (-1);
		rest = get_passed_time(&prev, &curr);
		if (pthread_mutex_lock(&data->death_lock) != 0)
			return (-1);
		is_dead = data->philo_died;
		if (pthread_mutex_unlock(&data->death_lock) != 0)
			return (-1);
	}
	if (is_dead == 1)
		return (-1);
	return (0);
}

long	gettimeofday_wrapper(void)
{
	t_time	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((time.tv_sec * 1000) + time.tv_usec / 1000);
}
