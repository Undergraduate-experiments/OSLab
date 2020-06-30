#include "list.h"
#include "debug.h"
#include "string.h"
#include "file.h"
#define KSTACKSIZE 1024
#define NR_PROC 64  
//4.3s
#define PROC_IMAGE_SIZE_DEFAULT  64*1024*1024 /*进程默认大小为64MB*/
//

//file
#define NR_FILES 4
//
enum proc_state{UNUSED=0,RUNNABLE,RUNNING,BLOCKED,STOPED};
struct semaphore
{
    int value;
    int used ;
    ListHead waitlist;
};
struct Message{
   pid_t src,dest;
   int type;
   char payload[20];
};
struct task_struct{
      TrapFrame *tf;
      char kstack[KSTACKSIZE];
      pid_t pid;
      char pname[20];
      enum proc_state state;
      ListHead linklist;

     struct semaphore       *message;
     struct Message  *msg;

     //4.3
	int nice; 
	int counter; 
	pid_t  ppid;             //父进程ID
	unsigned long start_code;   //代码段地址
	unsigned long end_code;   //代码段长度(字节数)
	unsigned long end_data;    //代码长度+数据长度（字节数）
	unsigned long brk;         //总长度（字节数）
	unsigned long start_stack;   //堆栈段地址 
       
     // 文件实验
        struct file_desc * filp[NR_FILES];
        int file_num;   
};
struct task_struct task[NR_PROC];//进程表

extern  int nextpid ;
struct task_struct *current; //当前进程
ListHead RunableList; //就绪队列

struct semaphore * Msg_sem;
void task_init();
void idle_init();
struct task_struct * init_pcb(struct task_struct *p,void(*proc)(void),const char *name);
struct task_struct * kthread_create(void(*proc)(void),const char* name);
struct task_struct * alloc_proc();
int schedule();


struct task_struct * find_pcb(int pid);

//4.3
int copy_mem(struct task_struct *p);
struct task_struct * do_fork(void(*proc)(void),const char* name);
//file
void do_open(char filename[MAX_FILENAME_LEN],int pcb_num);



