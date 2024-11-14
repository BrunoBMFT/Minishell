#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_sigint(int sig)
{
	printf("\nReceived SIGINT (Ctrl+C). Signal number: %d\n", sig);
}

void handle_sigquit(int sig)
{
	printf("\nReceived SIGQUIT. Signal number: %d\n", sig);
	printf("Terminating the program not gracefully.\n");
	exit(0);
}

int main(void)
{
	struct sigaction sa_int, sa_quit;

	sa_int.sa_handler = handle_sigint; // Set handler function
	sigemptyset(&sa_int.sa_mask);	   // Block no signals during handling
	sa_int.sa_flags = 0;			   // No special flags
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);

	printf("Press Ctrl+C to trigger SIGINT or send SIGTERM to terminate the program.\n");
	while (1)
	{
		printf("Running...\n");
		sleep(3);
	}

	return 0;
}
