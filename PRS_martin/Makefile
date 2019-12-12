all: serveur

serveur: serveur.o
	gcc serveur.o -o serveur

serveur.o: serveur.c
	gcc -c serveur.c -o serveur.o -Wall

clean:
	rm -rf *.o serveur
