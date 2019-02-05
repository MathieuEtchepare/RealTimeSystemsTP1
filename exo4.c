#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define P(sem) sem_wait(sem)
#define V(sem) sem_post(sem)
#define N 5
#define M 7



sem_t sem_rdecollage;
sem_t sem_ratterrissage;
sem_t sem_wdecollage;
sem_t sem_watterrissage;
pthread_mutex_t mutex;
int Air[N];
int Sol[M];

void* decollage(void* arg){
	int i = 0;	
	while(Sol[i] != -1) {
		pthread_mutex_lock(&mutex);
		P(&sem_rdecollage);
		printf("The plane n°%d takes off \n", Sol[i++]);
		fflush(stdout);
		usleep(10000);
		V(&sem_wdecollage);
		pthread_mutex_unlock(&mutex);
		usleep(10000);
		if(i == M) i = 0;
	}
	pthread_exit(NULL);
}

void* atterrissage(void* arg){
	int i = 0;	
	while(Air[i] != -1) {
		pthread_mutex_lock(&mutex);
		P(&sem_ratterrissage);
		printf("The plane n°%d lands\n", Air[i++]);
		usleep(10000);
		fflush(stdout);
		V(&sem_watterrissage);
		pthread_mutex_unlock(&mutex);
		usleep(10000);
		if(i == N) i = 0;
	}
	pthread_exit(NULL);
}

void* amenerAvion(void* arg){
	int i = 0;
	int* avions = arg;
	while(*avions > 0){
		P(&sem_watterrissage);
		Air[i++] = 1 + rand()%50000;
		printf("Flight n°%d added to Air buffer\n", Air[i-1]);
		fflush(stdout);
		usleep(10000);
		(*avions)--;
		V(&sem_ratterrissage);
		if(i == N) i = 0;
		usleep(10000);
	}
	Air[i] = -1;
	pthread_exit(NULL);	
}

void* sortirAvion(void* arg){
	int i = 0;
	int* avions = arg;
	while(*avions > 0){
		P(&sem_wdecollage);
		Sol[i++] = 1 + rand()%50000;
		printf("Flight n°%d added to Sol buffer\n", Sol[i-1]);
		fflush(stdout);
		usleep(10000);
		(*avions)--;
		V(&sem_rdecollage);
		if(i == M) i = 0;
		usleep(10000);
	}
	Sol[i] = -1;
	pthread_exit(NULL);
}

int main(void){
	srand(time(NULL));
	int avionAuSol = 15;
	int avionEnLAir = 20; 
	pthread_t SortirAvion, Decollage, AmenerAvion, Atterrissage;
	sem_init(&sem_rdecollage, M, 0);
	sem_init(&sem_ratterrissage, N, 0);
	sem_init(&sem_wdecollage, 0, M);
	sem_init(&sem_watterrissage, 0, N);
	if(pthread_create(&SortirAvion, NULL, sortirAvion, &avionAuSol) == -1){
		    perror("error sortirAvion");
	}
	if(pthread_create(&Decollage, NULL, decollage, NULL) == -1){
		    perror("error Decollage");
	}
	if(pthread_create(&AmenerAvion, NULL, amenerAvion, &avionEnLAir) == -1){
		    perror("error Amener Avion");
	}
	if(pthread_create(&Atterrissage, NULL, atterrissage, NULL) == -1){
		    perror("error Atterrissage");
	}
	pthread_join(SortirAvion, NULL);
	pthread_join(Decollage, NULL);
	pthread_join(AmenerAvion, NULL);
	pthread_join(Atterrissage, NULL);	
	return 0;

}
