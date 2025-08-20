#include <stdio.h>
#include <pthread.h>

long atol(const char *str);

typedef struct s_data
{
    pthread_mutex_t n_mutex;
    pthread_t   pid;
    long        res;
    long        n;
}               t_data;

void *print_thread(void *data)
{
    long n;
    pthread_t pid;

    n = (long)(data);
    pid = pthread_self();

    printf("\nCurrent Thread : %ld\n called with the value : %ld\n\n",pid, n);
    return (NULL);
}


void *calculette(void *data)
{
    t_data *arg_data;

    arg_data = (t_data *)data;
    arg_data->pid = pthread_self();
    
    pthread_mutex_lock(&arg_data->n_mutex);
    arg_data->res = arg_data->res + arg_data->n;
    pthread_mutex_unlock(&arg_data->n_mutex);
    
    printf("hello from the thread tid : %ld\nMy Value of N is : %ld\n", arg_data->pid, arg_data->n);
}

int main(int ac, char **av)
{
    pthread_t   thread[ac];
    t_data      arg_data;
    long        i;

    i = 1;

    if (ac < 2)
        return (0);

    pthread_mutex_init(&arg_data.n_mutex, NULL);
    arg_data.res = 0;
    while(i < ac)
    {
        arg_data.n = atol(av[i]);
        pthread_create(&thread[i], NULL, calculette, (void *)&arg_data);
        //pthread_join(thread[i], NULL);
        i++;
    }

    

    // while(i < ac)
    // {
    //     pthread_create(&thread[i], NULL, print_thread, (void *) i);
    //     pthread_join(thread[i], NULL);
    //     i++;
    // }
    
    i = 1;
    while (i < ac)
    {
        pthread_join(thread[i], NULL);
        i++;
    }

    printf("the res is : %ld\n", arg_data.res);

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
