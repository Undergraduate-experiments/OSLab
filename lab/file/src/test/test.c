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



void test()
{
    //while(1)
    //{
      int pcb_num = nextpid - 1 ;
      do_open("file_1",pcb_num);
      printk("进程%d创建文件信息如下:\n",pcb_num);
      if(task[pcb_num].file_num == 0 )  {printk("no file create\n");}
      else
      {
        int i = 0 ; 
        for(i = 0 ; i < task[pcb_num].file_num ; ++i)
        {
             printk(" 进程%d创建的第%d个文件信息如下:\n",pcb_num,i+1);
             struct file_desc * p = task[pcb_num].filp[i];
             printk(" 文件权限: ");
             if(p->fd_mode == read)   {printk("只读 ");}
             else                     {printk("可读可写 ");}
             
             struct inode* m = p->fd_inode;
             printk("文件类型: ");
             if(m->i_mode == I_CREAT_PROC)        {printk("进程创建文件 ");}
             else if (m->i_mode == I_DIRECTORY)   {printk("目录文件 ");}
             else                                 {printk("设备文件 ");}  

             printk("文件大小: %d, 文件第一个扇区号: %d, 文件占用扇区个数: %d \n",m->i_size,m->i_start_sect,m->i_nr_sects);          
        }
      }
     
      printk("\n打印所有目录信息\n");
      printk("结点号    文件名  \n");
      int i = 0 ;
      for(i = 0 ; i < dir_entry_cnt ; ++i)
      {    
         printk("  %d       %s\n",dir_array[i].inode_nr,dir_array[i].name);
      }
       
      printk("\nfile system is ok!\n");
      
      //wait_intr();
    //}
}


void file()
{
   init_fs(&super);
   kthread_create(test,"test");
}










