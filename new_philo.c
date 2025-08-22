#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>


/*Struct*/
typedef struct s_mega t_mega;

typedef struct  s_args
{
    int         nb_of_philo;
    long        time_to_die;
    long        time_to_eat;
    long        time_to_sleep;
    int         nb_of_time_to_eat;
}               t_args;

typedef struct  s_philo
{
    struct timeval tv;
    pthread_t   tid;
    int         index;
    long        start_time;
    long        time_lived;
    long        last_meal;
    t_args      *args;
    t_mega      *mega;
    pthread_mutex_t     *left_fork;
    pthread_mutex_t     *right_fork;
}               t_philo;

typedef struct  s_mega
{
    t_args      *args;
    t_philo     *philos;
    pthread_mutex_t     *mutex;
    int         simulation_active;
    pthread_mutex_t     *simulation_mutex;
}               t_mega;
/*Struct*/


/*utils general*/
void    print_debug_args(t_args *args)
{
    printf("\nDebug Args\n");
    printf("av[1] Number of Philo       : %i\n", args->nb_of_philo);
    printf("av[2] Time to die           : %ld\n", args->time_to_die);
    printf("av[3] Time to eat           : %ld\n", args->time_to_eat);
    printf("av[4] Time to sleep         : %ld\n", args->time_to_sleep);
    printf("av[5] Number of time to eat : %i\n", args->nb_of_time_to_eat);
}

void    print_debug_mega(t_mega *mega)
{
    int i;

    print_debug_args(mega->args);
    i = 0;
    while(i < mega->args->nb_of_philo)
    {
        printf("\nDebug of philo[%i]\n", mega->philos[i].index);
        printf("test arg du philo[%i], time to die : %ld\n", mega->philos[i].index, mega->philos[i].args->time_to_die);
        i++;
    }
}

/*utils general*/

/*Time Fonctions*/
long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return((tv.tv_sec * 1000) + (tv.tv_usec / 1000)); 
}
/*Time Fonctions*/

/*Set Fonctions*/

void    set_args(t_args *args, char **av)
{
    args->nb_of_philo       = atoi(av[1]);
    args->time_to_die       = atoi(av[2]);
    args->time_to_eat       = atoi(av[3]);
    args->time_to_sleep     = atoi(av[4]);
    args->nb_of_time_to_eat = atoi(av[5]);
}

void    set_philo(t_mega *mega, int index)
{
    mega->philos[index].args = mega->args; // set les args de chaque philo sur l'adresse de la struct arg dans mega
    mega->philos[index].mega = mega;
    mega->philos[index].start_time = 0;
    mega->philos[index].time_lived = 0;
    mega->philos[index].last_meal = get_time();
    if (index == mega->args->nb_of_philo - 1)
    {
        mega->philos[index].right_fork = &mega->mutex[index];
        mega->philos[index].left_fork = &mega->mutex[0];
    }
    else
    {
        mega->philos[index].right_fork = &mega->mutex[index];
        mega->philos[index].left_fork = &mega->mutex[index+1];
    }
}

void    set_mega(t_mega *mega)
{
    int i;
    int nbr;

    i = 0;
    nbr = mega->args->nb_of_philo; // c'est pour ca qu'on set mega apres arg, c'est un peu con en vrais mais ¨\_('-')_/¨
    mega->philos = malloc(sizeof(t_philo) * nbr);
    mega->mutex = malloc(sizeof(pthread_mutex_t) * nbr);
    mega->simulation_active = 1;
    mega->simulation_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mega->simulation_mutex, NULL);

    while(i < nbr)
    {
        pthread_mutex_init(&mega->mutex[i], NULL);
        i++;
    }
    i = 0;
    while(i < nbr)
    {
        mega->philos[i].index = i;
        set_philo(mega, i);
        i++;
    }
}

/*Set Fonctions*/

/*threads Fonctions*/

