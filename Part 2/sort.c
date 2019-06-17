/* Assignment 5 Part 1
 * Nikolay Nikolov
 */

/* libraries */
#include<stdio.h>
#include<assert.h>
#include<pthread.h>
#include<stdlib.h>
#include<limits.h>
#include<sys/time.h>
/* #defines */
#define THREADS 4 /* Number of threads */
#define NUMBERS_TO_SORT 64000 /* Number of numbers to sort */
#define NUMS_PER_THREAD (NUMBERS_TO_SORT/THREADS) /* number to sort per thread */

/* Global */

/* Lists */
int  nums[NUMBERS_TO_SORT]; /* Array for the numbers */
int    *sorted_nums[3]; /* Array for the sorted numbers */
int    *complete; /* Array for all of the sorted numbers */
int    *merged[1]; /* Array for the merged arrays */

/* Variables */
int count;
long selfid1;
long  selfid2;
long masterid;
pthread_t       tid[THREADS]; // the Thread ID
pthread_t       thr[2];

/* Barrier */
pthread_barrier_t b;
pthread_barrier_t a;

/* Locks */
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

/* Prototypes */ 
void *thead_function(void *arg);
void *merge(void *arg);
void checksort();
void destructor(void* arg);

/* Create Thread argument struct */
typedef struct _thread_data_t
{
    int tid;
    long index;

} thread_data_t;

/*  Worker thread to sort a portion of the set of numbers
 *  using Bubble Sort */
void *thread_function( void *arg) //void * arg 
{
    /* Passing the variable */
    thread_data_t *data = (thread_data_t *) arg;

    /* Setting the index and the id to pass */
    long index = data->index; // GETTING THE INDEX VALUE
    int id     = data->tid; // Getting an id value different 
    /* Array to be sorted */
    int *array_sort = &nums[index]; 
    /*variable to use for sorting and counting */
    int temp = 0;
    int x ;
    int y ; 

    /* n is just for making is more simple */
    int n = NUMS_PER_THREAD; 

    /* The loop that keeps track of the entire list */
    for (x =0;x <(n-1);x++)
    {
        for (y = 0;y < n-x-1;y++)
        {
            if(array_sort[y] > array_sort[y+1])
            {
                temp = array_sort[y];
                array_sort[y] = array_sort[y+1]; /* Swapping */
                array_sort[y+1] = temp;
            };
        }
    }
    /* Copy the array */
    if(array_sort==NULL)
    {
        printf("bump index: %d\n",id);
    }

    sorted_nums[id] = malloc((NUMS_PER_THREAD-1)*sizeof(int)); 
    sorted_nums[id] = array_sort;   
    pthread_barrier_wait(&b);
}
/* Merge the results of the individual sorted ranges */
void *merge(void *arg)
{
    
    /* Passing the variable */
    thread_data_t *data = (thread_data_t *) arg;

    /* Setting the id to pass */ 
    int id     = data->tid; // Getting an id value different 
    int tid;


    /* Temporary array */
    int temp[NUMBERS_TO_SORT/2];

    /* Variables for counting */

    if ( id == 0) tid = 0;
    else tid = 1;

    /* The two arrays that need to be sorted */
    int  *left = NULL;
    int  *right = NULL; 

    if ( tid == 0 )
    {
     //   while(sorted_nums[0] == NULL || sorted_nums[1] == NULL); // spin
        left = sorted_nums[0];
        right = sorted_nums[1];
    }
    else if ( tid == 1 )
    {
     //   while(sorted_nums[2] == NULL || sorted_nums[3] == NULL); // spin
        left = sorted_nums[2];
        right = sorted_nums[3];
    }
    else
    {
        // this will never get hit
    }

    if(left==NULL || right == NULL)
    {
        printf("You are about to have a really bad day...\n");
    }
    else
    {
        //printf("Good\n");
    }

    /* Variables for counting */
    int x = 0;
    int y = 0;
    int k = 0;

    while ( k < NUMS_PER_THREAD && y < NUMS_PER_THREAD)
    {
        if (left[k] <=  right[y]) /* if the integer from left is smaller */
        {
            temp[x] = left[k]; /*  place it in the temp array */
            x++;
            k++; /* increment the counter for the left */
        }
        else if(right [y] < left[k])
        {
            temp[x] = right[y];
            x++;
            y++;

        } 
    }
    //printf("Good\n");

    /* if the counter for the right is larger than NUM_PER_THREAD  then we are left 
       with only the numbers from left which we will put at 
       the end of the list */

    while(k != NUMS_PER_THREAD){
        temp[x] = left[k];
        ++x;
        k++;
    }
    /* if the counter for the left is larger than NUM_PER_THREAD 
       we have to append the number from the right 
       at the end of the temp array */

    while(y !=  NUMS_PER_THREAD){
        temp[x] = right[y];
        x++;
        ++y;
    }

    pthread_barrier_wait(&a);
    merged[count] = malloc((NUMS_PER_THREAD*2+2)*sizeof(int));
    merged[count] = temp;
    pthread_mutex_lock( &mutex1);
    count++;
    pthread_mutex_unlock( &mutex1);

}

