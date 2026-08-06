#ifndef EVENT_CENTER_TEST_SUPPORT_H
#define EVENT_CENTER_TEST_SUPPORT_H

#include <stdint.h>

void event_test_runtime_init(void);
void event_test_process_one(void);
void event_test_drain_queues(void);
void event_test_employees_reset(void);
uint32_t event_test_external_calls(void);
uint32_t event_test_internal_calls(void);
uint32_t event_test_hired_calls(void);

#endif
