

#include "dataManagerList.h"
#if PLATFORM_MCU
#pragma arm section code = "._entry_dataplat_api"
#endif
#define DATA_LIST_INSTANCE
#include "dataPlat/dataObj/dataClass.h"
#undef DATA_LIST_INSTANCE


