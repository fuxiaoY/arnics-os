#include "dma2d.h"




void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef* dma2dHandle)
{

    if(dma2dHandle->Instance==DMA2D)
    {
      __HAL_RCC_DMA2D_CLK_ENABLE();
    }
}
void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef* dma2dHandle)
{

    if(dma2dHandle->Instance==DMA2D)
    {
      __HAL_RCC_DMA2D_CLK_DISABLE();
    }
}

/* DMA2D 初始化函数 */
int bsp_dma2d_init(dma2d_t *dma2d)
{
    uint8_t layer_num = dma2d->layer_num;

    if(HAL_DMA2D_Init(&dma2d->hdma2d) != HAL_OK)
    {
      return -1;
    }

    for(uint8_t i = 0; i < layer_num; i++)
    {
      if(HAL_DMA2D_ConfigLayer(&dma2d->hdma2d, i) != HAL_OK)
      {
        return -2;
      }
    }
    return 0;
}


int bsp_dma2d_close(dma2d_t *dma2d)
{
  if(HAL_DMA2D_DeInit(&dma2d->hdma2d) != HAL_OK)
  {
    return -1;
  }
  return 0;
}
/**
  * @brief  使用DMA2D填充指定颜色的区域
  * @param  xPos: 目标缓冲区中的X坐标
  * @param  yPos: 目标缓冲区中的Y坐标
  * @param  xSize: 要填充区域的宽度
  * @param  ySize: 要填充区域的高度
  * @param  color: 用于填充的颜色值
  * 
  * @note  需要配置DMA2D为寄存器到内存模式 
  *        hdma2d.Init.Mode = DMA2D_R2M;
  */
int bsp_dma2d_fill_buffer(dma2d_t *dma2d,
                        uint32_t xPos, uint32_t yPos,
                        uint32_t xSize, uint32_t ySize, 
                        uint32_t color,
                        size_t Timeout)
{
    uint16_t *pDst = dma2d->dst_addr;

    dma2d->hdma2d.Init.Mode = DMA2D_R2M;
    dma2d->hdma2d.Init.OutputOffset = dma2d->width - xSize;
    if(HAL_DMA2D_Init(&dma2d->hdma2d) != HAL_OK)
    {
      return -1;
    }
    
    // 将16位RGB565转换为32位ARGB8888
    uint32_t color32 = ((uint32_t)((color & 0xF800) >> 11) << 19) |   // R
                       ((uint32_t)((color & 0x07E0) >> 5) << 10)  |   // G
                       ((uint32_t)(color & 0x001F) << 3);             // B

    /* 计算输出地址 */
    uint32_t output_address = (uint32_t)pDst + (yPos * dma2d->width + xPos) * dma2d->pixel_size;
    /* 开始填充操作 */
    if(HAL_DMA2D_Start(&dma2d->hdma2d, color32, output_address, xSize, ySize) != HAL_OK)
    {
      return -1;
    }
    /* 等待传输完成 */
    if(HAL_DMA2D_PollForTransfer(&dma2d->hdma2d, Timeout) != HAL_OK)
    {
      return -1;
    }
    return 0;
}

/**
  * @brief  使用DMA2D复制内存区域
  * @param  pSrc: 源缓冲区地址
  * @param  pDst: 目标缓冲区地址
  * @param  xSize: 要复制区域的宽度
  * @param  ySize: 要复制区域的高度
  * @retval 无
  */
/**
  * @brief  使用DMA2D复制内存区域
  * @param  pSrc: 源缓冲区地址
  * @param  pDst: 目标缓冲区地址
  * @param  xSize: 要复制区域的宽度
  * @param  ySize: 要复制区域的高度
  * @retval 无
  */
 int bsp_dma2d_copy_buffer(dma2d_t *dma2d,
                          uint32_t *pSrc, 
                          uint32_t xPos,  uint32_t yPos,
                          uint32_t xSize, uint32_t ySize,
                          size_t Timeout)
{
    uint16_t *pDst = dma2d->dst_addr;

    dma2d->hdma2d.Init.Mode = DMA2D_M2M;
    dma2d->hdma2d.Init.OutputOffset = dma2d->width - xSize;


    if(HAL_DMA2D_Init(&dma2d->hdma2d) != HAL_OK)
    {
      return -1;
    }

    uint8_t layer_num = dma2d->layer_num;
    for(uint8_t i = 0; i < layer_num; i++)
    {
      dma2d->hdma2d.LayerCfg[i].InputOffset = dma2d->width - xSize;
      if(HAL_DMA2D_ConfigLayer(&dma2d->hdma2d, i) != HAL_OK)
      {
        return -2;
      }
    }
    uint32_t output_address = (uint32_t)pDst + (yPos * dma2d->width + xPos) * dma2d->pixel_size;
    uint32_t src_address    = (uint32_t)pSrc + (yPos * dma2d->width + xPos) * dma2d->pixel_size;
    /* 开始复制操作 */
    if(HAL_DMA2D_Start(&dma2d->hdma2d, (uint32_t)src_address, output_address, xSize, ySize) != HAL_OK)
    {
      return -1;
    }

    /* 等待传输完成 */
    if(HAL_DMA2D_PollForTransfer(&dma2d->hdma2d, Timeout) != HAL_OK)
    {
      return -1;
    }
    return 0;
}

