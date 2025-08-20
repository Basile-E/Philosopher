#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct  s_data
{
    pthread_t   mutex_cout;
    int         counter_value;
}               t_data;


void *counterinette(void *data)
{
    int i;
    t_data *temp;

    temp = (t_data *)data;
    i = 0;
    pthread_mutex_lock(temp->mutex_cout);
    printf("counter value at the beg : %i", temp->counter_value);
    while(i < 21000)
    {
        temp->counter_value++;
        i++;
    }
    printf("counter value at the end : %i", temp->counter_value);
}

int main(int ac, char **av)
{
    int i;
    pthread_t thread[ac];
    t_data data;


    i = 0;
    if (ac == 1)
        return (0);

    while(i < ac)
    {
        pthread_create(&thread[i], NULL, counterinette, &data);
        i++;
    }
}