#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include "myConsts.h"

int main(int argc, char **argv)
{
	struct myMsg dataMsg;
	dataMsg.mtype = msgTyp;
	dataMsg.msg = arriveNorth;
	dataMsg.pid = getpid();
	dataMsg.direction = TUNNEL_NS;
	
	sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGCONT);
    


    int msgID = msgget(msqKey, IPC_CREAT | 0666);

    if (msgID < 0) {
        perror("msgget");
		return 1;
	}
	
	int msgSize = 3*sizeof(int);
	printf("Queue mit key %i ok\n", msqKey);
	printf("Sende Messagetyp %ld of size %i\n", dataMsg.mtype, msgSize);
	printf("Daten: msg=%i, pid=%i, direction=%i\n", dataMsg.msg, dataMsg.pid, dataMsg.direction);
	
	if (msgsnd(msgID, &dataMsg, msgSize, 0) == -1) {
		// Fehler beim Senden
		perror("msgsnd");
	} else {
		printf("Senden erfolgreich. Warte auf Signal.\n");
		int mySig = SIGCONT;
		sigwait(&set, &mySig);
	}
	printf("...und drin :)\n");
}