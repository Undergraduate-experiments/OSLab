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

//----------------------
void producer()
{
   item items ;
   items.num = in + 1 ;
   while(1){
    printk("full-value: %d.. empty-value: %d..\n",full->value,empty->value);
     P(empty);
     
     buffer[in] = items;
     in = (in + 1) % BUFFER_SIZE;
     V(full);
     printk("full-value: %d.. empty-value: %d..\n",full->value,empty->value);
     wait_intr();
   }

}

void consumer()
{
        
    item items ;
    while(1){
    printk("full-value: %d.. empty-value: %d..\n",full->value,empty->value);
    P(full);

    int a = 0  ; 
    a = items.num;
    items = buffer[out];
    a = buffer[out].num;
    buffer[out].num = a ;
    out  = (out+1)% BUFFER_SIZE;

    V(empty);

    printk("full-value: %d.. empty-value: %d..\n",full->value,empty->value);
    wait_intr();

    }
}



void test_proc_PV(){
     init_semtab();
     init_semaphore();
     kthread_create(producer,"producer_1");
     kthread_create(producer,"producer_2");
     kthread_create(consumer,"consumer_1"); 
     kthread_create(consumer,"consumer_2"); 
}

void init_semaphore()
{
   full   = new_sem(0);
   empty  = new_sem(10);
   mutex  = new_sem(1);
}

















