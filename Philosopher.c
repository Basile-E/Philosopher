#include <stdio.h> // take a wild guess
#include <pthread.h> // les threads 
#include <stdlib.h> // pour malloc
#include <sys/time.h> // pour get time 
#include <unistd.h> // pour sleep & wait

typedef struct s_philo
{
    pthread_t   tid;
    int         index;
    long        time_of_creation;
    long        time_to_die;
    long        time_to_eat;
    long        time_to_sleep;
    long        time_since_last_meal;
}               t_philo;

long    set_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void    *dining(void *data)
{
    t_philo *philo;

    philo = (t_philo *)data;
    philo->tid = pthread_self();
    philo->time_of_creation = set_time(philo);
    printf("time of creation of philosopher index : %i [%ld] :\n%ld\n", philo->index, philo->time_of_creation, philo->tid);
}

int     main(int ac, char **av)
{
    pthread_t   thread[ac];
    t_philo     philo_data[ac];
    int         i;

    i = 0;
    while(i < ac)
    {
        philo_data->index = i;
        philo_data->time_to_die = 1000;
        philo_data->time_to_eat = 500;
        philo_data->time_to_sleep = 500;
        i++;
    }

    i = 0;
    while(i < ac)
    {
        pthread_create(&thread[i], NULL, dining, &philo_data[i]);
        i++;
    }

    i = 0;
    while(i < ac)
    {
        pthread_join(thread[i], NULL);
        i++;
    }
   
    return (0);
}
