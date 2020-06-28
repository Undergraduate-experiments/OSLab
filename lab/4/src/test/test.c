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
/*void p1_send()  
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
*/
void print_mem_map()
{
    int cnt = 0 ;
    int empty = 0;
    int full = 0;
    for(int i = 0 ; i < PAGING_PAGES ; ++i)
    {
       printk("%d ", mem_map[i].ref);
       if(mem_map[i].ref)  {++full;}
       else {++empty;}
       ++cnt;
       if(cnt == 10)   {printk("\n");cnt = 0;}
    }
    printk("\nused is %d unused is %d \n",full,empty);
}



void  four_p1()
{
  while(1){
   mem_init(9 * 1024 * 1024, 128 * 1024 * 1024);
   print_mem_map();
   wait_intr();
}
}

void  four_p2()
{
  while(1){
   printk("\n\n\n\n\n\n");
   print_dir_page();
   wait_intr();
   }

}


void four_p3()
{
  while(1){
    printk("do_fork success!\n");
    printk("\n\n\nprintf_mem_map\n\n\n");
    print_mem_map();
    printk("\n\n\nprintf_dir_page\n\n\n");
    print_dir_page();
wait_intr();
  }
}

void four_one()
{
      kthread_create(four_p2,"p1");
      //kthread_create(four_p1,"p2");
}

void four_two()
{
   // kthread_create(four_p3,"p1");
   do_fork(four_p3,"p1");
}

void init_semaphore()
{
   Msg_sem  = new_sem(0);
}

















