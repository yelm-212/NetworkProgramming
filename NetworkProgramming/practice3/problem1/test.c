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
    struct hostent *host;

    while(1){
        char* message = malloc(sizeof(char) * BUF_SIZE);
        fputs("Enter a host name (Q to quit): ", stdout);
        scanf("%s", message);
        if(!strcmp(message, "q") || !strcmp(message, "Q"))
            break;
        
        host = gethostbyname(message);
        free(message);
        hostadr = malloc(sizeof(char) * 40);

        if (host == NULL){
            hostadr = "gethosterror()";
        }
        else if(*host->h_addr_list != NULL){
            strcpy(hostadr, inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
        }else
            hostadr = "gethosterror()";
        
        printf("IP addr: %s\n", hostadr);
        
    }

    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}