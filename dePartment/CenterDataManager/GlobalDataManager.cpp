
#include "GlobalDataManager.h"
#include "FreeRTOS.h"
#include "stm32l4xx_it.h"
#include "FWversion.h"
#include "DataPlat.h"
// 静态缓冲区
ST_RTConfig *unityRTCfgPara = &g_RTCfg;
ST_RTState *unityRTStatPara = &g_RTStat;
// S权限由督查中心开机后自动更新存取
static const unityParaList_t unity_system_ParaList[] = {
    {R,  Unity_System_DevNum,                      STRING_TYPE, NULL_TYPE,   (void*)_APP_VERSION_,                      strlen(_APP_VERSION_),  "versionCode"},
    {R,  Unity_System_DevDate,                      STRING_TYPE, NULL_TYPE,  (void*) _APP_FW_DATE_,                      strlen(_APP_VERSION_),  "versionDate"},
    {R,  Unity_System_ALLOWSLEEP,                  UINT8_TYPE,  NULL_TYPE,   (void*)&ALLOWSLEEP,                        sizeof(BOOL),           "ALLOWSLEEP"},
    {R,  Unity_System_MediaWantSleep,              UINT8_TYPE,  NULL_TYPE,   (void*)&MediaWantSleep,                    sizeof(BOOL),           "MediaWantSleep"},
    {R,  Unity_System_eventosWantSleep,            UINT8_TYPE,  NULL_TYPE,   (void*)&eventosWantSleep,                  sizeof(BOOL),           "eventosWantSleep"},
    {R,  Unity_System_ConsoleWantSleep,            UINT8_TYPE,  NULL_TYPE,   (void*)&ConsoleWantSleep,                  sizeof(BOOL),           "ConsoleWantSleep"},
    {R,  Unity_System_StartGuardWantSleep,         UINT8_TYPE,  NULL_TYPE,   (void*)&StartGuardWantSleep,               sizeof(BOOL),           "StartGuardWantSleep"},
    {R,  Unity_System_isFirstPowerOn,              UINT8_TYPE,  NULL_TYPE,   (void*)&isFirstPowerOn,                    sizeof(uint8_t),        "isFirstPowerOn"},
    {R,  Unity_System_systemResetFlag,             UINT32_TYPE, NULL_TYPE,   (void*)&system_reset_flag,                 sizeof(uint32_t),       "systemResetFlag"},
    {R,  Unity_System_systemResetFlagNun,          UINT32_TYPE, NULL_TYPE,   (void*)&system_reset_NUM,                 sizeof(uint32_t),       "systemResetNum"},
    {R,  Unity_System_SHELL_ACTIVE,                UINT8_TYPE,  NULL_TYPE,   (void*)&SHELL_ACTIVE,                      sizeof(BOOL),           "SHELL_ACTIVE"},
    {R,  Unity_System_FREERTOS_INIT_ISDONE,        UINT8_TYPE,  NULL_TYPE,   (void*)&FREERTOS_INIT_ISDONE,              sizeof(BOOL),           "FREERTOS_INIT_ISDONE"},
    {R,  Unity_System_EVENTOS_INIT_ISDONE,         UINT8_TYPE,  NULL_TYPE,   (void*)&EVENTOS_INIT_ISDONE,               sizeof(BOOL),           "EVENTOS_INIT_ISDONE"},
    {R|W,  Unity_System_g_InitTs,                  UINT32_TYPE, NULL_TYPE,   (void*)&g_InitTs,                          sizeof(time_t),         "g_InitTs"},
    {R|W,  Unity_System_usr_systick,               UINT32_TYPE, NULL_TYPE,   (void*)&usr_systick,                       sizeof(uint32_t),       "usr_systick"},
    {R|W,  Unity_System_Exti_Counter,              UINT32_TYPE, NULL_TYPE,   (void*)&Exti_Counter,                      sizeof(uint32_t),       "Exti_Counter"},
    {R,  Unity_System_debug_uart_Counter,          UINT32_TYPE, NULL_TYPE,   (void*)&debug_uart_Counter,                sizeof(uint32_t),       "debug_uart_Counter"},
    {R|W|S,  Unity_System_VirtualEnvironment,      UINT8_TYPE, NULL_TYPE,    (void*)&VirtualEnvironment,                sizeof(uint8_t),       "VirtualEnvironment"},
    {R|W|S,  Unity_System_systemAllowSleep,        UINT8_TYPE, NULL_TYPE,    (void*)&systemAllowSleep,                  sizeof(uint8_t),       "systemAllowSleep"},
    {R|W|S,  Unity_System_TSDBForceCleanCount,      UINT32_TYPE, NULL_TYPE,    (void*)&TSDBForceCleanCount,             sizeof(uint32_t),      "TSDBForceCleanCount"}
};




static const unityParaList_t unity_RTCfg_ParaList[] = {
    {R|W,  Unity_RTCfg_SaveTs,                      UINT32_TYPE, NULL_TYPE,   &unityRTCfgPara->SaveTs,                      sizeof(uint32),  "SaveTs"},
    {R|W, Unity_RTCfg_VolCalibrate,                 FLOAT_TYPE, NULL_TYPE,      &unityRTCfgPara->VolCalibrate,                sizeof(unityRTCfgPara->VolCalibrate),   "VolCalibrate"}
    };

