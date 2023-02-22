lib := libuthread.a
CC := gcc
CFLAGS := -Wall -Wextra -Werror
targets := libuthread.a queue.o uthread.o sem.o preempt.o context.o

all: $(lib)

libuthread.a: $(targets)
	ar rcs libuthread.a $(targets)

queue.o: queue.c queue.h
	$(CC) $(CFLAGS) -c -o $@ $<

context.o: context.c private.h
	$(CC) $(CFLAGS) -c -o $@ $<

uthread.o: uthread.c uthread.h context.o
	$(CC) $(CFLAGS) -c -o $@ $<

sem.o: sem.c sem.h queue.h context.o 
	$(CC) $(CFLAGS) -c -o $@ $<
preempt.o: preempt.c private.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(targets)