/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamaarab <hamaarab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 03:35:44 by hamaarab          #+#    #+#             */
/*   Updated: 2026/04/17 03:35:48 by hamaarab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	init_dongles(t_data *data, t_dongle *dongles)
{
	for (int i = 0; i < data->number_of_coders; i++)
	{
		dongles[i].id = i;
		dongles[i].last_used = 0;
		pthread_mutex_init(&dongles[i].dongle_mutex, NULL);
		dongles[i].capacity = data->number_of_coders;
		dongles[i].size = 0;
		dongles[i].queue = malloc(sizeof(t_coder *) * data->number_of_coders);
		if (!dongles[i].queue)
			return (0);
	}
	return (1);
}

int	init_coders(t_data *data, t_coder *coders, t_dongle *dongles)
{
	for (int i = 0; i < data->number_of_coders; i++)
	{
		coders[i].id = i + 1;
		coders[i].compile_done = 0;
		coders[i].last_compile = data->start_time;
		coders[i].data = data;
		coders[i].left_dongle = &dongles[i];
		coders[i].right_dongle = &dongles[(i + 1) % data->number_of_coders];
	}
	return (1);
}

int	init_simulation(t_data *data, t_coder **coders, t_dongle **dongles)
{
	// free!
	*coders = malloc(sizeof(t_coder) * data->number_of_coders);
	*dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!*coders || !*dongles)
		return (0);
	// stop flag + start time marker!
	data->start_time = get_time();
	data->stop = 0;
	// init print and stop mutex
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->stop_mutex, NULL);
	// critical for fifo
	data->current_turn = 1;
	pthread_mutex_init(&data->sched_mutex, NULL); //..
	// critical for edf
	data->coders = *coders;
	if (!init_dongles(data, *dongles))
		return (0);
	if (!init_coders(data, *coders, *dongles))
		return (0);
	return (1);
}
