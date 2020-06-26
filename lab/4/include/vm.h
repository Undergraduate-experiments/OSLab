#ifndef __VM_H__
#define __VM_H__

#define KOFFSET         0xC0000000
#define MEM_MAX         (128 * 1024 * 1024)
#define PAGING_PAGES  (128 * 1024 * 1024 / 4096)
#define va_to_pa(addr)  (((uint32_t)(addr)) - KOFFSET)
#define pa_to_va(addr)  (((uint32_t)(addr)) + KOFFSET)

void init_kvm(void);
void init_seg(void);


//4.1


typedef struct page{
	int ref;
}mem_map_t;

mem_map_t mem_map[PAGING_PAGES];

extern int cnt_use;
extern int cnt_unuse;
void mem_init(long start_mem,long end_mem);
unsigned long get_free_page(void);
void free_page(unsigned long addr);

//4.2
unsigned long put_page(unsigned long page,  unsigned long address);
void free_page_tables(unsigned long from,unsigned long size);
void print_dir_page();
#endif
