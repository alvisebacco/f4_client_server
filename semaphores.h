#ifndef SEMAPHORES_H
#define SEMAPHORES_H

void clean_semaphore(int id);
int traffic_light_creation();
void traffic_light_go();
void traffic_light_stop();
int client_light_creation(int key);
void client_light_go(int key);
void client_light_stop(int key);
int get_traffic_light();

#endif