/* This is master merge function */
void mastermerge(){

    /* The two arrays that need to be sorted */
    int  *left;
    int  *right;

    /* Temporary array */
    int temp[NUMBERS_TO_SORT+1];

     left  = merged[0];
    right = merged[1];

    if (left == NULL || right == NULL)
    {
        perror("List is empty \n");
    }
    /* Variables for counting */
    int x = 0;
    int y = 0;
    int k= 0;

    while ( k < NUMS_PER_THREAD*2 && y < NUMS_PER_THREAD*2)
    {

        if (left[k] <= right[y]) /* if the integer from left is smaller */
        {
            temp[x] = left[k]; /*  place it in the temp array */
            k++; /* increment the counter for the left */
        }
        else/* Otherwise place the integer from the right in the temp array */ 
        {
            temp[x]=right[y];
            y++;
        }
        x++;
    }
    /* if the counter for the right is larger than NUM_PER_THREAD  then we are left 
       with only the numbers from left which we will put at 
       the end of the list */

    while (k != NUMS_PER_THREAD*2){
        temp[x] = left[k];
        k++;
        x++;
    }
    /* if the counter for the left is larger than NUM_PER_THREAD 
       we have to append the number from the right 
       at the end of the temp array */

    while (y != NUMS_PER_THREAD*2){
        temp[x] = right[y];
        y++;
        x++;
    }
    complete = malloc((NUMBERS_TO_SORT)*sizeof(int));
    complete = temp;
}

/* Check if the resultant list is sorted */
void checksort(){

    int *list = complete;
    int i = 0;

    while (i < NUMBERS_TO_SORT - 1 )
    {
        if (list[i] <=list[i+1])
        {
         i++;
        }

    i++;
    }
    printf("List is sorted \n ");

}

/* Main starts here */
int main(){

    /*Initializations */
    unsigned long   i;
    struct          timeval start, end; // Start and end for the timer
    long long       startusec, endusec; // Mesuring in seconds
    double          elapsed; // timer elapsed
    int             err; // error checking

    count =0;
    /* ID for the Master */
    masterid = pthread_self();

    /* Create a thread_data_t arrugment array */
    thread_data_t thr_data[THREADS];

    /* Create the initial set of numbers to sort.*/
    srandom(1);
    for (i = 0 ; i < NUMBERS_TO_SORT; i++)
    {
        nums[i] = random(); // Random number generator
    }

    /* Start timer */
    gettimeofday(&start, NULL);

    /* Create a barrier */
    err = pthread_barrier_init(&b,NULL,4);
    if (err != 0)
    {
        perror("error! \n"); /* error check */
        exit(0);

    }

    /* Make 4 threads */ 
    for( i=0; i <THREADS ; i++)
    {
        thr_data[i].tid    = i;
        thr_data[i].index  = i*NUMS_PER_THREAD;
        err = pthread_create( &tid[i], NULL,thread_function, &thr_data[i]); //CREATE A THREAD
        /* Error Check */
        if (err !=0)
        {
            perror("Error! \n");
            exit(0); 
        }
    }


    /* Wait until all threads are done */
    for (i = 0; i < THREADS ;i++)
    {
        int rc =pthread_join(tid[i], NULL); // Wait till threads are complete before main continues
        assert (rc == 0);
    }
    ////////////////////////////
    //

    /* Create a barrier */
    err = pthread_barrier_init(&a,NULL,2);
    if (err != 0)
    {
        perror("error! \n"); /* error check */
        exit(0);

    }

    /* Make two more  threads */ 
    for( i=0; i <2 ; i++)
    {
        thr_data[i].tid    = i;
        thr_data[i].index  = i*NUMS_PER_THREAD;
        err = pthread_create( &tid[i], NULL,merge, &thr_data[i]); //CREATE A THREAD
        /* Error Check */
        if (err !=0)
        {
            perror("Error! \n");
            exit(0);
        }
    }

    /* Wait until all threads are done */
    for (i = 0; i < 2 ;i++)
    {
        int rc =pthread_join(tid[i], NULL); // Wait till threads are complete before main continues
        assert (rc == 0);
    }
    /////////////////////////////////

    /* The Master Thread Calls Merge */
    mastermerge();
    /* Turn off the timer */
    gettimeofday(&end, NULL); // STOP TIMER
    checksort();
    /* Check if the list is sorted */
    /* Getting the elapsed time from the timer */
    startusec = start.tv_sec * 1000000 + start.tv_usec; // MAGIC - TIME START
    endusec   = end.tv_sec *1000000  + end.tv_usec; // MAGIC - TIME END
    elapsed   = (double) ( endusec - startusec)/1000000 ;// ELAPSED = TIME END  - TIME START 
    printf (" Sort took %7.f seconds \n", elapsed);

    exit(0);
}


