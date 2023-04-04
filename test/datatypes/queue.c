#include <datatypes/queue.h>
#include "../test.h"

TEST_UNIT({
    /*
     * Testing the queue's API and the internal queue's structure
     * when the queue is empty.
     */
    TEST_CASE("Empty Queue", {
        queue(int) q;
        queue_init(q);

        /*
         * API Testing.
         */
         ASSERT_TRUE(queue_empty(q), "Queue must be empty\n");
         ASSERT_EQUALS(queue_size(q), 0, "Queue's size must be equals to 0\n");
         ASSERT_NULL(queue_front(q), "Queue's front must be equals to NULL\n");

         /*
          * Internal Testing
          */
         ASSERT_NULL(q.__head, "Queue's head must be NULL\n");
         ASSERT_NULL(q.__tail, "Queue's tail must be NULL\n");
    });

    /*
     * Testing the queue's API and the internal queue's structure
     * when the queue has one element inserted.
     */
    TEST_CASE("Insert One Element", {
       queue(int) q;
       queue_init(q);

       int value = 10;

       queue_insert(q, &value);

       /*
        * API Testing.
        */
       ASSERT_FALSE(queue_empty(q), "Queue must not be empty\n");
       ASSERT_EQUALS(queue_size(q), 1U, "Queue size must be equals to 1\n");
       ASSERT_EQUALS(*queue_front(q), value, "Queue front value must be equals to %d\n", value);

       /*
        * Internal Testing.
        */
       ASSERT_NOT_NULL(q.__head, "Queue's head must not be NULL\n");
       ASSERT_NOT_NULL(q.__tail, "Queue's tail must not be NULL\n");
       ASSERT_TRUE(q.__head == q.__tail, "Queue's head must be equals to the queue's tail\n");
       ASSERT_EQUALS(*q.__head->__elem, value, "Queue's head element must be equals to %d\n", value);
       ASSERT_EQUALS(*q.__head->__elem, value, "Queue's tail element must be equals to %d\n", value);
    });

    /*
     * Testing the queue's API and the internal queue's structure
     * when the queue has two elements inserted.
     */
    TEST_CASE("Insert Two Elements", {
        queue(int) q;
        queue_init(q);

        int first  = 10;
        int second = 15;

        queue_insert(q, &first);
        queue_insert(q, &second);

        /*
         * API Testing.
         */
        ASSERT_FALSE(queue_empty(q), "Queue must not be empty\n");
        ASSERT_EQUALS(queue_size(q), 2U, "Queue size must be equals to 2\n");
        ASSERT_EQUALS(*queue_front(q), first, "Queue front value must be equals to %d\n", first);

        /*
         * Internal Testing
         */
        ASSERT_NOT_NULL(q.__head, "Queue's head must not be NULL");
        ASSERT_NOT_NULL(q.__tail, "Queue's tail must not be NULL");
        ASSERT_TRUE(q.__head != q.__tail, "Queue's head must be different from the queue's tail.\n");
        ASSERT_EQUALS(*q.__head->__elem, first, "Queue's head element must be equals to %d\n", first);
        ASSERT_EQUALS(*q.__tail->__elem, second, "Queue's tail element must be equals to %d\n", second);
    });

    /*
     * Testing the queue's API and the queue's internal structure
     * after the dequeue of an element.
     */
    TEST_CASE("Dequeue", {
       queue(int) q;
       queue_init(q);

       int first  = 20;
       int second = 25;

       queue_insert(q, &first);
       queue_insert(q, &second);

       int removed_value = *queue_remove(q);

       /*
        * API Testing.
        */
       ASSERT_EQUALS(removed_value, first, "Queue first removal element must be equals to %d\n", first);
       ASSERT_EQUALS(*queue_front(q), second, "Queue's front value must be equals to %d\n", second);

       /*
        * Internal Testing.
        */
       ASSERT_NOT_NULL(q.__head, "Queue's head must not be NULL\n");
       ASSERT_NOT_NULL(q.__tail, "Queue's tail must not be NULL\n");
       ASSERT_TRUE(q.__head == q.__tail, "Queue's head must be equals to the queue's tail\n");
    });

    /*
     * Testing the queue's API and queue's internal structure
     * after a bunch of insertion and removal of elements.
     */
    TEST_CASE("Thousand Element Insertion and Removal", {
        queue(int) q;
        queue_init(q);

        const unsigned quantity = 1000U;

        /* Element Insertion */
        for (unsigned i = 0U; i < quantity; i++) {
            int *value = malloc(sizeof(int));
            *value     = (int)i;

            queue_insert(q, value);
        }

        /*
         * API Testing.
         */
        ASSERT_FALSE(queue_empty(q), "Queue must be not empty\n");
        ASSERT_EQUALS(queue_size(q), quantity, "Queue size must be equals to %d\n", quantity);

        /* Element Checking */
        for (unsigned i = 0U; i < quantity; i++)
            ASSERT_EQUALS(*queue_remove(q), (int) i, "Queue's current front element must be equals to %d\n", i);
    });
})