#ifndef ENGINE_CORE_HPP
#define ENGINE_CORE_HPP

/// \brief Indicates whether the debugging is enabled.
///
/// The \c DEBUG_ON flag is used to indicate whether the debugging
/// is enabled. When set to 1, it means that debugging is enabled,
/// and when sett to 0, debugging is disabled.
#define DEBUG_ON 1

/// \brief This macro is used to mark places (such as variables, functions,
///        etc.) to indicate that they are temporary and will be removed later.
///
/// The temporary marker macro serves as a visual indicator to developers that
/// the marked code should not be considered as part of the final implementation
/// and should be removed before release.
#define ENGINE_TEMPORARY

/// \brief This macro indicates if the `inline` keyword should be forced when
///        `ENGINE_INLINE` is being used.
///
/// The `ENGINE_USE_INLINE` macro allows developers to control whether the
/// `inline` keyword should be explicitly enforced in the code where
/// `ENGINE_INLINE` is used. When `ENGINE_USE_INLINE` is defined, the `inline`
/// keyword will be added to the `ENGINE_INLINE` macro. If `ENGINE_USE_INLINE`
/// is not defined, the `ENGINE_INLINE` macro will be empty.
///
/// Developers can customize the behavior of `ENGINE_USE_INLINE` based on the
/// compiler being used. For example, if the compiler is GCC, the
/// `always_inline` attribute is added to the `inline` keyword using the
/// `__attribute__((always_inline))` syntax.
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

/// \brief This macro is used to provide hints to the compiler about the
///        expected branch likelihood.
///
/// The `LIKELY` and `UNLIKELY` macros are used to indicate the expected branch
/// likelihood to the compiler. These macros are typically used in conditional
/// statements to provide hints for compiler optimizations.
///
/// When using the GNU C Compiler (GCC), the `LIKELY` macro is defined with the
/// `__builtin_expect` extension, which helps optimize instruction pipelining in
/// branch statements. The `LIKELY(EXPR)` macro indicates that the expression
/// `EXPR` is likely to evaluate to true.
///
/// When the compiler is not GCC, the `LIKELY` macro is defined as the
/// expression itself, without any optimization hints.
///
/// \param EXPR The expression to which the branch likelihood hint is applied.
/// \return The result of the expression.
#ifdef __GNUC__
#    define LIKELY(EXPR)   (__builtin_expect((EXPR), 1))
#    define UNLIKELY(EXPR) (__builtin_expect((EXPR), 0))
#else
#    define LIKELY(EXPR)   (EXPR)
#    define UNLIKELY(EXPR) (EXPR)
#endif // __GNUC__

/// \brief This macro is used to print debug messages when debugging is enabled.
///
/// The `DEBUG_ON` macro is used to indicate whether debugging is enabled or
/// disabled. When debugging is enabled, the `DEBUG` macro will print a
/// formatted message to the standard output using `printf` function. The
/// `DEBUG_BLOCK` macro is used to enclose a block of code that will only be
/// executed when debugging is enabled.
///
/// When `DEBUG_ON` is set to 1, the `DEBUG` macro will be active and print the
/// debug message. The `MESSAGE` parameter is the format string for the message,
/// and additional arguments can be provided using variadic macro arguments
/// `__VA_ARGS__`. The `DEBUG_BLOCK` macro is used to include a block of code
/// that will only be executed when debugging is enabled.
///
/// When `DEBUG_ON` is set to 0, the `DEBUG` macro and `DEBUG_BLOCK` macro will
/// be empty, effectively disabling the debug output.
///
/// \param MESSAGE The format string for the debug message.
/// \param ... Additional arguments for the format string.
#if DEBUG_ON == 1
#    define DEBUG(MESSAGE, ...)     printf(MESSAGE, __VA_ARGS__)
#    define DEBUG_BLOCK(CODE_BLOCK) CODE_BLOCK
#else
#    define DEBUG(MESSAGE, ...)
#    define DEBUG_BLOCK(CODE_BLOCK)
#endif // DEBUG_ON

/// \brief Prints a formatted message to the standard output and aborts the
///        program immediately.
///
/// This function is used to print a formatted message to the standard output
/// using a format string and additional arguments. After printing the message,
/// the program is aborted immediately.
///
/// It is important to note that when the program is aborted, any registered
/// functions using `atexit()` will not be called. Therefore, any cleanup or
/// finalization routines registered with `atexit()` will not be executed.
///
/// \param fmt The format string for the message.
/// \param ... Additional arguments for the format string.
void die(const char *fmt, ...);

#endif // ENGINE_CORE_HPP
