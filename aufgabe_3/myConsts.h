#define	TUNNEL_CAPACITY	999	// cars simultaniously allowed in tunnel

#define	TUNNEL_LEER	0
#define	TUNNEL_NS	1
#define	TUNNEL_SN	2

#define shmKey		2404	// key for sharedMemory

#define arriveNorth	97 		// just for easier debugging
#define arriveSouth	98
#define leaveNorth	99
#define leaveSouth	100


// Message queue
#define	msqKey		2312	// key for message queue

struct myMsg {
  long mtype;
  int msg;
  int pid;
  int direction;
};

#define msgTyp	1			// has to be != 0