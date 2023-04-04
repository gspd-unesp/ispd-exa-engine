#ifndef ENGINE_TEST_H
#define ENGINE_TEST_H

#include <stdio.h>

#ifdef __GNUC__
#   define LIKELY(EXPR)   (__builtin_expect((EXPR), 1))
#   define UNLIKELY(EXPR) (__builtin_expect((EXPR), 0))
#else
#   define LIKELY(EXPR)   (EXPR)
#   define UNLIKELY(EXPR) (EXPR)
#endif // __GNUC__

#define TEST_UNIT( CODE ) \
    int main( int __argc, char **__argv ) \
    CODE

#define TEST_CASE( DESCRIPTION, ASSERTS )                                                         \
    ({                                                                                            \
        unsigned __passed_cases = 0;                                                              \
        unsigned __error_cases  = 0;                                                              \
        ASSERTS                                                                                   \
        if ( __error_cases ) {                                                                    \
            fprintf( stderr, "[FAILED]: %s.\n", DESCRIPTION );                                    \
        } else {                                                                                  \
            fprintf( stdout, "[SUCCESS]: %s (%u cases passed).\n", DESCRIPTION, __passed_cases ); \
        }                                                                                         \
    })

#define ASSERT_NULL( VALUE, ... )           \
    ({                                      \
        if ( UNLIKELY( (VALUE) != NULL ) ) {  \
            __error_cases++;                \
            fprintf( stderr, __VA_ARGS__ ); \
        } else {                            \
            __passed_cases++;               \
        }                                   \
    })

#define ASSERT_NOT_NULL( VALUE, ... )       \
    ({                                      \
        if ( UNLIKELY( (VALUE) == NULL ) ) {  \
            __error_cases++;                \
            fprintf( stderr, __VA_ARGS__ ); \
        } else {                            \
            __passed_cases++;               \
        }                                   \
    })

#define ASSERT_TRUE( EXPRESSION, ... )      \
    ({                                      \
        if ( UNLIKELY( !(EXPRESSION) ) ) {    \
            __error_cases++;                \
            fprintf( stderr, __VA_ARGS__ ); \
        } else {                            \
            __passed_cases++;               \
        }                                   \
    })

#define ASSERT_FALSE( EXPRESSION, ... )     \
    ({                                      \
        if ( UNLIKELY( (EXPRESSION) ) ) {     \
            __error_cases++;                \
            fprintf( stderr, __VA_ARGS__ ); \
        } else {                            \
            __passed_cases++;               \
        }                                   \
    })


#define ASSERT_EQUALS( ACTUAL, EXPECTED, ... ) \
    ({                                         \
        if ( UNLIKELY( (ACTUAL) != (EXPECTED) )) { \
            __error_cases++;                   \
            fprintf( stderr, __VA_ARGS__ );    \
        } else {                               \
            __passed_cases++;                  \
        }                                      \
    })


#endif // ENGINE_TEST_H
