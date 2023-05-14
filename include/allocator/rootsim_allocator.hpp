#pragma once

#include <core/core.hpp>
#include <engine.hpp>

/**
 *  @brief    ROOT-Sim's C++ Memory Allocator.
 *
 *            The ROOT-Sim's C++ memory allocator is used to replace the
 *            standard C++'s `__new_allocator` by the `rs`-alloc functions
 *            provided by ROOT-Sim.
 *
 *  @tparam T The element type to be allocated.
 */
template <typename T = std::nullptr_t>
class ROOTSimAllocator
{
public:
    using value_type      = T;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = T *;
    using const_pointer   = const T *;
    using reference       = T &;
    using const_reference = const T &;

    /**
     *  @brief Default ctor.
     */
    ENGINE_INLINE ROOTSimAllocator() noexcept
    {}

    /**
     *  @brief       Default copy ctor.
     *  @param alloc A reference to an allocator.
     */
    ENGINE_INLINE
    ROOTSimAllocator(const ROOTSimAllocator &alloc) noexcept
    {}

    /**
     *  @brief       Default templated copy ctor.
     *  @param alloc A reference to an allocator.
     */
    template <typename U>
    ENGINE_INLINE ROOTSimAllocator(const ROOTSimAllocator<U> &alloc) noexcept
    {}

    /**
     *  @brief   Allocate memory.
     *  @param n The number of objects to allocate space for.
     *  @return  A pointer to the allocated space.
     */
    ENGINE_INLINE
    pointer allocate(size_type n)
    {
        return static_cast<pointer>(rs_malloc(sizeof(value_type) * n));
    }

    /**
     *  @brief   Deallocate memory.
     *  @param p Pointer to the memory to deallocate.
     *  @param n The number of objects space was allocated for.
     */
    ENGINE_INLINE
    void deallocate(pointer p, size_type n)
    {
        rs_free(p);
    }

    /**
     *  @brief    Allocate memory.
     *  @param  n The number of objects space was allocated for.
     *  @tparam U The element type to be allocated.
     *  @return   A pointer to the allocated space.
     */
    template <typename U>
    ENGINE_INLINE static U *allocate(std::size_t n)
    {
        return static_cast<U *>(rs_malloc(sizeof(U) * n));
    }

    /**
     *  @brief      Deallocate memory.
     *  @param  ptr Pointer to the memory to deallocate.
     *  @tparam U   The element type to be allocated.
     */
    template <typename U>
    ENGINE_INLINE static void deallocate(U *ptr)
    {
        rs_free(ptr);
    }

    /**
     *  @brief       Allocate memory and construct in-place.
     *  @param args  constructor's arguments
     *  @tparam U    The element type to be allocated.
     *  @tparam Args The argument types.
     *  @return      A pointer to the allocated space.
     */
    template <typename U, typename... Args>
    ENGINE_INLINE
    static U *construct(Args &&...args)
    {
        return new (rs_malloc(sizeof(U))) U(std::forward<Args>(args)...);
    }
};