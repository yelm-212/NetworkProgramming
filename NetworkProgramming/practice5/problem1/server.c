#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 100
void error_handling(char *buf);

/* 1. works as a message agent to deliver a message, 
which is from a client, to other clients in the chatting room;
2. when a client joins or leaves the chatting room,
informs other clients in the chatting room. */

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;
	fd_set reads, cpy_reads;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num, i;
	char buf[BUF_SIZE];
	char svmsg[BUF_SIZE] = { 0x00 ,};
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads); 
	// read < server socket data recieve
	// if server sock has data, it has connection request.
	fd_max=serv_sock;

	while(1)
	{
		cpy_reads=reads;
		timeout.tv_sec=5;
		timeout.tv_usec=5000;

		if((fd_num=select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1)
			break;
		
		if(fd_num==0)
			continue;

		for(i=0; i<fd_max+1; i++) //find if there is edited fd.
		{
			if(FD_ISSET(i, &cpy_reads))
			{
				if(i==serv_sock)     // connection request find
				{
					adr_sz=sizeof(clnt_adr);
					clnt_sock=
						accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					FD_SET(clnt_sock, &reads); // reads client socket fd
					if(fd_max<clnt_sock)
						fd_max=clnt_sock; // update if there is new fd
						// 뭘 새로 넣어서 각 클라이언트 메시지 처리해줘야할듯? 
						// select있으니까 pipe쓰는건 좀 아닌거같음 
					printf("connected client: %d \n", clnt_sock);
					sprintf(svmsg , "Server: the number of clients is %d, and client %d has joined the server now. \n", clnt_sock-3, clnt_sock);
					write(clnt_sock, svmsg, strlen(svmsg)); 
				}
				else    // reads message
				{
					str_len=read(i, buf, BUF_SIZE);
					if(str_len==0)    // close request
					{
						FD_CLR(i, &reads);
						close(i);
						printf("closed client: %d \n", i); // fd number
						for(int k=4; k<=clnt_sock;k++){
							if(k!=i){
								sprintf(svmsg , "Client %d has left the server\n", i);
								write(k, svmsg, strlen(svmsg));
							}
						} 
					}
					else // EOF
					{
						// sprintf(svmsg , "Client %d : %s", i, buf);
						// write(i, svmsg, strlen(svmsg));    // echo clt i msg to clt i
						// 전체 fd 에 write해주는 for문으로 수정하였다.
						for(int k=4; k<=clnt_sock;k++){
							sprintf(svmsg , "Client %d : %s", i, buf);
							write(k, svmsg, strlen(svmsg));
						}
						memset(svmsg, 0, sizeof(svmsg));
						memset(buf, 0, sizeof(buf));
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}