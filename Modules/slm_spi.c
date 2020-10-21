/*
*************************************************************
 Copyright (C), 2016-2020 smartlinkMicro. Co., Ltd.  All rights reserved.
 Filename:
 Complier:    ARM-MDK 5.1.2
 Target OS:   UCOS-ii
 Target HW:  athena
 Author:        htyi
 Version :     V1.0.0
 Date:          2016.4.16
 Description:
 History:
     <author>     <time>     <version >   <desc>

*************************************************************
*/
#include "stm32f10x.h"

void TIM4_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM4_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	
	TIM_DeInit(TIM4);
	
	TIM_TimeBaseStructure.TIM_Period = 65535;						//用于捕获模式时，TIM_Period的值没有意义
	TIM_TimeBaseStructure.TIM_Prescaler = 7199; 					//7199，计数10000次时间为1s，那么计时为10ms的计数值应为99
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);							//不清除，要进一次中断
	
	TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM4_OCInitStructure.TIM_Pulse = 0;
	TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM4_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);

	TIM_ClearFlag(TIM4,TIM_FLAG_CC1);
    TIM_ARRPreloadConfig(TIM4,DISABLE);
	
	TIM_Cmd(TIM4,DISABLE);
}

/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/
/**************************************************************************SL1102发送 IO配置***********************************************************************/
/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/




/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/
/**************************************************************************SL1102接收 IO配置***********************************************************************/
/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/
/**************************************************
Function: SL1102RX_Init();
Description:
	SL1102 TX chip Init
		GPIO/SPI/REGcfg
**************************************************/
void SL1102RX_Init(void)
{
	volatile UINT32_T i;
	
	log_title("初始化SL1102");
	TIM4_Init();
	USER_Delay1ms(15);
	
	SL1102RX_GPIO_Init();
	SL1102RX_SPI_Init();
	SLRX_CE_LOW;
	USER_Delay1ms(15);
	SLRX_CE_HIGH;
	USER_Delay1ms(15);
	SL1102RX_chip_init();
	SLRXrxtx_switch(RX_EN);
}
/**************************************************
Function: SL1102RX_GPIO_Init();
Description:
	SL1102 TX MCU GPIO Init
		SLRX CE;chip ce pin
		SLRX PWR;chip power contorl pin
**************************************************/
void SL1102RX_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//cfg SLRX_CE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SLRX_CE_HIGH;
	

}
/**************************************************
Function: SL1102RX_SPI_Init();
Description:
	SL1102 TX MCU SPI Init
		SCN CLK MOSI MISO cfg
**************************************************/
void  SL1102RX_SPI_Init(void)
{  	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);
	
	//PC9-->SCN	PC8-->CLK	PA8-->MOSI	输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//PC7-->MISO	输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	SLRX_CSN_HIGH;//CSN=1;			// Spi disable
	SLRX_SCK_LOW;//SCK=0;			// Spi clock line init high
}

/**************************************************
Function: u8 SL1102RX_SPI_RW(u8 _UINT8_T);
Description:
	send and read data from SLRX chip
Parameter:
	_UINT8_T;send data
Return:
	u8;read data
**************************************************/
void Sl1102_Delay1us(uint16_t ms_count)
{
	uint16_t i,j;
	
	for(i = 0; i < ms_count; i++)
	{
		for(j = 0; j < 15; j++)
		{
			__nop();
			__nop();
		}
	}
}

u8 SL1102RX_SPI_RW(u8 _UINT8)
{
    u8 bit_ctr;
    for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // output 8-bit
    {
			SLRX_SCK_LOW;
        if(_UINT8 & 0x80)
            SLRX_MOSI_HIGH ;         // output 'UINT8', MSB to MOSI
        else
            SLRX_MOSI_LOW ;         // output 'UINT8', MSB to MOSI

        _UINT8 = (_UINT8 << 1);           // shift next bit into MSB..
        SLRX_SCK_HIGH;                      // Set SCK high..
        _UINT8 |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);                 // capture current MISO bit
        SLRX_SCK_LOW;                      // ..then set SCK low again
    }

    return(_UINT8);                     // return read UINT8
}
/**************************************************
Function: SL1102RX_SPI_Read(u8 reg, u8 *pBuf, u8 _UINT8_T);
Description:
	read _UINT8_T num datas(pBuf) from reg register
Parameter:
	reg;read data from reg
	pBuf;will read data
	_UINT8_T;the number of data will be read
**************************************************/
void SL1102RX_SPI_Read(u8 reg, u8 *pBuf, INT32U _UINT8)
{
	u8 UINT8_ctr;

	SLRX_CSN_LOW;                            // Set CSN low, init SPI tranaction
	SL1102RX_SPI_RW(reg);               // Select register to write to and read status UINT8
	for(UINT8_ctr=0;UINT8_ctr<_UINT8;UINT8_ctr++)
			pBuf[UINT8_ctr] = SL1102RX_SPI_RW(0);    // Perform SPI_RW to read UINT8 from BK5822
	SLRX_CSN_HIGH;                           // Set CSN high again
//	return;
}

/**************************************************
Function: SL1102RX_SPI_Write(u8 reg, u8 *pBuf, u8 _UINT8_T);
Description:
	write _UINT8_T num datas(pBuf) to reg register
Parameter:
	reg;write data to reg
	pBuf;will write data
	_UINT8_T;the number of data will be write
**************************************************/

void SL1102RX_SPI_Write(u8 reg, u8 *pBuf, INT32U _UINT8)
{
    u8 UINT8_ctr;
    SLRX_CSN_LOW;                   								// Set CSN low, init SPI tranaction
    SL1102RX_SPI_RW(reg);    									// Select register to write to and read status UINT8
    for(UINT8_ctr=0; UINT8_ctr<_UINT8; UINT8_ctr++) // then write all UINT8 in buffer(*pBuf)
    {
        SL1102RX_SPI_RW(*pBuf++);
    }
    SLRX_CSN_HIGH;                 // Set CSN high again
//    return;
}	
/**************************************************
Function: SL1102RX_ReadSR(u8 sr);
Description:
	read register(sr)
**************************************************/

u8 SL1102RX_ReadSR(u8 sr)
{
    u8 byte = 0;
    SLRX_CSN_LOW;                            //使能器件
	
    SL1102RX_SPI_RW(sr & 0x7F);          //发送读取状态寄存器命令
    byte = SL1102RX_SPI_RW(0Xff);        //读取一个字节

    SLRX_CSN_HIGH;                           //取消片选
    return byte;
}
/**************************************************
Function: SL1102RX_WriteSR(u8 sr, u8 value);
Description:
	wirte value(value) to register(sr)
**************************************************/
void SL1102RX_WriteSR(u8 sr, u8 value)
{
    SLRX_CSN_LOW;
	SLdelay_us();SLdelay_us();SLdelay_us();SLdelay_us();SLdelay_us();
    //使能器件
    SL1102RX_SPI_RW(sr | 0x80);          //发送读取状态寄存器命令
    SL1102RX_SPI_RW(value);
	SLdelay_us();SLdelay_us();SLdelay_us();SLdelay_us();SLdelay_us();
    SLRX_CSN_HIGH;                           //取消片选

}


//endfile

