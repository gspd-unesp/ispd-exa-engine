#ifndef ENGINE_CORE_H
#define ENGINE_CORE_H

#define DEBUG_ON 0
#define ENGINE_TEMPORARY

#define ENGINE_USE_INLINE
#ifdef ENGINE_USE_INLINE
#    ifdef __GNUC__
#        define ENGINE_INLINE inline __attribute__((always_inline))
#    else
#        define ENGINE_INLINE inline
#    endif // __GNUC__
#else
#    define ENGINE_INLINE
#endif // ENGINE_USE_INLINE

#ifdef __GNUC__
#    define LIKELY(EXPR)   (__builtin_expect((EXPR), 1))
#    define UNLIKELY(EXPR) (__builtin_expect((EXPR), 0))
#else
#    define LIKELY(EXPR)   (EXPR)
#    define UNLIKELY(EXPR) (EXPR)
#endif // __GNUC__

#if DEBUG_ON == 1
#    define DEBUG(MESSAGE, ...) printf(MESSAGE, __VA_ARGS__)
#else
#    define DEBUG(MESSAGE, ...)
#endif // DEBUG_ON

/**
 * @brief It sends a formatted message to the standard
 *        error and aborts the program.
 *
 *        Note that since the program is aborted and not
 *        exited, the registered functions using atexit()
 *        will not be called.
 *
 * @param fmt the message
 */
void die(const char *fmt, ...);

#endif // ENGINE_CORE_H
