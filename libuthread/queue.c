#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct Node{
    void *data;
    struct Node *next;
};

struct queue{
    struct Node *front;
    struct Node *back;
    int size;
};

queue_t queue_create(void){
    queue_t q = (queue_t)malloc(sizeof(struct queue));
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
    return q;
}

int queue_destroy(queue_t queue){
    if(queue == NULL || queue_length(queue) == 0){
        return -1;
    }
    else{
        free(queue);
        return 0;
    }
}

int queue_enqueue(queue_t queue, void *_data){
    if (queue == NULL || _data == NULL){
        return -1;
    }
    struct Node *n = (struct Node *) malloc(sizeof(struct Node));
    if(n == NULL){
        return -1;
    }//malloc fails
    n -> data = malloc(sizeof(void *));
    n->data = _data;
    n->next = NULL;
    if(queue->back != NULL){
        queue->back->next = n;//add to the end of queue
    }
    else{
        queue->front = n;//adding as first element
    }
    queue->back = n;//setting new element as last element
    queue->size++;
    return 0;
}

int queue_dequeue(queue_t queue, void **data){
    if (queue == NULL || data == NULL){
        return -1;
    }
    if(queue_length(queue) == 0){
        return -1;
    }
    struct Node *n = queue->front;
    *data = n->data;//save data to pointer
    queue->front = queue->front->next;//remove links to front
    if(queue->front == NULL){//case where only one element exists
        queue->back = NULL;
    }
    queue->size--;
    return 0;
}

int queue_delete(queue_t queue, void *data){
    struct Node *n = queue->front;
    struct Node *p = NULL;
    while(n->next != NULL){
        if(n->data == data){
            
            return 0;
        }
        p = n;
        n = n->next;
    }
    if(p != NULL){
        p->next = n->next;
    }
    else{//case when first element is match
        queue->front = queue->front->next;
    }
    if(n->next == NULL){//if last matches
        queue->back = p;
    }
    queue->size--;
    return -1;
}
int queue_iterate(queue_t queue, queue_func_t func){
    if (queue == NULL || func == NULL){
        return -1;
    }
    struct Node *n = queue->front;

    while(n != NULL){
        func(queue,n->data);
        n = n->next;
    }
    return 0;
}
int queue_length(queue_t queue){
    if(queue == NULL){
        return -1;
    }
    return queue->size;
}