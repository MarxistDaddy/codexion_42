/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamaarab <hamaarab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 03:35:54 by hamaarab          #+#    #+#             */
/*   Updated: 2026/04/19 02:55:17 by hamaarab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup_simulation(t_data *data, t_coder *coders, t_dongle *dongles)
{
	int	i;

	if (dongles)
	{
		i = 0;
		while (i < data->number_of_coders)
		{
			pthread_mutex_destroy(&dongles[i].dongle_mutex);
			if (dongles[i].queue)
				free(dongles[i].queue);
			i++;
		}
		free(dongles);
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	pthread_mutex_destroy(&data->sched_mutex);
	if (coders)
		free(coders);
}

int	main(int ac, char **av)
{
	t_data		data;
	t_coder		*coders;
	t_dongle	*dongles;
	pthread_t	monitor_id;

	if (!parse_args(ac, av, &data))
		return (1);
	if (!init_simulation(&data, &coders, &dongles))
		return (1);
	if (!create_threads(coders, &data))
    {
        cleanup_simulation(&data, coders, dongles);
		return (1);
    }
	if (pthread_create(&monitor_id, NULL, monitor_routine, coders) != 0)
    {
        cleanup_simulation(&data, coders, dongles);
		return (1);
    }
	if (pthread_join(monitor_id, NULL) != 0)
		return (1);
	if (!join_threads(coders, &data))
		return (1);
	cleanup_simulation(&data, coders, dongles);
	return (0);
}
