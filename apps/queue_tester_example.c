#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

struct testStruct
{
    int intTest;
    char* stringTest;
};

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

void test_queue_refil(void)
{
    int data1 = 3;
    int data2 = 5;
    int *ptr;
    queue_t q;

    fprintf(stderr, "*** TEST queue_refil ***\n");

    q = queue_create();
    queue_enqueue(q, &data1);
    queue_dequeue(q, (void**)&ptr);
    queue_enqueue(q, &data2);
    queue_dequeue(q, (void**)&ptr);

    TEST_ASSERT(ptr == &data2);
    TEST_ASSERT(*ptr == data2);
}

void test_queue_complex(void)
{
    
    struct testStruct data;
    data.intTest = 5;
    data.stringTest = malloc(sizeof(char) * 10);
    strcpy(data.stringTest, "hello");
    struct testStruct* ptr;
    queue_t q;

    fprintf(stderr, "*** TEST queue_complex ***\n");

    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);

    fprintf(stderr, "stringTest: |%s|\n", ptr -> stringTest);

    TEST_ASSERT(ptr == &data);
    TEST_ASSERT(ptr -> intTest == 5);
    TEST_ASSERT(strcmp(ptr -> stringTest, "hello"));

}

int main(void)
{
	test_create();
	test_queue_simple();
    test_queue_refil();
    test_queue_complex();

	return 0;
}
