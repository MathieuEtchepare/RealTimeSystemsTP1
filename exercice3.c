/*

On s'aperçoit que sans sem_unlink() à la fin du programme, le lien vers le sémaphore dans /dev/shm ne disparait après la fin du programme.
Cela signifie que les liens sémaphore ne sont pas détruit et sont toujours présents sous forme de fichiers;

Si l'on relance le programme, le flag O_CREAT s'aperçoit que le sémaphore existe déjà
Et le sémaphore O_EXCL s'apreçoit qu'un sémaphore du même nom existe.
sem_open renvoie donc : SEM_FAILED


*/

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// On crée le sémaphore
sem_t *sem;


void *display(void *arg){
	// le paramètre est de type void, il faut donc le caster dans une variable temporaire
	char* c = arg;

	int i; // variable de boucle
	for(i = 0; i < 100; i++){
		sem_wait(sem);
		printf("%c", *c);
		fflush(stdout);
		usleep(100000);
		sem_post(sem);
		usleep(200000);
	}
	(void) arg;
	pthread_exit(NULL);

}
int main(void){

	// On crée le sémaphore nommé 
	// s'il n'existe pas déjà (O_CREAT) 
	// et si un sémaphore du même nom existe on renvoie une erreur (O_EXCL)
	sem = sem_open("semaphore", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);

	// si le semaphore existe déjà
	if(sem == SEM_FAILED){
		exit(0);
	}
	
	
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

	// le sémaphore est à 0 par défaut
	sem_post(sem); 
	// On affiche "A" sur la sortie standard (à partir du thread principal)
	int i; // variable de boucle
	for(i = 0; i < 100; i++){
		sem_wait(sem);
		usleep(100000);
		printf("A");
		fflush(stdout);
		sem_post(sem);
		usleep(290000);
	}

	pthread_join(thB, NULL);	

	// On attend la fin du thread C
	pthread_join(thC, NULL);

	sem_close(sem);

	sem_unlink("semaphore");
	
}

