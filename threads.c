#include "codexion.h"


int    create_threads(t_coder *coders, t_data *data)
{
    int i;

    i = 0;
    while (i < data->number_of_coders)
    {
        if (pthread_create(&coders[i].thread_id, NULL, &coder_routine, &coders[i]) != 0)
            return (0);
        i++;
    }
    return (1);
}


int     join_threads(t_coder *coders, t_data *data)
{
    int i;

    i = 0;
    while (i < data->number_of_coders)
    {
        if (pthread_join(coders[i].thread_id, NULL))
            return (0);
        i++;
    }

    return (1);
}
