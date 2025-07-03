/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:10:06 by kkoujan           #+#    #+#             */
/*   Updated: 2025/07/03 22:17:40 by kkoujan          ###   ########.fr       */
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

int	usleep_wrapper(long duration, t_data *data)
{
	long	start;

	start = gettimeofday_wrapper() + duration;
	while (gettimeofday_wrapper() < start)
	{
		if (has_sim_stopped(data))
			return (-1);
		usleep(200);
	}
	return (0);
}

long	gettimeofday_wrapper(void)
{
	t_time	time;

	if (gettimeofday(&time, NULL) == -1)
		return (-1);
	return ((time.tv_sec * 1000) + time.tv_usec / 1000);
}
