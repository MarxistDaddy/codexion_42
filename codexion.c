#include "codexion.h"

void print_data(t_data *data)
{
    printf("num code:           %i\n", data->number_of_coders);
    printf("time burn:          %i\n", data->time_to_burnout);
    printf("time compile:       %i\n", data->time_to_compile);
    printf("time debug:         %i\n", data->time_to_debug);
    printf("time refact:        %i\n", data->time_to_refactor);
    printf("required compiles   %i\n", data->required_compiles);
    printf("dongle cooldwon     %i\n", data->dongle_cooldown);
    printf("scheduler           %s\n", data->scheduler);
}   

void print_coder(t_coder *c, t_data *d)
{
    printf("\ncoder and data values\n");
    printf("\ndata valus:\n");
    printf("start_time: %li\n", d->start_time / 1000);
    printf("stop: %i\n", d->stop);
    printf("\n======\n");

    printf("id: %d\n", c->id);    
    printf("compile done: %d\n", c->compile_done);    
    printf("last compile: %li\n", c->last_compile / 1000);    
    printf("pointer to thread: %lu\n", c->thread_id);    
    printf("pointer to left dongle: %i\n", c->left_dongle->id);    
    printf("pointer to right dongle: %i\n", c->right_dongle->id);    
    printf("pointer to data->first->item: %d\n", c->data->number_of_coders);    
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
    
    return (0);
}
