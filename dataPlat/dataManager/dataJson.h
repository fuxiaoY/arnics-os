/* information */
/**
  ******************************************************************************
  * @file           : GlobalDataManager.h
  * @brief          : ȫ�����ݹ�����ص�ͷ�ļ�
  * 
  * ���ļ�������ȫ�����ݹ������غ궨�塢ö�١��ṹ��ͺ���������
  * ��Ҫ����ϵͳ�Ĳ�����������á�
  * 
  * @version        : 1.0.0
  * @date           : 2023-10-01
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 ARSTUDIO.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* information */
#ifndef _DATA_JSON_H_
#define _DATA_JSON_H_
#include "Inc/projDefine.h"
#include "Inc/typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "dataPlat/dataCore/dataCore.h"
#include "dataPlat/dataCore/dataStruct.h"

/* typedef -----------------------------------------------------------*/
typedef enum
{
    FORMAT = 0,
    UNFORMAT,
} jsonFomat_e;


extern void UnitySystemInterface(const char *ArgReq);
extern void UnityGlobalStatInterface(const char *ArgReq);
extern void UnityGlobalCfgInterface(const char *ArgReq);



#ifdef __cplusplus
}
#endif

#endif
