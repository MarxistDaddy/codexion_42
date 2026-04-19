/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamaarab <hamaarab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 03:36:30 by hamaarab          #+#    #+#             */
/*   Updated: 2026/04/19 01:37:05 by hamaarab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_deadline(t_coder *coder)
{
	long	deadline;

	pthread_mutex_lock(&coder->data->sched_mutex);
	deadline = coder->last_compile + coder->data->time_to_burnout;
	pthread_mutex_unlock(&coder->data->sched_mutex);
	return (deadline);
}

void	swap_dongles(t_dongle *dongle, int curr, int next)
{
	t_coder	*tmp;

	tmp = dongle->queue[curr];
	dongle->queue[curr] = dongle->queue[next];
	dongle->queue[next] = tmp;
}

void	heap_push(t_dongle *dongle, t_coder *coder)
{
	int	i;
	int	parent;

	i = dongle->size++;
	dongle->queue[i] = coder;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (get_deadline(dongle->queue[i]) < get_deadline(dongle->queue[parent]))
		{
			swap_dongles(dongle, i, parent);
			i = parent;
		}
		else
			break ;
	}
}

void	heap_pop(t_dongle *dongle)
{
	int	i;
	int	smallest;
	int	left;
	int	right;

	i = 0;
	if (dongle->size == 0)
		return ;
	dongle->queue[0] = dongle->queue[--dongle->size];
	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < dongle->size
			&& get_deadline(dongle->queue[left]) < get_deadline(dongle->queue[smallest]))
			smallest = left;
		if (right < dongle->size
			&& get_deadline(dongle->queue[right]) < get_deadline(dongle->queue[smallest]))
			smallest = right;
		if (smallest != i)
		{
			swap_dongles(dongle, i, smallest);
			i = smallest;
		}
		else
			break ;
	}
}