/*
    il faut : 
    
    une fonction check_if_dead qui verifie si le time lived est superieur au time to live
    une fonction add_time(t_philo *philo, long time) qui ajoute le time a philo.time_lived
    une fonction go_eat, dans cette fonction il faut une fonction try to take fork (ou est-ce que la fonction doit etre apres)


*/

/*
    il doit y avoir une boucle qui check que tout les philo ne soit pas encore mort (ou seulement un soit mort ?)
    dans cette boucle, les philo doivent essayer de manger, tant qu'ils ne peuvent pas manger ils pensent, manger, dormir, etc 

*/


/*
    faire un serveur basique qui fait manger et penser les philo sans prendre en compte leurs time to starve(time to die)
*/



void    think(t_philo *philo)
{
    long time;

    time = get_time();
    printf("[%ld] Philo[%i] is thinking\n",time, philo->index);
}

void    eat(t_philo *philo)
{
    long time;

    time = get_time();
    printf("[%ld] Philo[%i] is eating\n", time, philo->index);
    usleep(philo->args->time_to_eat * 1000);
    philo->last_meal = get_time();
}

void    die(t_philo *philo)
{
    long time;

    time = get_time();
    printf("[%ld] Philo[%i] is dead\n", time, philo->index);
    pthread_mutex_lock(philo->mega->simulation_mutex);
    philo->mega->simulation_active = 0;
    pthread_mutex_unlock(philo->mega->simulation_mutex);
}

int     try_to_eat(t_philo *philo)
{
    pthread_mutex_lock(philo->right_fork);
    pthread_mutex_lock(philo->left_fork);
    eat(philo);
    pthread_mutex_unlock(philo->right_fork);
    pthread_mutex_unlock(philo->left_fork);
}

int     is_dead(t_philo *philo)
{
    long time;

    time = get_time();
    if (time - philo->last_meal > philo->args->time_to_die)
    {
        die(philo);
        return(1);
    }
    else
        return(0);
}

void    go_sleep(t_philo *philo)
{
    long time;

    time = get_time();
    
    printf("[%ld] Philo[%i] went to sleep\n", time, philo->index);
    usleep(philo->args->time_to_sleep * 1000);
}

void    *be_philo(void *data)
{
    t_philo *philo;
    long    now;
    
    philo = (t_philo *)data; 
    //printf("\nje pense a l'index %i\ndonc je suis %ld\n", philo->index, philo->tid);
    while(philo->mega->simulation_active && is_dead(philo) == 0)
    {
        philo->start_time = get_time();
        think(philo);
        try_to_eat(philo);
        go_sleep(philo);
        now = get_time();
        philo->time_lived = now - philo->start_time;
    }
    //int i;
    
    // i = 0;
    // while (i < philo->args->nb_of_philo)
    // {
    //     pthread_join(philo->tid, NULL);
    //     i++;
    // }
}


/*Threads Fonctions*/



int main(int ac, char **av)
{
    t_mega *mega;
    int    i;

    /*vérrif*/
    if (ac < 5 || ac > 6)
        return(printf("Error\nUsage : ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"));  // WTF pourquoi les tab sont inclus meme apres un retour a la ligne :,( 
    mega = malloc(sizeof(t_mega));
    if (!mega)
        return("failed to malloc mega", 1);
    mega->args = malloc(sizeof(t_args));
    if (!mega->args)
        return("failled to malloc args", 1);
    /*vérrif*/

    set_args(mega->args, av);
    set_mega(mega);
    //set_philo(mega->philos, mega->args);
    print_debug_mega(mega);

    /*set toutes les donnés, sera une fonction set_all qui prend mega et av en params*/

    i = 0;
    while(i < mega->args->nb_of_philo)
    {
        pthread_create(&mega->philos[i].tid, NULL, be_philo, &mega->philos[i]);
        i++;
    }

    i = 0;
    while (i < mega->args->nb_of_philo)
    {
        pthread_join(mega->philos[i].tid, NULL);
        i++;
    }

    //usleep(1000); //sert a attendre la fin du programme pour remplacer les joins (le premier thread fini trop vite donc le programme se ferme)

}
