#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30
	/*
	timeout.tv_sec=5;
	timeout.tv_usec=5000;
	*/
int main(int argc, char *argv[])
{
	fd_set reads, temps; //fd_set
	int result, str_len;
	char buf[BUF_SIZE]; 
	struct timeval timeout; // timeout struct

	FD_ZERO(&reads); // macro
	FD_SET(0, &reads); // 0 is standard input(console)


	while(1)
	{
		temps=reads; // save read fd in temp(literally temp)
		timeout.tv_sec=5; 
		timeout.tv_usec=0;
		result=select(1, &temps, 0, 0, &timeout); // select returns changed values.
		if(result==-1) // select err
		{
			puts("select() error!");
			break;
		}
		else if(result==0) // no return on result - timeout
		{
			puts("Time-out!");
		}
		else  //outputs to console
		{
			if(FD_ISSET(0, &temps)) 
			{
				str_len=read(0, buf, BUF_SIZE);
				buf[str_len]=0;
				printf("message from console: %s", buf);
			}
		}
	}
	return 0;
}