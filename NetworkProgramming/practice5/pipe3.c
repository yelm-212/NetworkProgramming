#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fds1[2], fds2[2];
	char str1[]="Who are you?";
	char str2[]="Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;
	
	pipe(fds1), pipe(fds2); // now two pipes
	pid=fork();
	
	if(pid==0)
	{ // child
		write(fds1[1], str1, sizeof(str1));
		// use fds1 to send from child to parent
		read(fds2[0], buf, BUF_SIZE);
		// use fds1 to read from parent to child
		printf("Child proc output: %s \n",  buf);
		// this printed second
	}
	else
	{ // parent
		read(fds1[0], buf, BUF_SIZE);
		// use fds1 to read from child to parent
		printf("Parent proc output: %s \n", buf); 
		// this printed first
		write(fds2[1], str2, sizeof(str2));
		// use fds2 to send from parent to child
		sleep(3);
	}
	return 0;
}