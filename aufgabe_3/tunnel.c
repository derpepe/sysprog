#include <stdio.h>
#include <time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/msgbuf.h>
#include <sys/shm.h>

#include <signal.h>
#include <stdlib.h>

#include "myConsts.h"


int msg_id;
int shm_id;
char *shm_ptr;


// INThandler() is used to ensure, that shm and mq are freed correctly on interruption
void INThandler(int i) 
{	
	// SharedMemory freigeben
	shmdt(shm_ptr); // detach
	shmctl(shm_id, IPC_RMID, 0); // free
    
	// Message Queue freigeben
	msgctl(msg_id, IPC_RMID, 0); // free
	
	exit(0);
}


int main( int argc, const char* argv[] )
{
	int startzeit = time(NULL);
	int elapsed = 0;
	int now = 0;
	int now10 = 0;

	int tunnelStatus = TUNNEL_LEER;
	int sema = TUNNEL_CAPACITY;

	int simulationActive = 1;
	
	int myDirection;
	
	
	// MessageQueue initialisieren
	printf("Initialisiere MessageQueue...");
	struct myMsg dataMsg;
	msg_id = msgget(msqKey, IPC_CREAT | 0666);
	if (msg_id < 0) {
		perror("msgget");
	} else {
		printf("ok (waiting on key %i)\n", msqKey);
	}	
	
	// Shared Memory initialisieren	
	printf("Initialisiere SharedMemory...");
	shm_id = shmget(shmKey, sizeof(tunnelStatus) + sizeof(sema), 0666 | IPC_CREAT);
    if (shm_id < 0) {
		// Fehler
        perror("shmget");
	} else {
		shm_ptr = shmat(shm_id, 0, 0); // Speicher holen
		if (shm_ptr == (char *)-1) {
        	perror("shmat");
    	}
	}
	// write initial values into memory
	shm_ptr[0] = tunnelStatus;
	shm_ptr[sizeof(tunnelStatus)] = sema;
	printf("ok (using key %i)\n", shmKey);
	
	// STRG+C abfangen
	if (signal(SIGINT, INThandler) == SIG_ERR) perror("SignalFehler!");
	
	
	// Tunnel ist bereit und nimmt ab hier Nachrichten entgegen
	// ab diesem Zeitpunkt kann man den Monitor und die Autos starten
	printf("\nTunnel is ready and empty.\n\n");
	while (simulationActive == 1) {
		if (msgrcv(msg_id, &dataMsg, 3 * sizeof(int), msgTyp, IPC_NOWAIT) != -1) {
			printf("Process with PID %i sends message %i and direction %i\n", dataMsg.pid, dataMsg.msg, dataMsg.direction);

			// Frage Nachricht ab
			switch (dataMsg.msg) {
				case arriveNorth:
				case arriveSouth:
					myDirection = dataMsg.direction;
					if (((tunnelStatus == TUNNEL_LEER) || (tunnelStatus == myDirection)) && (sema > 0)) {
						// Auto darf einfahren
						printf("car %i is allowed to come in. Sending SIGCONT.\n", dataMsg.pid);
						kill(dataMsg.pid, SIGCONT); // Auto Signal schicken
						sema--; // Semaphor erwerben
						if (tunnelStatus != myDirection) {
							printf("As car %i drives in, direction changed to ", dataMsg.pid);
							switch (myDirection) {
								case TUNNEL_NS:
									printf("North/South\n");
									break;
								case TUNNEL_SN:
									printf("South/North\n");
									break;
							}
							tunnelStatus = myDirection;
						}
					} else {
						// Auto muss warten
						printf("car %i is NOT allowed to come in. Re-Adding car to end of message queue.\n", dataMsg.pid);
						if (msgsnd(msg_id, &dataMsg, 2* sizeof(int), 0) == -1) {
							// Fehler beim Senden
							printf("could not re-add car %i to message queue.\n", dataMsg.pid);
							perror("msgsnd");
						} else {
							printf("car %i re-added to message queue successfully.\n", dataMsg.pid);
						}
					}
					break;
				case leaveNorth:
				case leaveSouth:
					sema++; // Semaphor freigeben
					if (sema == TUNNEL_CAPACITY) {
						tunnelStatus = TUNNEL_LEER;
					}
					kill(dataMsg.pid, SIGCONT);
					break;
			}

			// Ausgabe erzwingen
			printf("\n");
			fflush(stdout);

			// Nachrichten wurden hier verarbeitet
			// jetzt noch in den SharedMemory schreiben
			shm_ptr[0] = tunnelStatus;
			shm_ptr[sizeof(int)] = sema;
	
		} // if

		// Programm beendet sich nach 100s von selbst
		elapsed = time(NULL) - startzeit;
		if (elapsed > now)
		{
			if (elapsed - now10 >= 10) {
				now10 = elapsed;
				printf("\n%is elapsed\n", elapsed);
			}
			now = elapsed;
			if (now >= 100) simulationActive = 0;
		}

		sleep(1);
		
	} // while
	
	// SharedMemory und Message Queue freigeben
	INThandler(0);
}