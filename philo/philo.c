/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:23:37 by kkoujan           #+#    #+#             */
/*   Updated: 2025/07/03 21:49:56 by kkoujan          ###   ########.fr       */
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

int	start_simulation(t_monitor *monitor)
{
	int				i;
	t_data			*data;
	int				num_philos;
	int				status;

	status = 0;
	data = monitor->data;
	num_philos = data->num_philos;
	i = -1;
	data->start_time = gettimeofday_wrapper();
	if (data->start_time == -1)
		return (1);
	status = init_threads(monitor, &num_philos);
	while (++i < num_philos)
	{
		if (pthread_join(monitor->philosophers[i]->thread, NULL) != 0)
			return (1);
	}
	if (!status && num_philos == data->num_philos \
		&& pthread_join(data->monitor, NULL) != 0)
		return (1);
	return (status);
}

int	run(t_monitor *monitor)
{
	t_data		*data;
	int			status;

	status = 0;
	data = monitor->data;
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (destroy_philo_mutex(monitor), free(data->forks), \
	free(monitor->philosophers), free(data), free(monitor), 1);
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
		return (pthread_mutex_destroy(&data->print_lock), \
		destroy_philo_mutex(monitor), free(data->forks), \
		free(monitor->philosophers), free(data), \
		free(monitor), 1);
	status = start_simulation(monitor);
	clean_up(monitor);
	return (status);
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
		return (free(data), free(monitor), 1);
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
	monitor->data = data;
	if (!monitor->philosophers || !data->forks)
		return (destroy_philo_mutex(monitor), \
			free_philosophers(monitor->philosophers), \
			free(data->forks), free(data), free(monitor), 1);
	return (run(monitor));
}
