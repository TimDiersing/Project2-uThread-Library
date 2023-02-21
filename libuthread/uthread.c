#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

enum threadStates {
    RUNNING,
    READY,
    BLOCKED,
    EXITED
};

struct uthread_tcb* currentThread;
static uthread_ctx_t idleThread;
static queue_t readyQueue;

struct uthread_tcb {
	uthread_ctx_t ctx;
    char* stack;
    enum threadStates state;
};

struct uthread_tcb *uthread_current(void)
{
    return currentThread;
	/* TODO Phase 2/3 */
}

void uthread_yield(void)
{
    // struct uthread_tcb* prevThread;
    // prevThread = currentThread;

    // if (prevThread -> state != BLOCKED)
    //     prevThread -> state = READY;

    // queue_enquque(readyQueue, prevThread);

    // queue_dequeue(readyQueue, (void**)&currentThread)

    // while (currentThread -> state == BLOCKED) {

    //     queue_enqueue(readyQueue, currentThread);
    //     queue_dequeue(readyQueue, (void**)&currentThread)
    // }

    // uthread_ctx_switch(prevThread, currentThread);


    // Put the running thread back into the queue and set to ready if its not blocked
    if (currentThread -> state != BLOCKED)
    {
        currentThread -> state = READY;
        queue_enqueue(readyQueue, currentThread);
    }

    // Switch back to the idle thread
	uthread_ctx_switch(&(currentThread -> ctx), &idleThread);
}

void uthread_exit(void)
{
    preempt_disable();
    // Put exited thread back in queue and set to exited
    uthread_current() -> state = EXITED;
    queue_enqueue(readyQueue, currentThread);

    // Switch back to idle thread
    uthread_ctx_switch(&(currentThread -> ctx), &idleThread);
}

int uthread_create(uthread_func_t func, void *arg)
{
    // Create new empty thread
    struct uthread_tcb* thread;
    thread = malloc(sizeof(struct uthread_tcb));
    if (thread == NULL)
        return -1;

    // Allocate memory to stack of new thread
    thread -> stack = uthread_ctx_alloc_stack();
    if (thread -> stack == NULL)
        return -1;

    preempt_disable();
    // Initizalize new thread
    if (uthread_ctx_init(&thread -> ctx, thread -> stack, func, arg) != 0)
        return -1;

    // Add new thread to the ready queue
    if (queue_enqueue(readyQueue, thread) != 0)
        return -1;

    preempt_enable();
        
    return 0;
}

static void deleteExited(queue_t q, void* data) 
{
    struct uthread_tcb* node = (struct uthread_tcb*)data;

    // If the given node is in an exited state 
    //      delete it from the queue and free its stack.
    if (node -> state == EXITED) {
        queue_delete(q, data);
        uthread_ctx_destroy_stack(node -> stack);
        free(node);
    }
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    preempt_start(preempt);

    preempt_disable();

    // Create new queue for threads
    readyQueue = queue_create();
    if (readyQueue == NULL)
        return -1;
	
    // Create the initial thread with given function and arguments
    if (uthread_create(func, arg) != 0)
        return -1;

    // Start the idle loop to schedule threads
    while (1) 
    {
        preempt_disable();
        // Iterate through the queue to delete exited threads and free their stacks
        queue_iterate(readyQueue, deleteExited);

        // While there are still other threads, switch to the next thread in queue
        if (queue_length(readyQueue) > 0)
        {
            if (queue_dequeue(readyQueue, (void**)&currentThread) != 0)
                return -1;

            while (currentThread -> state == BLOCKED) {
                queue_enqueue(readyQueue, currentThread);

                if (queue_dequeue(readyQueue, (void**)&currentThread) != 0)
                    return -1;
            }

            currentThread -> state = RUNNING;

            //preempt_enable();
            uthread_ctx_switch(&idleThread, &(currentThread -> ctx));
        }

        // Break out of idle loop if there are no more threads to run
        else
        {
            if (preempt)
                preempt_stop();

            break;
        }
    }

    if (queue_destroy(readyQueue) != 0)
        return -1;

    return 0;
}

void uthread_block(void)
{
    currentThread -> state = BLOCKED;
    uthread_yield();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
    uthread -> state = READY;
    queue_enqueue(readyQueue, uthread);
}