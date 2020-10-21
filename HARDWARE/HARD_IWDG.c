#include "stm32f10x.h"


//���Ź�ι��ʱ��Ϊ(349 + 1) * (1 / (40 / 32))ms = 280ms
void USER_IWDG_Config(void)
{
   IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		//�Ĵ���д����
   IWDG_SetPrescaler(IWDG_Prescaler_32);				//40Kʱ��128��Ƶ
   IWDG_SetReload(2499);                 				//��������ʼֵ
   IWDG_ReloadCounter();            					//��װ�س�ֵ
   IWDG_Enable();                       				//�������Ź�
}

//ι��
void USER_IWDG_Feed(void)
{
    IWDG_ReloadCounter();    
}







