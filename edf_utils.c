#include "codexion.h"

long    get_deadline(t_cder *coder)
{
    long    deadline;

    pthread_mutex_lock(&coder->data->sched_mutex);
    deadline = coder->last_compile + coder->data->time_to_burnout;
    pthread_mutex_unlock(&coder->data->sched_mutex);
    return (deadline);
}

void    heap_push(t_dongle *dongle, t_coder *coder)
{
    int     i;
    int     parent;
    t_cder  *tmp;

    i = dongle->size++;
    dongle->queue[i] = coder;
    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (get_deadline(dongle->queue[i]) < get_deadline(dongle->queue[parent]))
        {
            tmp = dongle->queue[i];
            dongle->queue[i] = dongle->queue[parent];
            dongle->queue[parent] = tmp;
            i = parent;
        }
        else
            break;
    }
}


void heap_pop(t_dongle *dongle)
{
    int i;
    int smallest;
    int left;
    int right;
    t_coder *tmp;
    
    i = 0;
    if (dongle->size == 0) return;
    dongle->queue[0] = dongle->queue[--dongle->size];
    while (1)
    {
        left = 2 * i + 1;
        right = 2 * i + 2;
        smallest = i;
        if (left < dongle->size && get_deadline(dongle->queue[left]) < get_deadline(dongle->queue[smallest]))
            smallest = left;
        if (right < dongle->size && get_deadline(dongle->queue[right]) < get_deadline(dongle->queue[smallest]))
            smallest = right;
        if (smallest != i)
        {
            tmp = dongle->queue[i];
            dongle->queue[i] = dongle->queue[smallest];
            dongle->queue[smallest] = tmp;
            i = smallest;
        }
        else break;
    }
}


