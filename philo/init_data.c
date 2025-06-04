/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:13:50 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/04 15:19:30 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	*init_forks(int forks_num)
{
	pthread_mutex_t	*forks;
	int				i;

	i = -1;
	forks = malloc(forks_num * sizeof(pthread_mutex_t));
	if (!forks)
		return (NULL);
	while (++i < forks_num)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (NULL);
	}
	return (forks);
}

t_philo	**init_philosophers(t_data	*data)
{
	t_philo	**philo_arr;
	int		i;

	i = -1;
	philo_arr = malloc((data->num_philos + 1) * sizeof(t_philo *));
	if (!philo_arr)
		return (NULL);
	while (++i < data->num_philos)
	{
		philo_arr[i] = malloc(sizeof(t_philo));
		if (!philo_arr[i])
			return (free_arr((void **)philo_arr), NULL);
	}
	philo_arr[i] = NULL;
	return (philo_arr);
}

t_philo	*init_philo(int id, t_data *data)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->num_meals_eaten = 0;
	philo->eat_time = 0;
	philo->data = data;
	if (pthread_mutex_init(&philo->num_meals_lock, NULL) != 0)
		return (free(philo), NULL);
	if (pthread_mutex_init(&philo->eat_time_lock, NULL) != 0)
		return (free(philo), NULL);
	return (philo);
}
