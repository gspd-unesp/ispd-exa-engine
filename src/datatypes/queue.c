#include <core/core.h>
#include <datatypes/queue.h>
#include <engine.h>
#include <string.h>

ENGINE_INLINE struct queue *queue_new(size_t elemsize)
{
    struct queue *q = malloc(sizeof(struct queue));

    // It checks if the queue could not be allocated, then
    // the program will be immediately aborted
    if (UNLIKELY(q == NULL))
        die("datatypes: queue with element size %zu could not be allocated.\n", elemsize);

    q->head = NULL;
    q->tail = NULL;
    q->size = 0U;
    q->elemsize = elemsize;

    return q;
}

ENGINE_INLINE void queue_enq(struct queue *q, void *elem)
{
    struct queue_node *node = malloc(sizeof(struct queue_node));

    node->elem = malloc(q->elemsize);
    node->next = NULL;

    // It checks if the element memory region could not be allocated,
    // then the program will be immediately aborted
    if (UNLIKELY(node->elem == NULL))
        die("datatypes: element space in the queue's node could not be allocated.\n");

    memcpy(node->elem, elem, q->elemsize);

    if (!q->tail)
        q->head = node;
    else
        q->tail->next = node;
    q->tail = node;

    q->size++;
}

ENGINE_INLINE void queue_deq(struct queue *q, void *placeholder)
{
    if (!q->head)
        return;

    struct queue_node *tmp = q->head;

    memcpy(placeholder, q->head->elem, q->elemsize);

    if (!q->head->next)
        q->tail = NULL;
    q->head = q->head->next;

    free(tmp);
    q->size--;
}

ENGINE_INLINE int queue_empty(struct queue *q)
{
    return q->size == 0;
}
