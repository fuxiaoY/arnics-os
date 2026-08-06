#include "rtosInterface/rtosInterface.h"
#include "rtosInterface/entry_rtos_api.h"
#include "common/TaskTimer.h"
#include "thirdParty/uflog/uflog.h"
#include <process.h>
#include <stdarg.h>
#include <windows.h>

#undef rtosThreadDelay
#undef rtosTaskCreate
#undef rtosTaskSelfDelete
#undef rtosEventosGetMsg
#undef rtosEventosSendMsg
#undef rtosTakeMsgFromEventos
#undef rtosDeliverMsgToEventos
#undef TakeEventosMsgQueueMutex
#undef ReleaseEventosMsgQueueMutex
#undef TakeEventosMutex
#undef ReleaseEventosMutex
#undef CheckEventRspMesgNum
#undef PeekEventRspMesg
#undef CheckEventQueueSpacesAvailable

typedef void (*test_task_fn_t)(void *);
typedef struct { test_task_fn_t fn; void *arg; } test_task_start_t;

static unsigned __stdcall test_task_entry(void *argument)
{
    test_task_start_t *start = (test_task_start_t *)argument;
    test_task_fn_t fn = start->fn;
    void *arg = start->arg;
    free(start);
    fn(arg);
    return 0u;
}

static void test_thread_delay(uint32_t ms)
{
    Sleep((DWORD)ms);
}

static void test_task_create(char *name, rtosPriority_e priority, void *func, uint32_t stack_size, void *arg)
{
    (void)name;
    (void)priority;
    test_task_start_t *start = (test_task_start_t *)malloc(sizeof(*start));
    if (start == NULL || func == NULL)
    {
        free(start);
        return;
    }
    start->fn = (test_task_fn_t)func;
    start->arg = arg;
    uintptr_t thread = _beginthreadex(NULL, stack_size, test_task_entry, start, 0u, NULL);
    if (thread == 0u)
    {
        free(start);
        return;
    }
    CloseHandle((HANDLE)thread);
}

static void test_task_self_delete(void)
{
    _endthreadex(0u);
}

uint32_t arnics_getTick(void)
{
    return (uint32_t)GetTickCount64();
}

uflog_t *uflog_default_p = NULL;
void uflog_log(uflog_t *logger, uflog_pri_e level, const char *facility,
    const char *kind, uint8_t *data, size_t len, bool force_store,
    const char *func, const char *file, int line, const char *fmt, ...)
{
    (void)logger; (void)level; (void)facility; (void)kind; (void)data; (void)len;
    (void)force_store; (void)func; (void)file; (void)line; (void)fmt;
}

const tRTOSEntry entry_rtos_list = {
    .t_rtosThreadDelay = test_thread_delay,
    .t_rtosTaskCreate = test_task_create,
    .t_rtosTaskSelfDelete = test_task_self_delete,
    .t_rtosEventosGetMsg = rtosEventosGetMsg,
    .t_rtosEventosSendMsg = rtosEventosSendMsg,
    .t_rtosTakeMsgFromEventos = rtosTakeMsgFromEventos,
    .t_rtosDeliverMsgToEventos = rtosDeliverMsgToEventos,
    .t_TakeEventosMsgQueueMutex = TakeEventosMsgQueueMutex,
    .t_ReleaseEventosMsgQueueMutex = ReleaseEventosMsgQueueMutex,
    .t_TakeEventosMutex = TakeEventosMutex,
    .t_ReleaseEventosMutex = ReleaseEventosMutex,
    .t_CheckEventRspMesgNum = CheckEventRspMesgNum,
    .t_PeekEventRspMesg = PeekEventRspMesg,
    .t_CheckEventQueueSpacesAvailable = CheckEventQueueSpacesAvailable,
};
