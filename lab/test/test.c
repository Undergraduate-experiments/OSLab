#include "test.h"
void meun(void){
printf("1.初始化就绪队列\n");
printf("2.插入进程\n");
printf("3.循环遍历进程\n");
printf("4.结束程序\n");
}
int main(void){
   int nextpid = 0 , order = 0 , flag = 1;
   ListHead RunableList;            //就绪队列
   struct task_struct task[NR_PROC];//进程表
   struct task_struct *current;
 while(flag){
   //input
   while(1){
        meun();
        int r = scanf("%d",&order);
        if(!r || order < 1 || order > 4)   {printf("please input again\n");}
        else                               {break;}
   }
  switch(order){
   case 1 :  {
                int i = 0;
                list_init(&RunableList);  
                for(i = 0 ;  i < NR_PROC ; ++i) {task[i].state = UNUSED;} 
                current = &task[0];
                current->pid = nextpid++;
                current->state = RUNNING;
                strcpy(current->name,"idle");
                break;
              }
   case 2 :  {
                  struct task_struct *data ;
                  int i = 1 ;
                  for(i=1; i < NR_PROC;i++){
                  if(task[i].state == 0) {data = &task[i]; break;} }  
                  printf("输入进程名\n");
                  scanf("%s",data->name);
                  data->pid = nextpid++;
                  data->state = RUNNABLE;
                  list_add_after(&RunableList, &data->linklist);
                  break;                
             }

   case 3 :  {
                int cnt = 0 ;
                while(1){
                struct task_struct *p;
                if(current->pid != 0){current->state  = RUNNABLE; list_add_before(&RunableList,&current->linklist);}
                p=list_entry (RunableList.next,struct task_struct,linklist);
                p->state = RUNNABLE;
                printf("%d..%s..",p->pid,p->name);
                list_del(&p->linklist);
                current =p;
                ++cnt;
                if(cnt == 15)  {break;}
                }
                printf("\n");
                break;
              }
   case 4 :   {
                   printf("the program is stoping...");
                   flag  = 0;
                   sleep(1);
                   break;
              }
  } 
 }  
return 0 ;
}
