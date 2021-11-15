#include "include.h"

void im_gonna_exiting()
{
	clear_screen();
	printf("Ctrl-C foud!");
	fflush(stdout);
	printf("\nSERVER Aborted.\n");
	system("ipcrm -a");
}

void clean_on_c(int sig_num)
{
	clear_screen();
	//printf("SIG NUM: %i", sig_num);
	signal(SIGINT, im_gonna_exiting);
	printf("Ctrl-C foud!, press agani to kill all and clean all memory games!");
	fflush(stdout);
}

int get_player_pid()
{
	FILE *pidof_cmd = popen("pidof -s client", "r");
	if (pidof_cmd == NULL)
	{
		fprintf(stderr, "popen(3) error");
		exit(EXIT_FAILURE);
	}
	
	static char pid[10];
	size_t n;
	while ((n = fread(pid, 1, sizeof(pid)-1, pidof_cmd)) > 0) {
	pid[n] = '\0';
	}
	int pid_ = atoi(pid);
	return pid_;
}

int am_i_winner(int h, int l, char matrix[50][50], char X_O)
{
	int victory = 4;
	int tr[50][50];
	printf("\nIs %c a winner?\n", X_O);
	for(int i=0; i<h; ++i)
	{
		for(int j=0; j<l; ++j)
		{
			printf("%c", matrix[j][i]);
			tr[i][j] = matrix[j][i];
			if (matrix[j][i] == X_O)
			{
				victory--;
				//DEBUG
				//printf("\n%c found! %i to win\n", X_O, victory);
				if (victory == 0)
					return 1;
			}
			else
			{
				victory = 4;
			}
		}
		printf("\n");
		
	}
	
	printf("\n90 degrees transposition: \n");
	int _victory = 4;
	for(int i=0; i<l; ++i)
	{
		for(int j=0; j<h; ++j)
		{
			printf("%c", tr[j][i]);
			if (tr[j][i] == X_O)
			{
				_victory--;
				if (_victory == 0)
					return 1;
			}
			else
			{
				_victory = 4;
			}
		}
		printf("\n");
		
	}
	
	printf("\n135 degrees transposition: \n");
	int row = 0;
	int col;
	int __victory = 4;
	while(row<l)
	{
		
		col = 0;
		int rowTemp=row;
		while(rowTemp>=0)
		{
			if(matrix[rowTemp][col] == X_O)
			{
				__victory--;
				if(__victory == 0)
				return 1;
			}
			else
			{
				__victory = 4;
			}
			if (col >= h)
			{
				//printf("\nNon square elements matrix!\n");
				//printf("%c", matrix[rowTemp][h]);
			}
			else
				printf("%c", matrix[rowTemp][col]);
			
			rowTemp--;
			col++;
			
		}
		printf("\n");
		row++;
	}
	
	col =1;
	while(col<=h)
	{
		
		int colTemp = col;
		row = l-1;
		while(colTemp<=h)
		{
			
			printf("%c", matrix[row][colTemp-1]);
			if(matrix[row][colTemp] == X_O)
			{
				__victory--;
				if(__victory == 0)
					return 1;
			}
			else
			{
				__victory = 4;
			}
			row--;
			colTemp++;
		}
		printf("\n");
		col++;
	}

	return 0;
}

int check_prerequisite(int h, int l)
{
	if (h >= MIN_MATRIX && l >= MIN_MATRIX)
	{
		color_normal();
		printf("I'm drawing a %i x %i matrix game.\n", h, l);
		return 0;
	}
	else
		return 1;
}

void server_stop()
{
	traffic_light_stop();
	traffic_light_stop();
}



