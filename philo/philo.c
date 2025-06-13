/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:23:37 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/11 09:03:32 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_arg(int ac, char **av)
{
	int	i;

	i = 0;
	while (++i < ac)
	{
		if (!is_number(av[i]))
			return (0);
		if (ft_atoi(av[i]) <= 0)
			return (0);
	}
	return (1);
}

int	stop_eating(t_data *philo_data)
{
	int	i;
	int	end;

	i = -1;
	end = 1;
	if (philo_data->eat_num == 0)
		return (0);
	while (++i < philo_data->num_philos)
	{
		if (pthread_mutex_lock(&philo_data->philosophers[i]->num_meals_lock)
			!= 0)
			return (-1);
		if (philo_data->philosophers[i]->num_meals_eaten != philo_data->eat_num)
			end = 0;
		if (pthread_mutex_unlock(&philo_data->philosophers[i]->num_meals_lock)
			!= 0)
			return (-1);
	}
	return (end);
}

void	start_simulation(t_data *data)
{
	int				i;
	t_philo			*philo;

	i = -1;
	data->start_time = gettimeofday_wrapper();
	while (++i < data->num_philos)
	{
		philo = init_philo(i + 1, data);
		if (!philo)
			return (free_philosophers(data->philosophers, i - 1));
		data->philosophers[i] = philo;
		if (pthread_create(&data->philosophers[i]->thread, NULL, \
				&philo_func, philo) != 0)
			data->num_philos = i;
	}
	if (pthread_create(&data->monitor, NULL, &monitoring, data) != 0)
		return ;
	i = -1;
	while (++i < data->num_philos)
	{
		if (pthread_join(data->philosophers[i]->thread, NULL) != 0)
			continue ;
	}
	if (pthread_join(data->monitor, NULL) != 0)
		printf("error in joining threads");
}

int	main(int ac, char **av)
{
	t_data		*data;

	if ((ac != 5 && ac != 6) || !validate_arg(ac, av))
		return (write(2, "BAD ARG", 7), 1);
	data = malloc(sizeof(t_data));
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->eat_num = 0;
	if (ac == 6)
		data->eat_num = ft_atoi(av[5]);
	data->philo_died = 0;
	data->philosophers = init_philosophers(data);
	data->forks = init_forks(data->num_philos);
	data->philo_died = 0;
	if (!data->philosophers || !data->forks)
		return (printf("something went wrong!"), 1);
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
		return (pthread_mutex_destroy(&data->print_lock), 1);
	start_simulation(data);
	destroy_data_mutex(data);
	free_philosophers(data->philosophers, data->num_philos - 1);
}
