#include "stm32f10x.h"



void SoftReset(void)
{
	__set_FAULTMASK(1);      // �ر������ж�
	NVIC_SystemReset();		 // ��λ
}



/*******************************************************************************
*
*����ԭ�ͣ�USER_NVIC_Config();
*��ڲ�������
*���ڲ�������
*�������ܣ��ж�����
*
*******************************************************************************/
void USER_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;   
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

