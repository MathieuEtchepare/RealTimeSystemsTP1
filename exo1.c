#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define P(sem) sem_wait(sem)
#define V(sem) sem_post(sem)


sem_t sem;

void *display1(void *arg){
	int i; 
	for(i = 0; i < 100; i++){	
		P(&sem);
		printf("Real");
		printf(" Time ");
		fflush(stdout);
		usleep(100);
		V(&sem);
        usleep(100);
	}
	pthread_exit(NULL);
}

void *display2(void *arg){
	int i;    
	for(i = 0; i < 100; i++){
		usleep(100);    	
		P(&sem);
    	printf("-");
    	printf(" systems\n");
		fflush(stdout);
		usleep(100);
    	V(&sem);
        
	}
	pthread_exit(NULL);
}

int main(void){
   sem_init(&sem, 0, 1);
   pthread_t thA;
   pthread_t thB;
   void * arg = NULL;
   if(pthread_create(&thA, NULL, display1, arg) == -1){
		perror("error C");
   }
   if(pthread_create(&thB, NULL, display2, arg) == -1){
		perror("error C");
   }
	pthread_join(thA, NULL);
    pthread_join(thB, NULL);
   
}
