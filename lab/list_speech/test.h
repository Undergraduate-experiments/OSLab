#include "list.h"
#define KSTACKSIZE 1024
#define NR_PROC 64
enum proc_state{UNUSED=0,RUNNABLE,RUNNING,BLOCKED,STOPED};
struct task_struct{
      pid_t pid;
      char name[20];
      enum proc_state state;
      ListHead linklist;
};

void meun();
ListHead init();

