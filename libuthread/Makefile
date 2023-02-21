# Target library
lib := libuthread.a

all: $(lib)

libuthread.a: queue.o uthread.o context.o sem.o preempt.o
	ar rcs libuthread.a queue.o uthread.o context.o sem.o preempt.o

queue.o: queue.c queue.h
	gcc -Wall -Wextra -Werror -c -o queue.o queue.c

context.o: context.c private.h
	gcc -Wall -Wextra -Werror -c -o context.o context.c

uthread.o: uthread.c uthread.h context.o
	gcc -c -o uthread.o uthread.c

sem.o: sem.c sem.h queue.h private.h
	gcc -c -o sem.o sem.c

preempt.o: preempt.c private.h uthread.h
	gcc -c -o preempt.o preempt.c

clean:
	rm libuthread.a queue.o uthread.o sem.o preempt.o