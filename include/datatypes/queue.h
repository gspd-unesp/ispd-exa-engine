#ifndef ENGINE_QUEUE_H
#define ENGINE_QUEUE_H

#include <stddef.h>

/*
 * A simple unbounded templated-queue implementation.
 *
 *   * The underlying queue implementation is using
 *     a singly-linked list; using head and tail pointers.
 *
 *   * The queue is said templated-queue because the
 *     data type of the element is the element's type itself
 *     instead of a void pointer (void *).
 *
 *   * The queue is said unbounded because there is no bound
 *     for the queue's size, except the physical memory.
 *
 * The implemented queue invariants are:
 *
 *   1. The HEAD is NULL, if and only if, the TAIL is NULL.
 *
 *   2. The queue is empty, if and only if, the HEAD and
 *      TAIL is NULL.
 *
 *   3. If the queue has the configuration (e_1, e_2, ..., e_n),
 *      then the enqueue of element e_(n+1) modifies the queue to
 *      the following configuration
 *
 *          (e_1, e_2, ..., e_n, e_(n+1))
 *
 *      that is, the element e_(n+1) is added at the queue's tail.
 *
 *   4. If the queue has the following configuration (e_1, e_2, ...., e_n)
 *      then the dequeue operation modifies the queue to the
 *      following configuration
 *
 *          (e_2, e_3, ..., e_n)
 *
 *      that is, the element e_1 is removed from the queue.
 *
 * The implemented queue time-complexities are:
 *
 *   1. All implemented queue operations are constant-time,
 *      that is, has the O(1) time-complexity.
 *
 */

/**
 * @brief It defines a queue with the specified type.
 *
 *        Further, note that the macro `queue_init`
 *        must be used before perform any operations
 *        in the queue.
 *
 * @param type the type of elements in the queue
 */
#define queue(type)         \
    struct {                \
        struct {            \
            type *__elem;   \
            void *__next;   \
        } *__head, *__tail; \
        size_t __size;      \
    }

/**
 * @brief It initializes the queue internal structure.
 *
 * @param self the queue to be initialized
 */
#define queue_init(self)    \
    ({                      \
        self.__head = NULL; \
        self.__tail = NULL; \
        self.__size = 0;    \
    })

/**
 * @brief It enqueues an element.
 *
 * @param self the queue to have the element inserted
 * @param elem the element to be inserted
 */
#define queue_insert(self, elem)                                       \
    ({                                                                 \
        __typeof__(self.__head) __node = malloc(sizeof(*self.__head)); \
        __node->__elem = elem;                                         \
        __node->__next = NULL;                                         \
        if (!self.__head)                                              \
            self.__head = __node;                                      \
        if (self.__tail)                                               \
            self.__tail->__next = __node;                              \
        self.__tail = __node;                                          \
        self.__size++;                                                 \
    })

/**
 * @brief It dequeues an element.
 *
 * @param self the queue to have an element dequeued
 *
 * @return the front element in the queue
 */
#define queue_remove(self)                      \
    ({                                          \
        __typeof__(self.__head->__elem) __elem; \
        if (!self.__head)                       \
            __elem = NULL;                      \
        else {                                  \
            __elem = self.__head->__elem;       \
            if (!self.__head->__next)           \
                self.__tail = NULL;             \
            self.__head = self.__head->__next;  \
            self.__size--;                      \
        }                                       \
        __elem;                                 \
    })

/**
 * @brief It returns the queue's front element.
 *        If the queue is empty, then NULL is
 *        returned.
 *
 * @return the queue's front element. If the queue
 *         is empty, then NULL is returned
 */
#define queue_front(self) (!self.__head ? NULL : self.__head->__elem)

/**
 * @brief It returns {@code 1} if the queue is empty.
 *        Otherwise, {@code 0} is returned.
 *
 * @return {@code 1} if queue is empty; otherwise,
 *         {@code 0} is returned
 */
#define queue_empty(self) (self.__head == NULL)

/**
 * @brief It returns the queue's size.
 *
 * @return the queue's size
 */
#define queue_size(self) (self.__size)

#endif // ENGINE_QUEUE_H
