/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamaarab <hamaarab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 03:36:03 by hamaarab          #+#    #+#             */
/*   Updated: 2026/04/18 21:34:15 by hamaarab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

// 1. Change this function to return a specific state
static int check_burnout_and_completion(t_data *data, t_coder *coders)
{
    int i;
    long now;
    int all_done;

    all_done = 1;
    now = get_time();
    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_mutex_lock(&data->sched_mutex);
        
        // --- BURNOUT CASE ---
        if (now - coders[i].last_compile >= data->time_to_burnout)
        {
            pthread_mutex_lock(&data->print_mutex);
            pthread_mutex_lock(&data->stop_mutex);
            data->stop = 1; // ONLY set stop here!
            printf("%ld %d burned out\n", now - data->start_time, coders[i].id);
            pthread_mutex_unlock(&data->stop_mutex);
            pthread_mutex_unlock(&data->print_mutex);
            pthread_mutex_unlock(&data->sched_mutex);
            return (1); // Burnout detected
        }

        if (data->required_compiles > 0 && coders[i].compile_done < data->required_compiles)
            all_done = 0;

        pthread_mutex_unlock(&data->sched_mutex);
        i++;
    }

    // --- SUCCESS CASE ---
    if (data->required_compiles > 0 && all_done)
    {
        // DO NOT set data->stop = 1 here.
        // Just return 2 to signal that work is finished normally.
        return (2); 
    }
    return (0);
}

// 2. Update the routine to react to the return values
void *monitor_routine(void *arg)
{
    t_coder *coders = (t_coder *)arg;
    t_data *data = coders[0].data;
    int status;

    while (1)
    {
        status = check_burnout_and_completion(data, coders);
        if (status == 1) // Burnout
        {
            break; 
        }
        else if (status == 2) // Success
        {
            // Simply exit the monitor. 
            // The coder threads will see their compile_done count 
            // in their next loop and exit gracefully.
            break;
        }
        usleep(250); 
    }
    return (NULL);
}



