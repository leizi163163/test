#ifndef __HARD_NVIC_H
#define	__HARD_NVIC_H


#define USER_NVIC_DISABLE()		__disable_irq()				//�����ж�
#define USER_NVIC_ENABLE()		__enable_irq()				//�����ж�


void SoftReset(void);
void USER_NVIC_Config(void);
	
#endif
