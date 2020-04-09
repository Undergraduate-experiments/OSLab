#include "test.h"
void a(){ 
  while(1){
	printk("A");
	wait_intr();
      }
}
void b(){ 
  while(1){
	printk("B");
	wait_intr();
      }
}
void c(){ 
  while(1){
	printk("C");
	wait_intr();
      }
}
void test_proc(){
     kthread_create(a,"A_proc");
     kthread_create(b,"B_proc");
     kthread_create(c,"C_proc"); 
}

void display(){
   struct task_struct* p;
   ListHead *list;
   list = &RunableList;
   while(list->next != &RunableList){
    p = list_entry(list->next,struct task_struct ,linklist);
    printk("pid=%d...name=%s\n",p->pid,p->pname);
    list = list->next;
  }

}
