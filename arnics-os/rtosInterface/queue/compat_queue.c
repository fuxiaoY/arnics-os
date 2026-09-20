/**
 * @file    compat_queue.c
 * @brief   消息队列统一收发 API。
 *
 * @copyright (c) 2026 arnics-os. Licensed under the project LICENSE.
 */

#include "rtosInterface/rtosInterface.h"
#include "rtosInterface/queue/queue_port.h"

/**
 * @brief 查询队列是否有空闲槽位。
 * @param id 队列 ID
 * @return true 有空闲；false 已满或无效
 */
static bool queue_spaces_available(queue_id_t id)
{
    const queue_t *q = queue_get(id);
    if (!q || !q->ops || !q->ops->count) return false;
    const uint32_t count = q->ops->count(q->handle);
    return count < q->len;
}

bool rtosAdGetMsg(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_AD_REQ);
    if (!q || !q->ops || !q->ops->recv) return false;
    return q->ops->recv(q->handle, msg, delay);
}

bool rtosAdSendMsg(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_AD_RSP);
    if (!q || !q->ops || !q->ops->send) return false;
    return q->ops->send(q->handle, msg, delay);
}

bool rtosTakeMsgFromAd(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_AD_RSP);
    if (!q || !q->ops || !q->ops->recv) return false;
    return q->ops->recv(q->handle, msg, delay);
}

bool rtosDeliverMsgToAd(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_AD_REQ);
    if (!q || !q->ops || !q->ops->send) return false;
    return q->ops->send(q->handle, msg, delay);
}

uint32_t CheckAdRspMesgNum(void)
{
    const queue_t *q = queue_get(QUEUE_ID_AD_RSP);
    if (!q || !q->ops || !q->ops->count) return 0u;
    return q->ops->count(q->handle);
}

uint32_t CheckAdReqMesgNum(void)
{
    const queue_t *q = queue_get(QUEUE_ID_AD_REQ);
    if (!q || !q->ops || !q->ops->count) return 0u;
    return q->ops->count(q->handle);
}

bool PeekAdRspMesg(void *receivedMsg)
{
    const queue_t *q = queue_get(QUEUE_ID_AD_RSP);
    if (!q || !q->ops || !q->ops->peek) return false;
    return q->ops->peek(q->handle, receivedMsg);
}

bool CheckAdQueueSpacesAvailable(void)
{
    return queue_spaces_available(QUEUE_ID_AD_REQ);
}

bool rtosEventosGetMsg(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_EVENTOS_REQ);
    if (!q || !q->ops || !q->ops->recv) return false;
    return q->ops->recv(q->handle, msg, delay);
}

bool rtosEventosSendMsg(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_EVENTOS_RSP);
    if (!q || !q->ops || !q->ops->send) return false;
    return q->ops->send(q->handle, msg, delay);
}

bool rtosTakeMsgFromEventos(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_EVENTOS_RSP);
    if (!q || !q->ops || !q->ops->recv) return false;
    return q->ops->recv(q->handle, msg, delay);
}

bool rtosDeliverMsgToEventos(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_EVENTOS_REQ);
    if (!q || !q->ops || !q->ops->send) return false;
    return q->ops->send(q->handle, msg, delay);
}

uint32_t CheckEventRspMesgNum(void)
{
    const queue_t *q = queue_get(QUEUE_ID_EVENTOS_RSP);
    if (!q || !q->ops || !q->ops->count) return 0u;
    return q->ops->count(q->handle);
}

bool PeekEventRspMesg(void *receivedMsg)
{
    const queue_t *q = queue_get(QUEUE_ID_EVENTOS_RSP);
    if (!q || !q->ops || !q->ops->peek) return false;
    return q->ops->peek(q->handle, receivedMsg);
}

bool CheckEventQueueSpacesAvailable(void)
{
    return queue_spaces_available(QUEUE_ID_EVENTOS_REQ);
}

bool rtosMediaGetMsg(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_MEDIA_REQ);
    if (!q || !q->ops || !q->ops->recv) return false;
    return q->ops->recv(q->handle, msg, delay);
}

bool rtosMediaSendMsg(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_MEDIA_RSP);
    if (!q || !q->ops || !q->ops->send) return false;
    return q->ops->send(q->handle, msg, delay);
}

bool rtosTakeMsgFromMedia(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_MEDIA_RSP);
    if (!q || !q->ops || !q->ops->recv) return false;
    return q->ops->recv(q->handle, msg, delay);
}

bool rtosDeliverMsgToMedia(void *msg, uint32_t delay)
{
    const queue_t *q = queue_get(QUEUE_ID_MEDIA_REQ);
    if (!q || !q->ops || !q->ops->send) return false;
    return q->ops->send(q->handle, msg, delay);
}

uint32_t CheckMediaRspMesgNum(void)
{
    const queue_t *q = queue_get(QUEUE_ID_MEDIA_RSP);
    if (!q || !q->ops || !q->ops->count) return 0u;
    return q->ops->count(q->handle);
}

uint32_t CheckMediaReqMesgNum(void)
{
    const queue_t *q = queue_get(QUEUE_ID_MEDIA_REQ);
    if (!q || !q->ops || !q->ops->count) return 0u;
    return q->ops->count(q->handle);
}

bool PeekMediaRspMesg(void *receivedMsg)
{
    const queue_t *q = queue_get(QUEUE_ID_MEDIA_RSP);
    if (!q || !q->ops || !q->ops->peek) return false;
    return q->ops->peek(q->handle, receivedMsg);
}

bool CheckMediaQueueSpacesAvailable(void)
{
    return queue_spaces_available(QUEUE_ID_MEDIA_REQ);
}
