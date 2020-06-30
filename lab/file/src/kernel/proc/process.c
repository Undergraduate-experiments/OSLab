#include "sched.h"
#include "vm.h"
//extern int nextpid = 0 ;
int nextpid = 0 ;
//struct task_struct * current;
//进程表初始化
void task_init(){ 
  int i = 0;
  for(i=0; i<NR_PROC; i++)
  {
       task[i].state = UNUSED; //当前系统中没有进程
  }
   list_init(&RunableList);
}

//创建 0 号进程
void idle_init(){
//使用 task[0],用于表示内核代码执行流代表的进程。
    struct task_struct *p;
    task[0].state=RUNNING;
    p=&task[0];
    //
     printk("pid = %d ,init OS ok !\n",nextpid);
    //
    p->pid=nextpid++; //nextpid 为全局变量,初始化为 0,用于记录进程号
    memcpy(p->pname,"idle",5);
    current=p; //系统启动时,0 号进程为当前进程
}

//初始化 PCB
struct task_struct * init_pcb(struct task_struct *p,void(*proc)(void),const char *name){
	p->pid =nextpid++; //定义进程号
        p->file_num = 0;
	p->state=RUNNABLE;//设置进程状态
        p->message = Msg_sem;
	TrapFrame *tf=(TrapFrame*)(memset(p->kstack,0,KSTACKSIZE)+KSTACKSIZE)-1; //初始寄存器组
	p->tf=tf;
	tf->eflags=(uint32_t)(0x01<<9);
	tf->cs=(uint32_t)KSEL(SEG_KCODE);
	tf->ds=(uint32_t)KSEL(SEG_KDATA);
	tf->gs=(uint32_t)KSEL(SEG_KDATA);
	tf->eip=(uint32_t)proc;
	tf->esp=(uint32_t)tf;
	tf->irq=0x03e8;
	memcpy(p->pname,name,7);//设置进程名字
	list_add_after(&RunableList,&p->linklist);
	return p;
}

struct task_struct * alloc_proc() {
   int i = 0 ;
    struct task_struct *p = NULL;
    for(i=1; i < NR_PROC;i++){
      if(task[i].state == 0) {p = &task[i]; break;}
  }
    return p ;
}
//进程创建
struct task_struct * kthread_create(void(*proc)(void),const char* name){
    struct task_struct *p = alloc_proc();
    init_pcb(p,proc,name);  
    //
    int pidnum = nextpid - 1 ;
    printk("%dcreated %s\n",pidnum,name);
    //
    return p ;     
}


struct task_struct * find_pcb(int pid)
{
     int pos = 0 ;
     for(int i = 0  ; i < NR_PROC ; i++ )  {if(i == pid)  {pos = i ; break;}}
     return &task[pos];
}

//4.3 进程创建
struct task_struct * do_fork(void(*proc)(void),const char* name)
{
   struct task_struct *p = alloc_proc();
   init_pcb(p,proc,name);
   copy_mem(p);
   int pidnum = nextpid - 1 ;
   printk("pid is %d created  name is %s father is 0 \n",pidnum,name);
   return p; 
}

int copy_mem(struct task_struct *p)
{
  	//int nice;
	p->counter = ++task[0].counter; 
	p->ppid = task[0].pid;             //父进程ID
	p->start_code = p->pid *  PROC_IMAGE_SIZE_DEFAULT ;  //代码段地址
	p->end_code   = PROC_IMAGE_SIZE_DEFAULT;    //代码段长度(字节数)
	p->end_data   = PROC_IMAGE_SIZE_DEFAULT;    //代码长度+数据长度（字节数）
	p->brk        = PROC_IMAGE_SIZE_DEFAULT;         //总长度（字节数）
	p->start_stack   = p->pid *  PROC_IMAGE_SIZE_DEFAULT;   //堆栈段地址 
        
        //int New_data_base = p->pid*PROC_IMAGE_SIZE_DEFAULT;
        //int New_code_base = p->pid*PROC_IMAGE_SIZE_DEFAULT;
         unsigned long address = p->pid*PROC_IMAGE_SIZE_DEFAULT;
        while(address < p->pid*PROC_IMAGE_SIZE_DEFAULT + PROC_IMAGE_SIZE_DEFAULT)
        {
           put_page(get_free_page(),p->pid*PROC_IMAGE_SIZE_DEFAULT);
           address = address +  4096;
        } 
   
      
        return p->pid;
}

void do_open(char filename[MAX_FILENAME_LEN],int pcb_num)
{
   int new_inode = create_file(filename);
   
   file_table[file_table_cnt].fd_mode  = read;
   file_table[file_table_cnt].fd_pos   = file_table_cnt;
   file_table[file_table_cnt].fd_inode = &inode_array[new_inode] ;
   
   
   if(task[pcb_num].file_num < NR_FILES) { task[pcb_num].filp[task[pcb_num].file_num] = &file_table[file_table_cnt];}
   else {printk("NR_FILES is full ,can not add file any more\n"); return;}

   file_table_cnt++;
   task[pcb_num].file_num++;
   
}
