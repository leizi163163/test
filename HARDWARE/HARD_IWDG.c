#include "stm32f10x.h"


//看门狗喂狗时间为(349 + 1) * (1 / (40 / 32))ms = 280ms
void USER_IWDG_Config(void)
{
   IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		//寄存器写允许
   IWDG_SetPrescaler(IWDG_Prescaler_32);				//40K时钟128分频
   IWDG_SetReload(2499);                 				//计数器初始值
   IWDG_ReloadCounter();            					//重装载初值
   IWDG_Enable();                       				//启动看门狗
}

//喂狗
void USER_IWDG_Feed(void)
{
    IWDG_ReloadCounter();    
}







