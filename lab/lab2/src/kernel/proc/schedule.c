#include "sched.h"
void schedule(){
     printk("here\n");
     struct task_struct *Old_Proc= NULL ;
     Old_Proc=current;
     Old_Proc->state=RUNNABLE;
     list_add_after(&RunableList,&Old_Proc->linklist);

      struct task_struct  *New_Proc;
      list_for_each_entry (New_Proc, &RunableList, linklist) {
            list_del(&New_Proc->linklist);
            current=New_Proc;
      } 
}
