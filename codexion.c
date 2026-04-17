/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamaarab <hamaarab@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 03:35:54 by hamaarab          #+#    #+#             */
/*   Updated: 2026/04/17 03:35:56 by hamaarab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

void cleanup_simulation(t_data *data, t_coder *coders, t_dongle *dongles)
{
    int i;

    // 1. Destroy Global Mutexes
    pthread_mutex_destroy(&data->print_mutex);
    pthread_mutex_destroy(&data->stop_mutex);
    pthread_mutex_destroy(&data->sched_mutex);

    // 2. Cleanup Dongles
    if (dongles)
    {
        i = 0;
        while (i < data->number_of_coders)
        {
            // Destroy the mutex for each dongle
            pthread_mutex_destroy(&dongles[i].dongle_mutex);
            // Free the priority queue array inside each dongle
            if (dongles[i].queue)
                free(dongles[i].queue);
            i++;
        }
        free(dongles);
    }

    // 3. Cleanup Coders and Data strings
    if (coders)
        free(coders);

    // If you used strdup for the scheduler name, free it here
    // free(data->scheduler);
}

int main(int ac, char **av)
{
    t_data      data;
    t_coder     *coders;
    t_dongle    *dongles;
    pthread_t   monitor_id;

    if(!parse_args(ac, av, &data)) 
        return (1);

    printf("Parsing successful!\n");
    //print_data(&data);
    
    if(!init_simulation(&data, &coders, &dongles))
        return (1);

    //print_coder(coders, &data);
    //================================//

    if (!create_threads(coders, &data))
        return (1);

    if (pthread_create(&monitor_id, NULL, monitor_routine, coders) != 0) //order
       return (1);

    if (!join_threads(coders, &data))   //why data here
        return (1);

    if (pthread_join(monitor_id, NULL) != 0) //here null
        return (1);
 
    //clean_simulation()
    return (0);
}
