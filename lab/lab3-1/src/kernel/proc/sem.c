#include "sem.h"
int in = 0 ,out = 0;
void init_semtab()
{
   int i = 0 ;
   for( i = 0 ; i < NR_SEMS ; ++i) {semtab[i].used = 0;}   
}

struct semaphore *new_sem(int value)
{
   int i = 0 ;
   int flag = 0;
   struct semaphore *sem = NULL;
   for( i = 0 ; i < NR_SEMS ; ++i){
     if(semtab[i].used == 0)  {sem = &semtab[i]; semtab[i].used = 1 ; semtab[i].value = value ; flag = 0 ; break;}
   }
   if(flag) {printk("no enough semaphore\n");}
   return sem;
}

int delete_sem(struct semaphore *sem)
{
   if(sem->used == 0)                      {return  SYSERR;}
   else if (!list_empty(&sem->waitlist))   {return SYSERR;}
   else                                    {sem->used = 0 ;}
   return SYSOK;
}

void P(struct semaphore *sem)
{   
    sem->value--;
    if(sem->value < 0){
        current->state = STOPED;
        list_del(&current->linklist); 
        list_add_after(&sem->waitlist,&current->linklist);
        asm volatile("int $0x80");   
    }
}


void V(struct semaphore *sem)
{
     sem->value++; 
     struct task_struct *pcb;  
     if(sem->value <= 0) {
     pcb=list_entry (sem->waitlist.next,struct task_struct,linklist);
     list_del(&pcb->linklist);  
     pcb->state = RUNNABLE;
     list_add_before(&RunableList,&pcb->linklist);
     }
}


















