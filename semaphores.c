#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include "graphical_functions.h"
#define SEM_KEY 14
//#define CLI_KEY 15
#define RW 0666



void clean_semaphore(int id)
{
	semctl(id, IPC_RMID, 0);
	color_red();
	printf("DEBUG: Semaphore %i has been cleared\n", id);
	color_normal();
}

int client_light_creation(int key)
{
	struct sembuf sem;
	int client_semid;
	if ((client_semid = semget(key, 1, IPC_CREAT|RW))== -1)
	{
		perror("Semget");
		exit(1);
	}
	sem.sem_op = -1;
	sem.sem_num = 0;
	sem.sem_flg = 0;
	return client_semid;
}

int traffic_light_creation()
{
	struct sembuf sem;
	int semid;
	if ((semid = semget(SEM_KEY, 1, IPC_CREAT | RW))== -1)
	{
		perror("Semget");
		exit(1);
	}
	sem.sem_op = -1;
	sem.sem_num = 0;
	sem.sem_flg = 0;
	return semid;
}

int get_client_light(int key)
{
	struct sembuf sem;
	int client_semid;
	if ((client_semid = semget(key, 1, RW)) == -1)
	{
		printf("GAME-OVER, server has cleared shared resources!");
		perror("semget");
		exit(1);
	}
	sem.sem_op = 1;
	sem.sem_num = 0;
	sem.sem_flg = 0;
	return client_semid;
}

int get_traffic_light()
{
	struct sembuf sem;
	int semid;
	if ((semid = semget(SEM_KEY, 1, RW)) == -1)
	{
		perror("semget");
		exit(1);
	}
	sem.sem_op = 1;
	sem.sem_num = 0;
	sem.sem_flg = 0;
	return semid;
}

void client_light_go(int key)
{
	struct sembuf sem;
	int client_semid;
	if ((client_semid = semget(key, 1, RW)) == -1)
	{
		printf("GAME-OVER, server has cleared shared resources!");
		perror("semget");
		exit(1);
	}
	sem.sem_op = 1;
	sem.sem_num = 0;
	sem.sem_flg = 0;
	semop(client_semid, &sem, 1);
}



void traffic_light_go()
{
	struct sembuf sem;
	int semid;
	if ((semid = semget(SEM_KEY, 1, RW)) == -1)
	{
		printf("\nGAME-OVER, server has cleared shared resources!\n\n");
		//perror("semget");
		exit(1);
	}
	sem.sem_op = 1;
	sem.sem_num = 0;
	sem.sem_flg = 0;
	semop(semid, &sem, 1);
}

void client_light_stop(int key)
{
	struct sembuf sem;
	int client_semid;
	if ((client_semid = semget(key, 1, RW)) == -1)
	{
		perror("semget");
		exit(1);
	}
	sem.sem_op = -1;
	sem.sem_num = 0;
	sem.sem_flg = 0;
	semop(client_semid, &sem, 1);
}

void traffic_light_stop()
{
	struct sembuf sem;
	int semid;
	if ((semid = semget(SEM_KEY, 1, 0666)) == -1)
	{
		perror("semget");
		exit(1);
	}
	sem.sem_op= -1;
	sem.sem_num = 0;
	sem.sem_flg = 0;
	semop(semid, &sem, 1);
}
