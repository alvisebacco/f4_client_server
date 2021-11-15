#include "include.h"

volatile int a = 0;

int computer_choose(int l)
{
	srand(time(NULL));
	int d1 = rand()%l;
	return d1;
}

int get_my_pid()
{
	pid_t pid = getpid();
	return pid;
}

int defense_move(int l)
{
	int counter;
	int m;
	printf("\nHINT: FOR DISK IN 5 POSITION INSERT 5, NOT 05!\n");
	printf("\nHINT: insert 100 to insert the best move!\n");
	do
	{
		printf("\n> ");
		scanf("%i", &m);
		if (m == 100)
		{
			m = computer_choose(l);
		}
	} while(m < 0 || m > l-1);
	return m;
}


void catch_init(int sig_num)
{
	printf("\nYou have left.\n");
	exit(0);
}

void head_of_matrix(int l)
{
	printf("\n");
	for (int i=0; i<l; ++i)
		{
			printf("%02i ", i);
		}
		printf("\n");
}

void username(int memory_player, char *user, int semaphore)
{
	int semid;
	int shmid;
	int client_light = client_light_creation(semaphore);
	
	key_t key;
	char *shm, *s;
	key = memory_player;
	signal(SIGINT, catch_init);
	
	int pid = get_my_pid();
	char pid_sz[10];
	sprintf(pid_sz, "%d", pid);
	printf("\nMy PID is: %s\n", pid_sz);
	
	
	shmid = get_memory(key);
	shm = attach_memory(shmid);

	strcpy(shm, user);
	traffic_light_go();
	client_light_stop(semaphore);
	
	
	
	//unlock the server
	traffic_light_go();
	//wait for server
	client_light_stop(semaphore);
	
	//leggo il simbolo
	char *sym = attach_memory(shmid);
	printf("\nServer told us symbols are: %s\n", sym);
	sleep(4);
	printf("\nPlayer1 will be: %c\n", sym[0]);
	sleep(1);
	printf("Player2 will be: %c\n\n", sym[1]);
		sleep(1);
	char X_O;
	char opponent;
	if (semaphore==P1_S)
	{
		X_O = sym[0];
		opponent = sym[1];
		printf("\nI am %c My opponent is %c\n", X_O, opponent);
		sleep(1);
	}
	else if (semaphore==P2_S)
	{
		X_O = sym[1];
		opponent = sym[0];
		printf("\nI am %c My opponent is %c\n", X_O, opponent);
		sleep(1);
	}
	
	traffic_light_go();
	client_light_stop(semaphore);
	
	//leggo il primo numero della matrice e sblocco mezzo server
	char *read_H = attach_memory(shmid);
	int h = atoi(read_H); 
	printf("Server told me H: %d\n", h);
	sleep(1);
	traffic_light_go();
	client_light_stop(semaphore);
	//leggo il secondo numero della matrice e sblocco mezzo server
	char *read_L = attach_memory(shmid);
	int l = atoi(read_L); 
	printf("Server told me L: %d\n", l);
	sleep(1);
	if (semaphore==P1_S)
	{
		printf("\nMy symbol is: %c", X_O);
		sleep(1);
	}
	else if (semaphore==P2_S)
	{
		printf("\nMy symbol is: %c", X_O);
		sleep(1);
	}
	printf("\n\n\t---WAIT FOR OPPONENT---\n");
	client_light_stop(semaphore);
	////////////////////////////////////////GOOD

	//leggo il vettore matrix[h] che ha creato il server
	int move = -1;//nessuna mossa e' stata fatta
	int winner = 0; 
	int play = 0;
	char matrix_l[l+1];
	char matrix_line[DIM*DIM];
	char matrix[DIM][DIM];
	if (semaphore==P1_S)
	{
		printf("\nMy symbol is: %c", X_O);
	}
	else if (semaphore==P2_S)
	{
		printf("\nMy symbol is: %c", X_O);
	}
	if(semaphore==P1_S)
	{
		if(move==-1)
		{	
			printf("\n\nI'm player1, i will generate matrix game play!\n\n");
			//matrix generation..
			head_of_matrix(l);
			for(int i=0; i<h; ++i)
			{
				for(int j=0; j<l; ++j)
				{
					matrix[i][j] = '.';
					printf("%c  ", matrix[i][j]);
				}
				printf("\n");
			}
			// faccio la prima mossa
			move = defense_move(l);
		
			for (int g=0; g<h; ++g)
			{
				if ((matrix[g][move]=='.') && (g+1==h))
				{
					matrix[g][move] = X_O;
				}
				//else if ((matrix[g][move]==X_O) || (matrix[g][move]=='O'))
				else if ((matrix[g][move]==X_O) || (matrix[g][move]==opponent))
				{
					matrix[g-1][move] = X_O;
					break;
				}
			}
					
			//stampo la matrice aggiornata della prima mossa
			clear_screen();
			printf("\n");
			head_of_matrix(l);
			for(int i=0; i<h; ++i)
			{
				for(int j=0; j<l; ++j)
				{
					printf("%c  ", matrix[i][j]);
				}
				printf("\n");
			}
			
			//if player 1
			
			for(int j=0; j<h; ++j)
			{
				//printf("ARRAY riga:%i\n", j);
				
				for(int i=0; i<l; ++i)//itero per lunghezza della matrice
				{
					matrix_l[i] = matrix[j][i];
					//printf("ARRAY column:%i=%c\n", i, matrix_l[i]);
				}
					//printf("\nARRAY->memory\n");
					strcpy (shm, matrix_l);
					traffic_light_go();
					usleep(5000);
					client_light_stop(semaphore);
			}
			traffic_light_go();
			
			sleep(1);
			client_light_stop(semaphore);
				//matrix_l[l+1] = '\0';
		}
		
		if(move!=-1)
		{
			while(1)
			{
				clear_screen();
				printf("\nXXXX PLAYER 1 MOVE XXXX\n");
				head_of_matrix(l);
				for(int i=0; i<h; ++i)
				{
					char *matrix_l = attach_memory(shmid);
					for(int j=0; j<l; ++j)
					{
					
						printf("%c  ", matrix_l[j]);
						matrix[i][j]=matrix_l[j];
					}
					printf("\n");
					traffic_light_go();
					usleep(5000);
					client_light_stop(semaphore);
				}
			// MOSSA
				move = defense_move(l);
				for (int g=0; g<h; ++g)
				{
					if  ((matrix[g][move]=='.') && (g+1==h))
					{
						matrix[g][move] = X_O;
					}
						
					//else if ((matrix[g][move]==X_O) || (matrix[g][move]=='O'))
					else if ((matrix[g][move]==X_O) || (matrix[g][move]==opponent))
					{
						matrix[g-1][move] = X_O;
						break;
					}
						
				}
				
				//stampo la matrice aggiornata della prima mossa
				clear_screen();
				head_of_matrix(l);
				for(int i=0; i<h; ++i)
				{
					for(int j=0; j<l; ++j)
					{
						printf("%c  ", matrix[i][j]);
					}
					printf("\n");
				}
				printf("\n");
				
				//la mando al server
				//printf("\n\nMatrix will be sent to server\n");
				
				for(int j=0; j<h; ++j)
				{
					//printf("ARRAY riga:%i\n", j);
					for(int i=0; i<l; ++i)//itero per lunghezza della matrice
					{
						matrix_l[i] = matrix[j][i];
						//printf("ARRAY column:%i=%c\n", i, matrix_l[i]);
					}
						//printf("\nARRAY->memory\n");
						strcpy (shm, matrix_l);
						traffic_light_go();
						client_light_stop(semaphore);
				}
				traffic_light_go();
				printf("---WAIT FOR OPPONENT---");
				usleep(5000);
				client_light_stop(semaphore);
			}
		}
	}
		
		else if(semaphore==P2_S)
		{
		while(1)
		{
			if (a=1)
				printf("\nI`m desolate, the other player has left\n");
			clear_screen();
			printf("\nOOOO PLAYER2 MOVE OOOO\n");
			head_of_matrix(l);
			for(int i=0; i<h; ++i)
				{
					char *matrix_l = attach_memory(shmid);
					for(int j=0; j<l; ++j)
					{
							
						printf("%c  ", matrix_l[j]);
						matrix[i][j]=matrix_l[j];
					}
					printf("\n");
					traffic_light_go();
					usleep(5000);
					client_light_stop(semaphore);
				}
			// MOSSA
			// faccio la prima mossa
			
			move = defense_move(l);
			
			
			
			for (int g=0; g<h; ++g)
				{
					if  ((matrix[g][move]=='.') && (g+1==h))
					{
						matrix[g][move] = X_O;
					}
					//else if ((matrix[g][move]==X_O) || (matrix[g][move]=='X'))
					else if ((matrix[g][move]==X_O) || (matrix[g][move]==opponent))
					{
						matrix[g-1][move] = X_O;
						break;
					}
					
				}
			
			//stampo la matrice aggiornata della prima mossa
			clear_screen();
			head_of_matrix(l);
			for(int i=0; i<h; ++i)
			{
				for(int j=0; j<l; ++j)
				{
				
					printf("%c  ", matrix[i][j]);
				}
				printf("\n");
			}
			printf("\n");
			
			//la mando al server
			//printf("\n\nMatrix will be sent to server\n");
			
			for(int j=0; j<h; ++j)
			{
				//printf("ARRAY riga:%i\n", j);
				for(int i=0; i<l; ++i)//itero per lunghezza della matrice
				{
					matrix_l[i] = matrix[j][i];
					//printf("ARRAY column:%i=%c\n", i, matrix_l[i]);
				}
					//printf("\nARRAY->memory\n");
					strcpy (shm, matrix_l);
					traffic_light_go();
					client_light_stop(semaphore);
			}
			traffic_light_go();
			printf("---WAIT FOR OPPONENT---");
			usleep(5000);
			client_light_stop(semaphore);
		}
	}	
}

int main(int argc[], char *argv[])
{
	clear_screen();
	if (argv[1] != NULL && argv[2] != NULL && argv[3])
	{
		printf("WELCOME! SYNC WITH SERVER...\n");
		printf("WAIT FOR OPPONENT\n");
		char *user = argv[1];
		int memory_player = atoi(argv[2]);
		int semaphore = atoi(argv[3]);
		username(memory_player, user, semaphore);
	}
	else
		istclient();
	return 0;
}
