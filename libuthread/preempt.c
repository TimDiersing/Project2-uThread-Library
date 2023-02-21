#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction interrupt_timer;
struct sigaction old_action;
struct itimerval old_timer;
sigset_t block_alarm;

void alarm_handler(int signum)
{
    preempt_disable();
    fprintf(stderr, "YIELDDDDDDDDDDDDDDDD\n");
    uthread_yield();
}

void preempt_disable(void)
{
    pthread_sigmask(SIG_BLOCK, &block_alarm, NULL);
}

void preempt_enable(void)
{
    pthread_sigmask(SIG_UNBLOCK, &block_alarm, NULL);
}

void preempt_start(bool preempt)
{
    if (!preempt)
        return;

    struct itimerval interval;

    sigemptyset(&block_alarm);
    sigaddset(&block_alarm, SIGVTALRM);

    interrupt_timer.sa_handler = alarm_handler;
    sigemptyset(&interrupt_timer.sa_mask);
    interrupt_timer.sa_flags = 0;

    interval.it_interval.tv_sec = 0;
    interval.it_interval.tv_usec = HZ;
    interval.it_value.tv_sec = 0; 
    interval.it_value.tv_usec = HZ; 

    sigaction(SIGVTALRM, &interrupt_timer, &old_action);
    setitimer(ITIMER_VIRTUAL, &interval, &old_timer);
}

void preempt_stop(void)
{
    sigaction(SIGVTALRM, &old_action, NULL);
    setitimer(ITIMER_VIRTUAL, &old_timer, NULL);
}

