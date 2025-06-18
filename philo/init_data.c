/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 15:13:50 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/18 16:33:21 by kkoujan          ###   ########.fr       */
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
			return (destroy_mutex_arr(forks, i - 1), free(forks), NULL);
	}
	return (forks);
}

t_philo	**init_philosophers(t_data	*data)
{
	t_philo	**philo_arr;
	int		i;
	int		j;

	i = -1;
	j = -1;
	philo_arr = malloc((data->num_philos + 1) * sizeof(t_philo *));
	if (!philo_arr)
		return (NULL);
	while (++i < data->num_philos)
	{
		philo_arr[i] = init_philo(i + 1, data);
		if (!philo_arr[i])
		{
			while (++j < i)
				free(philo_arr[j]);
			return (free(philo_arr), NULL);
		}
	}
	philo_arr[data->num_philos] = NULL;
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

void	destroy_data_mutex(t_monitor *data)
{
	int	i;

	i = -1;
	if (!data)
		return ;
	if (!data->philosophers)
		return ;
	pthread_mutex_destroy(&data->data->print_lock);
	pthread_mutex_destroy(&data->data->death_lock);
	while (data->philosophers[++i])
	{
		pthread_mutex_destroy(&data->philosophers[i]->num_meals_lock);
		pthread_mutex_destroy(&data->philosophers[i]->eat_time_lock);
		pthread_mutex_destroy(&data->data->forks[i]);
	}
}
