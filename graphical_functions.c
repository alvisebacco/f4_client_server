#include <stdio.h>
#include <stdlib.h>

void istclient()
{
	printf("\nWelcome in F4_client, usage: \n");
	printf("\n./client [nikname] [shared segment of server's instance] [your semaphore]\n");
	printf("\nEXAMPLE: ./server 10 15 69 XO\n");
}

void help()
{
	printf("\nWelcome in F4_server, usage: \n");
	printf("\n./server [numbers of raws] [numbers of columns] [shared segment number to be assigned] [symbols to use]\n");
	printf("\nEXAMPLE: ./server 10 15 69 XO\n");
}



void clear_screen()
{
	system("clear");
}

void color_red()
{
	printf("\033[0;31m");
}

void color_normal()
{
	printf("\033[0m");
}