int main(int argc, char *argv[])
{
	clear_screen();
	color_normal();
	signal(SIGINT, clean_on_c);
	if (argv[1] != NULL && argv[2] != NULL && argv[3] !=NULL && argv[4])
	{
		int h = atoi(argv[1]);
		int l = atoi(argv[2]);
		char sym[2];
		strcpy(sym, argv[4]);
		char *shm, *s;
		int memory_player = atoi(argv[3]);
		int player1_semaphore = P1_S;
		int player2_semaphore = P2_S;
		char matrix[DIM][DIM];
		if (check_prerequisite(h, l) == 0)
		{
			printf("Prerequisite passed!\n\n");
			
			// creo una memoria condivisa
			int memory_id = create_memory(memory_player);
			
			// creo un semaforo per il server
			int traffic_light = traffic_light_creation();
			// rimango in attesa del giocatore uno
			printf("Waiting...\nPlayer1 with semaphore %i\nPlayer2 with semaphore %i\non memory: %i\n", P1_S, P2_S, memory_player);
			traffic_light_stop();
			printf("\nWith symbol: %c\n", sym[0]);
			
			char *player1 = attach_memory(memory_id);
			printf("Player 1 has joined as: %s\n", player1);
			int pid_p1 = get_player_pid();
			printf("XXXX PLAYER1 PID: %i\n\n", pid_p1);
			traffic_light_stop();
			char *player2 = attach_memory(memory_id);
			printf("\nWith symbol: %c\n", sym[1]);
			printf("Player 2 has joined as: %s\n", player2);
			int pid_p2 = get_player_pid();
			printf("OOOO PLAYER2 PID: %i\n\n", pid_p2);	
			printf("Unlock player 1...\n");
			client_light_go(player1_semaphore);
			printf("Unlock player 2...\n");
			client_light_go(player2_semaphore);
			traffic_light_stop();
			printf("Player1 is creating table game...\n");
			signal(SIGINT, clean_on_c);
			//recupero il segmento di memoria per poterlo scrivere
			int shmid = get_memory(memory_player);
			shm = attach_memory(shmid);
			//butto il simbolo e lo faccio leggere ai client
			strcpy(shm, sym);
			client_light_go(player1_semaphore);
			traffic_light_stop();
			client_light_go(player2_semaphore);
			traffic_light_stop();
			//fine del recupero 
			//trasformo int in char
			char str[20];
			sprintf(str, "%d", h);
			//copio nel segmento condiviso
			strcpy(shm, str);
			//lo faccio leggere al client 1 sbloccandolo
			client_light_go(player1_semaphore);
			//blocco il server
			server_stop();
			
			//lo faccio leggere al client 2
			client_light_go(player2_semaphore);
			//fermo il server
			traffic_light_stop();
			//trasformo int in char 
			char str1[20];
			sprintf(str1, "%d", l);
			//copio nel segmento condiviso
			strcpy(shm, str1);
			//lo faccio leggere a client 1 sbloccandolo
			client_light_go(player1_semaphore);
			//lo faccio leggere al client 2
			client_light_go(player2_semaphore);
			//fermo il server
			printf("\nSegment created!\n");
			client_light_go(player1_semaphore);
			traffic_light_stop();
			///////////////////////GOOD

			//NEW BLOCK 1
		while(1){
			printf("PLAYER1 MOVE\n");
			//primo vettore ricevuto
			for(int j=0; j<h; ++j)
			{
				char *matrix_l = attach_memory(shmid);
				for(int i=0; i<l; ++i)
				{
					printf("%c ", matrix_l[i]);
					matrix[i][j]=matrix_l[i];
				}
				printf("\n");
				client_light_go(player1_semaphore);
				traffic_light_stop();
			}
			//creo la matrice
			sleep(1);
			clear_screen();
			printf("Matrix is: \n");
			//char p1 = 'X';
			char p1 = sym[0];
			
			//// AM I WINNER???
			int win = am_i_winner(h, l, matrix, p1);
			//DEBUG
			//printf("WIN=%i", win);
			if (win==1)
			{
				printf("\nPLAYER1 WINS\n");
			}
			else
			{
				printf("\nNO WINNER\nPLAYER2 TURN\n");
			}
			
			//la sparo nella shared e la faccio leggere a player2
			char matrix_l[l+1];
			for(int j=0; j<h; ++j)
			{
				//printf("ARRAY riga:%i\n", j);
				for(int i=0; i<l; ++i)//itero per lunghezza della matrice
				{
					matrix_l[i] = matrix[i][j];
					//printf("ARRAY column:%i=%c\n", i, matrix_l[i]);
				}
					//printf("\nARRAY->memory\n");
					if(win!=1)
					{
						strcpy (shm, matrix_l);
						client_light_go(player2_semaphore);
						traffic_light_stop();	
					}
					else
					{
						strcpy (shm, "P1 WINS");
						client_light_go(player2_semaphore);
						client_light_go(player1_semaphore);
						color_red();
						printf("\nDEBUG: I'm cleaning semaphores...\n");
						clean_semaphore(traffic_light);
						clean_semaphore(player2_semaphore);
						clean_semaphore(player1_semaphore);
						color_red();
						printf("DEBUG: I'm cleaning memory...\n");
						clean_memory(memory_id);
						exit(0);
						//traffic_light_stop();
					}
					
			}
			
			//inviata, avvio il client 2
			client_light_go(player2_semaphore);
			traffic_light_stop();
			
			
			//NEW BLOCK 2
			//ricevo da client 2
			printf("PLAYER2 TURN!\n");
			//char *matrix_l = attach_memory(shmid);//primo vettore ricevuto
			for(int j=0; j<h; ++j)
			{
				char *matrix_l = attach_memory(shmid);
				for(int i=0; i<l; ++i)
				{
					//printf("%c ", matrix_l[i]);
					matrix[i][j]=matrix_l[i];
				}
				printf("\n");
				client_light_go(player2_semaphore);
				traffic_light_stop();
			}
			//creo la matrice
			sleep(1);
			printf("Matrix is: \n");
			//char p2 = 'O';
			char p2 = sym[1];
			//// AM I WINNER???
			
			int win2 = am_i_winner(h, l, matrix, p2);
			if (win2==1)
			{
				printf("\nPLAYER2 WINS\n");
			}
			else
			{
				printf("\nNO WINNER\nPLAYER1 TURN!\n");
			}
			
			//la sparo nella shared e la faccio leggere a player1
			//char matrix_l[l+1];
			printf("Matrix is: ");
			for(int j=0; j<h; ++j)
			{
				//printf("ARRAY riga:%i\n", j);
				for(int i=0; i<l; ++i)//itero per lunghezza della matrice
				{
					matrix_l[i] = matrix[i][j];
					//printf("ARRAY column:%i=%c\n", i, matrix_l[i]);
				}
					//printf("\nARRAY->memory\n");
					
					if (win2!=1)
					{
						strcpy (shm, matrix_l);
						client_light_go(player1_semaphore);
						traffic_light_stop();
					}
					else
					{
						strcpy (shm, "P2 WINS");
						client_light_go(player1_semaphore);
						client_light_go(player2_semaphore);
						color_red();
						printf("\nDEBUG: I'm cleaning semaphores...\n");
						clean_semaphore(traffic_light);
						clean_semaphore(player2_semaphore);
						clean_semaphore(player1_semaphore);
						color_red();
						printf("DEBUG: I'm cleaning memory...\n");
						clean_memory(memory_id);
						exit(0);
						//traffic_light_stop();		
					}
					/*client_light_go(player1_semaphore);
					traffic_light_stop();*/
			}
			
			//creo la matrice
			sleep(1);
			//inviata, avvio il client 2
			/*strcpy (shm, matrix_l);*/
			client_light_go(player1_semaphore);
			traffic_light_stop();
			/*client_light_go(player1_semaphore);
			traffic_light_stop();*/
		}

		
			
		}
		else
		{
			matrix_lower();
		}
	}
	else
		help();
	return 0;
}
	
