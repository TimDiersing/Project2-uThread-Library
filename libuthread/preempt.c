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

struct sigaction old_action;
struct itimerval old_timer;
sigset_t block_alarm;
bool preemptE = false;

// Catches the SIGVTALRM and causes the thread to yeild to the next
void alarm_handler(int signum)
{
    (void)signum;
    uthread_yield();
}

void preempt_disable(void)
{
    // Do nothing if preempt isn't enabled
    if (!preemptE)
        return;

    // Add SIGVTALRM back to the set and block it
    sigaddset(&block_alarm, SIGVTALRM);
    if (pthread_sigmask(SIG_BLOCK, &block_alarm, NULL) == -1) 
    {
        perror("sigmask");
        exit(1);
    }
}

void preempt_enable(void)
{
    // Do nothing if preempt isn't enabled
    if (!preemptE) 
        return;

    // Add SIGVTALRM back to the set and unblock it
    sigaddset(&block_alarm, SIGVTALRM);
    if (pthread_sigmask(SIG_UNBLOCK, &block_alarm, NULL) == -1) 
    {
        perror("sigmask");
        exit(1);
    }
}

void preempt_start(bool preempt)
{
    if (!preempt)
        return;

    preemptE = true;

    struct itimerval interval;
    struct sigaction interrupt_timer;

    // setup the alarm handler to catch signals of SIGVTALRM
    interrupt_timer.sa_handler = alarm_handler;
    sigemptyset(&interrupt_timer.sa_mask);
    interrupt_timer.sa_flags = 0;
    if (sigaction(SIGVTALRM, &interrupt_timer, &old_action))
    {
        perror("sigaction");
        exit(1);
    }

    // Add SIGVTALRM to an empty set and block it
    sigemptyset(&block_alarm);
    sigaddset(&block_alarm, SIGVTALRM);
    if (pthread_sigmask(SIG_BLOCK, &block_alarm, NULL) == -1) 
    {
        perror("sigmask");
        exit(1);
    }

    // Setup a timer that calls SIGVTALRM 100 times a second
    interval.it_interval.tv_sec = 0;
    interval.it_interval.tv_usec = (1/HZ * 1000000);
    interval.it_value = interval.it_interval; 

    if (setitimer(ITIMER_VIRTUAL, &interval, &old_timer) == -1) {
        perror("setitimer");
        exit(1);
    }
}

void preempt_stop(void)
{
    // restore the process's old signal handler and timer
    sigaction(SIGVTALRM, &old_action, NULL);
    setitimer(ITIMER_VIRTUAL, &old_timer, NULL);
}