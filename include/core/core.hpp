#ifndef ENGINE_CORE_HPP
#define ENGINE_CORE_HPP

/**
 * @brief It indicates whether the debugging is enabled.
 */
#define DEBUG_ON 1

/**
 * @brief This macro is used to mark places (as variables, functions etc.)
 *        to indicates that it is temporary and will be removed later.
 */
#define ENGINE_TEMPORARY

/**
 * @brief It indicates if the inline must be forced where `ENGINE_INLINE`
 *        is being used.
 */
#define ENGINE_USE_INLINE
#ifdef ENGINE_USE_INLINE
#   ifdef __GNUC__
#       define ENGINE_INLINE inline __attribute__((always_inline))
#   else
#       define ENGINE_INLINE inline
#   endif // __GNUC__
#else
#   define ENGINE_INLINE
#endif // ENGINE_USE_INLINE

/**
 * @brief It uses an extension of the GNU C Compiler (GCC) to optimize the
 *        instruction pipelining in branch statements.
 */
#ifdef __GNUC__
#   define LIKELY(EXPR) (__builtin_expect((EXPR), 1))
#   define UNLIKELY(EXPR) (__builtin_expect((EXPR), 0))
#else
#   define LIKELY(EXPR) (EXPR)
#   define UNLIKELY(EXPR) (EXPR)
#endif // __GNUC__

/**
 * @brief If the debug is enabled then the `DEBUG` macro will effectively
 *         print a formatted message to the standard output.
 */
#if DEBUG_ON == 1
#   define DEBUG(MESSAGE, ...) printf(MESSAGE, __VA_ARGS__)
#else
#   define DEBUG(MESSAGE, ...)
#endif // DEBUG_ON

/**
 * @brief It prints a formatted message to the standard output and aborts
 *        the program immediately.
 *
 * @details
 *        Note that, since the program is aborted and not exited, then all
 *        of the registered functions using `atexit()` will not be called.
 *
 * @param fmt the formatted message
 */
void die(const char *fmt, ...);

#endif // ENGINE_CORE_HPP
