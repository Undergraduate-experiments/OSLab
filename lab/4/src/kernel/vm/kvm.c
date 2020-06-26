#include "vm.h"
#include "x86.h"
#include "string.h"
#include "kernel.h"
#define PG_ALIGN __attribute((aligned(PG_SIZE)))
PDE kpdir[NR_PDE] PG_ALIGN;            // the kernel page directory
PTE kptab[MEM_MAX / PG_SIZE] PG_ALIGN; // the kernel page tables
SegDesc gdt[NR_SEG];                   // the new GDT
TSS tss;                               // one TSS for all ring 3 processes

void
init_kvm(void) { // setup kernel memory mapping
	uint32_t pa;
	PTE *ptab = (PTE*)va_to_pa(kptab);
	PDE *pdir = (PDE*)va_to_pa(kpdir);
	for (pa = 0; pa < MEM_MAX; pa += PG_SIZE) {
		*ptab = PTE_P | PTE_W | pa;
		ptab ++;
	}
	memset((void*)va_to_pa(pdir), 0, PG_SIZE);
	for (pa = 0; pa < MEM_MAX; pa += PG_SIZE * NR_PTE) {
		*(pdir + KOFFSET / PG_SIZE / NR_PTE) = *pdir =
			PTE_P | PTE_W | va_to_pa(&kptab[pa / PG_SIZE]);
		pdir ++;
	}
	set_cr3((void*)va_to_pa(kpdir));  // set PDBR(CR3)
	set_cr0(get_cr0() | CR0_PG);      // enable paging
}

void
init_seg() { // setup kernel segements
	gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_KERN);
	gdt[SEG_KDATA] = SEG(STA_W,         0,       0xffffffff, DPL_KERN);
	gdt[SEG_UCODE] = SEG(STA_X | STA_R, 0,       0xffffffff, DPL_USER);
	gdt[SEG_UDATA] = SEG(STA_W,         0,       0xffffffff, DPL_USER);
	gdt[SEG_TSS] = SEG16(STS_T32A,      &tss, sizeof(tss)-1, DPL_KERN);
	set_gdt(gdt, sizeof(gdt));
}

//4.2


unsigned long put_page(unsigned long page,unsigned long address)
{
//参数page是get_free_page的返回值，表示分配主内存中某一页的地址
//参数address是线性地址
	//A.1 计算address在页目录表中对应的页目录指针
		int page_table=address>>22;       //dir
		int page_table1=page_table*4;
	//A.2 取得二级的页表首的 地址
                int p  = kpdir[page_table1]; 
                int q  = p&0xfffff000;
	//A.3 更新page_table中的表项内容，即将物理页面page地址填入表项中。
                 int a =  address >> 12;
                 a =  a&0x3ff;
                 *(&kptab[q] + a) = page;
      return page; 
}



void free_page_tables(unsigned long from,unsigned long size)
//参数from：起始线性基地址；
//参数size：释放的字节长度；
{
	//A.1 计算size所占用的页目录项数 即页表数；
	int Size =(size+0x3fffff)>>22;
	//A.2 基地址对应的起始页目录项:
	int dir=from>>22;
	//A.3 释放从dir开始的size个页目录项信息；
        int i = 0 ;
        for(i = 0 ; i < Size ; ++i)
        {
            int p  = kpdir[dir+i]; 
            int q  = p&0xfffff000;
            int j = 0 ;
            for(j = 0; j < 1024 ; ++j)
            {
                 *(&kptab[q] + j) = -1 ;    //-1代表空
            }           
        }
        
}


void print_dir_page()
{
     printk("\n\ndir printf\n");
     int cnt = 0;
     int i = 0 ;
     for(i = 0 ; i < 1024 ; ++i)
     {
        printk("%d ",kpdir[i]);
        cnt++;
        if(cnt == 20)  {printk("\n"); cnt = 0 ;}
     }

    printk("\n\npage_table printf\n");
     int cnts = 0;
     for(i = 0 ; i <  MEM_MAX / PG_SIZE ; ++i)
     {
        printk("%d ",kptab[i]);
        cnts++;
        if(cnts == 1024)  {printk("\n\n\n\n"); cnts = 0 ;}
     }    
}



