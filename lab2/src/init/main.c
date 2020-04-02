#include "kernel.h"
#include "x86.h"
#include "vm.h"
#include "irq.h"
#include "test.h"
//extern  int nextpid ;
void os_init(void) {
	init_seg();
	init_debug();
	init_idt();
	init_i8259();
	printk("%d+%d=%d",10,12,22);
	printk("\n%s","The OS is now working!\n");
	printk("%s","hello world\n");
	printk("Lu Tong OS ...\n");
	sti();
        //
        task_init();
        idle_init();
        test_proc();
        int i = 1;
         // printk("%d %d\n",i,task[1].pid);
        while(i < nextpid) {
        printk("%d...name=%s\n",task[i].pid,task[i].pname);
        ++i;
        }
	while (TRUE) {
		wait_intr();
	}
}

void
entry(void) {
	init_kvm();
	void(*volatile next)(void) = os_init;
	asm volatile("addl %0, %%esp" : : "p"(KOFFSET));
	next();
	panic("init code should never return");
}

