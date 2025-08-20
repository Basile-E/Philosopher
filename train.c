#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> // pour malloc

long atol(const char *str);

typedef struct s_data
{
    pthread_t   pid;
    long        res;
    long        n;
}               t_data;

void *calculette(void *data)
{
    t_data *my_data = (t_data *)data;
    
    // On récupère le thread ID
    my_data->pid = pthread_self();
    
    // Simple calcul pour exemple
    my_data->res = my_data->n * my_data->n;

    for (long i = 0; i < 1000000000; i++)
    {

    }
    
    printf("Thread %ld: Le carré de %ld est %ld\n", 
           my_data->pid, my_data->n, my_data->res);
           
    return NULL;
}

int main(int ac, char **av)
{
    pthread_t   *thread;
    t_data      *arg_data;
    long        i;

    if (ac < 2)
        return (0);

    // Allocation dynamique pour les threads
    thread = malloc(sizeof(pthread_t) * ac);
    if (!thread)
        return (1);
    
    // Allocation dynamique pour les arguments (un par thread)
    arg_data = malloc(sizeof(t_data) * ac);
    if (!arg_data)
    {
        free(thread);
        return (1);
    }
    
    i = 1;
    while(i < ac)
    {
        // Chaque thread a sa propre copie des données
        arg_data[i].res = 0;
        arg_data[i].n = atol(av[i]);

        // On passe l'adresse de la structure dédiée à ce thread
        pthread_create(&thread[i], NULL, calculette, (void *)&arg_data[i]);
        pthread_join(thread[i], NULL);
        i++;
    }

    // Attendre que tous les threads terminent
    i = 1;
    while(i < ac)
    {
        //pthread_join(thread[i], NULL);
        i++;
    }
    
    // Libération mémoire
    free(thread);
    free(arg_data);
    
    return (0);
}

long atol(const char *str)
{
    long result = 0;
    int sign = 1;

    while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result * sign;
}
