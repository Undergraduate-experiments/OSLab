#include "sched.h"
int schedule(){
     struct task_struct *p;
     // OLD == CURRENT
     if (current->pid != 0) {current->state  = RUNNABLE; list_add_before(&RunableList,&current->linklist);}
     p=list_entry (RunableList.next,struct task_struct,linklist);
     p->state = RUNNABLE;
     list_del(&p->linklist);
     current =p;
     return current->pid;
}
