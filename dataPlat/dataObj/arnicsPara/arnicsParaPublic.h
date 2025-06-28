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
extern volatile uint32_t arnics_systick;
extern bool arnics_para_save(void);
extern bool arnics_para_load(void);
#undef X 
#define X(auth,index,type,subtype,var,len,key) + 1
#define ARNICS_PARA_ENTRIES \
X(R|W, DATA(arnics_start)         , TYPE_BOOL      , TYPE_NULL, &arnics_start           , sizeof(bool)     , "arnics_start"        )  \
X(R|W, DATA(arnics_systick)       , TYPE_UINT32    , TYPE_NULL, (void*)&arnics_systick  , sizeof(uint32_t) , "arnics_systick"      )  \
X(R|W, DATA(sys_allow_sleep)      , TYPE_BOOL      , TYPE_NULL, &sys_allow_sleep        , sizeof(bool)     , "sys_allow_sleep"     )  \
X(R|W, DATA(virtual_environment)  , TYPE_UINT8     , TYPE_NULL, &virtual_environment    , sizeof(uint8_t)  , "virtual_environment" )  \
X(R|W, DATA(eventos_want_sleep)   , TYPE_BOOL      , TYPE_NULL, &eventos_want_sleep     , sizeof(bool)     , "eventos_want_sleep"  )  \
X(R|W, DATA(business_want_sleep)  , TYPE_BOOL      , TYPE_NULL, &business_want_sleep    , sizeof(bool)     , "business_want_sleep" )  \
X(R|W, DATA(media_want_sleep)     , TYPE_BOOL      , TYPE_NULL, &media_want_sleep       , sizeof(bool)     , "media_want_sleep"    )  \
X(R|W, DATA(guard_want_sleep)     , TYPE_BOOL      , TYPE_NULL, &guard_want_sleep       , sizeof(bool)     , "guard_want_sleep"    )  \
X(R|W, DATA(console_want_sleep)   , TYPE_BOOL      , TYPE_NULL, &console_want_sleep     , sizeof(bool)     , "console_want_sleep"  )  

#ifdef __cplusplus
}
#endif
#endif


