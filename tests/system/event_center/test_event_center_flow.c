#include "unity.h"
#include "event_center_test_config.h"
#include "event_center_test_support.h"
#include "dePartment/centerEvent/eventProcess/eventCore.h"

static messageUnion_u round_trip(eventFlag_t *request)
{
    messageUnion_u response = {0};
    const uint32_t id = SendEventCallToEventCenter(*request, 1000u);
    TEST_ASSERT_NOT_EQUAL(0u, id);
    event_test_process_one();
    TEST_ASSERT_TRUE(GetResponseMessageFromEventCenter(id, 1000u, &response));
    return response;
}

void test_event_center_external_round_trip(void)
{
    eventFlag_t request = {0};
    event_test_runtime_init();
    TEST_ASSERT_TRUE(set_event_flag(&request, "test_external_employee", true));
    request.msg.message_deliver.test_external_employee_msg.request = 23u;

    const messageUnion_u response = round_trip(&request);
    TEST_ASSERT_EQUAL_UINT32(123u, response.message_deliver.test_external_employee_msg.response);
    TEST_ASSERT_EQUAL_UINT32(1u, event_test_external_calls());
    TEST_ASSERT_EQUAL_UINT32(0u, event_test_hired_calls());
}

void test_event_center_internal_round_trip(void)
{
    eventFlag_t request = {0};
    event_test_runtime_init();
    TEST_ASSERT_TRUE(set_event_flag(&request, "test_internal_employee", true));
    request.msg.message_deliver.test_internal_employee_msg.request = 21u;

    const messageUnion_u response = round_trip(&request);
    TEST_ASSERT_EQUAL_UINT32(42u, response.message_deliver.test_internal_employee_msg.response);
    TEST_ASSERT_EQUAL_UINT32(1u, event_test_internal_calls());
}

void test_event_center_hired_round_trip(void)
{
    eventFlag_t request = {0};
    event_test_runtime_init();
    TEST_ASSERT_TRUE(set_event_flag(&request, "test_hired_employee", true));
    request.msg.message_deliver.test_hired_employee_msg.request = 0x12345678u;

    const messageUnion_u response = round_trip(&request);
    TEST_ASSERT_EQUAL_HEX32(0x486e0c22u, response.message_deliver.test_hired_employee_msg.response);
    TEST_ASSERT_EQUAL_UINT32(1u, event_test_hired_calls());
}

void test_event_center_flag_validation(void)
{
    eventFlag_t request = {0};
    event_test_runtime_init();
    TEST_ASSERT_FALSE(set_event_flag(&request, "missing_employee", true));
    TEST_ASSERT_TRUE(add_event_flag(&request, "test_external_employee", true));
    TEST_ASSERT_TRUE(add_event_flag(&request, "test_internal_employee", true));
}
