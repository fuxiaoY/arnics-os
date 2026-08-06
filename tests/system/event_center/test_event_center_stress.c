#include "unity.h"
#include "event_center_test_config.h"
#include "event_center_test_support.h"
#include "dePartment/centerEvent/eventProcess/eventCore.h"
#include "rtosInterface/rtosInterfacePublic.h"
#include <process.h>
#include <windows.h>

#define STRESS_PRODUCERS 4u
#define STRESS_MESSAGES_PER_PRODUCER 250u
#define STRESS_TOTAL_MESSAGES (STRESS_PRODUCERS * STRESS_MESSAGES_PER_PRODUCER)

typedef struct {
    uint32_t producer;
    volatile LONG failures;
} producer_context_t;

static unsigned __stdcall stress_processor(void *argument)
{
    (void)argument;
    for (uint32_t i = 0u; i < STRESS_TOTAL_MESSAGES; ++i)
    {
        event_test_process_one();
    }
    return 0u;
}

static unsigned __stdcall stress_producer(void *argument)
{
    producer_context_t *context = (producer_context_t *)argument;
    for (uint32_t i = 0u; i < STRESS_MESSAGES_PER_PRODUCER; ++i)
    {
        eventFlag_t request = {0};
        messageUnion_u response = {0};
        const uint32_t value = context->producer * STRESS_MESSAGES_PER_PRODUCER + i;
        if (!set_event_flag(&request, "test_external_employee", true))
        {
            InterlockedIncrement(&context->failures);
            continue;
        }
        request.msg.message_deliver.test_external_employee_msg.request = value;
        const uint32_t id = SendEventCallToEventCenter(request, BLOCK_DELAY);
        if (id == 0u || !GetResponseMessageFromEventCenter(id, BLOCK_DELAY, &response) ||
            response.message_deliver.test_external_employee_msg.response != value + 100u)
        {
            InterlockedIncrement(&context->failures);
        }
    }
    return 0u;
}

void test_event_center_concurrent_stress(void)
{
    HANDLE threads[STRESS_PRODUCERS + 1u] = {0};
    producer_context_t contexts[STRESS_PRODUCERS] = {0};
    event_test_runtime_init();

    threads[0] = (HANDLE)_beginthreadex(NULL, 0u, stress_processor, NULL, 0u, NULL);
    TEST_ASSERT_NOT_NULL(threads[0]);
    for (uint32_t i = 0u; i < STRESS_PRODUCERS; ++i)
    {
        contexts[i].producer = i;
        threads[i + 1u] = (HANDLE)_beginthreadex(NULL, 0u, stress_producer, &contexts[i], 0u, NULL);
        TEST_ASSERT_NOT_NULL(threads[i + 1u]);
    }

    TEST_ASSERT_EQUAL_UINT32(WAIT_OBJECT_0,
        WaitForMultipleObjects(STRESS_PRODUCERS + 1u, threads, TRUE, 60000u));

    LONG failures = 0;
    for (uint32_t i = 0u; i < STRESS_PRODUCERS; ++i)
    {
        failures += contexts[i].failures;
    }
    for (uint32_t i = 0u; i < STRESS_PRODUCERS + 1u; ++i)
    {
        CloseHandle(threads[i]);
    }

    TEST_ASSERT_EQUAL_INT32(0, failures);
    TEST_ASSERT_EQUAL_UINT32(STRESS_TOTAL_MESSAGES, event_test_external_calls());
}
