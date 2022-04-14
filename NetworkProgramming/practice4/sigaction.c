#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig==SIGALRM)
		puts("Time out!");
	alarm(2);	
}

int main(int argc, char *argv[])
{
	int i;
	struct sigaction act;
	act.sa_handler=timeout; // signal handler - function
	sigemptyset(&act.sa_mask); // sa_mask init to 0 no block
	act.sa_flags=0;
	sigaction(SIGALRM, &act, 0); // set SIGALRM 

	alarm(2);

	for(i=0; i<3; i++)
	{
		puts("wait...");
		sleep(100);
	}
	return 0;
}