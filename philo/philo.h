/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkoujan <kkoujan@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:35:19 by kkoujan           #+#    #+#             */
/*   Updated: 2025/06/15 14:53:57 by kkoujan          ###   ########.fr       */
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

typedef struct s_philo
{
	int				id;
	int				num_meals_eaten;
	pthread_t		thread;
	pthread_mutex_t	num_meals_lock;
	pthread_mutex_t	eat_time_lock;
	long			eat_time;
	struct s_data	*data;
}				t_philo;

typedef struct s_data
{
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

typedef struct timeval	t_time;

long			get_timestamp(long start_time);
long			gettimeofday_wrapper(void);
int				usleep_wrapper(long duration, t_data *data);
int				print_message(t_philo *philo_data, int type);
int				ft_atoi(const char *str);
int				is_number(char *nbr);
void			free_arr(void	**arr);
int				is_dead(t_data *data);
int				stop_eating(t_data *philo_data);
pthread_mutex_t	*get_fork(t_philo *philo_data, int fork_pos);
int				take_fork(t_philo *philo_data);
int				think(t_philo *philo_data);
int				sleeping(t_philo *philo_data);
int				eat(t_philo *philo_data);
void			*philo_func(void *data);
void			*monitoring(void *data);
int				is_dead(t_data *data);
long			get_last_meal_time(t_data *philo_data, int i);
pthread_mutex_t	*init_forks(int forks_num);
t_philo			**init_philosophers(t_data	*data);
t_philo			*init_philo(int id, t_data *data);
void			destroy_data_mutex(t_data *data);
void			free_philosophers(t_philo **philos);
void			single_philo(t_philo *data);
void			destroy_mutex_arr(pthread_mutex_t *arr, int i);
void			clean_up(t_data *data);
int				run(t_data *data);
#endif