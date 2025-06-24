#ifndef _DATA_CORE_H_
#define _DATA_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/ProjDefine.h"
#include "thirdParty/Assert/usr_assert.h"
/* define ------------------------------------------------------------*/
#define DATA(NAME) DATA_PLAT_##NAME

typedef struct dataPlatInstance_t dataPlatInstance_t;

extern void* dataParaGetPtrEx(const dataPlatInstance_t* instance, uint32_t id);
extern bool dataParaGetEx(const dataPlatInstance_t* instance, uint32_t id, void* out_buf, uint32_t buf_len);
extern bool dataParaSetEx(const dataPlatInstance_t* instance, uint32_t id, const void* data, uint32_t len);
extern bool dataAuthCheck(void);

#ifdef __cplusplus
}
#endif
#endif

