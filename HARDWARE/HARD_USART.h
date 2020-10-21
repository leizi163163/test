#ifndef	__HARD_USART_H
#define	__HARD_USART_H

#include "stm32f10x.h"


#define	RING_DATA_SIZE		250

#define	USART1_DR_Base	(uint32_t)(&USART1->DR)
#define	USART3_DR_Base	(uint32_t)(&USART3->DR)


typedef struct
{
	uint8_t Head;
	uint8_t Tail;
	uint8_t Length;
	uint8_t RX_RingDataBuf[RING_DATA_SIZE];	
}RX_RingBufDef;


extern uint16_t USART_IdleClear;

extern uint16_t USART1_ReceiveDataCount;

extern uint8_t USART1_ReceiveBuff[RING_DATA_SIZE];
extern uint8_t DataFrameBuff[RING_DATA_SIZE];
extern uint8_t DataFrameCount;

extern uint8_t USART1_ReceiveBCC;

//extern uint8_t USART1_Temp;
extern uint8_t SoftResetFlag;

extern uint8_t ReceiveOverFlag;
extern uint8_t RevRepFlag;

extern RX_RingBufDef RX_RingBufStructure;

extern char const String_ResetDeviceCommand[];


void USER_USART1_Config(uint32_t baud_rate);
void USER_USART2_Config(uint32_t baud_rate);
void USER_USART3_Config(uint32_t baud_rate);

void DMA_USART1_Config_SendMode(void);
void DMA_USART1_Config_ReceiveMode(void);
void DMA_USART3_Config_SendMode(void);


void USART_Send_String(USART_TypeDef* USARTx,uint8_t *usart_string,uint8_t usart_string_count);

void RX_RingBuf_Init(void);
uint8_t RX_RingBuf_WriteBuf(uint8_t *data_buf,uint8_t data_len);
uint8_t RX_RingBuf_ReadData(uint8_t *data);



void log_title(char *title);
void log_hex(char *title,uint8_t *hex,uint16_t len);

#endif

