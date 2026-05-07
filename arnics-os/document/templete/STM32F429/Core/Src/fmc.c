#include "fmc.h"
#include "port/delay.h"


void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    GPIO_Initure.Alternate = GPIO_AF12_FMC;

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3; // PC0, PC2, PC3
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15; // PD0,1,8,9,10,14,15
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; // PE0..PE15(部分)
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; // PF0,1,2,3,4,5,11..15
    HAL_GPIO_Init(GPIOF, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15; // PG0,1,2,4,5,8,15
    HAL_GPIO_Init(GPIOG, &GPIO_Initure);
}
void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef *hsdram)
{
    // 关闭FMC时钟
    __HAL_RCC_FMC_CLK_DISABLE();
    
    // 关闭相关GPIO时钟
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    __HAL_RCC_GPIOE_CLK_DISABLE();
    __HAL_RCC_GPIOF_CLK_DISABLE();
    __HAL_RCC_GPIOG_CLK_DISABLE();
    
    // 复位FMC模块
    __HAL_RCC_FMC_FORCE_RESET();
    __HAL_RCC_FMC_RELEASE_RESET();
    
    // 反初始化各个GPIO端口的SDRAM相关引脚
    
    // 反初始化GPIOC的引脚
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);
    
    // 反初始化GPIOD的引脚
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | 
                    GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15);
    
    // 反初始化GPIOE的引脚
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | 
                    GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | 
                    GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    
    // 反初始化GPIOF的引脚
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | 
                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | 
                    GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    
    // 反初始化GPIOG的引脚
    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | 
                    GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15);
}
// 向 SDRAM 发送命令
// bankx: 0 -> BANK5, 1 -> BANK6
static uint8_t SDRAM_Send_Cmd(SDRAM_HandleTypeDef *hsdram,
                              uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval)
{
    uint32_t target_bank = 0;
    FMC_SDRAM_CommandTypeDef Command;

    if (bankx == 0)
    {
        target_bank      = FMC_SDRAM_CMD_TARGET_BANK1;
    }

    else if (bankx == 1) 
    {
        target_bank      = FMC_SDRAM_CMD_TARGET_BANK2;
    }
    Command.CommandMode            = cmd;
    Command.CommandTarget          = target_bank;
    Command.AutoRefreshNumber      = refresh;
    Command.ModeRegisterDefinition = regval;

    if (HAL_SDRAM_SendCommand(hsdram, &Command, HAL_MAX_DELAY) == HAL_OK)
    {
        return 0;
    }
    else 
    {
        return 1;
    }

}

// 发送 SDRAM 初始化序列
static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
    uint32_t temp = 0;

    // 时钟配置使能
    SDRAM_Send_Cmd(hsdram, 0, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0);
    Delay_us(500); // 至少 200us

    // 预充电所有存储区
    SDRAM_Send_Cmd(hsdram, 0, FMC_SDRAM_CMD_PALL, 1, 0);

    // 设置自刷新次数（通常为 8 次）
    SDRAM_Send_Cmd(hsdram, 0, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0);

    // 配置模式寄存器: 突发长度/类型/CAS/写突发等
    temp = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1        |
           SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL           |
           SDRAM_MODEREG_CAS_LATENCY_3                   |
           SDRAM_MODEREG_OPERATING_MODE_STANDARD         |
           SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    SDRAM_Send_Cmd(hsdram, 0, FMC_SDRAM_CMD_LOAD_MODE, 1, (uint16_t)temp);
}

int bsp_sdram_init(sdram_t *sdram)
{
    
    if(HAL_OK != HAL_SDRAM_Init(&sdram->SDRAM_Handler, &sdram->SDRAM_Timing))
    {
        return -1;
    }

    SDRAM_Initialization_Sequence(&sdram->SDRAM_Handler);

    // 设置刷新频率计数器 (以 SDCLK 频率计数)
    // 例: 刷新周期 64ms, SDCLK = 180/2 = 90MHz, 行数 = 8192 -> COUNT ≈ 683
    //     COUNT = (刷新周期_us × SDCLK_MHz) / 行数 - 20
    //     COUNT = (64000 × 90) / 8192 - 20
    //     COUNT = 5760000 / 8192 - 20
    //     COUNT = 703.125 - 20
    //     COUNT ≈ 683
    // 动态计算并设置SDRAM刷新率
    uint32_t sdram_clock_mhz = SystemCoreClock / 2 / 1000000;        // SDCLK频率MHz
    uint32_t refresh_count   = (64000 * sdram_clock_mhz) / 8192 - 20;  // 64ms刷新周期

    if(HAL_OK != HAL_SDRAM_ProgramRefreshRate(&sdram->SDRAM_Handler, refresh_count))
    {
        return -1;
    }
    return 0;
}


int bsp_sdram_close(sdram_t *sdram)
{
    return HAL_SDRAM_DeInit(&sdram->SDRAM_Handler);
}

