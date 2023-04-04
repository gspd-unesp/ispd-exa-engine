#ifndef ENGINE_ARRAY_H
#define ENGINE_ARRAY_H

#include <core/core.h>
#include <stdint.h>

/*
 * A simple templated-array implementation.
 *
 *   * The array is said templated because the data
 *     type of the element is the element's type itself
 *     instead of a void pointer (void *).
 *
 *   * The queue is not unbounded, although it can contain
 *     a huge number of elements, at least 2^32 elements.
 *
 * The implemented array time-complexities are:
 *
 *   1. array:        constant-time, that is, O(1).
 *   2. array_init:   constant-time, that is, O(1).
 *   3. array_insert: amortized constant-time, that is, amortized O(1).
 *   4. array_get:    constant-time, that is, O(1).
 *   5. array_expand: linear-time, that is, O(n).
 *
 */

/**
 * It represents the initial number of elements an
 * array can have before its first expansion.
 */
#define DEFAULT_ARRAY_SIZE (8U)

/**
 * @brief It defines an array with the specified type.
 *
 *        Further, note that the macro `array_init`
 *        must be called before perform any operations
 *        in the array.
 *
 *        Otherwise, unexpected behavior may occur, since
 *        the array is not correctly initialized.
 *
 * @param type the type of elements in the array
 */
#define array(type)                 \
    struct {                        \
        type          **__elems;    \
        uint_fast32_t   __capacity; \
        uint_fast32_t   __size;     \
    }

/**
 * @brief It initializes thee array internal structure.
 *
 * @param self the array to be initialized
 */
#define array_init(self)                                                            \
    ({                                                                              \
        self.__elems    = malloc(sizeof(__array_type(self)) * DEFAULT_ARRAY_SIZE);  \
        self.__capacity = DEFAULT_ARRAY_SIZE;                                       \
        self.__size     = 0;                                                        \
    })

/**
 * @brief It inserts an element at the end of the array.
 *
 * @param self the array to have the element added at its end
 * @param elem the element to be added at the end of the array
 */
#define array_insert(self, elem)          \
    ({                                    \
        array_expand(self);               \
        self.__elems[self.__size] = elem; \
        self.__size++;                    \
    })

/**
 * @brief It gets the element at the i-th index in the array.
 *
 *       Further, note that if the index overflows the array
 *       bounds, then the program will immediately aborted.
 *
 * @param self the array to be queried
 * @param i    the element index
 */
#define array_get(self, i)                          \
    ({                                              \
        if (UNLIKELY(i >= self.__capacity))         \
            die("array_get: Index out of bounds."); \
        self.__elems[i];                            \
    })

/**
 * @brief It expands the internal array capacity if it is needed.
 *
 *        If the array does not satisfies the condition to be
 *        expanded, then nothing is done.
 *
 * @param self the array to be expanded
 */
#define array_expand(self)                                                                           \
    ({                                                                                               \
        if (UNLIKELY(self.__size == self.__capacity)) {                                              \
            self.__capacity <<= 1U; /* Double it! */                                                 \
            self.__elems      = realloc(self.__elems, sizeof(__array_type(self)) * self.__capacity); \
        }                                                                                            \
    })

/**
 * @brief It returns the element type being stored in the array.
 *
 * @param self the array
 */
#define __array_type(self) (__typeof__(*self.__elems))

#endif // ENGINE_ARRAY_H
