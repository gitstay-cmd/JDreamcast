#include "store_queue.h"
#include <stdio.h>

void init_store_queue(s_queue *queue){
    queue->first = queue->last = 0;
    queue->size = 0;
    queue->max_size = 8;
}

void enqueue(s_queue *queue, uint32_t value){
    if(queue->size == queue->max_size){
        printf("Queue full\n");
    }
    queue->size += 1;
    queue->queue[queue->last++] = value;
    queue->last %= queue->max_size;
}

uint32_t dequeue(s_queue *queue){
    if(queue->size == 0){
        printf("Queue empty\n");
    }
    queue->size -= 1;
    uint32_t value = queue->queue[queue->first++];
    queue->first %= queue->max_size;
    return value;
} 
