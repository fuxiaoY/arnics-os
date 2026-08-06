#include "event_center_test_config.h"

static volatile uint32_t external_calls;
static volatile uint32_t internal_calls;
static volatile uint32_t hired_calls;

void event_test_employees_reset(void)
{
    external_calls = 0u;
    internal_calls = 0u;
    hired_calls = 0u;
}

uint32_t event_test_external_calls(void) { return external_calls; }
uint32_t event_test_internal_calls(void) { return internal_calls; }
uint32_t event_test_hired_calls(void) { return hired_calls; }

void test_external_employee(void *argv)
{
    external_calls++;
    if (argv != NULL)
    {
        test_external_employee_msg_t *msg = (test_external_employee_msg_t *)argv;
        msg->response = msg->request + 100u;
    }
}

void test_internal_employee(void *argv)
{
    internal_calls++;
    if (argv != NULL)
    {
        test_internal_employee_msg_t *msg = (test_internal_employee_msg_t *)argv;
        msg->response = msg->request * 2u;
    }
}

void test_hired_employee(void *argv)
{
    hired_calls++;
    if (argv != NULL)
    {
        test_hired_employee_msg_t *msg = (test_hired_employee_msg_t *)argv;
        msg->response = msg->request ^ 0x5a5a5a5au;
    }
}
