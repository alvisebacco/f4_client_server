#!/bin/bash

gcc F4_server.c errors.c graphical_functions.c semaphores.c shared_memory.c -o server
gcc F4_client.c errors.c graphical_functions.c semaphores.c shared_memory.c -o client
