/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:23:37 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/12 16:19:25 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_number(char *nbr)
{
	int	i;

	i = -1;
	while (nbr[++i])
	{
		if (!(nbr[i] >= '0' && nbr[i] <= '9'))
			return (0);
	}
	return (1);
}

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
void	free_arr(void	**arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}
t_philo	**init_philo(t_data	*data)
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
			return (free_arr(philo_arr), NULL);
	}
	return (philo_arr);
}
void	*monitor_routine(void *data)
{
	t_data	*td;
	t_philo	*philo;
	int		i;

	i = -1;
	td = data;
	philo = NULL;
	init_philo(&td, &philo);
	if (!philo)
		return (NULL);
	// initialize philos
	return (td);
}

int	main(int ac, char **av)
{
	t_data		*data;
	pthread_t	monitor;

	if (ac != 5 && ac != 6)
		return (write(2, "BAD ARG", 7), 1);
	if (!validate_arg(ac, av))
		return (write(2, "BAD ARG", 7), 1);
	data = malloc(sizeof(t_data));
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->eat_num = ft_atoi(av[5]);
	data->philo_died = 0;
	data->philosophers = init_philosophers(data);
	if (pthread_create(&monitor, NULL, &monitor_routine, data) != 0)
		return (printf("error in creating thread"), 1);
	if (pthread_join(monitor, NULL) != 0)
		return (printf("error in creating thread"), 1);
}
