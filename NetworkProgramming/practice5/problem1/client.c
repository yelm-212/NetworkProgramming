#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

/* 1. it can receive messages from other clients in the chatting;
2. it can send messages to other clients. */

int main(int argc, char *argv[])
{
	int sock;
	pid_t pid;
	char buf[BUF_SIZE];
	char svmsg[BUF_SIZE] = { 0x00 ,};
	struct sockaddr_in serv_adr;
	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);  
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");

	// read_routine(sock, buf);

	// 서버 문제인줄 알았는데 클라 건들면서 fork 모르고 지움 ㅠㅠ 왜 눈치못챘지
	pid=fork();
	if(pid==0) 
		write_routine(sock, buf);
	else // server에서 읽어온 메시지는 svmsg로 바뀜
		read_routine(sock, svmsg);

	close(sock);
	return 0;
}

void read_routine(int sock, char *buf)
{
	while(1) 
	{
		int str_len=read(sock, buf, BUF_SIZE);
		if(str_len==0)
			return;

		buf[str_len]=0;
		printf("%s", buf);
	}
}
void write_routine(int sock, char *buf)
{
	while(1)
	{
		fgets(buf, BUF_SIZE, stdin); 
		if(!strcmp(buf,"q\n") || !strcmp(buf,"Q\n"))
		{	
			shutdown(sock, SHUT_WR); 
			return;
			exit(0);
		}
		write(sock, buf, strlen(buf)); 
		fflush(stdin); // erase stdin buf after writing socket
	}
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}