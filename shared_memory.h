#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

int create_memory(int memory);
int get_memory(int memory);
char *attach_memory(int shmid);
void clean_memory(int id);

#endif
