#ifndef	__FM25256_H
#define	__FM25256_H

#include "stm32f2xx.h"

//FM25256---256Kbit---32KB
//0x0000--0x7FFF
#define  FM25256_CHIP_SIZE			0x8000

#define  FM25256_COMMAND_WREN		0x06            // Set Write Enable Latch
#define  FM25256_COMMAND_WRDI		0x04            // Write Disable
#define  FM25256_COMMAND_RDSR   	0x05            // Read Status Register
#define  FM25256_COMMAND_WRSR		0x01            // Write Status Register
#define  FM25256_COMMAND_READ       0x03            // Read Memory Data
#define  FM25256_COMMAND_WRITE      0x02            // Write Memory Data

#define  FM25256_INIT_NUM	      	10				//LaneMode----nFlag

#define  FM25256_INIT_ADDR      	0x1000
#define  FM25256_PARA_ADDR      	0x1100			//RSU正常工作时的参数


#define  PHASE_CONTROL_ADDR      	0x4000
#define	 TxNormalPhanseAddr			PHASE_CONTROL_ADDR + 0
#define	 TxNearFiledPhanseAddr		TxNormalPhanseAddr + 2
#define	 TxMidFiledPhanseAddr		TxNearFiledPhanseAddr + 2
#define	 TxFarFiledPhanseAddr		TxMidFiledPhanseAddr + 2

#define	 Rx_XNormalPhanseAddr		TxFarFiledPhanseAddr + 2
#define	 Rx_YNormalPhanseAddr		Rx_XNormalPhanseAddr + 2

#define	 Rx_XPositionValueListAddr    Rx_YNormalPhanseAddr + 2
#define	 Rx_XDifferenceRssiListAddr    Rx_XPositionValueListAddr + 260 
#define	 Rx_YPositionValueListAddr    Rx_XDifferenceRssiListAddr + 130
#define	 Rx_YDifferenceRssiListAddr    Rx_YPositionValueListAddr + 260
#define	 Rx_DifferPosiParaListAddr    Rx_YDifferenceRssiListAddr + 130

//CS
#define FM25256_CS_High()				(GPIOE->BSRRL = GPIO_Pin_0)
#define FM25256_CS_Low()				(GPIOE->BSRRH = GPIO_Pin_0)
//SCK
#define FM25256_SCK_High()				(GPIOE->BSRRL = GPIO_Pin_4)
#define FM25256_SCK_Low()				(GPIOE->BSRRH = GPIO_Pin_4)
//SI--FM25256输入端口
#define FM25256_SI_High()				(GPIOE->BSRRL = GPIO_Pin_3)
#define FM25256_SI_Low()				(GPIOE->BSRRH = GPIO_Pin_3)
//SO--FM25256输出端口
#define FM25256_SO_Status()				(GPIOE->IDR & GPIO_Pin_2)



void  FM25256_Init(void);
void FM25256_ReadData(uint16_t addr,uint8_t *buf,uint16_t buf_size);
void FM25256_Program(uint16_t addr,uint8_t *buf,uint16_t buf_size);
uint8_t FM25256_WriteData(uint16_t addr,uint8_t *buf,uint16_t buf_size);
void FM25256_Clear(void);
void FM25256_Fill(void);

#endif

