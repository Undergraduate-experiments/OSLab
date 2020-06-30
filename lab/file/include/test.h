#include "msg.h"
#include "vm.h"
#define BUFFER_SIZE 10
typedef struct item {
    int num;
}item;

item buffer[BUFFER_SIZE];
extern int in ;
extern int out ;
void producer();
void consumer();
void test_proc();
void a();
void b();
void c();
void test_proc();
void display();
void test_proc_Msg();
void init_semaphore();
void  four_p1();
void four_one();
void print_mem_map();
void  four_p2();
void four_two();
void four_p3();
//void print_dir_page();
void test();
void file();
