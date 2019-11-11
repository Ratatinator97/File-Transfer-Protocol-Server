#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define RCVSIZE 508


int main (int argc, char *argv[]) {
    
    struct sockaddr_in adresse;
    FILE *file;
    int port,port2 = 5001;
    char msg[RCVSIZE];
    char *buffer_ecriture = malloc (sizeof(char)*104857600);

    if(argc > 3){
        printf("Too many arguments\n");
        exit(0);
    }
    if(argc < 3){
        printf("Argument expected\n");
        exit(0);
    }
    if(argc == 3){
        port = atoi(argv[2]);
    }
    
    int server_desc = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&adresse, 0, sizeof(adresse));
    if (server_desc < 0) {
        perror("cannot create socket\n");
        return -1;
    }
        
    adresse.sin_family= AF_INET;
    adresse.sin_port= htons(port);
    adresse.sin_addr.s_addr= inet_addr(argv[1]);
  

    socklen_t len = sizeof(adresse);
    int  n; 

    printf("SYNC...\n");
    strcpy(msg, "SYN");
    
    sendto(server_desc,(const char*)msg, strlen(msg),MSG_CONFIRM, (const struct sockaddr *) &adresse,sizeof(adresse));
    printf("sending SYN\n");
    n = recvfrom(server_desc, (char *)msg, RCVSIZE,MSG_WAITALL, (struct sockaddr *) &adresse,&len);
    printf("%d\n",ntohs(adresse.sin_port));
    msg[n] = '\0';
        
    char *token;
    token = strtok(msg, "SYN-ACK");
    if(token != NULL){
        port2 =  atoi(token);
        printf("Le port lu dans syn ack est : %d\n",port2);
        printf("SYN-ACK received\n");
        strcpy(msg, "ACK");
        sendto(server_desc,(const char*)msg, strlen(msg),MSG_CONFIRM, (const struct sockaddr *) &adresse,sizeof(adresse));
        printf("sending ack, connected\n");
    }else{
        printf("bad syn-ack\n");
        exit(0);
    }
    adresse.sin_port = htons(port2);
    int fini = 0;
    char nom_fichier[20];
    char num_seq_s[7];
    int num_seq;
    char ack_s[11];
    while(1){

        fgets(msg, RCVSIZE, stdin);
        strcpy(msg,strtok(msg, "\n"));
        printf("Le msg lu est : %s...\n",msg);
        strcpy(nom_fichier,msg);
        printf("Le nom du fichier est : %s...\n",nom_fichier);
        sendto(server_desc,(const char*)msg, strlen(msg),MSG_CONFIRM, (const struct sockaddr *) &adresse,sizeof(adresse)); 
        printf("the value of sent is:%lu\n", strlen(msg));
        fini = 0;
        
        while(fini == 0){
            
            n = recvfrom(server_desc, (char *)msg, RCVSIZE,MSG_WAITALL, (struct sockaddr *) &adresse,&len);
            msg[n]='\0';
           
            if(strcmp(msg,"FIN")==0){
                
                printf("On a recu FIN\n");
                fini = 1;
                sendto(server_desc,"ACK", strlen("ACK"),MSG_CONFIRM, (const struct sockaddr *) &adresse,sizeof(adresse));
            	
                if((file=fopen("blabla.jpg","w")) == NULL){
		            //if the file does not exist print the string
		            printf("Cannot open the file...\n");
		            exit(1);
	            }
	            //write the values on the file 
                printf("L'echange est fini !\n");
                printf("Le buffer d'ecriture contient : %s\n",buffer_ecriture);
                printf("Le buffer contient %d bytes\n",strlen(buffer_ecriture));
	            fwrite(buffer_ecriture,sizeof(char),strlen(buffer_ecriture),file);
	            //close the file
	            fclose(file);
                free(buffer_ecriture);
            }else{
                strcpy(ack_s,"ACK_");
                memcpy(num_seq_s,msg,6);
                num_seq = atoi(num_seq_s);
                memcpy(buffer_ecriture+(RCVSIZE-6)*num_seq,msg,RCVSIZE-6);

                strcat(ack_s,num_seq_s);
                strcpy(msg, ack_s);   
                printf("Acquitement : %s , no : %d\n",msg,num_seq);             
                sendto(server_desc,(const char*)msg, strlen(msg),MSG_CONFIRM, (const struct sockaddr *) &adresse,sizeof(adresse));
            }
        }
        


    }

    return 0;

}