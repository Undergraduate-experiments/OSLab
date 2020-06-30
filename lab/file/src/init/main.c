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
        //
        task_init();
        idle_init();
        /*test_proc();
        display();*/
        
        /*
        //4.1
        mem_init(9 * 1024 * 1024, 128 * 1024 * 1024);
        //printk("\n\n init_map \n\n");
        //print_mem_map();
        printk("\n\n\n\n\\n");
        //4.2
        // four_one();
        //4.3
        four_two();
        */
        
        //file
        file();
     
        sti();
	while (TRUE) {
		wait_intr();
	}
        sti();
}

void
entry(void) {
	init_kvm();
	void(*volatile next)(void) = os_init;
	asm volatile("addl %0, %%esp" : : "p"(KOFFSET));
	next();
	panic("init code should never return");
}

