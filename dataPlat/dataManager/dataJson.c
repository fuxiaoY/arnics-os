
#include "dataJson.h"
#include "thirdParty/struct2json/inc/s2j.h"
#include "common/commonInclude.h"
#include "dataPlat/dataManager/dataManagerList.h"



// ���ò��������� JSON
static void setToJson(cJSON *obj, const dataParaList_t *field, cJSON *json_obj)
{
    switch (field->type)
    {
    case TYPE_UINT8:
    case TYPE_CHAR:
    case TYPE_BOOL:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->auth & W) != NA)
            {
                *(uint8_t *)field->pVar = (uint8_t)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "auth W denied");
            }
        }
        break;

    case TYPE_INT:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->auth & W) != NA)
            {
                *(int *)field->pVar = (int)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "auth W denied");
            }
        }
        break;

    case TYPE_INT16:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->auth & W) != NA)
            {
                *(int16_t *)field->pVar = (int16_t)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "auth W denied");
            }
        }
        break;

    case TYPE_UINT16:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->auth & W) != NA)
            {
                *(uint16_t *)field->pVar = (uint16_t)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "auth W denied");
            }
        }
        break;

    case TYPE_UINT32:
    case TYPE_ENUM:
    case TYPE_SIZE_T:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->auth & W) != NA)
            {
                *(uint32_t *)field->pVar = (uint32_t)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "auth W denied");
            }
        }
        break;

    case TYPE_FLOAT:
        if ((obj->type & cJSON_Number) != 0)
        {
            if ((field->auth & W) != NA)
            {
                *(float *)field->pVar = (float)obj->valuedouble;
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "auth W denied");
            }
        }
        break;

    case TYPE_STRING:
        if ((obj->type & cJSON_String) != 0)
        {
            if ((field->auth & W) != NA)
            {
                strcpy((char *)field->pVar, obj->valuestring);
                cJSON_AddStringToObject(json_obj, field->key, "OK");
            }
            else
            {
                cJSON_AddStringToObject(json_obj, field->key, "auth W denied");
            }
        }
        break;

    case TYPE_ARRAY:
    case TYPE_STRUCT:
    case TYPE_UNION:
    {
        float *values = (float *)field->pVar;
        cJSON *array = obj;
        cJSON *item = array->child;
        int index = 0;
        while (item != NULL)
        {
            if ((item->type & cJSON_Number) != 0)
            {
                if ((field->auth & W) != NA)
                {
                    values[index++] = (float)item->valuedouble;
                }
                else
                {
                    cJSON_AddStringToObject(json_obj, field->key, "auth W denied");
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
// ���������������������� JSON ֵ
static void packToJSON(cJSON *obj, const dataParaList_t *field)
{
    switch (field->type)
    {
    case TYPE_UINT8:
    case TYPE_CHAR:
    case TYPE_BOOL:
        if ((field->auth & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(uint8_t *)field->pVar);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "auth R denied");
        }
        break;

    case TYPE_INT:
        if ((field->auth & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(int *)field->pVar);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "auth R denied");
        }
        break;

    case TYPE_INT16:
        if ((field->auth & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(int16_t *)field->pVar);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "auth R denied");
        }
        break;

    case TYPE_UINT16:
        if ((field->auth & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(uint16_t *)field->pVar);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "auth R denied");
        }
        break;

    case TYPE_UINT32:
    case TYPE_ENUM:
    case TYPE_SIZE_T:
        if ((field->auth & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(uint32_t *)field->pVar);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "auth R denied");
        }
        break;

    case TYPE_FLOAT:
        if ((field->auth & R) != NA)
        {
            cJSON_AddNumberToObject(obj, field->key, *(float *)field->pVar);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "auth R denied");
        }
        break;

    case TYPE_STRING:
        if ((field->auth & R) != NA)
        {
            cJSON_AddStringToObject(obj, field->key, (char *)field->pVar);
        }
        else
        {
            cJSON_AddStringToObject(obj, field->key, "auth R denied");
        }
        break;

    case TYPE_ARRAY:
    case TYPE_STRUCT:
    case TYPE_UNION:
        if ((field->auth & R) != NA)
        {
            cJSON *array = cJSON_CreateArray();
            if (field->sub_type == TYPE_FLOAT)
            {
                float *values = (float *)field->pVar;
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
            cJSON_AddStringToObject(obj, field->key, "auth R denied");
        }
        break;

    default:
        break;
    }
}
// ���ṹ��ת��Ϊ JSON ����
static cJSON *jsonMethod(const char *ArgReq, const dataParaList_t *unityPara, uint16_t len)
{
    cJSON *json_obj = cJSON_CreateObject();

    // �ж� ArgReq �Ƿ�Ϊ "all"
    if (strcmp(ArgReq, "all") == 0)
    {
        // ���������ֶ�
        for (uint16_t i = 0; i < len; i++)
        {
            if ((unityPara[i].auth & R) != NA)
            {
                packToJSON(json_obj, &unityPara[i]);
            }

        }
    }
    else
    {
        // ���� JSON �ַ���
        cJSON *argReq = cJSON_Parse(ArgReq);
        if (argReq == NULL)
        {
            printf("Error parsing JSON string.\n");
            cJSON_Delete(json_obj);
            return NULL;
        }

        // ���� argReq �е����м�
        cJSON *item = argReq->child;
        while (item != NULL)
        {
            for (uint16_t i = 0; i < len; ++i)
            {
                if (strcmp(item->string, unityPara[i].key) == 0)
                {
                    if (strcmp(item->valuestring, "~") == 0)
                    {
                        // ��ȡ��ֵ
                        packToJSON(json_obj, &unityPara[i]);
                        break;
                    }
                    else
                    {
                        // ���ø�ֵ
                        setToJson(item, &unityPara[i], json_obj);
                        break;
                    }
                }
            }
            item = item->next;
        }

        cJSON_Delete(argReq); // �ͷŽ����� JSON ����
    }

    return json_obj;
}

