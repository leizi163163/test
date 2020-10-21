
#ifndef __PSAM_H
#define __PSAM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
	


#define PSAM_TIMX		TIM3
#define PSAM_TIMX_CLK	RCC_APB1Periph_TIM3

#define PSAM_TIMEOUT		1000000 //4000000		// Timeout 100ms

#define PSAMn					3

//IO≈‰÷√
#define PSAM_IO_Gpio_1			GPIO_Pin_0
#define PSAM_IO_Gpio_2			GPIO_Pin_1

#define PSAM_IO_Port_1			GPIOA
#define PSAM_IO_Port_2			GPIOB

#define PSAM_IO_Clk_1			RCC_APB2Periph_GPIOA
#define PSAM_IO_Clk_2			RCC_APB2Periph_GPIOB

//RST≈‰÷√
#define PSAM_RST_Gpio_1			GPIO_Pin_1
#define PSAM_RST_Gpio_2			GPIO_Pin_1

#define PSAM_RST_Port_1			GPIOC
#define PSAM_RST_Port_2			GPIOA

#define PSAM_RST_Clk_1			RCC_APB2Periph_GPIOC
#define PSAM_RST_Clk_2			RCC_APB2Periph_GPIOA

//RST  IO ±÷”
#define PSAM_GPIO_CLK_1			(PSAM_RST_Clk_1 | PSAM_IO_Clk_1)
#define PSAM_GPIO_CLK_2			(PSAM_RST_Clk_2 | PSAM_IO_Clk_2)



typedef enum 
{
	PSAM1 = 0,
	PSAM2 = 1,
	PSAM3 = 2
}PSAM_TypeDef;

// typedef enum {FALSE = 0, TRUE = !FALSE} bool;

//#define SystemCoreClock1	60000000	// 3.5712M	æß’Ò
//#define SystemCoreClock1	35712000  	// 6M æß’Ò
//#define SystemCoreClock1	49996800  	// 6M æß’Ò	168M÷˜∆µ  
#define SystemCoreClock1	42527200  		// 6M æß’Ò	72M÷˜∆µ   21007058   30007058

#define psam_offset	26

#define Baud_9600 		SystemCoreClock1/9600 //+ psam_offset
#define Baud_9600H 		SystemCoreClock1/9600/2 +psam_offset	//Baud_9600/2
#define Baud_9600S 		SystemCoreClock1/9600/2*3 //+psam_offset

#define Baud_38400 		SystemCoreClock1/38400-20-80
#define Baud_38400H 	SystemCoreClock1/38400/2-20
#define Baud_38400S 	SystemCoreClock1/38400/2*3-40

//#define Baud_115200 	SystemCoreClock1/115200-psam_offset-1-45
//#define Baud_115200H 	SystemCoreClock1/115200/2-psam_offset
//#define Baud_115200S 	SystemCoreClock1/115200/2*3-psam_offset-9
#define Baud_115200 	SystemCoreClock1/115200-psam_offset-1-43
#define Baud_115200H 	SystemCoreClock1/115200/2-psam_offset
#define Baud_115200S 	SystemCoreClock1/115200/2*3-psam_offset-7



#define NO_ERROR					0x00
#define PSAM_RESET_OK				0x00
#define PSAM_TIMEOUTERR			0x20
#define PSAM_CODE_ERR				0x21 	//invalid parameter
#define PSAM_NOT_EXIST				0x22
#define PSAM_CRCERR					0x23
#define PCD_OUTOFBUFFER			0x24	//out of PCD's max buffer
#define PSAM_NOT_RESET				0x25

typedef struct 
{
	u8	status;        // communication status
	u8	nBytesSent;    // how many bytes have sent
	u8 nBytesToSend;  // how many bytes to send
	u8 nBytesReceived; // how many bytes have received
	u8 nBytesToRec ;   // how many bytes to receive
	u8 cmdcomplete ;  //cmd complete? "1" complete; "0",not complete
} SamInfo;

#define PsamInfrst(inf)    \
		inf.status         = PSAM_RESET_OK;   \
		inf.nBytesSent     = 0;   \
		inf.nBytesToSend   = 0;  \
		inf.nBytesReceived = 0;  \
		inf.nBytesToRec    = 0;  \
		inf.cmdcomplete    = 0;  

//extern void PSAM_test(void);

 void PSAM_Delayms(  __IO uint32_t nCount);

 void PSAM_Configulation(void);

 u8 PSAM_RestCard(PSAM_TypeDef psam,u8 * rstdata, u8 * rstlen);

 u8 PSAM_APDU(PSAM_TypeDef psam, u8 cCmdLen, u8 *cCMD,u8 *cReplyLen, u8 * cReplay);

u8  PSAM_PPS(PSAM_TypeDef psam, u8 cTA );

#ifdef __cplusplus
}
#endif

#else
extern void PSAM_Delayms(  __IO uint32_t nCount);

extern void PSAM_Configulation(void);

extern u8 PSAM_RestCard(PSAM_TypeDef psam,u8 * rstdata, u8 * rstlen);

extern u8 PSAM_APDU(PSAM_TypeDef psam, u8 cCmdLen, u8 *cCMD,u8 *cReplyLen, u8 * cReplay);

#endif 



