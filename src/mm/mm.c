#include <stdlib.h>
#include <core/core.h>
#include <mm/mm.h>

ENGINE_INLINE void *mm_malloc(size_t size) {
    if (UNLIKELY(!size))
        die("zero-sized mm_alloc");
    void *ptr = malloc(size);
    if (UNLIKELY(!ptr))
        die("out of memory allocating %zu bytes", size);
    return ptr;
}

ENGINE_INLINE void *mm_calloc(size_t nmemb, size_t size) {
    if (UNLIKELY(!nmemb || !size))
        die("zero-sized mm_calloc");
    void *ptr = calloc(nmemb, size);
    if (UNLIKELY(!ptr))
        die("out of memory allocating %zu bytes", nmemb * size);
    return ptr;
}

ENGINE_INLINE void *mm_realloc(void *ptr, size_t size) {
    if (UNLIKELY(!ptr))
        return mm_malloc(size);
    ptr = realloc(ptr, size);
    if (UNLIKELY(!ptr))
        die("out of memory re-allocating %zu bytes", size);
    return ptr;
}

ENGINE_INLINE void mm_free(void *ptr) {
    if (UNLIKELY(!ptr))
        die("NULL pointer passed to mm_free");
    free(ptr);
}