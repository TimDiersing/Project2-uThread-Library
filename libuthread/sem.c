#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	size_t count;//count of resources
	queue_t waitingList;//waiting list of threads in case count = 0
};

sem_t sem_create(size_t count)
{
	sem_t  s = (sem_t)malloc(sizeof(struct semaphore));
	if(s == NULL){
		return NULL;
	}
	s->count = count;
	s->waitingList = queue_create();
	if(s->waitingList == NULL){//if create fails
		free(s);
		return NULL;
	}
	return s;
}

int sem_destroy(sem_t sem)
{
	if(sem == NULL){
        return -1;
    }
	queue_destroy(sem->waitingList);//remove all data
    free(sem);
    return 0;
}

int sem_down(sem_t sem)
{
	if(sem == NULL){
		return -1;
	}
	while(sem->count == 0){//wait for sem to have resources
		struct uthread_tcb* currentthreadcop = uthread_current();//temporary variable so current thread doesn't get lost
		queue_enqueue(sem->waitingList,currentthreadcop);//add thread to waiting line for threads
		uthread_block();//block thread thats trying to access resources
	}
	sem->count--;
	return 0;
}

int sem_up(sem_t sem)
{
	if(sem == NULL){
		return -1;
	}
	if(queue_length(sem->waitingList)!=0){//if there are any waiting, let them access
		struct uthread_tcb* threadub;
		queue_dequeue(sem->waitingList,(void**)&threadub);
		uthread_unblock(threadub);
	}
	sem->count++;
	return 0;
}





















// #include <stddef.h>
// #include <stdlib.h>

// #include "queue.h"
// #include "sem.h"
// #include "private.h"

// struct semaphore {
//     size_t count;

// };

// sem_t sem_create(size_t count)
// {
//     struct semaphone sem;
//     sem.count = count;

//     return sem;
// }

// int sem_destroy(sem_t sem)
// {
// 	/* TODO Phase 3 */
// }

// int sem_down(sem_t sem)
// {
// 	sem.count -= 1;
// }

// int sem_up(sem_t sem)
// {
// 	sem.count += 1;
// }

