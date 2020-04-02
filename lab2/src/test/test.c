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

