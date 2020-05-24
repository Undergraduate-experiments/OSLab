#include "x86.h"
#include "list.h"
#include "string.h"
#include "debug.h"
#include "string.h"
#define KSTACKSIZE 1024
#define NR_PROC 64
enum proc_state{UNUSED=0,RUNNABLE,RUNNING,BLOCKED,STOPED};
struct semaphore
{
    int value;
    int used ;
    ListHead waitlist;
};
struct Message{
   pid_t src,dest;
   int type;
   char payload[20];
};
struct task_struct{
      TrapFrame *tf;
      char kstack[KSTACKSIZE];
      pid_t pid;
      char pname[20];
      enum proc_state state;
      ListHead linklist;

     struct semaphore       *message;
     struct Message  *msg;
};
struct task_struct task[NR_PROC];//进程表

extern  int nextpid ;
struct task_struct *current; //当前进程
ListHead RunableList; //就绪队列

struct semaphore * Msg_sem;
void task_init();
void idle_init();
struct task_struct * init_pcb(struct task_struct *p,void(*proc)(void),const char *name);
struct task_struct * kthread_create(void(*proc)(void),const char* name);
struct task_struct * alloc_proc();
int schedule();


struct task_struct * find_pcb(int pid);


