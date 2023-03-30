#ifndef ENGINE_MM_H
#define ENGINE_MM_H

#include <stddef.h>

/**
 * @brief It allocates `size` bytes of memory and returns
 *        a pointer to the allocated memory. However, if
 *        there is not enough memory to be allocated, then
 *        the program is aborted.
 *
 *        Further, if the user tries to allocate a region
 *        with {@code 0} bytes, the program will also be
 *        aborted.
 *
 *        Note that this memory allocation function works
 *        similarly as xmalloc().
 *
 * @param size amount of bytes to be allocated
 *
 * @return a pointer to the memory allocated
 */
void *mm_malloc(size_t size);

/**
 * @brief It allocates a allocated memory space containing
 *        `nmemb` elements of `size` bytes each and returns
 *        a pointer to the allocated memory. However, if
 *        there is not enough memory to be allocated, then
 *        then program is aborted.
 *
 *        Further, if the user tries to allocate a region
 *        with {@code 0} bytes, the program will also be
 *        aborted. Note that the region will have {@code 0}
 *        bytes, since `nmemb` or `size` is equals to 0.
 *
 *        In addition, note that the actual size of the
 *        allocated memory region is `nmemb * size` bytes.
 *
 * @param nmemb the number of elements of `size` bytes each
 * @param size  the size of each element
 *
 * @return a pointer to the memory allocated
 */
void *mm_calloc(size_t nmemb, size_t size);

/**
 * @brief It changes the size of the memory block pointed
 *        to by `ptr` to `size` bytes. The contents will
 *        be unchanged in the range from the start of the
 *        region up to the minimum of the old and new sizes.
 *        If the new size is larger than the old size, the
 *        added memory is not initialized. If `ptr` is NULL,
 *        the call is equivalent to `mm_malloc(size)`. However,
 *        if there is not enough memory to be allocated, then
 *        the program is aborted.
 *
 *        Further, if the user tries to allocate a region with
 *        {@code 0} bytes, the program will also be aborted.
 *
 * @param ptr  the memory block to be reallocated
 * @param size the new size of the memory block
 *
 * @return a pointer to the memory alllocated
 */
void *mm_realloc(void *ptr, size_t size);

/**
 * @brief It frees the memory space pointed to by `ptr` which
 *        must have been returned by a previous call to `malloc()`,
 *        `calloc()` or `realloc()`. Further, if the `ptr` is NULL,
 *        then the program is aborted.
 *
 * @param ptr the pointer to the memory space to be freed
 */
void mm_free(void *ptr);

#endif // ENGINE_MM_H
