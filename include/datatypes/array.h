#ifndef DATATYPES_ARRAY_H
#define DATATYPES_ARRAY_H

#include <stdlib.h>
#include <stdint.h>

#define INIT_ARRAY_SIZE (8U)
#define ARRAY_NEW_CAPACITY(old_cap) ((old_cap) << 1)

#define array(type)               \
    struct {                      \
        type *__elems;            \
        uint_fast32_t __capacity; \
        uint_fast32_t __size;     \
    }

#define array_init(self) \
    __extension__({      \
        array_capacity(self) = INIT_ARRAY_SIZE; \
        array_elems(self)    = malloc(array_capacity(self) * sizeof(*array_elems(self))); \
        array_size(self)     = 0U; \
    })

#define array_insert(self, elem)                    \
    __extension__({                                 \
        array_expand(self);                         \
        array_elems(self)[array_size(self)] = elem; \
        array_size(self)++;                         \
    })

#define array_pop(self)                      \
    __extension__({                          \
        array_size(self)--;                  \
        array_elems(self)[array_size(self)]; \
    })

#define array_get(self, i) (array_elems(self)[i])

#define array_empty(self) (array_size(self) == 0U)

#define array_expand(self)                                                   \
    __extension__({                                                          \
        if (UNLIKELY(array_size(self) == array_capacity(self))) {            \
            array_capacity(self) = ARRAY_NEW_CAPACITY(array_capacity(self)); \
            array_elems(self)    = realloc(array_elems(self),                \
                array_capacity(self) * sizeof(*array_elems(self)));          \
        }                                                                    \
    })

#define array_free(self)           \
    __extension__({                \
        free(array_elems(self));   \
        array_capacity(self) = 0U; \
        array_size(self)     = 0U; \
    })

#define array_elems(self) ((self).__elems)
#define array_capacity(self) ((self).__capacity)
#define array_size(self) ((self).__size)

#endif // DATATYPES_ARRAY_H
