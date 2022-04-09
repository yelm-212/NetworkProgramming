#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h> // need to get host addr

#define TRUE 1
#define FALSE 0

void error_handling(char* message);

char* gethostadr(char* msg, struct hostent* host){
    int i=0;
    char* hostadr;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(msg);
    host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
    if(!host)
        error_handling("gethost...error");
    
    hostadr = inet_ntoa(*(struct in_addr*)host->h_addr_list[i]);

    return hostadr;
}

int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;
    char message[30];
    int option, str_len;
    socklen_t optlen, clnt_adr_sz;
    struct sockaddr_in serv_adr, clnt_adr;
    
    struct hostent *host;
    char* hostadr;

    if( argc != 2 ){
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if( serv_sock == -1 )
        error_handling("socket() error");


    /*optlen = sizeof(option);
    option = TRUE;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);*/

    
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
        hostadr = gethostadr(message,host);
        write(clnt_sock, hostadr, sizeof(hostadr));
        write(1, hostadr, sizeof(hostadr));
    }

    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
