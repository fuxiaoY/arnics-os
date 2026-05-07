#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_ltdc.h"
#include "stm32f4xx_hal_rcc_ex.h"
#include <string.h>
#include <stdio.h>
#include "ltdc.h"
#include "fmc.h"
#include "drivers/bsp/bsp_def.h"

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_LTDC_CLK_ENABLE();                //使能LTDC时钟
    __HAL_RCC_DMA2D_CLK_ENABLE();               //使能DMA2D时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();               //使能GPIOB时钟
    __HAL_RCC_GPIOF_CLK_ENABLE();               //使能GPIOF时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();               //使能GPIOG时钟
    __HAL_RCC_GPIOH_CLK_ENABLE();               //使能GPIOH时钟
    __HAL_RCC_GPIOI_CLK_ENABLE();               //使能GPIOI时钟
    
    //初始化PB5，背光引脚
    GPIO_Initure.Pin        =   GPIO_PIN_5;                //PB5推挽输出，控制背光
    GPIO_Initure.Mode        =   GPIO_MODE_OUTPUT_PP;      //推挽输出
    GPIO_Initure.Pull        =   GPIO_PULLUP;              //上拉        
    GPIO_Initure.Speed        =   GPIO_SPEED_HIGH;         //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);   //背光开启
    
    //初始化PF10
    GPIO_Initure.Pin        =   GPIO_PIN_10; 
    GPIO_Initure.Mode        =   GPIO_MODE_AF_PP;          //复用
    GPIO_Initure.Pull        =   GPIO_NOPULL;              
    GPIO_Initure.Speed        =   GPIO_SPEED_HIGH;         //高速
    GPIO_Initure.Alternate        =   GPIO_AF14_LTDC;      //复用为LTDC
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    
    //初始化PG6,7,11,12
    GPIO_Initure.Pin        =   GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11|GPIO_PIN_12;
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
    
    //初始化PH9,10,11,12,13,14,15
    GPIO_Initure.Pin        =   GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    //初始化PI0,1,2,5,6,7,9,10
    GPIO_Initure.Pin        =   GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_5|\
                     GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI,&GPIO_Initure); 
}




/**
 * @brief  Configure LTDC pixel clock (PLLSAI) using HAL API.
 */
uint8_t LTDC_Clk_Set(uint32_t pllsain,uint32_t pllsair,uint32_t pllsaidivr)
{
    RCC_PeriphCLKInitTypeDef PeriphClkIniture;

    PeriphClkIniture.PeriphClockSelection           =   RCC_PERIPHCLK_LTDC;    //LTDC时钟     
    PeriphClkIniture.PLLSAI.PLLSAIN                 =   pllsain;    
    PeriphClkIniture.PLLSAI.PLLSAIR                 =   pllsair;  
    PeriphClkIniture.PLLSAIDivR                     =   pllsaidivr;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture) ==  HAL_OK)    //配置像素时钟
    {
        return 0;   //成功
    }
    else return 1;  //失败    
}

