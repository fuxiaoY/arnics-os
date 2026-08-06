#include "unity.h"

void test_crc16(void);
void test_ringbuffer(void);
void test_queue_win(void);
void test_data_platform(void);

void setUp(void) {}
void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_crc16);
    RUN_TEST(test_ringbuffer);
    RUN_TEST(test_queue_win);
    RUN_TEST(test_data_platform);
    return UNITY_END();
}
