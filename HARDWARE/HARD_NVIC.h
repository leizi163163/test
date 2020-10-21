#ifndef __HARD_NVIC_H
#define	__HARD_NVIC_H


#define USER_NVIC_DISABLE()		__disable_irq()				//关总中断
#define USER_NVIC_ENABLE()		__enable_irq()				//开总中断


void SoftReset(void);
void USER_NVIC_Config(void);
	
#endif
