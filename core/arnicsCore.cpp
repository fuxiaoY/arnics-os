#include "arnicsCore.h"

ArnicsCoreData arnics_core_data;

static void nop_process(void) {}


const ArnicsInitItem arnics_init_item_start _SECTION("arnics.init.0") = 
{     
    "", nop_process
};
const ArnicsInitItem arnics_init_item_end   _SECTION("arnics.init.5") = 
{       
    "", nop_process
};

static void init_items(void)
{
    const pArnicsInitItem it = &arnics_init_item_start;
    while (it < &arnics_init_item_end) 
    {
        it++->init();
    }   
}

void arnics_systick_handler()
{
    arnics_core_data.arnics_systick++;
}
void arnics_addTick(uint32_t addTime)
{
  arnics_core_data.arnics_systick += addTime;
}


void arnics_core_init()
{
    init_items();
}
