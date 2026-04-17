#include "codexion.h"

void release_dongle(t_coder *coder, t_dongle *dongle)
{
    int i;
    int j;

    pthread_mutex_lock(&dongle->dongle_mutex);
    dongle->last_used = get_time();
    i = 0;
    while (i < dongle->size && dongle->queue[i] != coder)
        i++;
    if (i < dongle->size)
    {
        j = i;
        while (j < dongle->size - 1)
        {
            dongle->queue[j] = dongle->queue[j + 1];
            j++;
        }   
        dongle->size--;
    }
    pthread_mutex_unlock(&dongle->dongle_mutex);
}

void take_dongle(t_coder *coder, t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->dongle_mutex);
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
        usleep(500);
        pthread_mutex_lock(&dongle->dongle_mutex);
    }
    pthread_mutex_unlock(&dongle->dongle_mutex);
}

void take_dongles(t_coder *coder)
{
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

void *coder_routine(void *arg)
{
    t_coder *coder;
    coder = (t_coder *)arg;

    while (!check_stop(coder->data))
    {
        take_dongles(coder);
        
        pthread_mutex_lock(&coder->data->sched_mutex);
        coder->last_compile = get_time();
        pthread_mutex_unlock(&coder->data->sched_mutex);
        
        safe_print(coder->data, coder->id, "is compiling");
        usleep(coder->data->time_to_compile * 1000);
        
        release_dongle(coder, coder->left_dongle);
        release_dongle(coder, coder->right_dongle);
        
        pthread_mutex_lock(&coder->data->sched_mutex);
        coder->compile_done++;
        pthread_mutex_unlock(&coder->data->sched_mutex);
        
        safe_print(coder->data, coder->id, "is debugging");
        usleep(coder->data->time_to_debug * 1000);
        
        safe_print(coder->data, coder->id, "is refactoring");
        usleep(coder->data->time_to_refactor * 1000);
    }
    return NULL;
}
