#include "list.h"
#include "x86.h"
#include "string.h"
#include "debug.h"
#define KSTACKSIZE 1024
#define NR_PROC 64
enum proc_state{UNUSED=0,RUNNABLE,RUNNING,BLOCKED,STOPED};
struct task_struct{
      TrapFrame *tf;
      char kstack[KSTACKSIZE];
      pid_t pid;
      char pname[20];
      enum proc_state state;
      ListHead *linklist;
};
struct task_struct task[NR_PROC];//进程表

extern  int nextpid ;
//extern struct task_struct *current; //当前进程
ListHead RunableList; //就绪队列


void task_init();
void idle_init();
struct task_struct * init_pcb(struct task_struct *p,void(*proc)(void),const char *name);
struct task_struct * kthread_create(void(*proc)(void),const char* name);
struct task_struct * alloc_proc();


