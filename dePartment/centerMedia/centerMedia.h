#ifndef _CENTER_MEDIA_H_
#define _CENTER_MEDIA_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "Inc/typedef.h"
#include "Inc/ProjDefine.h"

typedef struct {
    time_t ID_Ts;                         //��ϢID�������ڼ��������Ϣ����
    char buf[20];                         // ���Ը�����Ҫ��Ӹ�����ֶ�
    int length;                           // ���ݳ���
} mediaMessage_t;


#ifdef __cplusplus
}
#endif
#endif
