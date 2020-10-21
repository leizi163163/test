#ifndef __BK5823CODE_H
#define __BK5823CODE_H

#include "stm32f10x.h"


/*---------------------------------------------------
	TX
		F0-->MISO 
		F1-->MOSI  
		F2-->CLK  
		F3-->CS
		F5->DIO
		F7->power amplifier
---------------------------------------------------*/
#define 	BK_TX_MISO_High		GPIOC->BSRR=GPIO_Pin_10			
#define		BK_TX_MISO_Low		GPIOC->BRR=GPIO_Pin_10

#define 	BK_TX_MOSI_High		GPIOD->BSRR=GPIO_Pin_2					
#define		BK_TX_MOSI_Low		GPIOD->BRR=GPIO_Pin_2

#define 	BK_TX_SCK_High		GPIOC->BSRR=GPIO_Pin_11			
#define		BK_TX_SCK_Low		GPIOC->BRR=GPIO_Pin_11

#define 	BK_TX_CSN_High		GPIOC->BSRR=GPIO_Pin_12			
#define		BK_TX_CSN_Low		GPIOC->BRR=GPIO_Pin_12

#define 	BK_TX_DIO_High		GPIOB->BSRR=GPIO_Pin_5			
#define		BK_TX_DIO_Low		GPIOB->BRR=GPIO_Pin_5
/*---------------------------------------------------
	RX
		PE7-->MISO
		PE8-->MOSI  
		PE9-->CLK  
		PE10-->CS
---------------------------------------------------*/
#define BK_RX_MISO_High		GPIOE->BSRR=GPIO_Pin_7		
#define	BK_RX_MISO_Low		GPIOE->BRR=GPIO_Pin_7

#define BK_RX_MOSI_High		GPIOE->BSRR=GPIO_Pin_8					
#define	BK_RX_MOSI_Low		GPIOE->BRR=GPIO_Pin_8

#define BK_RX_SCK_High		GPIOE->BSRR=GPIO_Pin_9			
#define	BK_RX_SCK_Low		GPIOE->BRR=GPIO_Pin_9

#define BK_RX_CSN_High		GPIOE->BSRR=GPIO_Pin_10			
#define	BK_RX_CSN_Low		GPIOE->BRR=GPIO_Pin_10

#define TIM5823              				((TIM_TypeDef *) TIM4_BASE)
#define RCC_APB1Periph_TIM5823              ((uint32_t)0x00000004)


//------------------------------------------------------
/*--
	IRQ		= F4	
	DIO		= F5
	PWR		= F7
	717A	= F8
--*/ 

// #define Pin_717A_High			GPIOF->BSRRL=GPIO_Pin_8			
// #define	Pin_717A_Low			GPIOF->BSRRH=GPIO_Pin_8

// #define Pin_717B_High			GPIOF->BSRRL=GPIO_Pin_9			
// #define	Pin_717B_Low			GPIOF->BSRRH=GPIO_Pin_9

// #define Pin_5823RXDIO_High					GPIOE->BSRRL=GPIO_Pin_12			
// #define	Pin_5823RXRXDIO_Low				GPIOE->BSRRH=GPIO_Pin_12
//------------------------------------------------------
// void  Init_BK5823TX_SPI(void);
//uint8_t SPITX_RW(uint8_t _UINT8);
//------------------------------------------------------
//PE11-->IRQÊäÈë	
// #define BK_RX_IRQ_Status()		GPIOE->IDR & GPIO_Pin_11
#define BKREAD_REG        		0x00  // Define read command to register
#define BKWRITE_REG       		0x20  // Define write command to register


typedef enum 
{
	NormalMode=0,
	CarriterMode,
	AllZeroMode
}TxTypeModeEnum;


////*********************************gather******************************
//uint8_t BK5823_RXPwr(uint8_t in_u8_Level);
//uint8_t BK5823_TXPwr(uint8_t in_u8_Level);
//uint8_t BK5823_Init(uint8_t in_u8_channel,uint8_t in_u8_Level);
//void BK5823_Pwr(uint8_t in_u8_SW);

