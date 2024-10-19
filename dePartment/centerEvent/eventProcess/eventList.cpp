#include "eventCore.h"


void led_action(void)
{

}
ARNICS_REGISTER("led_action",led_action,EVENT_TAG,1);

void battery_check(void)
{

}
ARNICS_REGISTER("battery_check",battery_check,EVENT_TAG,1);
