#include "sched.h"
#define NR_SEMS 32
#define SYSERR -1
#define SYSOK   1
struct semaphore
{
    int value;
    int used ;
    ListHead waitlist;
};

struct semaphore semtab[NR_SEMS];

void init_semtab();
struct semaphore *new_sem(int value);
int delete_sem(struct semaphore *sem);
void P(struct semaphore *sem);
void V(struct semaphore *sem);
