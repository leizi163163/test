/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "shell_port.h"

/*******************************************************************************
*
*����ԭ�ͣ�USER_Variable_Init();
*��ڲ�������
*���ڲ�������
*�������ܣ�������ʼ��
*
*******************************************************************************/
void USER_Variable_Init(void)
{
	RX_RingBuf_Init();
}

/*******************************************************************************
*
*����ԭ�ͣ�USER_GPIO_Init();
*��ڲ�������
*���ڲ�������
*�������ܣ�������ʼ��
*
*******************************************************************************/
void USER_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;							//RF_VCC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_4 | GPIO_Pin_5);									//��BK5824��SL1102ͨ��
	
	USER_Delay1ms(10);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_6;				//Red,Green
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
//	GPIO_SetBits(GPIOC,GPIO_Pin_0);										//Red��
//	GPIO_SetBits(GPIOC,GPIO_Pin_1);										//Green��
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);										//Red��
	GPIO_SetBits(GPIOC,GPIO_Pin_6);										//Green��
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;							//��ť����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;							//BEEP
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void Delay50us(u16 i)
{
    u16 j;

    for(; i > 0; i--)
        for(j = 510;j> 0; j--)//4096
            ;
}

void USER_Beep(void)
{
	uint16_t i;
	for(i = 0; i < 80; i++)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		Delay50us(4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		Delay50us(4);
	}	
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), USER_Beep, USER_Beep, test);

void  BSP_Init (void)
{		
//	RCC_GetClocksFreq(&RCC_Clocks);
	USER_SysTick_Init();
	USER_SysTick_Start();
	
	USER_CpuClk_Init();
	USER_CpuClk_Start();
	
	USER_GPIO_Init();

	USER_Beep();
	USER_USART1_Config(460800);
	USER_USART3_Config(460800);
	
	USER_NVIC_Config();
}

int main(void)
{
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);			//�ж�����ӳ��
	
	BSP_Init();
	userShellInit();
	USER_Variable_Init();												//������ʼ��
	
	log_title("Start");
	
	while (1)
	{
		if(ReceiveOverFlag == 1)								//���������ݴ���6�ֽڲŴ���
		{
			ReceiveOverFlag = 0;
			uint8_t i;
			for(i = 0; i < USART1_ReceiveDataCount; i++)
			{
				shellHandler(&shell, USART1_ReceiveBuff[i]);
			}
		}
	}
}

