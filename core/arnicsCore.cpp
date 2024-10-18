#include "arnicsCore.h"

ArnicsCoreData arnics_core_data;

void arnics_systick_handler()
{
    arnics_core_data.arnics_systick++;
}
void arnics_addTick(uint32_t addTime)
{
  arnics_core_data.arnics_systick += addTime;
}

/*----------------------------------------------------------*/
/*----------------------------------------------------------*/

/*----------------------------------------------------------*/
/*----------------------------------------------------------*/

DEFINE_ARNICS_FUNC_ITEM_RANGE(arnics_init_item, "init", 0, 5);

void arnics_core_init()
{
    EXECUTE_FUNC_ALLSECTION(arnics_init_item);
}


