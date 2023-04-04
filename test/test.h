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

#define TEST_CASE( DESCRIPTION, ASSERTS ) \
    ({                                    \
        unsigned __passed_cases = 0;      \
        unsigned __error_cases  = 0;      \
        ASSERTS                           \
    })

#define ASSERT_TRUE( EXPRESSION, FAIL_MESSAGE )  \
    ({                                           \
        if ( UNLIKELY(!EXPRESSION) ) {             \
            __error_cases++; \
            fprintf( stderr, FAIL_MESSAGE );     \
        } else {                                 \
            __passed_cases++;                                         \
        }               \
    })


#endif // ENGINE_TEST_H
