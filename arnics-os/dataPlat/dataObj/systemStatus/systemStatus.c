
#include "systemStatus.h"
#include "port/portInclude.h"
#if PLATFORM_MCU
#pragma arm section code = "._entry_dataplat_api"
#endif
systemStatus_t g_system_status;
