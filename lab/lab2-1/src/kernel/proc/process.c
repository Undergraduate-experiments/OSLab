#include "sched.h"
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
	p->state=RUNNABLE;//设置进程状态
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
