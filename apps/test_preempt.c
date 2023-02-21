#include <stdio.h>
#include <stdlib.h>

#include <sem.h>
#include <uthread.h>

static void thread3(void *arg)
{
    (void)arg;

    fprintf(stderr, "hi3\n");

}

static void thread2(void *arg)
{
    (void)arg;

    fprintf(stderr, "hi2\n");

}

static void thread1(void *arg)
{
    (void)arg;

    uthread_create(thread2, NULL);
    uthread_create(thread3, NULL);

    for (int i = 0; i < 100000; i++) {
            fprintf(stderr, "hey%d\n", i);
    }
    fprintf(stderr, "hi1\n");
}

int main(void) 
{
    uthread_run(true, thread1, NULL);
}