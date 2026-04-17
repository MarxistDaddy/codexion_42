#ifndef CODEXION_H
#define CODEXION_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>

typedef struct s_coder t_coder;

typedef struct s_data
{
    int     number_of_coders;
    int     time_to_burnout;
    int     time_to_compile;
    int     time_to_debug;
    int     time_to_refactor;
    int     required_compiles;
    int     dongle_cooldown;
    char    *scheduler;     //dynamic strdup!

    pthread_mutex_t print_mutex;
    pthread_mutex_t stop_mutex;
 
    long     start_time; //i will change to long
    int     stop;       //flag but shared
   
    int     current_turn;
    pthread_mutex_t sched_mutex;

    t_coder *coders;

}   t_data;


typedef struct s_dongle
{
    int             id;
    pthread_mutex_t dongle_mutex;
//   pthread_cond_t dongle_cond; 
    long            last_used;  //needs to be in long! gettimeofday!
    
    t_coder     **queue;    //array of pointers to coders waiting | for fifo/edf
    int         size;       //current n of coders waiting
    int         capacity;   //max number allowed which data->number_of_coders!

 //   pthread_cond_t cond;
}   t_dongle;


typedef struct s_coder
{
    int         id;
    int         compile_done;
    long        last_compile;
    pthread_t   thread_id;
    t_dongle    *left_dongle;
    t_dongle    *right_dongle;
    t_data      *data;

}   t_coder;


//      parsing
int     parse_args(int ac, char **av, t_data *data);

//      init
int      init_simulation(t_data *data, t_coder **coders, t_dongle **dongles);
long     get_time();

//      Routine
void    *coder_routine(void *arg);

//      threads
int     create_threads(t_coder *coders, t_data *data);
int     join_threads(t_coder *coders, t_data *data);

//      monitor
void    *monitor_routine(void *arg);
int     check_sim_stop(t_data *data, t_coder *coders);

//      helper!
void    safe_print(t_data *data, int id, char *msg);
void    safe_stop(t_data *data, int value);
int     check_stop(t_data *data);

//      routine
void    *coder_routine(void *arg);
void    take_dongle(t_coder *coder, t_dongle *dongle);
void    take_dongles(t_coder *coder);

//      schedler
int     edf(t_coder *coder, t_dongle *dongle);
int     scheduler(t_coder *coder, t_dongle *dongle);

//      edf utils
void    swap_dongles(t_dongle *dongle, int curr, int next);
void    heap_push(t_dongle *dongle, t_coder *coder);
void    heap_pop(t_dongle *dongle);
long    get_deadline(t_coder *coder);

 #endif //CODEXION_H
