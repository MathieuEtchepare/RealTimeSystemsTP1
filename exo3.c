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


sem_t sem_read;
sem_t sem_write;
char buffer[N];

void * read_buffer(void* arg){
	int i = 0;	
	while(buffer[i] != '\0'){
		P(&sem_read);
		printf("%c", buffer[i++]);
		V(&sem_write);
		if(i == N) i = 0;	
	}
	printf("\n");
	pthread_exit(NULL);
}

void * write_buffer(void* arg){
	char* data = (char*) arg;
	int buff_count = 0;
	while(*data != '\0'){
		P(&sem_write);			
		buffer[buff_count++] = *data++;
		V(&sem_read);
		if(buff_count == N) buff_count = 0;		
	}
	buffer[buff_count] = '\0';
	
	pthread_exit(NULL);
}

int main(int argc, const char* argv[]){
	pthread_t read;
	pthread_t write;
	sem_init(&sem_write, 0, N);
	sem_init(&sem_read, N, 0);
	if(pthread_create(&read, NULL, read_buffer, NULL) == -1){
		    perror("error read");
	}
	if(pthread_create(&write, NULL, write_buffer, (void*)argv[1]) == -1){
		    perror("error write");
	}
	pthread_join(write, NULL);
	pthread_join(read, NULL);
	return 0;
}
