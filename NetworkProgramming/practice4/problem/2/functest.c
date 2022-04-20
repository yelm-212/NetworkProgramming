#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h> // need to get host addr

#define BUF_SIZE 1024
#define GETHOST_ERR "gethost...error"

int main(){
    struct sockaddr_in addr;
    char* hostadr;
    struct hostent *host = malloc(sizeof(struct hostent));

    while(1){
        char* message = malloc(sizeof(char) * BUF_SIZE);
        fputs("Enter a host name (Q to quit): ", stdout);
        scanf("%s", message);
        if(!strcmp(message, "q") || !strcmp(message, "Q"))
            break;
        
        host = gethostbyname(message);
        free(message);

        hostadr = malloc(sizeof(char) * 40);
        char* temp = malloc(sizeof(char) * 40);

        for(int i; host->h_aliases[i];i++){
            char* temp = nametoadr(host, i);
            strcpy(hostadr, temp);
            printf("IP addr %d : %s\n", i, hostadr);
        }
        free(hostadr);
        free(temp);

        
    }

    return 0;
}

char* nametoadr(struct hostent * host, int i){
    char* adr = malloc(sizeof(char) * 40);
    char* err = "gethost...error";

    if (host == NULL){
        return err;
    }
    else if(*host->h_addr_list != NULL){
        return inet_ntoa(*(struct in_addr*)host->h_addr_list[i]);
    }else
        return err;
}


void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}