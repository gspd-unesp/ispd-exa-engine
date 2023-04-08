#ifndef DATATYPES_QUEUE_H
#define DATATYPES_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdlib.h>
#include <string.h>

#define TYPE(x) __typeof__(x)

/*
 * A simple unbounded templated-queue implementation.
 *
 *   * The underlying queue implementation is relying on
 *     a singly-linked list implementation, using head and
 *     tail pointers.
 *
 *   * The queue is said templated because the data type of
 *     the element being held by the node is the element's type
 *     itself instead of a void pointer (void *).
 *
 *   * The queue is said unbounded because there is no bound for
 *     the queue's size, except the physical memory.
 *
 *   * A queue is a FIFO (First-In First-Out) data structure, and
 *     this implementation does not impose any priority order among
 *     the queue's elements.
 *
 *   * For the time being, the current queue's implementation is unsafe,
 *     in the manner that it is the caller's responsibility to ensure
 *     that some pre-conditions is satisfied before a `macro` be called.
 *
 * The implemented queue invariants are:
 *
 *   1. The HEAD is NULL, if and only if, the TAIL is NULL.
 *
 *   2. The queue is empty, if and only if, the HEAD and TAIL
 *      is NULL.
 *
 *   3. If the queue has the configuration (e_1, e_2, ..., e_n),
 *      then the enqueue of an element e_(n+1) modifies the queue to
 *      the following configuration:
 *
 *          (e_1, e_2, ..., e_n, e_(n+1))
 *
 *      that is, the element e_(n+1) is added at the queue's tail.
 *
 *    4. If the queue has the configuration (e_1, e_2, ..., e_n),
 *       then the dequeue operation modifies the queue to the
 *       following configuration:
 *
 *          (e_2, e_3, ..., e_n)
 *
 *       that is, the element e_1 is removed from the queue.
 *
 * The implemented queue time-complexities are:
 *
 *   1. All implemented queue operations are constant-time,
 *      that is, has the O(1) time-complexity.
 *
 *      1.1. Expect the `queue_foreach`. This macro has
 *           linear time-complexity, that is, O(n).
 *
 */

/**
 * @brief It defines a queue of the given type.
 *
 *        Further, note that the macro `queue_init`
 *        must be used before any operation be performed.
 *        Otherwise, unexpected behavior may occur.
 *
 *        It is the caller's responsibility to ensure that
 *        the specified type has previously been defined.
 *        Otherwise, a compile-time error may occur.
 *
 * @param type the type of elements in the queue
 */
#define queue(type)         \
    struct {                \
        struct {            \
            type  __elem;   \
            void *__next;   \
        } *__head, *__tail; \
        size_t __size;      \
    }

/**
 * @brief It initializes the queue's internal structure.
 *
 *        This macro must be called before any operation
 *        be performed using the queue. Otherwise, unexpected
 *        behavior may occur.
 *
 *        It is the caller's responsibility to ensure that `self`
 *        is a previously initialized queue. Otherwise, unexpected
 *        behavior may occur.
 *
 * @param self the queue to be initialized
 */
#define queue_init(self)    \
    __extension__({         \
        self.__head = NULL; \
        self.__tail = NULL; \
        self.__size = 0U  ; \
    })

/**
 * @brief It enqueues an element into the queue.
 *
 *        It is the caller's responsibility to ensure that `self`
 *        parameter is a previously initialized queue. Otherwise,
 *        unexpected behavior may occur.
 *
 * @param self the queue to have the element inserted
 * @param elem the element to be inserted
 */
#define queue_insert(self, elem)             \
    __extension__({                          \
        TYPE(self.__head) __node             \
            = malloc(sizeof(*self.__head));  \
        __node->__elem = elem;               \
        __node->__next = NULL;               \
        if (!self.__head)                    \
            self.__head = __node;            \
        if (self.__tail)                     \
            self.__tail->__next = __node;    \
        self.__tail = __node;                \
        self.__size++;                       \
    })

/**
 * @brief It dequeues an element from the queue.
 *
 *        It is the caller's responsibility to ensure that `self`
 *        is a previously initialized queue. Otherwise, unexpected
 *        behavior may occur.
 *
 *        Further, it is also the caller's responsibility to call
 *        this macro, if and only if, the queue is not empty. Otherwise,
 *        unexpected behavior may occur.
 *
 * @param self the queue to have an element dequeued
 *
 * @return the dequeued element, that was the front
 *         element in the queue
 */
#define queue_remove(self, ptr)                                         \
    __extension__({                                                     \
        memcpy(ptr, &self.__head->__elem, sizeof(self.__head->__elem)); \
        if (!self.__head->__next)                                       \
            self.__tail = NULL;                                         \
        void *__temp = self.__head;                                     \
        self.__head = self.__head->__next;                              \
        self.__size--;                                                  \
    })

/**
 * @brief It traverses the queue applying the specified function `f`
 *        to each element in the queue.
 *
 *        It is the caller's responsibility to ensure that the `self`
 *        argument is a previously initialized queue. Otherwise,
 *        unexpected behavior may occur.
 *
 * @param self the queue to be traversed
 * @param f    the function to be applied in each element
 */
#define queue_foreach(self, f)                        \
    __extension__({                                   \
        TYPE(self.__head) __curr;                     \
        for (__curr = self.__head;                    \
             __curr != NULL;                          \
             __curr = __curr->__next)                 \
            f(__curr->__elem);                        \
    })

/**
 * @brief It returns the queue's front element without remove it.
 *
 *        It is the caller's responsibility to ensure that the
 *        `self` argument is a previously initialized queue.
 *        Otherwise, unexpected behavior may occur.
 *
 * @param self the queue to have its front element polled
 *
 * @return the queue's front element
 */
#define queue_front(self) (self.__head->__elem)

/**
 * @brief It returns 0 if the queue is empty. Otherwise,
 *        1 is returned.
 *
 *        It is the caller's responsibility to ensure that the
 *        `self` argument is a previously initialized queue.
 *        Otherwise, unexpected behavior may occur.
 *
 * @param self the queue to be checked
 *
 * @return 0 if the queue is empty. Otherwise, 1 is returned.
 */
#define queue_empty(self) (self.__head == NULL)

/**
 * @brief It returns the queue's size.
 *
 *        It is the caller's responsibility to ensure that the
 *        `self` argument is a previously initialized queue.
 *        Otherwise, unexpected behavior may occur.
 *
 * @param self the queue to have its size polled
 *
 * @return the queue's size
 */
#define queue_size(self) (self.__size)

/**
 * @brief It frees the memory allocated by the queue.
 *
 *        It is the caller's responsibility to ensure that the
 *        `self` argument is a previously initialized queue.
 *        Otherwise, unexpected behavior may occur.
 *
 * @param self the queue to be freed
 */
#define queue_free(self)                        \
    __extension__({                             \
        TYPE(self.__head) __curr = self.__head; \
        TYPE(self.__head) __temp = NULL;        \
        while (__curr) {                        \
            __temp = __curr;                    \
            __curr = __curr->__next;            \
            free(__temp);                       \
        }                                       \
    })

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DATATYPES_QUEUE_H
