/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamaarab <hamaarab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 03:36:43 by hamaarab          #+#    #+#             */
/*   Updated: 2026/04/19 01:22:50 by hamaarab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	scheduler(t_coder *coder, t_dongle *dongle)
{
	long	now;

	if (dongle->size == 0)
		return (0);
	if (dongle->queue[0] != coder)
		return (0);
	now = get_time();
	if (now - dongle->last_used < coder->data->dongle_cooldown)
		return (0);
	return (1);
}
