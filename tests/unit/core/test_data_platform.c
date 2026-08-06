#include "unity.h"
#include "dataPlat/dataCore/dataCore.h"
#include "dataPlat/dataCore/dataStruct.h"
#include "dataPlat/dataManager/dataEvent.h"

void test_data_platform(void)
{
    uint32_t value = 0x11223344u;
    dataParaList_t table[] = {
        {R | W, 0u, TYPE_UINT32, TYPE_NULL, &value, sizeof(value), "value"}
    };
    dataPlatInstance_t instance = {table, 1u};
    uint32_t output = 0u;

    TEST_ASSERT_EQUAL_PTR(&value, dataParaGetPtrEx(&instance, 0u));
    TEST_ASSERT_TRUE(dataParaGetEx(&instance, 0u, &output, sizeof(output)));
    TEST_ASSERT_EQUAL_HEX32(0x11223344u, output);

    uint16_t short_output = 0u;
    TEST_ASSERT_TRUE(dataParaGetEx(&instance, 0u, &short_output, sizeof(short_output)));
    TEST_ASSERT_EQUAL_HEX16(0x3344u, short_output);

    uint8_t oversized_output[sizeof(value) + 2u] = {0};
    TEST_ASSERT_FALSE(dataParaGetEx(&instance, 0u, oversized_output, sizeof(oversized_output)));
    TEST_ASSERT_EQUAL_MEMORY(&value, oversized_output, sizeof(value));

    const uint32_t replacement = 0xa5a55a5au;
    TEST_ASSERT_TRUE(dataParaSetEx(&instance, 0u, &replacement, sizeof(replacement)));
    TEST_ASSERT_EQUAL_HEX32(replacement, value);

    const uint16_t partial = 0x7788u;
    TEST_ASSERT_TRUE(dataParaSetEx(&instance, 0u, &partial, sizeof(partial)));
    TEST_ASSERT_EQUAL_HEX16(partial, (uint16_t)value);

    const uint8_t oversized_input[sizeof(value) + 2u] = {1u, 2u, 3u, 4u, 5u, 6u};
    TEST_ASSERT_FALSE(dataParaSetEx(&instance, 0u, oversized_input, sizeof(oversized_input)));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(oversized_input, &value, sizeof(value));
    TEST_ASSERT_TRUE(dataAuthCheck());

    event_status_t event = {0xffffffffu};
    evt_state_init(&event);
    TEST_ASSERT_EQUAL_HEX32(0u, event.status);
    TEST_ASSERT_FALSE(evt_state_get(&event, BIT0));
    evt_state_set(&event, BIT0);
    evt_state_set(&event, BIT3);
    TEST_ASSERT_TRUE(evt_state_get(&event, BIT0));
    TEST_ASSERT_TRUE(evt_state_get(&event, BIT3));
    TEST_ASSERT_FALSE(evt_state_get(&event, BIT2));
    evt_state_clear(&event, BIT0);
    TEST_ASSERT_FALSE(evt_state_get(&event, BIT0));
    TEST_ASSERT_TRUE(evt_state_get(&event, BIT3));
}
