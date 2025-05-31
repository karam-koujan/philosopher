/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:23:37 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/31 15:36:19 by kkoujan          ###   ########.fr       */
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

t_philo	**init_philosophers(t_data	*data)
{
	t_philo	**philo_arr;
	int		i;

	i = -1;
	philo_arr = malloc((data->num_philos + 1) * sizeof(t_data *));
	if (!philo_arr)
		return (NULL);
	while (++i < data->num_philos)
	{
		philo_arr[i] = malloc(sizeof(t_data *));
		if (!philo_arr[i])
			return (free_arr((void **)philo_arr), NULL);
	}
	philo_arr[i] = NULL;
	return (philo_arr);
}

void	*monitor_routine(void *data)
{
	t_data	*p_data;

	p_data = data;
	printf("philo routine\n");
	return (NULL);
}

t_philo	*init_philo(int id)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->id = id;
	philo->num_meals_eaten = 0;
	return (philo);
}

pthread_mutex_t	*get_fork(t_philo_data *philo_data, int fork_pos)
{
	pthread_mutex_t	*forks;
	int				idx;
	int				philo_num;

	philo_num =  philo_data->data->num_philos;
	forks = philo_data->data->forks;
	if (fork_pos == 0)
		return (&forks[philo_data->philo->id - 1]);
	idx = philo_data->philo->id - 1;
	if (idx + 1 < philo_num)
		return (&forks[idx + 1]);
	return (&forks[(idx + 1) % philo_num]);
}

int	take_fork(t_philo_data *philo_data)
{
	if (pthread_mutex_lock(get_fork(philo_data, 0)) != 0)
		return (-1);
	print_message(philo_data, 0);
	if (pthread_mutex_unlock(get_fork(philo_data, 0)) != 0)
		return (-1);
	if (pthread_mutex_lock(get_fork(philo_data, 1)) != 0)
		return (-1);
	print_message(philo_data, 0);
	if (pthread_mutex_unlock(get_fork(philo_data, 0)) != 0)
		return (-1);
	return (0);
}

int	eat(t_philo_data *philo_data)
{
	if (pthread_mutex_lock(&philo_data->philo->eat_time_lock) != 0)
		return (-1);
	philo_data->philo->eat_time = get_timestamp(philo_data->data->start_time);
	if (pthread_mutex_lock(&philo_data->philo->eat_time_lock) != 0)
		return (-1);
	if (print_message(philo_data, 1) == -1)
		return (-1);
	if (usleep_wrapper(philo_data->data->time_to_eat, \
		philo_data->data->philo_died) == -1)
		return (-1);
}

void	*philo_func(void *data)
{
	t_philo_data	*philo_data;

	philo_data = data;
	if (take_fork(philo_data) == -1)
		return (NULL);
	if (eat(philo_data) == -1)
		return (NULL);
	return (NULL);
}



void	start_simulation(t_data *data)
{
	int				i;
	t_philo			*philo;
	t_philo_data	philo_data;

	i = -1;
	philo_data.data = data;
	philo_data.data->start_time = gettimeofday_wrapper();
	while (++i < data->num_philos)
	{
		philo = init_philo(i + 1);
		if (!philo)
			return ;
		philo_data.philo = philo;
		if ((i + 1) % 2 != 0)
			usleep_wrapper(200, data->philo_died);
		if (pthread_create(&data->philosophers[i]->thread, NULL, \
				&philo_func, &philo_data) != 0)
				printf("error in creating thread");
	}
	i = -1;
	while (++i < data->num_philos)
	{
		if (pthread_join(data->philosophers[i]->thread, NULL) != 0)
			printf("error in joining threads");
	}
}

pthread_mutex_t	*init_forks(int	forks_num)
{
	pthread_mutex_t	*forks;
	int				i;

	i = -1;
	forks = malloc(forks_num * sizeof(pthread_mutex_t));
	if (!forks)
		return (NULL);
	while (++i < forks_num)
	{
		if (pthread_mutex_init(&forks[i], NULL) == -1)
			return (NULL);
	}
	return (forks);
}

int	main(int ac, char **av)
{
	t_data		*data;
	pthread_t	monitor;
	t_time		curr;
	t_time		prev;

	if (ac != 5 && ac != 6)
		return (write(2, "BAD ARG", 7), 1);
	if (!validate_arg(ac, av))
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
	start_simulation(data);
	// if (pthread_create(&monitor, NULL, &monitor_routine, data) != 0)
	// 	return (printf("error in creating thread"), 1);
	// if (pthread_join(monitor, NULL) != 0)
	// 	return (printf("error in creating thread"), 1);
	
}
