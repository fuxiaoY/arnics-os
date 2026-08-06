#include "unity.h"
#include "crc16.h"

uint16_t crc16_update_tab(uint16_t crc, const void* cp, uint16_t size);
uint16_t crc16_update_revbit(uint16_t crc, const void* cp, uint16_t size);
uint16_t crc16_update_bit(uint16_t crc, const void* cp, uint16_t size);

void test_crc16(void)
{
    static const uint8_t standard_vector[] = "123456789";
    static const uint8_t binary_vector[] = {0x00u, 0x01u, 0x7fu, 0x80u, 0xfeu, 0xffu};

    TEST_ASSERT_EQUAL_HEX16(0x906eu,
        crc16_update_revtab(0u, standard_vector, (uint16_t)(sizeof(standard_vector) - 1u)));
    TEST_ASSERT_EQUAL_HEX16(0x31c3u,
        crc16_update_tab(0u, standard_vector, (uint16_t)(sizeof(standard_vector) - 1u)));

    TEST_ASSERT_EQUAL_HEX16(
        crc16_update_revbit(0u, binary_vector, (uint16_t)sizeof(binary_vector)),
        crc16_update_revtab(0u, binary_vector, (uint16_t)sizeof(binary_vector)));
    TEST_ASSERT_EQUAL_HEX16(
        crc16_update_bit(0u, binary_vector, (uint16_t)sizeof(binary_vector)),
        crc16_update_tab(0u, binary_vector, (uint16_t)sizeof(binary_vector)));

    TEST_ASSERT_EQUAL_HEX16(0u, crc16_update_revtab(0u, binary_vector, 0u));
    TEST_ASSERT_EQUAL_HEX16(0u, crc16_update_tab(0u, binary_vector, 0u));

    const uint16_t first = crc16_update_tab(0u, standard_vector, 4u);
    TEST_ASSERT_EQUAL_HEX16(
        crc16_update_tab(first, standard_vector + 4u, 5u),
        crc16_update_tab(0u, standard_vector, 9u));
}
