#include "sched.h"
int schedule(){
     struct task_struct *p;
     // OLD == CURRENT
     if (current->pid != 0 && current->state == RUNNING  ) {current->state  = RUNNABLE; list_add_before(&RunableList,&current->linklist);}
     p=list_entry (RunableList.next,struct task_struct,linklist);
     p->state = RUNNING;
     list_del(&p->linklist);
     current =p;
     printk("调度函数：  当前pid is %d... 信号量值 is %d\n",p->pid,p->message->value);
     return current->pid;
}
