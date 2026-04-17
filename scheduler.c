#include "codexion.h"

int edf(t_coder *coder, t_dongle *dongle)
{
    int earliest_idx;
    long earliest_deadline;
    long deadline;
    int i;

    earliest_idx = 0;
    earliest_deadline = dongle->queue[0]->last_compile + dongle->queue[0]->data->time_to_burnout;
    i = 1;

    while (i < dongle->size)
    {
        deadline = dongle->queue[i]->last_compile
                        + dongle->queue[i]->data->time_to_burnout;
        if (deadline < earliest_deadline)
        {
            earliest_deadline = deadline;
            earliest_idx = i;
        }
        i++;
    }
    return dongle->queue[earliest_idx] == coder;
}

int fifo(t_dongle *dongle, t_coder *coder)
{
    long    now;

    if (dongle->queue[0] != coder)
        return (0);
    now = get_time();
    //whats this??
    if (now - dongle->last_used < coder->data->dongle_cooldown)
        return (0);
    return (1);
}


int scheduler(t_coder *coder, t_dongle *dongle)
{
    if (dongle->size == 0)
        return (0); //no coder in queue!

    if (strcmp(coder->data->scheduler, "fifo") == 0)
        return (fifo(dongle, coder));

    else if (strcmp(coder->data->scheduler, "edf") == 0)
        return (edf(coder, dongle));

    return (0);
}


