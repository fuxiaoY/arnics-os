
#include "globalDataManager.h"
#include "globalData.h"
#include "../thirdParty/thirdPartyInclude.h"
#include "../common/commonInclude.h"
#pragma arm section code = "._entry_dataPlat_api"
// 静态缓冲区
SytemCfg *unityGlobalCfgPara = &global_cfg;
SytemState *unityGlobalStatPara = &global_state;
// S权限由督查中心开机后自动更新存取
static const unityParaList_t unity_system_ParaList[] = 
{
    {R|W,  Unity_System_usr_systick,               UINT32_TYPE, NULL_TYPE,   (void*)&usr_systick,                       sizeof(uint32_t),       "usr_systick"}
};




static const unityParaList_t unity_GlobalCfg_ParaList[] = 
{
    {R|W, Unity_global_cfg_SaveTs,                      UINT32_TYPE, NULL_TYPE,   &unityGlobalCfgPara->SaveTs,                      sizeof(uint32_t),  "SaveTs"}
};

static const unityParaList_t unity_GlobalStat_ParaList[] = 
{
    {R|W,  Unity_global_state_SaveTs,            UINT32_TYPE,NULL_TYPE, &unityGlobalStatPara->SaveTs,             sizeof(uint32_t),  "SaveTs"},
    {R,    Unity_global_cfg_WorkStat,            UINT8_TYPE, NULL_TYPE, &unityGlobalStatPara->WorkStat,           sizeof(uint8_t),  "WorkStat"},
    {R,    Unity_global_cfg_PreWorkStat,         UINT8_TYPE, NULL_TYPE, &unityGlobalStatPara->PreWorkStat,        sizeof(uint8_t),  "PreWorkStat"}
};

static uint16_t Unity_systemParaNumGet(void)
{
    return (sizeof(unity_system_ParaList) / sizeof(unityParaList_t));
}


static uint16_t Unity_GlobalCfgParaNumGet(void)
{
    return (sizeof(unity_GlobalCfg_ParaList) / sizeof(unityParaList_t));
}

static uint16_t Unity_GlobalStatParaNumGet(void)
{
    return (sizeof(unity_GlobalStat_ParaList) / sizeof(unityParaList_t));
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
    arnicsFree(JsonOut);
}

void UintyGlobalStatInterface(const char *ArgReq)
{
    char *JsonOut = NULL;
    if (strstr(ArgReq, "{all}") != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet("all", unity_GlobalStat_ParaList, Unity_GlobalStatParaNumGet(),FORMAT);
    }
    else if (ArgReq != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet(ArgReq, unity_GlobalStat_ParaList, Unity_GlobalStatParaNumGet(),FORMAT);
    }
    printf("%s\r\n", JsonOut);
    arnicsFree(JsonOut);
}

void UintyGlobalCfgInterface(const char *ArgReq)
{
    char *JsonOut = NULL;
    if (strstr(ArgReq, "{all}") != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet("all", unity_GlobalCfg_ParaList, Unity_GlobalCfgParaNumGet(),FORMAT);
    }
    else if (ArgReq != NULL)
    {
        JsonOut = UintyParaInterfaceGetSet(ArgReq, unity_GlobalCfg_ParaList, Unity_GlobalCfgParaNumGet(),FORMAT);
    }
    printf("%s\r\n", JsonOut);
    arnicsFree(JsonOut);
}


