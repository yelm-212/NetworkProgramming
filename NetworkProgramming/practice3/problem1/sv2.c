#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h> // need to get host addr

#define BUF_SIZE 1024

void error_handling(char* message);


int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t optlen, clnt_adr_sz;
    struct sockaddr_in serv_adr, clnt_adr;

    // int i;
    struct hostent *host;
    char* hostadr; // needed to get host adr

    if( argc != 2 ){
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if( serv_sock == -1 )
        error_handling("socket() error");

    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if( bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)))
        error_handling("bind() error");

    if( listen(serv_sock, 5) == -1 )
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

    while( (str_len = read(clnt_sock, message, sizeof(message))) != 0 ){
        host = gethostbyname(message);
        hostadr = malloc(sizeof(char) * 2048);

        if (!host){
            hostadr = "gethosterror()";
        }
        
        if(*host->h_addr_list != NULL){
            strcpy(hostadr, inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
        }else
            hostadr = "gethosterror()";

        write(clnt_sock, hostadr, sizeof(hostadr));
        // write(1, hostadr, sizeof(hostadr));
    }
    free(hostadr);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
