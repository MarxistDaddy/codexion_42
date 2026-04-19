/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamaarab <hamaarab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 03:36:03 by hamaarab          #+#    #+#             */
/*   Updated: 2026/04/19 23:10:07 by hamaarab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_coder_burned_out(t_data *data, t_coder *coder, long now)
{
	if (now - coder->last_compile >= data->time_to_burnout)
	{
		pthread_mutex_lock(&data->print_mutex);
		pthread_mutex_lock(&data->stop_mutex);
		data->stop = 1;
		printf("%ld %d burned out\n", now - data->start_time, coder->id);
		pthread_mutex_unlock(&data->stop_mutex);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	return (0);
}

int	check_burnout_and_completion(t_data *data, t_coder *coders)
{
	int		i;
	long	now;
	int		all_done;

	all_done = 1;
	now = get_time();
	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->sched_mutex);
		if (is_coder_burned_out(data, &coders[i], now))
		{
			pthread_mutex_unlock(&data->sched_mutex);
			return (1);
		}
		if (data->required_compiles > 0
			&& coders[i].compile_done < data->required_compiles)
			all_done = 0;
		pthread_mutex_unlock(&data->sched_mutex);
		i++;
	}
	if (data->required_compiles > 0 && all_done)
		return (2);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_coder	*coders;
	t_data	*data;
	int		status;

	coders = (t_coder *)arg;
	data = coders[0].data;
	while (1)
	{
		status = check_burnout_and_completion(data, coders);
		if (status != 0)
		{
			pthread_mutex_lock(&data->stop_mutex);
			data->stop = 1;
			pthread_mutex_unlock(&data->stop_mutex);
			break ;
		}
		usleep(50);
	}
	return (NULL);
}
