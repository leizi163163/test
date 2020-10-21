#ifndef __USER_SYSTICK_H
#define	__USER_SYSTICK_H

#include "stm32f10x.h"

/*************************************CPU CLKœ‡πÿ**************************************/
#define  DEM_CR                           (*(uint32_t *)0xE000EDFC)
#define  DWT_CR                           (*(uint32_t *)0xE0001000)
#define  DWT_CYCCNT                       (*(uint32_t *)0xE0001004)
#define  DBGMCU_CR                        (*(uint32_t *)0xE0042004)


#define  DEF_BIT_00                       0x01u
#define  DEF_BIT_24                       0x01000000u

#define  DEM_CR_TRCENA                    DEF_BIT_24
#define  DWT_CR_CYCCNTENA                 DEF_BIT_00

#define  CPU_CLK_VALUE                 	  72000000
/************************************************************************************/

extern uint16_t TickCount;
extern uint8_t SysTickCount;
extern uint8_t UnixTimeCount;



void USER_CpuClk_Init(void);
void USER_CpuClk_Start(void);
void USER_CpuClk_Stop(void);
uint32_t USER_CpuClk_Read(void);


void USER_SysTick_Init(void);
void USER_SysTick_Start(void);
void USER_SysTick_Stop(void);

void USER_Delay1ms(uint16_t ms_count);

#endif
