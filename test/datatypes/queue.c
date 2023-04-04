#include <datatypes/queue.h>
#include "../test.h"

TEST_UNIT({
    TEST_CASE("Empty Queue", {
        queue(int) q;
        queue_init(q);

        /*
         * API Testing.
         */
         ASSERT_TRUE(queue_empty(q), "Queue must be empty.");
    });
})