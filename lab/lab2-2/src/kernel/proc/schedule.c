#include "sched.h"
int schedule(){
     struct task_struct *p;
     // OLD == CURRENT
     if(list_empty(&RunableList))  {current = &task[0];}
     else if(current->state == STOP)      {
        list_add_before(&RunableList,&current->linklist);
        p =  select_max();
        p->state = RUNNING;
        list_del(&p->linklist);
        current =p;
     }
     else if (current == &task[0]){
        p =  select_max();
        p->state = RUNNING;
        list_del(&p->linklist);
        current =p;
     }
     else {current = current ;}
    -- current->cnt; 
     return current->pid;
}

struct task_struct * select_max(){
         ListHead* h = RunableList.next;
         struct task_struct  * pre ;
         struct task_struct * now ;
         pre=list_entry (RunableList.next,struct task_struct,linklist);
         now = pre ;
         int precnt = pre->cnt ;
         int mun =  1 ;
         for(mun  = 1 ; mun < nextpid ; ++mun){
            now =  list_entry (h,struct task_struct,linklist);
            if(now->state == RUNNABLE && now->cnt >= precnt)  {pre = now; precnt = now->cnt;}            
            h = h->next ;          
         }
         //reset cnt
        if(precnt == 0){
          reset();
          pre =  select_max();
        }
        return pre ;        
}

void reset(){
      ListHead* h = RunableList.next;
      struct task_struct * now ;
      int mun = 1 ;
      for(mun  = 1 ; mun < nextpid ; ++mun){
           now = list_entry (h,struct task_struct,linklist);
           now->cnt = now->nice;
           now->state = RUNNABLE;
           h = h->next ;
      }

}









