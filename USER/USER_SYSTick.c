#include "stm32f10x.h"

uint16_t TickCount;
uint8_t SysTickCount;
uint8_t UnixTimeCount;

/**********************************CPU CLK���***********************************/
void USER_CpuClk_Init(void)
{
	DEM_CR |= DEM_CR_TRCENA;    		// ����DWT
//    DWT_CYCCNT = 0;						// �������
//    DWT_CR |= DWT_CR_CYCCNTENA;			// ����CPU������
}

void USER_CpuClk_Start(void)
{
	DWT_CR &= ~DWT_CR_CYCCNTENA;		// �ر�CPU������
    DWT_CYCCNT = 0;						// �������
    DWT_CR |= DWT_CR_CYCCNTENA;			// ����CPU������
}

void USER_CpuClk_Stop(void)
{
	DWT_CR &= ~DWT_CR_CYCCNTENA;		// �ر�CPU������
    DWT_CYCCNT = 0;						// �������
    DEM_CR &= ~DEM_CR_TRCENA;    		// �ر�DWT
}

uint32_t USER_CpuClk_Read(void)
{
	return(DWT_CYCCNT);
}



/*******************************************************************************
*
*����ԭ�ͣ�USER_SysTick_Init();
*��ڲ�������
*���ڲ�������
*�������ܣ�ϵͳ�δ�ʱ����ʼ��
*		5 * 9000Ϊ5ms
*******************************************************************************/
void USER_SysTick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   			//����SysTick�ж�
	SysTick->LOAD = 100 * 9000; 							//5ms����һ���ж�   
}


/*******************************************************************************
*
*����ԭ�ͣ�USER_SysTick_Start();
*��ڲ�������
*���ڲ�������
*�������ܣ�����ϵͳ�δ�ʱ������ʱʱ�伴Ϊ��װ�����ֵ45000/9=5000us
*
*******************************************************************************/
void USER_SysTick_Start(void)
{
	SysTick->VAL = 0x00;	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   	//����SysTick 
}


/*******************************************************************************
*
*����ԭ�ͣ�USER_SysTick_Stop();
*��ڲ�������
*���ڲ�������
*�������ܣ��ر�ϵͳ�δ�ʱ��
*
*******************************************************************************/
void USER_SysTick_Stop(void)
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   	//�ر�SysTick
//	SysTick->VAL = 0x00;
}


/*******************************************************************************
*
*����ԭ�ͣ�USER_Delay1ms(uint16_t ms_count);
*��ڲ�����ms_count��ms��ֵ
*���ڲ�������
*�������ܣ���ʱms_count��1ms
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

