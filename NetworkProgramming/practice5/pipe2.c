#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fds[2];
	char str1[]="Who are you?";
	char str2[]="Thank you for your message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds); // one pipe
	pid=fork();

	if(pid==0)
	{	// child proc.
		write(fds[1], str1, sizeof(str1));
		// sends str from child to parent
		sleep(2); // block using sleep
		read(fds[0], buf, BUF_SIZE);
		printf("Child proc output: %s \n",  buf);
	}
	else
	{	/// parent proc.
		read(fds[0], buf, BUF_SIZE); // read from child fds[0]
		printf("Parent proc output: %s \n", buf);
		write(fds[1], str2, sizeof(str2)); // str2 sended to child. 
		sleep(3); // block using sleep
	}
	return 0;
}
