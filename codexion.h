/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamaarab <hamaarab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 03:37:06 by hamaarab          #+#    #+#             */
/*   Updated: 2026/04/19 23:15:00 by hamaarab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_coder	t_coder;

typedef struct s_data
{
	int					number_of_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					required_compiles;
	int					dongle_cooldown;
	char				*scheduler;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		stop_mutex;
	long				start_time;
	int					stop;
	pthread_mutex_t		sched_mutex;
}						t_data;

typedef struct s_dongle
{
	int					id;
	pthread_mutex_t		dongle_mutex;
	long				last_used;
	t_coder				**queue;
	int					size;
	int					capacity;
}						t_dongle;

typedef struct s_coder
{
	int					id;
	int					compile_done;
	long				last_compile;
	pthread_t			thread_id;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	t_data				*data;
}						t_coder;

void					cleanup_simulation(t_data *data, t_coder *coders,
							t_dongle *dongles);
//      parsing
int						parse_args(int ac, char **av, t_data *data);

//      init
int						init_simulation(t_data *data, t_coder **coders,
							t_dongle **dongles);
long					get_time(void);

//      Routine
void					*coder_routine(void *arg);

//      threads
int						create_threads(t_data *data, t_coder *coders,
							t_dongle *dongles);
int						join_threads(t_data *data, t_coder *coders,
							t_dongle *dongles);

//      monitor
void					*monitor_routine(void *arg);
int						check_sim_stop(t_data *data, t_coder *coders);

//      helper!
void					safe_print(t_data *data, int id, char *msg);
void					safe_stop(t_data *data, int value);
int						check_stop(t_data *data);
int						error(char *error_msg);
void					safe_increment(t_coder *coder);

//      routine
void					*coder_routine(void *arg);
void					take_dongle(t_coder *coder, t_dongle *dongle);
void					take_dongles(t_coder *coder);

//      schedler
int						scheduler(t_coder *coder, t_dongle *dongle);
long					get_deadline(t_coder *coder);

//      edf utils
void					swap_dongles(t_dongle *dongle, int curr, int next);
void					heap_push(t_dongle *dongle, t_coder *coder);
void					heap_pop(t_dongle *dongle);
long					get_deadline(t_coder *coder);

#endif // CODEXION_H
