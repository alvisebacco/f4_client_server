#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphical_functions.h"
#define DIM_MSG 32
#define RW 0666

void clean_memory(int id)
{
	shmctl(id, IPC_RMID, 0);
	color_red();
	printf("DEBUG: Memory %i has been cleared\n", id);
	color_normal();
}

int get_memory(int memory)
{
	int shmid;
	if ((shmid = shmget(memory, DIM_MSG, RW)) == -1)
	{
		printf("GAME-OVER server has left!");
		perror("shmget");
		exit(1);
	}
	return shmid;
}

int create_memory(int memory)
{
	int shmid;
	key_t key;
	key = memory;
	if((shmid = shmget(key, DIM_MSG, IPC_CREAT|RW)) == -1)
	{
		printf("GAME-OVER\nSERVER HAS LEFT :(");
		perror("shmget");
		exit(1);
	}
	return shmid;
}

char *attach_memory(int shmid)
{
	char *shm, *s;
	if((shm = shmat(shmid, NULL, 0)) == (void *) -1)
	{
		perror("shamt");
		exit(1);
	}
	s = (char *) shmat(shmid, NULL, 0);
	return s;
}
