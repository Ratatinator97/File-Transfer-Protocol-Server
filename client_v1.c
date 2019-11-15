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
    int length = 0;
    int iteration;
    while(1){

        fgets(msg, RCVSIZE, stdin);
        fflush(stdin);
        strcpy(msg,strtok(msg, "\n"));
        printf("Le msg lu est : %s...\n",msg);
        strcpy(nom_fichier,msg);
        printf("Le nom du fichier est : %s\n",nom_fichier);
        sendto(server_desc,(const char*)msg, strlen(msg),MSG_CONFIRM, (const struct sockaddr *) &adresse,sizeof(adresse)); 
        printf("the value of sent is:%lu\n", strlen(msg));
        fini = 0;
        iteration = 0;
        while(fini == 0){
            
            n = recvfrom(server_desc, (char *)msg, RCVSIZE,MSG_WAITALL, (struct sockaddr *) &adresse,&len);
            msg[n]='\0';
            printf("Le nom du fichier est : %s\n",nom_fichier);
            if(strcmp(msg,"FIN")==0){
                
                printf("On a recu FIN\n");
                fini = 1;
                sendto(server_desc,"ACK", strlen("ACK"),MSG_CONFIRM, (const struct sockaddr *) &adresse,sizeof(adresse));
            	
                printf("Le nom du fichier est %s\n",nom_fichier);
                if((file=fopen("texte2.txt","w")) == NULL){
		            
		            printf("Cannot open the file...\n");
		            exit(1);
	            }
	            //write the values on the file 
                printf("L'echange est fini !\n");
                
                printf("On sauvegarde au total %d bytes dans le fichier \n",iteration*(RCVSIZE-6)+length);
	            fwrite(buffer_ecriture,sizeof(char),iteration*(RCVSIZE-6)+length,file); 
	            fclose(file);
                free(buffer_ecriture);
            }else{
                
                strcpy(ack_s,"ACK_");
                printf("Contenu de msg : %s\n",msg);
                
                for(int u=0;u<6;u++){
                    num_seq_s[u]=msg[u];
                }
                num_seq_s[6]='\0';
               
                printf("num_seq_s contient : %s\n",num_seq_s);
                num_seq = atoi(num_seq_s);
                printf("On copie le msg a l'emplacement no %d du buffer\n",((RCVSIZE-6)*num_seq));
                
                length = RCVSIZE-6;
                
                // -------------------------
                for(int v=0;v<502;v++){
                    if(msg[v] == EOF){
                        printf("EOF trouvé en %d\n",v);
                        length = v;
                    }
                }
                memcpy(buffer_ecriture+((RCVSIZE-6)*num_seq),msg+6,length); //Todo Probleme sur fin de fichier
                
                printf("ack_s contient : %s\n",ack_s);
                printf("num_seq_s contient : %s\n",num_seq_s);
                strcat(ack_s,num_seq_s);
                printf("ack_s contient : %s\n",ack_s);
                strcpy(msg, ack_s);  
                printf("Le buffer msg contient : %s\n",msg); 
                printf("Acquitement : %s , no : %d\n",msg,num_seq);             
                sendto(server_desc,(const char*)msg, strlen(msg),MSG_CONFIRM, (const struct sockaddr *) &adresse,sizeof(adresse));
                iteration++;
            }
        }
        


    }

    return 0;

}