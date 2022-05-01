#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fds[2];
	char str[]="Who are you?"; // msg
	char buf[BUF_SIZE];
	pid_t pid;
	
	pipe(fds); // make new pipe fds[0], fds[1]  
	pid=fork(); 
	if(pid==0)
	{ // child process
		write(fds[1], str, sizeof(str)); 
		//fds[1]: from child to parent. stdout
	}
	else
	{ // parent process
		read(fds[0], buf, BUF_SIZE); 
		//fds[0]: from parent to child. stdin
		puts(buf);
	}
	return 0;
}