//LTDC,基本参数设置.
//注意:此函数,必须在LTDC_Layer_Window_Config之前设置.
//layerx:层值,0/1.
//bufaddr:层颜色帧缓存起始地址
//pixformat:颜色格式.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:层颜色Alpha值,0,全透明;255,不透明
//alpha0:默认颜色Alpha值,0,全透明;255,不透明
//bfac1:混合系数1,4(100),恒定的Alpha;6(101),像素Alpha*恒定Alpha
//bfac2:混合系数2,5(101),恒定的Alpha;7(111),像素Alpha*恒定Alpha
//bkcolor:层默认颜色,32位,低24位有效,RGB888格式
//返回值:无
void LTDC_Layer_Parameter_Config(LTDC_HandleTypeDef*  p_LTDC_Handler,
								 uint8_t layerx, 
                                 uint32_t bufaddr, 
                                 uint32_t pixformat, 
                                 uint8_t alpha, 
                                 uint8_t alpha0, 
                                 uint8_t bfac1, 
                                 uint8_t bfac2, 
                                 uint32_t bkcolor,
                                 uint32_t pwidth, 
                                 uint32_t pheight)
{
    LTDC_LayerCfgTypeDef pLayerCfg;
    
    pLayerCfg.WindowX0           =   0;                                   // 窗口起始X坐标
    pLayerCfg.WindowY0           =   0;                                   // 窗口起始Y坐标
    pLayerCfg.WindowX1           =   pwidth;                              // 窗口终止X坐标
    pLayerCfg.WindowY1           =   pheight;                             // 窗口终止Y坐标
    pLayerCfg.PixelFormat        =   pixformat;                           // 像素格式
    pLayerCfg.Alpha              =   alpha;                               // Alpha值设置，0~255,255为完全不透明
    pLayerCfg.Alpha0             =   alpha0;                              // 默认Alpha值
    pLayerCfg.BlendingFactor1    =   (uint32_t)bfac1 << 8;                     // 设置层混合系数
    pLayerCfg.BlendingFactor2    =   (uint32_t)bfac2 << 8;                     // 设置层混合系数
    pLayerCfg.FBStartAdress      =   bufaddr;                             // 设置层颜色帧缓存起始地址
    pLayerCfg.ImageWidth         =   pwidth;                              // 设置颜色帧缓冲区的宽度    
    pLayerCfg.ImageHeight        =   pheight;                             // 设置颜色帧缓冲区的高度
    pLayerCfg.Backcolor.Red      =   (uint8_t)(bkcolor & 0X00FF0000) >> 16;    // 背景颜色红色部分
    pLayerCfg.Backcolor.Green    =   (uint8_t)(bkcolor & 0X0000FF00) >> 8;     // 背景颜色绿色部分
    pLayerCfg.Backcolor.Blue     =   (uint8_t) bkcolor & 0X000000FF;           // 背景颜色蓝色部分
    
    HAL_LTDC_ConfigLayer(p_LTDC_Handler, &pLayerCfg, layerx);              // 设置所选中的层
}
//LTDC,层颜窗口设置,窗口以LCD面板坐标系为基准
//注意:此函数必须在LTDC_Layer_Parameter_Config之后再设置.
//layerx:层值,0/1.
//sx,sy:起始坐标
//width,height:宽度和高度
void LTDC_Layer_Window_Config(LTDC_HandleTypeDef*  p_LTDC_Handler,uint8_t layerx,uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
    HAL_LTDC_SetWindowPosition(p_LTDC_Handler,sx,sy,layerx);  //设置窗口的位置
    HAL_LTDC_SetWindowSize(p_LTDC_Handler,width,height,layerx);//设置窗口大小    
}




void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* hltdc)
{

    /* Reset and disable LTDC/DMA2D clocks */
    __HAL_RCC_LTDC_FORCE_RESET();
    __HAL_RCC_LTDC_RELEASE_RESET();
    __HAL_RCC_LTDC_CLK_DISABLE();
    __HAL_RCC_DMA2D_CLK_DISABLE();
    
    // 反初始化PB5，背光引脚
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5);
    
    // 反初始化PF10
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10);
    
    // 反初始化PG6,7,11,12
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11|GPIO_PIN_12);
    
    // 反初始化PH9,10,11,12,13,14,15
    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
                           GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
    
    // 反初始化PI0,1,2,5,6,7,9,10
    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_5|\
                           GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10);

}

