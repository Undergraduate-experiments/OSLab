#include "msg.h"
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
