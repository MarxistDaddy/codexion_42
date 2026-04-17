#include "codexion.h"

void safe_print(t_data *data, int id, char *msg)
{
    pthread_mutex_lock(&data->print_mutex);
    pthread_mutex_lock(&data->stop_mutex); // Lock the stop flag

    if (!data->stop)
    {
        printf("%ld %d %s\n", get_time() - data->start_time, id, msg);
    }

    pthread_mutex_unlock(&data->stop_mutex);
    pthread_mutex_unlock(&data->print_mutex);
}

void safe_stop(t_data *data, int value)
{
    pthread_mutex_lock(&data->stop_mutex);
    data->stop = value;
    pthread_mutex_unlock(&data->stop_mutex);
}

int check_stop(t_data *data)
{
    int stop;
    pthread_mutex_lock(&data->stop_mutex);
    stop = data->stop;
    pthread_mutex_unlock(&data->stop_mutex);
    return stop;
}
