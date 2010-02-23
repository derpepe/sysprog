#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include "myConsts.h"



void randomWait() {
	int sleepCycle = rand() % 2 + 3; // 3 - 5
	printf("Fahre %i s in der Gegend rum\n", sleepCycle);
	fflush(stdout); // flush output
	sleep(sleepCycle);
}


int main( int argc, const char* argv[] )
{
	srand(time(0)); //initializing random number generator
	
	int direction = TUNNEL_NS;
	if (argc > 1) {
		if (*argv[1] == '1') {
			direction = TUNNEL_SN;
		}
	}
	
	switch (direction) {
		case TUNNEL_NS:
			printf("car goes North/South\n");
			break;
		case TUNNEL_SN:
			printf("car goes South/North\n");
			break;
	}


	int simulationActive = 1;
	
	struct myMsg dataMsg;
	dataMsg.mtype = msgTyp;
	dataMsg.pid = getpid();

	// variables for signal handling
	sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCONT);
    sigaddset(&set, SIGINT);
	int mySig = SIGCONT;

	// message queue besorgen
    int msgID = msgget(msqKey, IPC_CREAT | 0666);
    if (msgID < 0) {
        perror("msgget");
		return 1;
	}
	
	int msgSize = 3*sizeof(int);
	printf("Queue mit key %i ok\n", msqKey);
	
	printf("id of car is %i\n", getpid());
	
	
	// starte Simulation
	while (simulationActive)
	{
		printf("\nAuto %i ist nun in Richtung ",getpid());
		switch(direction) {
			case TUNNEL_NS:
				printf("Nord/Süd");
				break;
			case TUNNEL_SN:
				printf("Süd/Nord");
				break;
		}
		printf(" unterwegs.\n");
		
		
		// an Tunnel heranfahren
		randomWait(); // 3-5s warten
	
		printf("Einfahrt...");
		dataMsg.direction = direction;
		if (direction == TUNNEL_NS) dataMsg.msg = arriveNorth; else dataMsg.msg = arriveSouth;
	
		fflush(stdout); // flush output
		if (msgsnd(msgID, &dataMsg, msgSize, 0) == -1) {
			// Fehler beim Senden
			perror("msgsnd");
			return 1;
		} else {
			printf("warte auf Bestätigungs-Signal...");
			fflush(stdout); // flush output
			sigwait(&set, &mySig);
		}
		printf("...und drin.\n");


		// Fahrzeug im Tunnel
		randomWait();

		printf("Ausfahrt...");
		dataMsg.direction = direction;
		if (direction == TUNNEL_NS) dataMsg.msg = leaveSouth; else dataMsg.msg = leaveNorth;
	
		fflush(stdout); // flush output
		if (msgsnd(msgID, &dataMsg, msgSize, 0) == -1) {
			// Fehler beim Senden
			perror("msgsnd");
			return 2;
		} else {
			printf("warte auf Bestätigungs-Signal...");
			sigwait(&set, &mySig);
		}
		printf("...und wieder draußen.\n");
		
		
		// neuen Status setzen (Auto "umdrehen")
		if (direction == TUNNEL_NS) direction = TUNNEL_SN; else direction = TUNNEL_NS;
	}
	
	return 0;
}