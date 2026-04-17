/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamaarab <hamaarab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 03:36:03 by hamaarab          #+#    #+#             */
/*   Updated: 2026/04/17 03:36:05 by hamaarab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

static int check_burnout_and_completion(t_data *data, t_coder *coders)
{
    int     i;
    long    now;
    int     all_done;

    all_done = 1;
    now = get_time();
    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_mutex_lock(&data->sched_mutex);
        
        // 1. Burnout Check (Deadline = last_compile + time_to_burnout)
        if (now - coders[i].last_compile >= data->time_to_burnout)
        {
            // Lock print to ensure "burned out" is the absolute last message
            pthread_mutex_lock(&data->print_mutex);
            // Lock stop to prevent others from changing state
            pthread_mutex_lock(&data->stop_mutex);
            
            data->stop = 1;
            printf("%ld %d burned out\n", now - data->start_time, coders[i].id);
            
            pthread_mutex_unlock(&data->stop_mutex);
            pthread_mutex_unlock(&data->print_mutex);
            pthread_mutex_unlock(&data->sched_mutex);
            return (1);
        }

        // 2. Completion Check
        if (data->required_compiles > 0 && coders[i].compile_done < data->required_compiles)
            all_done = 0;

        pthread_mutex_unlock(&data->sched_mutex);
        i++;
    }

    if (data->required_compiles > 0 && all_done)
    {
        pthread_mutex_lock(&data->stop_mutex);
        data->stop = 1;
        pthread_mutex_unlock(&data->stop_mutex);
        return (1);
    }
    return (0);
}

void *monitor_routine(void *arg)
{
    t_coder *coders = (t_coder *)arg;
    t_data  *data = coders[0].data;
    int     i;

    while (1)
    {
        if (check_burnout_and_completion(data, coders))
        {
            i = 0;
            while (i < data->number_of_coders)
            {
                pthread_mutex_lock(&coders[i].left_dongle->dongle_mutex);
                pthread_mutex_unlock(&coders[i].left_dongle->dongle_mutex);
                i++;
            }
            break;
        }
        usleep(500); 
    }
    return (NULL);
}
