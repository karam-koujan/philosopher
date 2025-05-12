/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:35:19 by kkoujan           #+#    #+#             */
/*   Updated: 2025/05/12 11:03:16 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <limits.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>

int	ft_atoi(const char *str);

typedef struct	s_philo
{
	int				id;
	int				num_meals_eaten;
	struct s_philo	*left;
	struct s_philo	*right;
	pthread_t		thread;
	pthread_mutex_t	arg_lock;
	pthread_mutex_t	fork;
}				t_philo;

typedef struct s_data {
    pthread_mutex_t	print_lock;
    pthread_mutex_t	death_lock;
    int				num_philos;
    int				time_to_sleep;
    int				time_to_die;
    int				time_to_eat;
    int				eat_num;
    long			start_time;
    int				philo_died;
    t_philo			**philosophers;
}				t_data;

#endif