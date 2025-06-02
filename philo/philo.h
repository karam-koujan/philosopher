/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:35:19 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/02 23:05:07 by kkoujan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <limits.h>
# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <time.h>
# include <sys/time.h>

typedef struct	s_philo
{
	int				id;
	int				num_meals_eaten;
	pthread_t		thread;
	pthread_mutex_t	num_meals_lock;
	pthread_mutex_t	eat_time_lock;
	long			eat_time;
	pthread_mutex_t	arg_lock;
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
	pthread_t		monitor;
	pthread_mutex_t	*forks;
}				t_data;

typedef struct s_philo_data
{
	t_data	*data;
	t_philo	*philo;
}				t_philo_data;
typedef struct timeval	t_time;

long	get_timestamp(long start_time);
long	gettimeofday_wrapper(void);
int		usleep_wrapper(int duration, int is_dead);
int		print_message(t_philo_data *philo_data, int type);
int		ft_atoi(const char *str);
int		is_number(char *nbr);
void	free_arr(void	**arr);

#endif