/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:23:37 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/18 16:40:33 by kkoujan          ###   ########.fr       */
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

int	stop_eating(t_monitor *philo_data)
{
	int	i;
	int	end;

	i = -1;
	end = 1;
	if (philo_data->data->eat_num == 0)
		return (0);
	while (++i < philo_data->data->num_philos)
	{
		if (pthread_mutex_lock(&philo_data->philosophers[i]->num_meals_lock)
			!= 0)
			return (-1);
		if (philo_data->philosophers[i]->num_meals_eaten \
			!= philo_data->data->eat_num)
			end = 0;
		if (pthread_mutex_unlock(&philo_data->philosophers[i]->num_meals_lock)
			!= 0)
			return (-1);
	}
	return (end);
}

void	start_simulation(t_monitor *monitor)
{
	int				i;
	t_philo			*philo;
	t_data			*data;

	data = monitor->data;
	i = -1;
	data->start_time = gettimeofday_wrapper();
	while (++i < data->num_philos)
	{
		philo = monitor->philosophers[i];
		if (pthread_create(&monitor->philosophers[i]->thread, NULL, \
				&philo_func, philo) != 0)
			data->num_philos = i;
	}
	if (pthread_create(&data->monitor, NULL, &monitoring, monitor) != 0)
		return ;
	i = -1;
	while (++i < data->num_philos)
	{
		if (pthread_join(monitor->philosophers[i]->thread, NULL) != 0)
			continue ;
	}
	pthread_join(data->monitor, NULL);
}

int	run(t_monitor *monitor)
{
	t_data		*data;

	data = monitor->data;
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (destroy_mutex_arr(data->forks, data->num_philos - 1), \
	free(data->forks), free(monitor->philosophers), free(data), 1);
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
		return (pthread_mutex_destroy(&data->print_lock), \
		destroy_mutex_arr(data->forks, data->num_philos - 1), \
		free(data->forks), free(monitor->philosophers), free(data), 1);
	start_simulation(monitor);
	clean_up(monitor);
	return (0);
}

int	main(int ac, char **av)
{
	t_data		*data;
	t_monitor	*monitor;

	if ((ac != 5 && ac != 6) || !validate_arg(ac, av))
		return (write(2, "BAD ARG", 7), 1);
	data = malloc(sizeof(t_data));
	monitor = malloc(sizeof(t_monitor));
	if (!data || !monitor)
		return (1);
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->eat_num = 0;
	if (ac == 6)
		data->eat_num = ft_atoi(av[5]);
	data->philo_died = 0;
	monitor->philosophers = init_philosophers(data);
	data->forks = init_forks(data->num_philos);
	data->philo_died = 0;
	monitor->data = data;
	if (!monitor->philosophers || !data->forks)
		return (free_philosophers(monitor->philosophers), \
			free(data->forks), free(data), 1);
	return (run(monitor));
}