/**
  * @brief  使用DMA2D复制内存区域
  * @param  pSrc: 源缓冲区地址
  * @param  pDst: 目标缓冲区地址
  * @param  xSize: 要复制区域的宽度
  * @param  ySize: 要复制区域的高度
  * @retval 无
  */
int bsp_dma2d_free_copy_buffer(dma2d_t *dma2d,
                      uint32_t *pSrc, 
                      uint32_t xPos,  uint32_t yPos,
                      uint32_t xSize, uint32_t ySize,
                      size_t Timeout)
{

    uint16_t *pDst = dma2d->dst_addr;

    dma2d->hdma2d.Init.Mode = DMA2D_M2M;
    dma2d->hdma2d.Init.OutputOffset = dma2d->width - xSize;


    if(HAL_DMA2D_Init(&dma2d->hdma2d) != HAL_OK)
    {
      return -1;
    }

    uint8_t layer_num = dma2d->layer_num;
    for(uint8_t i = 0; i < layer_num; i++)
    {
      dma2d->hdma2d.LayerCfg[i].InputOffset =0;
      if(HAL_DMA2D_ConfigLayer(&dma2d->hdma2d, i) != HAL_OK)
      {
        return -2;
      }
    }
    uint32_t output_address = (uint32_t)pDst + (yPos * dma2d->width + xPos) * dma2d->pixel_size;
    /* 开始复制操作 */
    if(HAL_DMA2D_Start(&dma2d->hdma2d, (uint32_t)pSrc, output_address, xSize, ySize) != HAL_OK)
    {
      return -1;
    }

    /* 等待传输完成 */
    if(HAL_DMA2D_PollForTransfer(&dma2d->hdma2d, Timeout) != HAL_OK)
    {
      return -1;
    }
    return 0;
}

int bsp_dma2d_blending(dma2d_t *dma2d,
                      uint16_t *background_buffer, 
                      uint16_t *foreground_buffer, 
                      uint32_t xPos,  uint32_t yPos,
                      uint32_t xSize, uint32_t ySize,
                      size_t Timeout)
{

    dma2d->hdma2d.Init.Mode = DMA2D_M2M_BLEND;
    dma2d->hdma2d.Init.OutputOffset = dma2d->width - xSize;

    if(HAL_DMA2D_Init(&dma2d->hdma2d) != HAL_OK)
    {
      return -1;
    }
    uint8_t layer_num = dma2d->layer_num;
    for(uint8_t i = 0; i < layer_num; i++)
    {
      dma2d->hdma2d.LayerCfg[i].InputOffset = dma2d->width - xSize;
      if(HAL_DMA2D_ConfigLayer(&dma2d->hdma2d, i) != HAL_OK)
      {
        return -2;
      }
    }

    uint32_t offset_address = (yPos * dma2d->width + xPos) * dma2d->pixel_size;
    // 启动混合操作
    HAL_DMA2D_BlendingStart(&dma2d->hdma2d, 
                            (uint32_t)background_buffer + offset_address,  // 背景层
                            (uint32_t)foreground_buffer + offset_address,  // 前景层
                            (uint32_t)dma2d->dst_addr   + offset_address,  // 输出地址
                            xSize,                                         // 宽度
                            ySize);                                        // 高度

    // 等待传输完成
    while(HAL_DMA2D_PollForTransfer(&dma2d->hdma2d, Timeout) != HAL_OK)
    {
        return -1;
    }
    return 0;
}
