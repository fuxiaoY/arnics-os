#ifdef DATA_ALL_STRUCT
#include "dataPlat/dataObj/arnicsPara/arnicsPara.h"
#include "dataPlat/dataObj/systemStatus/systemStatus.h"
#include "dataPlat/dataObj/systemCfg/systemConfig.h"
#endif
#ifdef DATA_CLASS
ARNICS_PARA_ENTRIES
SYSTEM_STATUS_ENTRIES
SYSTEM_CFG_ENTRIES
#endif
#ifdef DATA_PUBLIC_STRUCT
#include "dataPlat/dataObj/arnicsPara/arnicsParaPublic.h"
#include "dataPlat/dataObj/systemCfg/systemConfigPublic.h"  
#include "dataPlat/dataObj/systemStatus/systemStatusPublic.h"
#endif


