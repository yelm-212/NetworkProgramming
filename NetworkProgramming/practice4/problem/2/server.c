#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h> // need to get host addr

#define BUF_SIZE 30
void error_handling(char *message);
void read_childproc(int sig);
char* gethostadr_serv(char* message, int str_len);

#define GETHOST_ERR "gethost...error"

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	
	pid_t pid;
	struct sigaction act; 
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD, &act, 0);
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	while(1)
	{
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if(clnt_sock==-1)
			continue;
		else
			puts("new client connected...");
		pid=fork(); 
		if(pid==-1)
		{
			close(clnt_sock);
			continue;
		}
		if(pid==0) 
		{
			// close(serv_sock); 
			// keep listnening
			while((str_len=read(clnt_sock, buf, BUF_SIZE))!=0){
				strcpy(buf, gethostadr_serv(buf, str_len));
				write(clnt_sock, buf, strlen(buf));
			}
			
			close(clnt_sock); 
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock); 
            					
	}
	close(serv_sock);
	return 0;
}

char* gethostadr_serv(char* message, int str_len){
	char* msgptr = malloc(sizeof(char) * BUF_SIZE);
	struct hostent *host;
    char* hostadr; // needed to get host adr

    strcpy(msgptr, message);
    free(msgptr);
	host = gethostbyname(msgptr);
    hostadr = malloc(sizeof(char) * BUF_SIZE);

    if (host == NULL){
        strcpy(hostadr, GETHOST_ERR);
    }
    else if(*host->h_addr_list != NULL){
        strcpy(hostadr, inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
    }else
        strcpy(hostadr, GETHOST_ERR);

	return hostadr;
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG);
	printf("removed proc id: %d \n", pid);
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}