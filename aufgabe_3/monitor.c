#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "myConsts.h"

#define ever ;; 


int main(int argc, char **argv)
{
	int shID;
	char *myPtr;
	int direction = TUNNEL_LEER;
	int old_direction;
	int sema = TUNNEL_CAPACITY;
	int old_sema;
	int carCount;
	
	// initialize SharedMemory
    shID = shmget(shmKey, 2 * sizeof(int), 0666);
    if (shID < 0) {
		// Fehler
		puts("Did you forget to start the 'tunnel'?");
        perror("shmget");
		return 1;
    }
	printf("Monitor listens to sharedMemory using key %i\n\n", shmKey);
	   myPtr = shmat(shID, 0, 0);
	   if (myPtr==(char *)-1) {
	       perror("shmat");
	   }
	
	// loop as long as sharedMemory is accessible
	while (shmget(shmKey, 2 * sizeof(int), 0666) >= 0)
	//for(ever)
	{
	   	direction = myPtr[0];
	   	sema = myPtr[sizeof(int)];
		if ((direction != old_direction) || (sema != old_sema)) {
			old_direction = direction;
			old_sema = sema;
			carCount = TUNNEL_CAPACITY - sema - 1024;
			switch (direction) {
				case TUNNEL_LEER:
					printf("tunnel empty\n");
					break;
				case TUNNEL_NS:
					printf("tunnel used from North to South by %i cars\n", carCount);
					break;
				case TUNNEL_SN:
					printf("tunnel used from South to Noth by %i cars\n", carCount);
					break;
				default:
					printf("whatever may happen in that tunnel :-?\n");
					break;
			}
			fflush(stdout);
		}
		sleep(1);
	} // while
	
	printf("\nsharedMemory no longer available. Most likely, 'tunnel' just ended.\n");
	
	// detach sharedMemory
	shmdt(myPtr); 
}