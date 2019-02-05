#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


void *display(void *arg){
	// le paramètre est de type void, il faut donc le caster dans une variable temporaire
	char* c = arg;
	int i; // variable de boucle
	// affichage de 100 fois le même caractère
	for(i = 0; i < 100; i++){
		printf("%c", *c);
		fflush(stdout);
	}
	(void) arg;
	pthread_exit(NULL);

}
int main(void){
	
	// On affiche "A" sur la sortie standard (à partir du thread principal)
	int i; // variable de boucle
	for(i = 0; i < 100; i++){
		printf("A");
		fflush(stdout);
	}

	// On crée les deux threads qui afficheront les 100 "B" et les 100 "C"
	pthread_t thB;
	pthread_t thC;

	char b = 'B', c = 'C';

	// On lance le thread thB qui exécutera la fonction display avec comme paramètre le caractère 'B'
	if(pthread_create(&thB, NULL, display, &b) == -1){
		perror("error B");
	}
	// Le thread C doit attendre la fin du thread B pour démarrer
	pthread_join(thB, NULL);

	// On lance le thread C qui exécutera la fonction display avec comme paramètre le caractère 'C'
	if(pthread_create(&thC, NULL, display, &c) == -1){
		perror("error C");
	}

	// On attend la fin du thread C
	pthread_join(thC, NULL);
	
}
