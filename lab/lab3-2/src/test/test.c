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
void p1_send()  
{

   while(1)
  {
     struct Message  m ;
     m.src = 1;
     m.dest = 2;
     memcpy(m.payload,"hello",7);
     send(2,&m);
     wait_intr();
   }

}

void p2_recevice()
{
        
    while(1)
   {
     struct Message  m ;
     receive(1,&m);
     printk("recevice: %s\n",m.payload);
     wait_intr();
    }
}



void test_proc_Msg()
{
     init_semtab();
     init_semaphore();
     list_init(&msg_list);
     kthread_create(p1_send,"p1");
     kthread_create(p2_recevice,"P2");
}

void init_semaphore()
{
   Msg_sem  = new_sem(0);
}

















