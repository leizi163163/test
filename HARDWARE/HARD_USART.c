
#include "stm32f10x.h"

uint16_t USART_IdleClear;

uint16_t USART1_ReceiveDataCount = 0;

uint8_t USART1_ReceiveBuff[RING_DATA_SIZE];
uint8_t DataFrameBuff[RING_DATA_SIZE];
uint8_t DataFrameCount = 0;

uint8_t USART1_ReceiveBCC = 0;

//uint8_t USART1_Temp;
uint8_t SoftResetFlag = 0;

uint8_t ReceiveOverFlag = 0;
uint8_t RevRepFlag = 0;

RX_RingBufDef RX_RingBufStructure;

char RevData = 0;

char const String_ResetDeviceCommand[] = "Reset Device Command!";



/*******************************************************************************
*
*函数原型：USER_USART1_Config(uint32_t baud_rate);
*入口参数：baud_rate，波特率
*出口参数：无
*函数功能：串口1初始化
*
*******************************************************************************/

void USER_USART1_Config(uint32_t baud_rate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud_rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1,&USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

//	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	
//	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
//	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	
	USART_GetFlagStatus(USART1, USART_FLAG_TC);						//解决第一字节丢失现象

//	DMA_USART1_Config_SendMode();
//	DMA_USART1_Config_ReceiveMode();
}


/*******************************************************************************
*
*函数原型：USER_USART2_Config(uint32_t baud_rate);
*入口参数：baud_rate，波特率
*出口参数：无
*函数功能：串口2初始化
*
*******************************************************************************/
void USER_USART2_Config(uint32_t baud_rate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud_rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_GetFlagStatus(USART2, USART_FLAG_TC);					//解决第一字节丢失现象
	
}

void USER_USART3_Config(uint32_t baud_rate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = baud_rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3,&USART_InitStructure);

//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

//	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART3, ENABLE);
	
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
//	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	
	USART_GetFlagStatus(USART3, USART_FLAG_TC);						//解决第一字节丢失现象
	
	DMA_USART3_Config_SendMode();

}


void DMA_USART1_Config_SendMode(void)
{
    DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//开启DMA时钟

 	/*设置DMA源：内存地址&串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   

	/*内存地址(要传输的变量的指针)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DataFrameBuff;
	
	/*方向：从内存到外设*/		
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
	
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
    DMA_InitStructure.DMA_BufferSize = 0;
	
	/*外设地址不增*/	    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	
	/*内存地址自增*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	
	/*外设数据单位*/	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	/*内存数据单位 8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	
	/*DMA模式：一次传输，循环*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	 
	
	/*优先级：中*/	
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  
	
	/*禁止内存到内存的传输	*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/*配置DMA1的4通道*/		   
    DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
	
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,DISABLE);  //配置DMA发送完成后产生中断

	DMA_Cmd (DMA1_Channel4,ENABLE);
}

void DMA_USART1_Config_ReceiveMode(void)
{
    DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//开启DMA时钟

 	/*设置DMA源：内存地址&串口数据寄存器地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   

	/*内存地址(要传输的变量的指针)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_ReceiveBuff;
	
	/*方向：从内存到外设*/		
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
	
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
    DMA_InitStructure.DMA_BufferSize = RING_DATA_SIZE;
	
	/*外设地址不增*/	    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	
	/*内存地址自增*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	
	/*外设数据单位*/	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	/*内存数据单位 8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	
	/*DMA模式：一次传输，循环*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;	 
	
	/*优先级：中*/	
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
	
	/*禁止内存到内存的传输	*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/*配置DMA1的4通道*/		   
    DMA_Init(DMA1_Channel5, &DMA_InitStructure); 	   
	
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,DISABLE);  //配置DMA接收完成后产生中断

	DMA_Cmd (DMA1_Channel5,ENABLE);
}

void DMA_USART3_Config_SendMode(void)
{
    DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//开启DMA时钟

 	/*设置DMA源：内存地址&串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Base;	   

	/*内存地址(要传输的变量的指针)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DataFrameBuff;
	
	/*方向：从内存到外设*/		
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
	
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
    DMA_InitStructure.DMA_BufferSize = 0;
	
	/*外设地址不增*/	    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	
	/*内存地址自增*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	
	/*外设数据单位*/	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	/*内存数据单位 8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	
	/*DMA模式：一次传输，循环*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	 
	
	/*优先级：中*/	
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  
	
	/*禁止内存到内存的传输	*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/*配置DMA1的4通道*/		   
    DMA_Init(DMA1_Channel2, &DMA_InitStructure); 	   
	
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,DISABLE);  //配置DMA发送完成后产生中断

	DMA_Cmd (DMA1_Channel2,ENABLE);
}

