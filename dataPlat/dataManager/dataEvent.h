#ifndef _DATA_EVET_H_
#define _DATA_EVET_H_
#include "common/commonInclude.h"
#define DATA_PUBLIC_STRUCT
#include "dataPlat/dataObj/dataClass.h"
#undef DATA_PUBLIC_STRUCT
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ----------------------------------------------------------*/
#include "Inc/projDefine.h"
#include "Inc/typedef.h"


/* function prototypes -----------------------------------------------*/
/**
 *  @brief:         获取事件相关的状态
 *  @param [in] :    event - 事件实例 state - 状态字位标志
 *  @return:        状态值
 */
extern bool evt_state_get(event_status_t* event, EVT_STA state);

/**
 *  @brief:        设置事件相关的状态
 *  @param [in] :   event - 事件实例 state - 预置的状态位
 *  
 */
extern void evt_state_set(event_status_t* event, EVT_STA state);

/**
 *  @brief:        清除事件相关的状态
 *  @param [in] :   event - 事件实例 state - 状态字位标志
 *  
 */
extern void evt_state_clear(event_status_t* event, EVT_STA state);

/**
 * @brief:         事件初始化
 * @param [in] :   event - 事件实例
 */
extern void evt_state_init(event_status_t* event);
#ifdef __cplusplus
}
#endif
#endif
