#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define P(sem) sem_wait(sem)
#define V(sem) sem_post(sem)
#define N 50


sem_t sem;
int i;

void *action(void *arg){
	int x = (int)arg;    
	usleep(rand()%1000000);
	V(&sem);
	printf("Thread %d over \n", x);
	pthread_exit(NULL);
}

void *barrier(void *arg){
	int j;
	for(j = 0; j < N; j++)P(&sem);
	printf("Barrier Thread Over\n");
	pthread_exit(NULL);
} 

int main(void){
	srand(time(NULL));
	i = 0;
	int j;
    sem_init(&sem, 0, 1);
    pthread_t th;
    pthread_t thBarrier;
    void * arg = NULL;
	for(j = 0; j < N; j++){
	    if(pthread_create(&th, NULL, action, (void*)(j)) == -1){
		    printf("error %d", i);
	    }
	}
	if(pthread_create(&thBarrier, NULL, barrier, arg) == -1){
		    perror("error B");
	}
	pthread_join(thBarrier, NULL);
}