//uint8_t BK5823_RXTX(  uint8_t in_u8_IS14k,\
//											uint8_t *in_u8_TXbuf,\
//											uint8_t in_u8_TXlen,\
//											uint8_t *out_u8_rvbuf,\
//											uint8_t in_u8_TxCount,\
//											uint8_t *out_u8_ResCount,\
//											uint8_t in_u8_overtime );
//											
//uint8_t BK5823_RXTX02(  uint8_t in_u8_IS14k,\
//											uint8_t in_u8_staue,\
//											uint8_t *in_u8_TXbuf,\
//											uint8_t in_u8_TXlen,\
//											uint8_t *out_u8_rvbuf,\
//											uint16_t in_u8_TxCount,\
//											uint8_t *out_u8_ResCount,\
//											uint8_t in_u8_overtime );
////*********************************tx******************************
//void 	BK5823TX_Test_Singalcarrier(uint8_t in_u8_channel,uint8_t in_u8_Level);
//void BK5823TX_Singalcarrier(uint8_t in_u8_channel,uint8_t in_u8_Level);
//void 		BK5823TX_SPI_Write(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8);
//uint8_t BK5823TX_SPI_Read(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8);
//void 		BK5823_delay(void);
//void 		BK5823TX_AutoASKReg();
//void 		BK5823TX_TX_En(uint8_t b_enable);
//void 		BK5823TX_RXTx_Reset(uint8_t b_enable);
//void		BK5823TX_RST();
//void		BK5823TX_FreCorrect();
//// void 		BK5823TX_SpeedAGC();
//void 		BK5823TX_Rx_En(uint8_t b_enable);
//void		BK5823TX_Sleep();
//void		BK5823TX_WakeUPEnd();
//void		BK5823TX_Pwr(uint8_t b_enable);
//uint8_t	BK5823TX_Init(uint8_t in_u8_channel,uint8_t in_u8_Level);
//void  	BK5823TX_PinIni();
//void		BK5823TX_14K(uint8_t b_enable);
//uint8_t	BK5823TX_CheckReg9To18();

//void		BK5823TX_CHANGECHANNEL(uint8_t in_u8_Channel);
//void BK5823TX_ConfigPower(uint8_t in_u8_Level);
//void BK5823TX_ClrRX(void);
//void	BK5823TX_Type(uint8_t TxMode);


///////////////////////////////////RX//////////////////////////////////////
//void 			BK5823RX_Tim_Config(void);
//void 			BK5823RX_SPI_Write(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8);
//uint8_t 	BK5823RX_SPI_Read(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8);
////void 		BK5823RX_SPI_Write();
//void 			BK5823RX_AutoASKReg();
//void BK5823RX_AGC_Config(uint8_t agc_config);
//void 			BK5823RX_TX_En(uint8_t b_enable);
//void 			BK5823RX_RXTx_Reset(uint8_t b_enable);
//void			BK5823RX_RST();
//void			BK5823RX_FreCorrect();
//// void 			BK5823RX_SpeedAGC();
//void 			BK5823RX_Rx_En(uint8_t b_enable);
//void			BK5823RX_Sleep();
//void			BK5823RX_WakeUPEnd();
//void			BK5823RX_Pwr(uint8_t b_enable);
//uint8_t		BK5823RX_Init();
//void  		BK5823RX_PinIni();
//void			BK5823RX_14K(uint8_t b_enable);
//uint8_t		BK5823RX_CheckReg9To18();

//void 			BK5823RX_ClrRX(void);
//uint8_t 	BK5823RX_Rx(uint8_t *out_u8_rvbuf,uint8_t in_u8_overtime); 
//uint8_t		BK5823RX_02Init(uint8_t in_u8_channel);






void bk5823_delay(void);
uint8_t BK5823_Init(uint8_t in_u8_channel,uint8_t in_u8_Level);
u8 BK5823_RXTX( u8 in_u8_IS14k, u8 *in_u8_TXbuf,
							u8 in_u8_TXlen,u8 *out_u8_rvbuf,
							u8 in_u8_TxCount,u8 *out_u8_ResCount,
							u8 in_u8_overtime );
							

uint8_t	BK5823TX_Init(uint8_t in_u8_channel,uint8_t in_u8_Level);
void		BK5823TX_14K(uint8_t b_enable);
void 		BK5823TX_SPI_Write(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8);
uint8_t BK5823TX_SPI_Read(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8);
void BK5823TX_ConfigPower(uint8_t in_u8_Level);
void BK5823TX_CHANGECHANNEL(uint8_t in_u8_Channel);
void  BK5823TX_PinIni(void);
void	BK5823TX_Pwr(uint8_t b_enable);
void	BK5823TX_Type(uint8_t TxMode);
void 	BK5823TX_Test_Singalcarrier(uint8_t in_u8_channel,uint8_t in_u8_Level);
void BK5823TX_SPI_Write14k(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8);




void 			BK5823RX_ClrRX(void);
void 			BK5823RX_Rx_En(uint8_t b_enable);
uint8_t		BK5823RX_Init(void);
uint8_t 	BK5823RX_SPI_Read(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8);
void BK5823RX_Tim_Config(void);
void	BK5823RX_Sleep(void);
void BK5823RX_AGC_Config(uint8_t agc_config);


void BK5824_AllPinInit(void);

#endif


