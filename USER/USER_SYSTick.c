#include "stm32f10x.h"

uint16_t TickCount;
uint8_t SysTickCount;
uint8_t UnixTimeCount;

/**********************************CPU CLK相关***********************************/
void USER_CpuClk_Init(void)
{
	DEM_CR |= DEM_CR_TRCENA;    		// 开启DWT
//    DWT_CYCCNT = 0;						// 清计数器
//    DWT_CR |= DWT_CR_CYCCNTENA;			// 开启CPU计数器
}

void USER_CpuClk_Start(void)
{
	DWT_CR &= ~DWT_CR_CYCCNTENA;		// 关闭CPU计数器
    DWT_CYCCNT = 0;						// 清计数器
    DWT_CR |= DWT_CR_CYCCNTENA;			// 开启CPU计数器
}

void USER_CpuClk_Stop(void)
{
	DWT_CR &= ~DWT_CR_CYCCNTENA;		// 关闭CPU计数器
    DWT_CYCCNT = 0;						// 清计数器
    DEM_CR &= ~DEM_CR_TRCENA;    		// 关闭DWT
}

uint32_t USER_CpuClk_Read(void)
{
	return(DWT_CYCCNT);
}



/*******************************************************************************
*
*函数原型：USER_SysTick_Init();
*入口参数：无
*出口参数：无
*函数功能：系统滴答定时器初始化
*		5 * 9000为5ms
*******************************************************************************/
void USER_SysTick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   			//开启SysTick中断
	SysTick->LOAD = 100 * 9000; 							//5ms进入一次中断   
}


/*******************************************************************************
*
*函数原型：USER_SysTick_Start();
*入口参数：无
*出口参数：无
*函数功能：开启系统滴答定时器，定时时间即为重装载里的值45000/9=5000us
*
*******************************************************************************/
void USER_SysTick_Start(void)
{
	SysTick->VAL = 0x00;	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   	//开启SysTick 
}


/*******************************************************************************
*
*函数原型：USER_SysTick_Stop();
*入口参数：无
*出口参数：无
*函数功能：关闭系统滴答定时器
*
*******************************************************************************/
void USER_SysTick_Stop(void)
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   	//关闭SysTick
//	SysTick->VAL = 0x00;
}


/*******************************************************************************
*
*函数原型：USER_Delay1ms(uint16_t ms_count);
*入口参数：ms_count，ms数值
*出口参数：无
*函数功能：延时ms_count个1ms
*
*******************************************************************************/
void USER_Delay1ms(uint16_t ms_count)
{
	uint16_t i,j;
	
	for(i = 0; i < ms_count; i++)
	{
		for(j = 0; j < 7200; j++)
		{
			__nop();
			__nop();
		}
	}
}