int bsp_ltdc_init(ltdc_t *lcd_ltdc)
{
    LTDC_Clk_Set(lcd_ltdc->pllsain,lcd_ltdc->pllsair,lcd_ltdc->pllsaidivr);
 
    //LTDC配置
    lcd_ltdc->LTDC_Handler.Instance                 =   LTDC;
    lcd_ltdc->LTDC_Handler.Init.HSPolarity          =   lcd_ltdc->HSPolarity;                   // 水平同步极性
    lcd_ltdc->LTDC_Handler.Init.VSPolarity          =   lcd_ltdc->VSPolarity;                   // 垂直同步极性
    lcd_ltdc->LTDC_Handler.Init.DEPolarity          =   lcd_ltdc->DEPolarity;                   // 数据使能极性
    lcd_ltdc->LTDC_Handler.Init.PCPolarity          =   lcd_ltdc->PCPolarity;                   // 像素时钟极性
    lcd_ltdc->LTDC_Handler.Init.HorizontalSync      =   lcd_ltdc->ltdc_dev.hsw-1;               // 水平同步宽度
    lcd_ltdc->LTDC_Handler.Init.VerticalSync        =   lcd_ltdc->ltdc_dev.vsw-1;               // 垂直同步宽度
    lcd_ltdc->LTDC_Handler.Init.AccumulatedHBP      =   lcd_ltdc->ltdc_dev.hsw + lcd_ltdc->ltdc_dev.hbp - 1;    // 水平同步后沿宽度
    lcd_ltdc->LTDC_Handler.Init.AccumulatedVBP      =   lcd_ltdc->ltdc_dev.vsw + lcd_ltdc->ltdc_dev.vbp - 1;    // 垂直同步后沿高度
    lcd_ltdc->LTDC_Handler.Init.AccumulatedActiveW  =   lcd_ltdc->ltdc_dev.hsw + lcd_ltdc->ltdc_dev.hbp + lcd_ltdc->ltdc_dev.pwidth - 1;  // 有效宽度
    lcd_ltdc->LTDC_Handler.Init.AccumulatedActiveH  =   lcd_ltdc->ltdc_dev.vsw + lcd_ltdc->ltdc_dev.vbp + lcd_ltdc->ltdc_dev.pheight - 1; // 有效高度
    lcd_ltdc->LTDC_Handler.Init.TotalWidth          =   lcd_ltdc->ltdc_dev.hsw + lcd_ltdc->ltdc_dev.hbp + lcd_ltdc->ltdc_dev.pwidth + lcd_ltdc->ltdc_dev.hfp - 1;   // 总宽度
    lcd_ltdc->LTDC_Handler.Init.TotalHeigh          =   lcd_ltdc->ltdc_dev.vsw + lcd_ltdc->ltdc_dev.vbp + lcd_ltdc->ltdc_dev.pheight + lcd_ltdc->ltdc_dev.vfp - 1;  // 总高度
    lcd_ltdc->LTDC_Handler.Init.Backcolor.Red       =   lcd_ltdc->backcolor_red;               // 屏幕背景层红色部分
    lcd_ltdc->LTDC_Handler.Init.Backcolor.Green     =   lcd_ltdc->backcolor_green;             // 屏幕背景层绿色部分
    lcd_ltdc->LTDC_Handler.Init.Backcolor.Blue      =   lcd_ltdc->backcolor_blue;              // 屏幕背景色蓝色部分
    HAL_LTDC_Init(&(lcd_ltdc->LTDC_Handler));

    for(uint8_t i = 0; i < lcd_ltdc->ltdc_config_num; i++)
    {
        //层配置
        LTDC_Layer_Parameter_Config(&(lcd_ltdc->LTDC_Handler),
                                    lcd_ltdc->ltdc_configer[i].layerx,
                                    lcd_ltdc->ltdc_configer[i].bufaddr,
                                    lcd_ltdc->ltdc_configer[i].pixformat,
                                    lcd_ltdc->ltdc_configer[i].alpha,
                                    lcd_ltdc->ltdc_configer[i].alpha0,
                                    lcd_ltdc->ltdc_configer[i].bfac1,
                                    lcd_ltdc->ltdc_configer[i].bfac2,
                                    lcd_ltdc->ltdc_configer[i].bkcolor,
                                    lcd_ltdc->ltdc_dev.pwidth,
                                    lcd_ltdc->ltdc_dev.pheight
                                );//层参数配置
    }

    // LTDC_Layer_Window_Config(&(lcd_ltdc->LTDC_Handler),0,0,0,lcd_ltdc->ltdc_dev.pwidth,lcd_ltdc->ltdc_dev.pheight);    //层窗口配置,以LCD面板坐标系为基准,不要随便修改!
    return 0;
}

int bsp_ltdc_close(ltdc_t *lcd_ltdc)
{
    return HAL_LTDC_DeInit(&(lcd_ltdc->LTDC_Handler));
}
