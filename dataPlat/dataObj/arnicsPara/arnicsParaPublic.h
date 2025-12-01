#ifndef _ARNICS_PARA_H_
#define _ARNICS_PARA_H_

#ifdef __cplusplus
extern "C" {
#endif
/* typedef private ----------------------------------------------------*/
/**
 * @brief 对外'公有文档'在此处定义
 *        Definition of publicly accessible data struct is here.
 */
/* --------------------------------------------------------------------*/
/**
 * @struct 系统全局状态字
 */
typedef struct
{    
    union
    {
        struct
        {
            uint32_t eventos_is_working     :  1;
            uint32_t media_is_working       :  1;
            uint32_t guard_is_working       :  1;
            uint32_t console_is_working     :  1;
        };
        uint32_t working_status;
    };
}sleepStatus_t;
/* event instance ----------------------------------------------------*/
/**
 * @struct 系统事件
 */
typedef uint32_t EVT_STA;
typedef struct 
{
    EVT_STA status;
} event_status_t;

extern volatile uint32_t                                arnics_systick;
extern volatile sleepStatus_t                         sys_sleep_status;
extern event_status_t                                 sys_event_status;
extern bool arnics_para_save(void);
extern bool arnics_para_load(void);
#undef X 
#define X(auth,index,type,subtype,var,len,key) + 1
#define ARNICS_PARA_ENTRIES \
X(R|W, DATA(arnics_start)         , TYPE_BOOL      , TYPE_NULL, &arnics_start                               , sizeof(bool)          , "arnics_start"        )  \
X(R|W, DATA(arnics_systick)       , TYPE_UINT32    , TYPE_NULL, (void*)&arnics_systick                      , sizeof(uint32_t)      , "arnics_systick"      )  \
X(R  , DATA(sys_sleep_status)     , TYPE_UINT32    , TYPE_NULL, (void*)&sys_sleep_status.working_status     , sizeof(sleepStatus_t) , "sys_sleep_status"    )  \
X(R|W, DATA(sys_allow_sleep)      , TYPE_BOOL      , TYPE_NULL, &sys_allow_sleep                            , sizeof(bool)          , "sys_allow_sleep"     )  \
X(R|W, DATA(virtual_environment)  , TYPE_UINT8     , TYPE_NULL, &virtual_environment                        , sizeof(uint8_t)       , "virtual_environment" )  


#ifdef __cplusplus
}
#endif
#endif


