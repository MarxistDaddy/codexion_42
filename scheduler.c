#include "codexion.h"

int scheduler(t_coder *coder, t_dongle *dongle)
{
    long now;

    // 1. Safety check: Is anyone even waiting?
    if (dongle->size == 0)
        return (0);

    // 2. Position check: Are you the "King of the Hill"?
    // If it's FIFO, Index 0 is the first arrival.
    // If it's EDF, Index 0 is the earliest deadline (thanks to heap_push).
    if (dongle->queue[0] != coder)
        return (0);

    // 3. Hardware Cooldown: Has the dongle "rested" enough?
    now = get_time();
    if (now - dongle->last_used < coder->data->dongle_cooldown)
        return (0);

    // If you passed both checks, you get the dongle!
    return (1);
}
