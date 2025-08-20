#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>


typedef struct  s_philo
{
    struct timeval tv;
    pthread_t   tid;
    int         index;
    long        start_time;
    long        time_to_die;
    long        time_to_eat;
    long        nb_of_time_to_eat;
    long        time_lived;
}               t_philo;

typedef struct  s_Mdata
{
    int         nb_of_philo;
    t_philo     *philo_data;
}               t_Mdata;

void get_time_struct(t_philo *philo_data)
{
    gettimeofday(&philo_data->tv, NULL);   
}

long ftime(struct timeval tv)
{
    return((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
long get_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return((tv.tv_sec * 1000) + (tv.tv_usec / 1000)); 
}

void set_philo(t_philo *philo_data)
{
    get_time_struct(philo_data);
    philo_data->start_time = ftime(philo_data->tv);
}
void    wait_til_you_die(t_philo *philo_data)
{
    long now;
    long before;

    before = get_time();
    while(philo_data->time_lived != philo_data->time_to_die)
    {
        usleep((philo_data->time_to_die * 1000) / 2);
        now = get_time();
        philo_data->time_lived = before - now;
        
        //philo_data->time_lived = now - philo_data->start_time; 
    }
    //if (philo_data->time_lived == philo_data->time_to_die)
    printf("philo[%i] died afer living %ld\n%ld/%ld\n", philo_data->index, philo_data->time_lived, philo_data->time_lived, philo_data->time_to_die);
    return;
}

void    *be_philo(void *data)
{
    t_philo *philo_data;
    philo_data = (t_philo *)data;
    set_philo(philo_data);
    printf("\nje pense a l'index %i\ndonc je suis %ld\n", philo_data->index, philo_data->tid);
    printf("je suis née a : %ld\n", philo_data->start_time);
    wait_til_you_die(philo_data);
    return NULL;
}

void set_mdata(t_Mdata *mdata, int ac, char **av)
{
    t_Mdata *temp;

    temp = mdata;
    temp->nb_of_philo                   = atoi(av[1]);
    temp->philo_data                    = malloc(sizeof(t_philo) * temp->nb_of_philo);
    temp->philo_data->time_to_die       = atoi(av[2]);
    temp->philo_data->time_to_eat       = atoi(av[3]);
    temp->philo_data->nb_of_time_to_eat = atoi(av[4]);
}




int main(int ac, char **av)
{
    int i;
    t_Mdata Mdata;

    if (ac < 5 || ac > 6)
        return(printf("Error\nUsage : ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"));  // WTF pourquoi les tab sont inclus meme apres un retour a la ligne :,( 
    
    set_mdata(&Mdata, ac, av);

    printf("debug : value of nb_of_philo = %i\n", Mdata.nb_of_philo);

    i = 0;
    while (i < Mdata.nb_of_philo)
    {
        // set les threads des philos
        // et set leurs struct puis les print pour chaque philo
        // faire ecouler le temps pour tuer un philo qpres son time to die
        Mdata.philo_data[i].index = i;
        pthread_create(&Mdata.philo_data[i].tid, NULL, be_philo, &Mdata.philo_data[i]);
        i++;
    }

    /*
        il faut en faire une fonction de close all qui sera appeler a la fin du program 
        pour "attendre" que tout les threads soit dead avant de finir le program

        elle verrifiera si un thread existe toujour et le fermera
    */
    i = 0;
    while (i < Mdata.nb_of_philo)
    {
        pthread_join(Mdata.philo_data[i].tid, NULL);
        i++;
    }
    return (0);
}





typedef struct perfect_philo
{
    struct timeval tv;
    int         index;
    long        start_time;
}   t_nphilo;

typedef struct new_perfect_struct
{
    int         nb_of_philo;
    t_nphilo    *philo_data;
    pthread_t   *philo;
    int         index;
    long        time_to_die;
    long        time_to_eat;
    long        nb_of_time_to_eat;
    long        time_lived;
}   perf;


/*
    comme ca au debut du programme je peux set tout la struct et malloc chaque sous struct
    ensuite avant chaque appel a la fonction creat thread j'update l'index de la structure "mere" et je la passe a la fonction
    comme ca par la suite je peux faire exactement la meme chose que mon code actuel mais utiliser la mega structure et me baser sur l'index 

    comment evité la data race au moment de l'atribution ? je pense que au moment de cree tout les thread certains auront le meme index, 
    mais il faut essayer quand meme et dans la fonction set philo dire que l'index dans mdata et l'indexe dans philo et peut etre que vu qu'on les appels les un apres les autre ca ira
*/

/*
    Pseudo

    init mega strucure 
    attribuer les av au variables correspondantes 
    tant que index n'est pas == a nbr de philo 
        set l'index de cette iteration dans la struct (megastruct.index = index)
        crée le thread en lui passant megastruct.philo[index] et &megastruct en param de fucntion

    Dans la fonction de thread
    /!\ il faut debug pour voir si les index sont bons, un peut comme le code existant /!\
    set la struct du philo au bon index (et du coup verifier que chaque philo a un index unique)

*/