static const unityParaList_t unity_RTStat_ParaList[] = {
    {R|W,  Unity_RTStat_SaveTs,              UINT32_TYPE,NULL_TYPE, &unityRTStatPara->SaveTs,             sizeof(uint32),  "SaveTs"},
    {R,    Unity_RTStat_WorkStat,            UINT8_TYPE, NULL_TYPE, &unityRTStatPara->WorkStat,           sizeof(uint8),  "WorkStat"},
    {R,    Unity_RTStat_PreWorkStat,         UINT8_TYPE, NULL_TYPE, &unityRTStatPara->PreWorkStat,        sizeof(uint8),  "PreWorkStat"}
};
static uint16 Unity_systemParaNumGet(void)
{
    return (sizeof(unity_system_ParaList) / sizeof(unityParaList_t));
}


static uint16 Unity_RTCfgParaNumGet(void)
{
    return (sizeof(unity_RTCfg_ParaList) / sizeof(unityParaList_t));
}

static uint16 Unity_RTStatParaNumGet(void)
{
    return (sizeof(unity_RTStat_ParaList) / sizeof(unityParaList_t));
}

// 设置参数并更新 JSON
static void setToJson(cJSON *obj, const unityParaList_t *field, cJSON *json_obj)
{
    switch (field->type)
    {
    case UINT8_TYPE:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->access & W) != NA)
            {
                *(uint8_t *)field->addr = (uint8_t)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "access W denied");
            }
        }
        break;

    case INT_TYPE:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->access & W) != NA)
            {
                *(int *)field->addr = (int)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "access W denied");
            }
        }
        break;

    case INT16_TYPE:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->access & W) != NA)
            {
                *(int16_t *)field->addr = (int16_t)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "access W denied");
            }
        }
        break;

    case UINT16_TYPE:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->access & W) != NA)
            {
                *(uint16_t *)field->addr = (uint16_t)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "access W denied");
            }
        }
        break;

    case UINT32_TYPE:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->access & W) != NA)
            {
                *(uint32_t *)field->addr = (uint32_t)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "access W denied");
            }
        }
        break;

    case FLOAT_TYPE:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->access & W) != NA)
            {
                *(float *)field->addr = (float)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "access W denied");
            }
        }
        break;

    case STRING_TYPE:
        if ((obj->type & cJSON_String) != 0)
        {
            if ((field->access & W) != NA)
            {
                strcpy((char *)field->addr, obj->valuestring);
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "access W denied");
            }
        }
        break;

    case ARRAY_TYPE:
    {
        float *values = (float *)field->addr;
        cJSON *array = obj;
        cJSON *item = array->child;
        int index = 0;
        while (item != NULL)
        {
            if ((item->type & cJSON_Number) != 0)
            {
                if ((field->access & W) != NA)
                {
                    values[index++] = (float)item->valuedouble;
                }
                else
                {
                    cJSON_AddStringToObject(json_obj, field->key, "access W denied");
                }
            }
            item = item->next;
        }
        cJSON_AddStringToObject(json_obj, field->key, "OK");
    }
    break;

    default:
        break;
    }
}
// 辅助函数，根据类型设置 JSON 值
// 辅助函数，根据类型设置 JSON 值
static void packToJSON(cJSON *obj, const unityParaList_t *field)
{
    switch (field->type)
    {
    case UINT8_TYPE:
        if ((field->access & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(uint8_t *)field->addr);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "access R denied");
        }
        break;

    case INT_TYPE:
        if ((field->access & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(int *)field->addr);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "access R denied");
        }
        break;

    case INT16_TYPE:
        if ((field->access & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(int16_t *)field->addr);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "access R denied");
        }
        break;

    case UINT16_TYPE:
        if ((field->access & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(uint16_t *)field->addr);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "access R denied");
        }
        break;

    case UINT32_TYPE:
        if ((field->access & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(uint32_t *)field->addr);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "access R denied");
        }
        break;

    case FLOAT_TYPE:
        if ((field->access & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(float *)field->addr);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "access R denied");
        }
        break;

    case STRING_TYPE:
        if ((field->access & R) != NA)
        {
            cJSON_AddStringToObject(obj, field->key, (char *)field->addr);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "access R denied");
        }
        break;

    case ARRAY_TYPE:
        if ((field->access & R) != NA)
        {
            cJSON *array = cJSON_CreateArray();
            if (field->subType == FLOAT_TYPE)
            {
                float *values = (float *)field->addr;
                for (int i = 0; i < field->len / sizeof(float); ++i)
                {
                    cJSON *number = cJSON_CreateNumber(values[i]);
                    cJSON_AddItemToArray(array, number);
                }
            }
            cJSON_AddItemToObject(obj, field->key, array);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "access R denied");
        }
        break;

    default:
        break;
    }
}
// 将结构体转换为 JSON 对象
static cJSON *jsonMethod(const char *ArgReq, const unityParaList_t *unityPara, uint16_t len)
{
    cJSON *json_obj = cJSON_CreateObject();

    // 判断 ArgReq 是否为 "all"
    if (strcmp(ArgReq, "all") == 0)
    {
        // 处理所有字段
        for (uint16_t i = 0; i < len; i++)
        {
            packToJSON(json_obj, &unityPara[i]);
        }
    }
    else
    {
        // 解析 JSON 字符串
        cJSON *argReq = cJSON_Parse(ArgReq);
        if (argReq == NULL)
        {
            printf("Error parsing JSON string.\n");
            cJSON_Delete(json_obj);
            return NULL;
        }

        // 遍历 argReq 中的所有键
        cJSON *item = argReq->child;
        while (item != NULL)
        {
            for (uint16_t i = 0; i < len; ++i)
            {
                if (strcmp(item->string, unityPara[i].key) == 0)
                {
                    if (strcmp(item->valuestring, "~") == 0)
                    {
                        // 获取该值
                        packToJSON(json_obj, &unityPara[i]);
                        break;
                    }
                    else
                    {
                        // 设置该值
                        setToJson(item, &unityPara[i], json_obj);
                        break;
                    }
                }
            }
            item = item->next;
        }

        cJSON_Delete(argReq); // 释放解析的 JSON 对象
    }

    return json_obj;
}

