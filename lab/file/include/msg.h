#include "sem.h"
#define MSG_FREE 0
#define MSG_USED 1 
#define NR_MSG 20
/*struct Message{
   pid_t src,dest;
   int type;
   char payload[20];
};
*/
struct MsgBuff{
   struct Message msg;
   int state;
   ListHead linklist;
};

struct MsgBuff msg_buff[NR_MSG];
ListHead msg_list;

void init_msgtab();
void receive(int pid , struct Message *m);
void send(int pid , struct Message *m);
