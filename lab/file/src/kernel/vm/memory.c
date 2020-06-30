#include "vm.h"
int cnt_use = 8 * 1024 * 1024 / 4096;
int cnt_unuse = 120 * 1024 * 1024 / 4096;
void mem_init(long start_mem,long end_mem)
//参数start_mem是可用作页面分配的主内存区起始地址；
//end_mem是实际物理内存最大值。
//used = 100 unused = 0 ;
{
         int i = 0 ;
	//1. 将PAGING_PAGES页的物理内存设置为已占用状态USED(100)
		for(i=0 ;i < PAGING_PAGES ; ++i )  {mem_map[i].ref= 100;}
	//2. 设置可用主内存区，将物理内存页框设置为未占用状态UNUSED(0)
	 int n=(end_mem-start_mem)/4096;
         int start = start_mem / 4096 ;
         for(int i = 0 ; i < n ; ++i)  {  
             mem_map[start++].ref= 0;
         } 
}


unsigned long get_free_page(void)
{
     for(int i = 0 ; i < PAGING_PAGES ; ++i )
     {
          if(mem_map[i].ref == 0)  { mem_map[i].ref = 100 ;cnt_use++ ;return  i * 4096 ;}
     }
     return -1;
}

void free_page(unsigned long addr)
{
        cnt_unuse++;
        mem_map[addr/4096].ref = 0;
}