char *UintyParaInterfaceGetSet(const char *ArgReq, const unityParaList_t *unityPara, uint16_t len,JsonFomat format)
{
    // 转换为 JSON
    cJSON *json = jsonMethod(ArgReq, unityPara, len);
    char *JsonOut = NULL;
    // 打印 JSON
    switch(format)
    {
        case FORMAT:
        JsonOut = cJSON_Print(json);
        break;
        case UNFORMAT:
        JsonOut = cJSON_PrintUnformatted(json);
        break;
        default:
        JsonOut = cJSON_Print(json);
        break;
    }


    // printf("%s\n", JSonStr);
    //  清理
    cJSON_Delete(json);
    return JsonOut;
}

/*---------------------------------------------------------------------------------------------*/
/*-接口----------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------*/
void UintySystemInterface(const char *ArgReq)
{
    char *JsonOut = NULL;
    if (strstr(ArgReq, "{all}") != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet("all", unity_system_ParaList, Unity_systemParaNumGet(),FORMAT);
    }
    else if (ArgReq != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet(ArgReq, unity_system_ParaList, Unity_systemParaNumGet(),FORMAT);
    }
    printf("%s\r\n", JsonOut);
    vPortFree(JsonOut);
}

void UintyRTStatInterface(const char *ArgReq)
{
    char *JsonOut = NULL;
    if (strstr(ArgReq, "{all}") != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet("all", unity_RTStat_ParaList, Unity_RTStatParaNumGet(),FORMAT);
    }
    else if (ArgReq != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet(ArgReq, unity_RTStat_ParaList, Unity_RTStatParaNumGet(),FORMAT);
    }
    printf("%s\r\n", JsonOut);
    vPortFree(JsonOut);
}

void UintyRTCfgInterface(const char *ArgReq)
{
    char *JsonOut = NULL;
    if (strstr(ArgReq, "{all}") != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet("all", unity_RTCfg_ParaList, Unity_RTCfgParaNumGet(),FORMAT);
    }
    else if (ArgReq != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet(ArgReq, unity_RTCfg_ParaList, Unity_RTCfgParaNumGet(),FORMAT);
    }
    printf("%s\r\n", JsonOut);
    vPortFree(JsonOut);
}


char *UintyWanInterface(const char *ArgReq)
{
    char *JsonOut = NULL;
    TSDB_Record("Got Wan Message!",DB_SYSTEM);
    TSDB_Record(ArgReq,DB_SYSTEM);
    if (strstr(ArgReq, "{all}") != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet("all", unity_RTCfg_ParaList, Unity_RTCfgParaNumGet(),FORMAT);
    }
    else if (ArgReq != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet(ArgReq, unity_RTCfg_ParaList, Unity_RTCfgParaNumGet(),FORMAT);
    }

    // vPortFree(JsonOut);在外释放
    return JsonOut;
}
/*---------------------------------------------------------------------------------------------*/
/*-数据平台初始化-------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------*/
// 接口实现

void SystemParaLoad()
{
    uint16 arraySize = Unity_systemParaNumGet();
    for (uint16 i = 0; i < arraySize; i++)
    {
        const unityParaList_t* item = &unity_system_ParaList[i];
        if ((item->access & S) != NA)
        {
            KVDB_Get(item->key, (uint8_t *)item->addr, item->len);
        }
    }
    SystemParaStore();
}

void SystemParaStore()
{
    uint16 arraySize = Unity_systemParaNumGet();
    for (uint16 i = 0; i < arraySize; i++)
    {
        const unityParaList_t* item = &unity_system_ParaList[i];
        if ((item->access & S) != NA)
        {
            KVDB_Set(item->key, (uint8_t *)item->addr, item->len);
        }
    }
}

