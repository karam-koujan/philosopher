/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:23:37 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/04 14:30:55 by kkoujan          ###   ########.fr       */
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

void	*philo_func(void *data)
{
	t_philo	*philo_data;

	philo_data = data;
	if (philo_data->id % 2 != 0)
	{
		if (usleep_wrapper(500 / 1000 ,philo_data->data) == -1)
			return (NULL);
	}
	while (!is_dead(philo_data->data))
	{
		if (take_fork(philo_data) == -1)
			return (NULL);
		if (eat(philo_data) == -1)
			return (NULL);
		if (pthread_mutex_lock(&philo_data->num_meals_lock) != 0)
			return (NULL);
		if (philo_data->num_meals_eaten == philo_data->data->eat_num)
			return (pthread_mutex_unlock(&philo_data->num_meals_lock), NULL);
		if (pthread_mutex_unlock(&philo_data->num_meals_lock) != 0)
			return (NULL);
		if (sleeping(philo_data) == -1)
			return (NULL);
		if (think(philo_data) == -1)
			return (NULL);
	}
	return (NULL);
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

void	*monitoring(void *data)
{
	t_data	*philo_data;
	int		is_dead;
	int		i;

	philo_data = data;
	is_dead = 0;
	i = -1;
	while (!is_dead && stop_eating(philo_data) == 0)
	{
		while (++i < philo_data->num_philos)
		{
			if (get_last_meal_time(philo_data, i) >= philo_data->time_to_die)
			{
				is_dead = 1;
				if (pthread_mutex_lock(&philo_data->death_lock) != 0)
					return (NULL);
				philo_data->philo_died = 1;
				if (pthread_mutex_unlock(&philo_data->death_lock) != 0)
					return (NULL);
				print_message(philo_data->philosophers[i], 4);
				break;
			}
		}
		i = -1;
	}
	return (NULL);
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
			return ;
		data->philosophers[i] = philo;
		if (pthread_create(&data->philosophers[i]->thread, NULL, \
				&philo_func, philo) != 0)
			printf("error in creating thread");
	}
	if (pthread_create(&data->monitor, NULL, &monitoring, data) != 0)
		printf("error in creating thread");
	i = -1;
	while (++i < data->num_philos)
	{
		if (pthread_join(data->philosophers[i]->thread, NULL) != 0)
			printf("error in joining threads");
	}
	if (pthread_join(data->monitor, NULL) != 0)
		printf("error in joining threads");
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
		if (pthread_mutex_init(&forks[i], NULL) != 0)
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
	if (pthread_mutex_init(&data->print_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
		return (1);
	start_simulation(data);
}
