#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_1(void *arg) {
	printf("Nous sommes dans le thread.\n");
	// Arrêt propre du thread
	pthread_exit(EXIT_SUCCESS);
}

int main(void) {
	// Création de la variable qui va contenir le thread
	pthread_t thread1;
	printf("Avant la création du thread.\n");
	// Création du thread
	pthread_create(&thread1, NULL, thread_1, NULL);
	printf("Après la création du thread.\n");
	return EXIT_SUCCESS;
}