/*******************************************************************************
*
*函数原型：USART_Send_String(USART_TypeDef* USARTx,uint8_t *usart_string,uint8_t usart_string_count);
*入口参数：USARTx,串口编号，USART1,	USART2,	USART3, UART4 or UART5	;
*		   usart_string,要发送的数据的首地址
*		   usart_string_count，要发送的数据个数
*出口参数：无
*函数功能：串口数据发送
*
*******************************************************************************/
void USART_Send_String(USART_TypeDef* USARTx,uint8_t *usart_string,uint8_t usart_string_count)
{
	uint8_t i;
	for(i = 0; i < usart_string_count; i++)
	{
//		USART_SendData(USARTx,*usart_string++);
		USARTx->DR = (*usart_string & (uint16_t)0x01FF);
		usart_string++;		
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);
		USART_ClearFlag(USARTx,USART_FLAG_TC);
	}
		
}

void RX_RingBuf_Init(void)
{
	RX_RingBufStructure.Head = 0;
	RX_RingBufStructure.Tail = 0;
	RX_RingBufStructure.Length = 0;
}

uint8_t RX_RingBuf_WriteBuf(uint8_t *data_buf,uint8_t data_len)
{
	uint8_t empty_len = 0;
	
	if(RX_RingBufStructure.Length >= RING_DATA_SIZE)
	{
		return 1;
	}
	
	empty_len = RING_DATA_SIZE - RX_RingBufStructure.Tail;
	
	if(empty_len< data_len)
	{
		memcpy(RX_RingBufStructure.RX_RingDataBuf + RX_RingBufStructure.Tail,data_buf,empty_len);
		RX_RingBufStructure.Tail = 0;
		memcpy(RX_RingBufStructure.RX_RingDataBuf + RX_RingBufStructure.Tail,data_buf + empty_len,data_len - empty_len);
		RX_RingBufStructure.Tail += (data_len - empty_len);
	}
	else
	{
		memcpy(RX_RingBufStructure.RX_RingDataBuf + RX_RingBufStructure.Tail,data_buf,data_len);
		RX_RingBufStructure.Tail += data_len;
	}
	RX_RingBufStructure.Length += data_len;
	
	return 0;
}

uint8_t RX_RingBuf_ReadData(uint8_t *data)
{
	if(RX_RingBufStructure.Length == 0)
	{
		return 1;
	}
	
	if(RX_RingBufStructure.Head >= RING_DATA_SIZE)
	{
		RX_RingBufStructure.Head = 0;
	}
	*data = RX_RingBufStructure.RX_RingDataBuf[RX_RingBufStructure.Head];
	
	RX_RingBufStructure.Head++;
	RX_RingBufStructure.Length--;
	
	return 0;
}


#define	USER_Debug
//#define	Usart1Debug
//#define	Usart6Debug


#ifdef	USER_Debug

uint8_t DebugTemp[1024];

void log_title(char *title)
{	
	//72MHz运行时,计数器加1,时间为13.89ns;	因此计数器增加72,即为1us;
	
	uint8_t temp1 = 0;
	uint8_t temp2 = 0;
	
	while(DMA1_Channel2->CNDTR != 0);
	DMA_ClearFlag(DMA1_FLAG_TC2);
	
	temp1 = sprintf((char *)DebugTemp,"%05d | ",DWT_CYCCNT / 72000);
	temp2 = strlen(title);
	memcpy(DebugTemp + temp1,title,temp2);
	DebugTemp[temp1 + temp2] = '\r';
	DebugTemp[temp1 + temp2 + 1] = '\n';

	DMA_Cmd (DMA1_Channel2,DISABLE);
	DMA1_Channel2->CMAR = (uint32_t)DebugTemp;
	DMA1_Channel2->CNDTR = temp1 + temp2 + 2;
	DMA_Cmd(DMA1_Channel2, ENABLE);
}

void log_hex(char *title,uint8_t *hex,uint16_t len)
{	
	//72MHz运行时,计数器加1,时间为13.89ns;	因此计数器增加72,即为1us;
	
	uint8_t temp1 = 0;
	uint8_t temp2 = 0;
	uint16_t Debug_i;
	
	while(DMA1_Channel2->CNDTR != 0);
	DMA_ClearFlag(DMA1_FLAG_TC2);
	
	temp1 = sprintf((char *)DebugTemp,"%05d | ",DWT_CYCCNT / 72000);
	temp2 = strlen(title);
	memcpy(DebugTemp + temp1,title,temp2);
	
	for(Debug_i = 0; Debug_i < len; Debug_i++)
	{
		sprintf((char *)(DebugTemp + temp1 + temp2 + Debug_i * 2),"%02X",hex[Debug_i]);
	}
	
	DebugTemp[temp1 + temp2 + len * 2] = '\r';
	DebugTemp[temp1 + temp2 + len * 2 + 1] = '\n';
	
	DMA_Cmd (DMA1_Channel2,DISABLE);
	DMA1_Channel2->CMAR = (uint32_t)DebugTemp;
	DMA1_Channel2->CNDTR = temp1 + temp2 + len * 2 + 2;
	DMA_Cmd(DMA1_Channel2, ENABLE);
}

#else
void log_title(char *title)
{

}

void log_hex(char *title,uint8_t *hex,uint16_t len)
{

}

#endif