char *UnityParaInterfaceGetSet(const char *ArgReq, const dataParaList_t *unityPara, uint16_t len,jsonFomat_e format)
{
    // ת��Ϊ JSON
    cJSON *json = jsonMethod(ArgReq, unityPara, len);
    char *JsonOut = NULL;
    // ��ӡ JSON
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
    //  ����
    cJSON_Delete(json);
    return JsonOut;
}

/*---------------------------------------------------------------------------------------------*/
/*-�ӿ�----------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------*/

// �ص���������ӡ��ȡ����־��Ϣ
bool print_str_callback(char *str) 
{
    printf("%s\r\n", str);
    return true; // ���� true ������ȡ
}

void UnitySystemInterface(const char *ArgReq,unityCb cb)
{
    char *JsonOut = NULL;
    if (strstr(ArgReq, "{all}") != NULL)
    {
        JsonOut = UnityParaInterfaceGetSet("all", unity_system_paralist, unity_systemParaNumGet(),FORMAT);
    }
    else if (ArgReq != NULL)
    {
        JsonOut = UnityParaInterfaceGetSet(ArgReq, unity_system_paralist, unity_systemParaNumGet(),FORMAT);
    }
    cb(JsonOut);
    arnicsFree(JsonOut);
}

void UnityGlobalStatInterface(const char *ArgReq,unityCb cb)
{
    char *JsonOut = NULL;
    if (strstr(ArgReq, "{all}") != NULL)
    {
        JsonOut = UnityParaInterfaceGetSet("all", unity_global_status_paralist, unity_GlobalStatParaNumGet(),FORMAT);
    }
    else if (ArgReq != NULL)
    {
        JsonOut = UnityParaInterfaceGetSet(ArgReq, unity_global_status_paralist, unity_GlobalStatParaNumGet(),FORMAT);
    }
    cb(JsonOut);
    arnicsFree(JsonOut);
}

void UnityGlobalCfgInterface(const char *ArgReq,unityCb cb)
{
    char *JsonOut = NULL;
    if (strstr(ArgReq, "{all}") != NULL)
    {
        JsonOut = UnityParaInterfaceGetSet("all", unity_global_cfg_paralist, unity_GlobalCfgParaNumGet(),FORMAT);
    }
    else if (ArgReq != NULL)
    {
        JsonOut = UnityParaInterfaceGetSet(ArgReq, unity_global_cfg_paralist, unity_GlobalCfgParaNumGet(),FORMAT);
    }
    cb(JsonOut);
    arnicsFree(JsonOut);
}


