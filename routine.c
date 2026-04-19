/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamaarab <hamaarab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 03:36:10 by hamaarab          #+#    #+#             */
/*   Updated: 2026/04/19 02:18:21 by hamaarab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	int	i;

	pthread_mutex_lock(&dongle->dongle_mutex);
	dongle->last_used = get_time();
	if (strcmp(coder->data->scheduler, "edf") == 0)
		heap_pop(dongle);
	else
	{
		i = 0;
		while (i < dongle->size - 1)
		{
			dongle->queue[i] = dongle->queue[i + 1];
			i++;
		}
		dongle->size--;
	}
	pthread_mutex_unlock(&dongle->dongle_mutex);
}

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	if (strcmp(coder->data->scheduler, "edf") == 0)
		heap_push(dongle, coder);
	else
		dongle->queue[dongle->size++] = coder;
	while (!check_stop(coder->data))
	{
		if (scheduler(coder, dongle))
		{
			safe_print(coder->data, coder->id, "has taken a dongle");
			pthread_mutex_unlock(&dongle->dongle_mutex);
			return ;
		}
		pthread_mutex_unlock(&dongle->dongle_mutex);
		usleep(250);
		pthread_mutex_lock(&dongle->dongle_mutex);
	}
	pthread_mutex_unlock(&dongle->dongle_mutex);
}

void	take_dongles(t_coder *coder)
{
	if (coder->data->number_of_coders == 1)
	{
		take_dongle(coder, coder->left_dongle);
		while (!check_stop(coder->data))
			usleep(500);
		return ;
	}
	if (coder->id % 2 == 0)
	{
		take_dongle(coder, coder->right_dongle);
		take_dongle(coder, coder->left_dongle);
	}
	else
	{
		take_dongle(coder, coder->left_dongle);
		take_dongle(coder, coder->right_dongle);
	}
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (1)
	{
		take_dongles(coder);
		pthread_mutex_lock(&coder->data->sched_mutex);
		coder->last_compile = get_time();
		pthread_mutex_unlock(&coder->data->sched_mutex);
		safe_print(coder->data, coder->id, "is compiling");
		usleep(coder->data->time_to_compile * 1000);
		release_dongle(coder, coder->left_dongle);
		release_dongle(coder, coder->right_dongle);
		safe_increment(coder);
		safe_print(coder->data, coder->id, "is debugging");
		usleep(coder->data->time_to_debug * 1000);
		safe_print(coder->data, coder->id, "is refactoring");
		usleep(coder->data->time_to_refactor * 1000);
		if (coder->compile_done >= coder->data->required_compiles
			|| check_stop(coder->data))
			break ;
	}
	return (NULL);
}
