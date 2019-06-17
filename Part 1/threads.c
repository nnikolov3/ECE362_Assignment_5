/* Assignment 5 Part 1
 * Nikolay Nikolov
 */

#include<stdio.h>
#include<assert.h>
#include<pthread.h>
#include<stdlib.h>
#define numThreads 4

void *print_thread_id ( void *ptr);
int i,j;// Global Variables
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

pthread_t thread_id[numThreads];


int counter =0;

int main( int argc, char * argv[]){

    int rc;
    for (i = 0; i < numThreads;i++){


        rc= pthread_create( &thread_id[i], NULL,print_thread_id,(void*)&thread_id[i]);// create the threads
        assert(rc == 0);// error check

    }

    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */

    for (j = 0; j < numThreads ;j++){

        rc =pthread_join(thread_id[j], NULL); // Wait till threads are complete before main continues
        assert (rc == 0);
    }

    printf("We are all Threads!\n Our Master has ID %ld ! \n Our Master knows Kung Fu ! \n", pthread_self());

    exit(0);

}

void *print_thread_id (void *ptr){

    int *ID =(int*)ptr;
    pthread_mutex_lock( &mutex1 );// Mutexes are used for serializing shared resources
    printf(" I'am the Thread %d my ID is %ld !! \n and -> %ls \n" ,counter,pthread_self(),ID); // Print Thread Number and ID number
    counter++;
    pthread_mutex_unlock( &mutex1 );
}


