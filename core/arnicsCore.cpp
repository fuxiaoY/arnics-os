#include "arnicsCore.h"

static unsigned int arnics_systick;

void arnics_systick_handler()
{
  arnics_systick++;
}
void arnics_addTick(uint32_t addTime)
{
  arnics_systick += addTime;
}

/*----------------------------------------------------------*/
/*----------------------------------------------------------*/

/*----------------------------------------------------------*/
/*----------------------------------------------------------*/

DEFINE_ARNICS_FUNC_ITEM_RANGE(arnics_init_item, INIT_TAG, 0, 5);

void arnics_core_init()
{
    EXECUTE_FUNC_ALLSECTION(arnics_init_item);
}


