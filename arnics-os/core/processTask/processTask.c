
#include "processTask.h"

static struct process *pLink = NULL;
static struct process *curLink = NULL;

static bool process_FoundSame(struct process *handle)
{
    struct process *cur = pLink;
    for( ; (handle != cur) && (NULL != cur); )
    {
        if(NULL != cur->next)
        {
            cur = cur->next;
        }
        else
        {
            break;
        }
    }
    if(handle == cur)
    {
        return true;
    }
    return false;
}

void PROCESS_INIT(void)
{
    pLink = NULL;
    curLink = NULL;
}

bool PROCESS_ADD(struct process *handle)
{
    struct process **cur = &pLink;
    if(NULL == handle)
    {
        return false;
    }
    if(true == process_FoundSame(handle))
    {
        return true;
    }
    for( ; NULL != (*cur) ; cur = &((*cur)->next)){}
    (*cur) = handle;
    (*cur)->init();
    (*cur)->next = NULL;
    return true;
}


bool PROCESS_REMOVE(struct process *handle)
{
    struct process *cur = pLink;
    if(NULL == handle)
    {
        return false;
    }
    if(handle == cur)
    {
        pLink = cur->next;
        return true;
    }
    for( ; handle != cur->next; )
    {
        if(NULL != cur->next->next)
        {
            cur = cur->next;
        }
    }
    if(handle == cur->next)
    {
        cur->next = cur->next->next;
        return true;
    }
    else
    {
        return false;
    }
}


void PROCESS_TASK(uint16_t id, void *para,void* argv)
{
    for(curLink = pLink; NULL != curLink; curLink = curLink->next)
    {
        curLink->fun(id, para, argv);
    }
}


