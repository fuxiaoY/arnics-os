#include "unity.h"
#include "rtosInterface/queue/queue_port.h"

extern const queue_ops_t queue_ops_win;

typedef struct {
    uint32_t value;
} test_item_t;

void test_queue_win(void)
{
    const uint32_t length = 2u;
    void *queue = queue_ops_win.create(sizeof(test_item_t), length);
    TEST_ASSERT_NOT_NULL(queue);

    test_item_t first = {1u};
    test_item_t second = {2u};
    test_item_t third = {3u};
    test_item_t received = {0u};
    test_item_t peeked = {0u};

    TEST_ASSERT_FALSE(queue_ops_win.send(NULL, &first, 0u));
    TEST_ASSERT_FALSE(queue_ops_win.send(queue, NULL, 0u));
    TEST_ASSERT_FALSE(queue_ops_win.recv(queue, NULL, 0u));
    TEST_ASSERT_FALSE(queue_ops_win.peek(queue, NULL));
    TEST_ASSERT_EQUAL_UINT(0u, queue_ops_win.count(queue));
    TEST_ASSERT_FALSE(queue_ops_win.recv(queue, &received, 0u));

    TEST_ASSERT_TRUE(queue_ops_win.send(queue, &first, 0u));
    TEST_ASSERT_TRUE(queue_ops_win.send(queue, &second, 0u));
    TEST_ASSERT_EQUAL_UINT(2u, queue_ops_win.count(queue));
    TEST_ASSERT_TRUE(queue_ops_win.peek(queue, &peeked));
    TEST_ASSERT_EQUAL_UINT(1u, peeked.value);
    TEST_ASSERT_EQUAL_UINT(2u, queue_ops_win.count(queue));

    TEST_ASSERT_FALSE(queue_ops_win.send(queue, &third, 0u));
    TEST_ASSERT_FALSE(queue_ops_win.send(queue, &third, 1u));

    TEST_ASSERT_TRUE(queue_ops_win.recv(queue, &received, 0u));
    TEST_ASSERT_EQUAL_UINT(1u, received.value);
    TEST_ASSERT_TRUE(queue_ops_win.recv(queue, &received, 0u));
    TEST_ASSERT_EQUAL_UINT(2u, received.value);
    TEST_ASSERT_EQUAL_UINT(0u, queue_ops_win.count(queue));

    TEST_ASSERT_TRUE(queue_ops_win.send(queue, &third, 0u));
    TEST_ASSERT_TRUE(queue_ops_win.recv(queue, &received, 0u));
    TEST_ASSERT_EQUAL_UINT(3u, received.value);

    TEST_ASSERT_NULL(queue_ops_win.create(0u, length));
    TEST_ASSERT_NULL(queue_ops_win.create(sizeof(test_item_t), 0u));
}
