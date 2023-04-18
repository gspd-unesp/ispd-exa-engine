#ifndef ENGINE_QUEUE_H
#define ENGINE_QUEUE_H

#include <stddef.h>

struct queue_node
{
    void              *elem;
    struct queue_node *next;
};

struct queue
{
    struct queue_node *head;
    struct queue_node *tail;
    size_t             size;
    size_t             elemsize;
};

struct queue *queue_new(size_t elemsize);

void queue_enq(struct queue *q, void *elem);

void queue_deq(struct queue *q, void *placeholder);

int queue_empty(struct queue *q);

#endif // ENGINE_QUEUE_H
