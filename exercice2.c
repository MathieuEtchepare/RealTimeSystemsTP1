#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

// On crée le sémaphore
sem_t sem;

void *display(void *arg){
	// le paramètre est de type void, il faut donc le caster dans une variable temporaire
	char* c = arg;

	int i; // variable de boucle
	for(i = 0; i < 100; i++){
		sem_wait(&sem);
		printf("%c", *c);
		fflush(stdout);
		usleep(100000);
		sem_post(&sem);
		usleep(200000); // on utilise sleep pour ne pas avoir d'erreur de coordination
	}
	(void) arg;
	pthread_exit(NULL);

}
int main(void){

	// On initalise le sémaphore tel qu'une resource ne peut être utilisée que par un thread à la fois
	sem_init(&sem, 1, 1);


	// On crée les deux threads qui afficheront les 100 "B" et les 100 "C"
	pthread_t thB;
	pthread_t thC;

	char b = 'B', c = 'C';

		// On lance le thread C qui exécutera la fonction display avec comme paramètre le caractère 'C'
	if(pthread_create(&thC, NULL, display, &c) == -1){
		perror("error C");
	}

	// On lance le thread thB qui exécutera la fonction display avec comme paramètre le caractère 'B'
	if(pthread_create(&thB, NULL, display, &b) == -1){
		perror("error B");
	}
	

	// On affiche "A" sur la sortie standard (à partir du thread principal)
	int i; // variable de boucle
	for(i = 0; i < 100; i++){
		sem_wait(&sem);
		usleep(100000); 
		printf("A");
		fflush(stdout);
		sem_post(&sem);
		usleep(290000); // on utilise sleep pour ne pas avoir d'erreur de coordination
	}
	pthread_join(thB, NULL);


	// On attend la fin du thread C
	pthread_join(thC, NULL);
	
}
