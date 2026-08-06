#include "unity.h"
#include "ringbuffer.h"

void test_ringbuffer(void)
{
    unsigned char storage[8] = {0};
    unsigned char input[12];
    unsigned char output[12] = {0};
    ring_buf_t ring = {0};

    for (unsigned int i = 0u; i < sizeof(input); ++i)
        input[i] = (unsigned char)i;

    TEST_ASSERT_TRUE(ring_buf_init(&ring, storage, sizeof(storage)));
    TEST_ASSERT_EQUAL_UINT(0u, ring_buf_len(&ring));
    TEST_ASSERT_EQUAL_UINT(8u, ring_buf_free_space(&ring));

    TEST_ASSERT_EQUAL_UINT(6u, ring_buf_put(&ring, input, 6u));
    TEST_ASSERT_EQUAL_UINT(6u, ring_buf_len(&ring));
    TEST_ASSERT_EQUAL_UINT(2u, ring_buf_free_space(&ring));
    TEST_ASSERT_EQUAL_UINT(3u, ring_buf_get(&ring, output, 3u));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(input, output, 3u);

    TEST_ASSERT_EQUAL_UINT(5u, ring_buf_put(&ring, input + 6u, 5u));
    TEST_ASSERT_EQUAL_UINT(8u, ring_buf_len(&ring));
    TEST_ASSERT_EQUAL_UINT(0u, ring_buf_free_space(&ring));
    TEST_ASSERT_EQUAL_UINT(0u, ring_buf_put(&ring, input, 4u));

    TEST_ASSERT_EQUAL_UINT(8u, ring_buf_get(&ring, output, 12u));
    TEST_ASSERT_EQUAL_UINT8(3u, output[0]);
    TEST_ASSERT_EQUAL_UINT8(4u, output[1]);
    TEST_ASSERT_EQUAL_UINT8(5u, output[2]);
    TEST_ASSERT_EQUAL_UINT8(6u, output[3]);
    TEST_ASSERT_EQUAL_UINT8(7u, output[4]);
    TEST_ASSERT_EQUAL_UINT8(8u, output[5]);
    TEST_ASSERT_EQUAL_UINT8(9u, output[6]);
    TEST_ASSERT_EQUAL_UINT8(10u, output[7]);

    ring_buf_clr(&ring);
    TEST_ASSERT_EQUAL_UINT(0u, ring_buf_len(&ring));
    TEST_ASSERT_EQUAL_UINT(8u, ring_buf_free_space(&ring));

    ring_buf_t invalid = {0};
    TEST_ASSERT_FALSE(ring_buf_init(&invalid, storage, 7u));
    TEST_ASSERT_TRUE(ring_buf_init(&invalid, storage, 1u));
}
