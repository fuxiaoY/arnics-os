#include "unity.h"

void test_event_center_external_round_trip(void);
void test_event_center_internal_round_trip(void);
void test_event_center_hired_round_trip(void);
void test_event_center_flag_validation(void);
void test_event_center_concurrent_stress(void);

void setUp(void) {}
void tearDown(void) {}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_event_center_external_round_trip);
    RUN_TEST(test_event_center_internal_round_trip);
    RUN_TEST(test_event_center_hired_round_trip);
    RUN_TEST(test_event_center_flag_validation);
    RUN_TEST(test_event_center_concurrent_stress);
    return UNITY_END();
}
