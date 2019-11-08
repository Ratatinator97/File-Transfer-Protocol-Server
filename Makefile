all: serveur_v1  client_v1

client_v1: client_v1.o
	gcc client_v1.o -o client_v1

client_v1.o: client_v1.c
	gcc -c client_v1.c -o client_v1.o -Wall

serveur_v1: serveur_v1.o
	gcc serveur_v1.o -o serveur_v1

serveur_v1.o: serveur_v1.c
	gcc -c serveur_v1.c -o serveur_v1.o -Wall

clean:
	rm -rf *.o  client_v1 serveur_v1
