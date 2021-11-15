#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/shm.h>

#include "graphical_functions.h"
#include "errors.h"
#include "semaphores.h"
#include "shared_memory.h"

#define P1_S 20
#define P2_S 21
#define MIN_MATRIX 5
#define DIM_MSG 32
#define SEM_KEY 14
#define RW 0666
#define KEY 14
#define MSGKEY 75
#define MSGTYPE 1
#define DIM 50
#define RW 0666
