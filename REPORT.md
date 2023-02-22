# ECS150PROJ2
ECS 150 Project 2 Writeup:
###### Overview:
The goal of this project was to gain knowledge about threads by implementing a 
user-level thread library. This was completed in four phases, with each one 
leading into the one after. These phases were called queue API, uthread API, 
semaphore API, and preemption. These phases created a pathway where the goal of 
this project is obtained. 
###### Queue API:
This simple FIFO queue was implemented with the assistance of the header file
given. The constraint given for this API is that the program must have a runtime
of O(1) for most of the functions. This meant that an appropriate data structure
must be used. We decided to go with a linked list that held the pointers to the
first and last nodes and the length of the list. The Node struct held the data
itself and the next Node to make the implementation simpler. The creation and
destruction of the queue was simple. The creation of the queue entailed a malloc
call to allocate space and the initialization of the struct variable to their
respective initial values. The destruction of the queue was just a call to the
free method. The enqueue and dequeue methods are also similar to each other. The
usage of a linked list made the enqueue and dequeue process rather 
straightforward. The front and back had to be changed with the addition of the
new node. The dequeue method also performed the same way. A node was removed 
from the front, and the front would be altered. The iterate method was a little
trickier, but with assistance from the header file, a loop was created to access
each node in the list to run the function passed in as a parameter on all of the
functions. The last function was the length function which was the simplest of
all the functions. Since we chose to save the length of the list in the data 
structure, we just had to return the length as the size. All of these methods 
allowed the other APIs to build on top of the queue to create some interesting
programs. Testing for the queue was done by creating the methods that ran each
queue method so the functions perform as defined. The given queue_tester helped
greatly in completing the tester.
###### Uthread API:
The uthread library consists of several functions: uthread_run, uthread_create,
uthread_yield, and uthread_exit. The function uthread_run is called only by the
process's original thread and starts the multithreading library. It assigns the
original process as the “idle” thread and initializes the process that is given
to the function as the initial process. This initial process then has access to
all of the other functions in the library. The “idle” thread is initialized by 
saving the context of the original process it then runs an infinite loop that 
is in charge of destroying any excited processes and yielding to the next 
available process if there is one. 
The loop is exited when there are no more threads running (when the ready queue
is empty), and then executes queue_destroy and preempt_stop (if preempts were 
enabled) to clean up the process and restore it to its original state. 
Uthread_create is implemented by first, creating a new empty thread struct on
the heap and allocating memory to it. We then initialize it with the given 
function uthread_ctx_init using the passed function and arguments. This local
thread is then enqueued into the ready queue and awaits its turn to be called.
The uthread_yeild function is in charge of putting the current thread back on 
the ready queue (if it hasn't been blocked) and then context switching back to 
the idle thread to schedule the next thread to run. The scheduling works in a 
round-robin fashion that takes the thread from the ready queue that has been 
in the queue the longest and schedules it to run next. Uthread_exit works in 
a very similar fashion to uthread_yeild except for it marks the thread as 
exited so the idle thread can destroy its stack after it is switched. 
###### Semaphore API:
The semaphore API was rather simple regarding the implementation. The data
structure chosen was a struct that held the number of resources and the 
waiting list of queues that needed access to the resource if there weren't
any remaining. Using the queue as a data structure was very useful since a 
FIFO line was exactly what was needed for resource sharing. The sem up and 
down methods were completed using the queue create and destroy functions, 
along with a malloc and free respectively. The sem down function used the count
variable that the semaphore struct held to run a while loop until a resource
opens up. This is don by blocking the thread. This makes sure that the corner
case where thread starvation doesn't occur. The sem up function checked if 
the waiting list had any thread waiting and unblocks the thread waiting. 
Choosing this data structure made the implementation of each function simpler.
###### Preemption:
To start adding preemption we need to set a few things up. First, we need to 
use sigaction to direct the virtual alarm signal to the signal handler. We 
then make that signal handler yield the currently running process. We then 
needed to set up a timer that would send a virtual alarm signal 100 times a 
second. These actions together would let us yield the current running process
100 times a second to schedule out threads preemptively. Some precautions need
to be made however, we need preempt disable/enable functions in order to not 
yield the thread in areas where it isn't safe to do so. This involved changing
some code in the uthread.c file to ensure the process doesn't get interrupted 
unsafely. To test preempts, a new test file was created that never yielded 
voluntarily. The first thread needed the other threads to run in order to 
continue and print “done” to the console. This shows that the preemptive 
scheduler works because it was able to preemptively switch to the other 
threads and back to the initial thread in order to complete its execution.