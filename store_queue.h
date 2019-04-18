#ifndef S_queue_H
#define S_queue_H

#include  <stdint.h>
typedef struct S_queue {
    uint32_t queue[8];
    uint32_t size;
    uint8_t first;
    uint8_t last;
    uint8_t max_size;
}s_queue;

void init_store_queue(s_queue *queue);
void enqueue(s_queue *queue, uint32_t value);
uint32_t dequeue(s_queue *queue);

#endif
