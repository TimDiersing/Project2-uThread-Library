#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sem.h>
#include <uthread.h>

int flag1, flag2;
static void thread3(void *arg)
{
    (void)arg;

    flag2 = 0;
}

static void thread2(void *arg)
{
    (void)arg;

    flag1 = 0;
}

static void thread1(void *arg)
{
    (void)arg;
    flag1 = 1;
    flag2 = 1;
    int loopAmount = 100000000;
    int count = loopAmount;

    uthread_create(thread2, NULL);
    uthread_create(thread3, NULL);

    while (flag1 == 1 || flag2 == 1)
    {
        if (count <= 0) 
        {
            if (write(2, "IN LOOP\n", 9)) {}
            count = loopAmount;
        }
        count--;
    }

    if(write(2, "DONE!\n", 7)) {}

}

int main(void) 
{
    uthread_run(true, thread1, NULL);
}