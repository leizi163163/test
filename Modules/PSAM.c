
#include "stm32f10x.h"

typedef struct
{
	uint16_t BAUD;    	// 波特率
	uint16_t BAUDS;  	// 波特率起始
	uint16_t BAUDH;  	// 波特率半周期
	uint8_t   RST_STATE;	// 复位状态
	uint8_t T01;    		// 通讯类型: T0 , T1
	uint8_t SendOrder;   	// T1 发送序列
	uint8_t TA;
} PSAM_ParaTYPE;

#define Psampararst(para)    	\
		para.BAUD        = 0;	\
		para.BAUDS         = PSAM_RESET_OK;   \
		para.BAUDH     = 0;   	\
		para.RST_STATE = 0;	\
		para.T01   = 0;  		\
		para.SendOrder = 0; 	\
		para.TA = 0; 	\


PSAM_ParaTYPE	PSAM_PARA[PSAMn];


uint8_t ta;			// =1可pps
uint8_t psam_timeoutflag = 0;
SamInfo   PsamInfo ;    //static volatile
uint8_t cCRCcheck;



void TIM_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;// 定义用于初始化的结构体
    TIM_OCInitTypeDef  Psam_OCInitStructure;
    u16 Psam_PrescalerValue = 0;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 使能TIM3定时器的时钟

    // 定时器基本配置
    TIM_TimeBaseStructure.TIM_Period = 0xffff;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_PrescalerConfig(TIM3, Psam_PrescalerValue, TIM_PSCReloadMode_Immediate);

    //Output Compare Timing Mode configuration: Channel1
    Psam_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
    Psam_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
    Psam_OCInitStructure.TIM_Pulse = 0;
    Psam_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM3, &Psam_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

    // 清除溢出中断标志 //
    TIM_ClearFlag(TIM3, TIM_FLAG_CC1);
    // 禁止ARR预装载缓冲器 //
    TIM_ARRPreloadConfig(TIM3, DISABLE);

}


// 设置PSAM的IO口 为输入
void PSAM_IO_Set_Input(PSAM_TypeDef psam)
{
	uint16_t PSAM_GPIO_n;
	uint32_t PSAM_CLK_n;
	GPIO_TypeDef* PSAM_PORT_n;			//判断PSAM是否为PSAM2
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	if(psam == 1)
	{
		PSAM_PORT_n = PSAM_IO_Port_1;
		PSAM_CLK_n = PSAM_IO_Clk_1;
		PSAM_GPIO_n = PSAM_IO_Gpio_1;
	}
	if(psam == 2)
	{
		PSAM_PORT_n = PSAM_IO_Port_2;
		PSAM_CLK_n = PSAM_IO_Clk_2;
		PSAM_GPIO_n = PSAM_IO_Gpio_2;
	}

	RCC_APB2PeriphClockCmd(PSAM_CLK_n, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = PSAM_GPIO_n;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PSAM_PORT_n, &GPIO_InitStructure);
	GPIO_WriteBit(PSAM_PORT_n,GPIO_InitStructure.GPIO_Pin,Bit_SET);
}

// 设置PSAM的IO口 为输出
void PSAM_IO_Set_Output(PSAM_TypeDef psam)
{
	uint16_t PSAM_GPIO_n;
	uint32_t PSAM_CLK_n;
	GPIO_TypeDef* PSAM_PORT_n;			//判断PSAM是否为PSAM2
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	if(psam == 1)
	{
		PSAM_PORT_n = PSAM_IO_Port_1;
		PSAM_CLK_n = PSAM_IO_Clk_1;
		PSAM_GPIO_n = PSAM_IO_Gpio_1;
	}
	if(psam == 2)
	{
		PSAM_PORT_n = PSAM_IO_Port_2;
		PSAM_CLK_n = PSAM_IO_Clk_2;
		PSAM_GPIO_n = PSAM_IO_Gpio_2;
	}

	RCC_APB2PeriphClockCmd(PSAM_CLK_n, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = PSAM_GPIO_n;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PSAM_PORT_n, &GPIO_InitStructure);
	GPIO_WriteBit(PSAM_PORT_n,GPIO_InitStructure.GPIO_Pin,Bit_SET);

}


void PSAM_Configulation(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;


	Psampararst(PSAM_PARA[PSAM1]);			// 初始化各个psam状态
	Psampararst(PSAM_PARA[PSAM2]);			// 初始化各个psam状态
	Psampararst(PSAM_PARA[PSAM3]);			// 初始化各个psam状态

	
/****************初始化配置PSAM1端口******************/
	RCC_APB2PeriphClockCmd(PSAM_GPIO_CLK_1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PSAM_RST_Gpio_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PSAM_RST_Port_1, &GPIO_InitStructure);
	GPIO_WriteBit(PSAM_RST_Port_1,GPIO_InitStructure.GPIO_Pin,Bit_SET);
	
	GPIO_InitStructure.GPIO_Pin = PSAM_IO_Gpio_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PSAM_IO_Port_1, &GPIO_InitStructure);
	GPIO_WriteBit(PSAM_IO_Port_1,GPIO_InitStructure.GPIO_Pin,Bit_SET);
	
	
/****************初始化配置PSAM2端口******************/	
	RCC_APB2PeriphClockCmd(PSAM_GPIO_CLK_2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PSAM_RST_Gpio_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PSAM_RST_Port_2, &GPIO_InitStructure);
	GPIO_WriteBit(PSAM_RST_Port_2,GPIO_InitStructure.GPIO_Pin,Bit_SET);
	
	GPIO_InitStructure.GPIO_Pin = PSAM_IO_Gpio_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PSAM_IO_Port_2, &GPIO_InitStructure);
	GPIO_WriteBit(PSAM_IO_Port_2,GPIO_InitStructure.GPIO_Pin,Bit_SET);
	
	TIM_Config();
}


void PSAM_Delayms( u32 nCount)
{
    u32 m = 10500;

    for (; nCount != 0; nCount--)
    {
        m = 10500;
        for (; m  != 0; m --);
    }
}

void PSAM_Delayus( u32 nCount)
{
    u32 m = 11;

    for (; nCount != 0; nCount--)
    {
        m = 11;
        for (; m  != 0; m --);
    }
}

void PSAM_Send(PSAM_TypeDef psam, uint8_t inch)			// seat 1 send data to card
{
	uint8_t mscnt =0;			// 计算1的位数
	uint8_t cdata = 0;
	GPIO_TypeDef* PSAM_PORT_n;			//判断PSAM是否为PSAM2
	uint16_t PSAM_GPIO_n;
		
	if(psam == 1)
	{
		PSAM_PORT_n = PSAM_IO_Port_1;
		PSAM_GPIO_n = PSAM_IO_Gpio_1;
	}
	if(psam == 2)
	{
		PSAM_PORT_n = PSAM_IO_Port_2;
		PSAM_GPIO_n = PSAM_IO_Gpio_2;
	}
	
	

	while(1)
	{
		cdata = inch;		//  先发低位
		mscnt = 0;
		

		USER_NVIC_DISABLE();		
	    // Disable the TIM Counter 
		TIM_Cmd(PSAM_TIMX,DISABLE);
		
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;		// clear the flag
		PSAM_TIMX->CCR1 = Baud_9600S;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		GPIO_ResetBits(PSAM_PORT_n,PSAM_GPIO_n);	// 起始位
		TIM_Cmd(PSAM_TIMX,ENABLE);

		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		if(cdata & 0x01)				// bit0
		{
			PSAM_PORT_n->BSRR = PSAM_GPIO_n;
			mscnt++;
		}
		else
		{
			PSAM_PORT_n->BRR = PSAM_GPIO_n;
		}
						
		cdata = cdata>> 1;

		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		if(cdata & 0x01)				// bit1
		{
			PSAM_PORT_n->BSRR = PSAM_GPIO_n;
			mscnt++;
		}
		else
		{
			PSAM_PORT_n->BRR = PSAM_GPIO_n;
		}
		
			

		cdata = cdata>> 1;

		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		if(cdata & 0x01)				// bit2
		{
			PSAM_PORT_n->BSRR = PSAM_GPIO_n;
			mscnt++;
		}
		else
		{
			PSAM_PORT_n->BRR = PSAM_GPIO_n;
		}
		
			

		cdata = cdata>> 1;

		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		if(cdata & 0x01)				// bit3
		{
			PSAM_PORT_n->BSRR = PSAM_GPIO_n;
			mscnt++;
		}
		else
		{
			PSAM_PORT_n->BRR = PSAM_GPIO_n;
		}
		
			
		cdata = cdata>> 1;

		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		if(cdata & 0x01)				// bit4
		{
			PSAM_PORT_n->BSRR = PSAM_GPIO_n;
			mscnt++;
		}
		else
		{
			PSAM_PORT_n->BRR = PSAM_GPIO_n;
		}
		
			
		cdata = cdata>> 1;

		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		if(cdata & 0x01)				// bit5
		{
			PSAM_PORT_n->BSRR = PSAM_GPIO_n;
			mscnt++;
		}
		else
		{
			PSAM_PORT_n->BRR = PSAM_GPIO_n;
		}
		
			
		cdata = cdata>> 1;

		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		if(cdata & 0x01)				// bit6
		{
			PSAM_PORT_n->BSRR = PSAM_GPIO_n;
			mscnt++;
		}
		else
		{
			PSAM_PORT_n->BRR = PSAM_GPIO_n;
		}
		
			
		cdata = cdata>> 1;

		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		if(cdata & 0x01)				// bit7
		{
			PSAM_PORT_n->BSRR = PSAM_GPIO_n;
			mscnt++;
		}
		else
		{
			PSAM_PORT_n->BRR = PSAM_GPIO_n;
		}
		
			

		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		if(mscnt%2 == 1)				// p bit,字节+p位中1个数为偶数个
		{
			PSAM_PORT_n->BSRR = PSAM_GPIO_n;
		}
		else
		{
			PSAM_PORT_n->BRR = PSAM_GPIO_n;
		}
		
			
		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;

		PSAM_PORT_n->BSRR = PSAM_GPIO_n;		// set 1

			


		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		
		PSAM_IO_Set_Input(psam);

		
		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;

			

		if((PSAM_PORT_n->IDR & PSAM_GPIO_n))		// bit0
		{
//			TIM_Cmd(PSAM_TIMX,DISABLE);
			PSAM_IO_Set_Output( psam);
			USER_NVIC_ENABLE();
			return;
		}
		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;

			

		while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
		PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;

			
		USER_NVIC_ENABLE();	
	}
}

uint8_t PSAM_Receive(PSAM_TypeDef psam)
{
	uint8_t mscnt ;			// 计算1的位数
	uint32_t ccount =PSAM_TIMEOUT ;  // timeout count == 100ms
	uint8_t cdata = 0;
	
	GPIO_TypeDef* PSAM_PORT_n;			//判断PSAM是否为PSAM2
	uint16_t PSAM_GPIO_n;
		
	if(psam == 1)
	{
		PSAM_PORT_n = PSAM_IO_Port_1;
		PSAM_GPIO_n = PSAM_IO_Gpio_1;
	}
	if(psam == 2)
	{
		PSAM_PORT_n = PSAM_IO_Port_2;
		PSAM_GPIO_n = PSAM_IO_Gpio_2;
	}

 
	psam_timeoutflag = 0;

	
	
    // Disable the TIM Counter 
	TIM_Cmd(PSAM_TIMX,DISABLE);
	
	PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;		// clear the flag
	PSAM_TIMX->CCR1 = PSAM_PARA[psam].BAUDS-90;
	PSAM_TIMX->CNT = 0; //PSAM_TIMX->CCR1-Baud_9600S;
	
	USER_NVIC_DISABLE();
	
	while((ccount != 0 ))
	{
		if ((PSAM_PORT_n->IDR & PSAM_GPIO_n) == (uint32_t)Bit_RESET)
			break;
		
		ccount --;
	}

//	USER_NVIC_DISABLE();							//进临界区改到842行--2019.4.1
		
	
	if(ccount == 0)	// Timeout
	{
		psam_timeoutflag = 1;
		USER_NVIC_ENABLE();
		return 0;
	}
	
	TIM_Cmd(PSAM_TIMX,ENABLE);
		
	while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
	PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
//	PSAM_TIMX->CCR1 += Baud_9600;
	PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;

		

	if((PSAM_PORT_n->IDR & PSAM_GPIO_n))		// bit0
	{
		cdata |=0x80;
		mscnt = 1;
	}
	else
	{
		mscnt = 0;
	}

	cdata = cdata>> 1;
	while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
	PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
	PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		
	
	if((PSAM_PORT_n->IDR & PSAM_GPIO_n))		// bit1
	{
		cdata |=0x80;
		mscnt ++;
	}

	cdata = cdata>> 1;
	while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
	PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
	PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		
	if((PSAM_PORT_n->IDR & PSAM_GPIO_n))		// bit2
	{
		cdata |=0x80;
		mscnt ++;
	}

	cdata = cdata>> 1;
	while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
	PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
	PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		
	if((PSAM_PORT_n->IDR & PSAM_GPIO_n))		// bit3
	{
		cdata |=0x80;
		mscnt ++;
	}

	cdata = cdata>> 1;
	while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
	PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
	PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		
	if((PSAM_PORT_n->IDR & PSAM_GPIO_n))		// bit4
	{
		cdata |=0x80;
		mscnt ++;
	}

	cdata = cdata>> 1;
	while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
	PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
	PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		
	if((PSAM_PORT_n->IDR & PSAM_GPIO_n))		// bit5
	{
		cdata |=0x80;
		mscnt ++;
	}

	cdata = cdata>> 1;
	while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
	PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
	PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		
	if((PSAM_PORT_n->IDR & PSAM_GPIO_n))		// bit6
	{
		cdata |=0x80;
		mscnt ++;
	}

	cdata = cdata>> 1;
	while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
	PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
	PSAM_TIMX->CNT = PSAM_TIMX->CCR1-PSAM_PARA[psam].BAUD;
		
	if((PSAM_PORT_n->IDR & PSAM_GPIO_n))		// bit7
	{
		cdata |=0x80;
		mscnt ++;
	}

	while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
	PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;
//	PSAM_TIMX->CNT = PSAM_TIMX->CCR1-Baud_9600;
		
	if((PSAM_PORT_n->IDR & PSAM_GPIO_n))		// bit p
	{
//		cdata |=0x80;
		mscnt ++;
	}

    // Disable the TIM Counter 
//	PSAM_TIMX->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));
	while((PSAM_PORT_n->IDR & PSAM_GPIO_n) == (uint32_t)Bit_RESET);

	USER_NVIC_ENABLE(); 
	return cdata;

}

uint8_t PSAM_RestCard(PSAM_TypeDef psam,uint8_t * rstdata, uint8_t * rstlen)
{
	volatile uint8_t data_in;
	uint8_t i;
	GPIO_TypeDef* PSAM_PORT_n;			//判断PSAM是否为PSAM2
	
	uint16_t PSAM_GPIO_n;
		
	if(psam == 1)
	{
		PSAM_PORT_n = PSAM_RST_Port_1;
		PSAM_GPIO_n = PSAM_RST_Gpio_1;
	}
	if(psam == 2)
	{
		PSAM_PORT_n = PSAM_RST_Port_2;
		PSAM_GPIO_n = PSAM_RST_Gpio_2;
	}

	PsamInfrst(PsamInfo); 
	
	Psampararst(PSAM_PARA[psam]);			// 初始化各个psam状态
	PSAM_PARA[psam].BAUD = Baud_9600;
	PSAM_PARA[psam].BAUDS= Baud_9600S;
	
	GPIO_WriteBit(PSAM_PORT_n, PSAM_GPIO_n, Bit_SET);
	PSAM_Delayms(4);
	GPIO_WriteBit(PSAM_PORT_n, PSAM_GPIO_n, Bit_RESET);
	
	rstdata[PsamInfo.nBytesReceived] = PSAM_Receive(psam);
	if(rstdata[PsamInfo.nBytesReceived] != 0x3B)
	{		
		PSAM_PARA[psam].BAUD = Baud_38400;
		PSAM_PARA[psam].BAUDS= Baud_38400S;
		
		GPIO_WriteBit(PSAM_PORT_n, PSAM_GPIO_n, Bit_RESET);
		PSAM_Delayms(4);
		GPIO_WriteBit(PSAM_PORT_n, PSAM_GPIO_n, Bit_SET);
		
		rstdata[PsamInfo.nBytesReceived] = PSAM_Receive(psam);
	}
	if(rstdata[PsamInfo.nBytesReceived] != 0x3B)
	{
		PSAM_PARA[psam].BAUD = Baud_115200;
		PSAM_PARA[psam].BAUDS= Baud_115200S;
		
		GPIO_WriteBit(PSAM_PORT_n, PSAM_GPIO_n, Bit_RESET);
		PSAM_Delayms(4);
		GPIO_WriteBit(PSAM_PORT_n, PSAM_GPIO_n, Bit_SET);
		
		rstdata[PsamInfo.nBytesReceived] = PSAM_Receive(psam);
	}
	

	if(rstdata[PsamInfo.nBytesReceived] == 0x3B)
	{
//USER_NVIC_DISABLE();			
		PsamInfo.nBytesReceived++;
		data_in = PSAM_Receive(psam);						// T0 格式字节
		rstdata[PsamInfo.nBytesReceived++] = data_in;
		PsamInfo.nBytesToRec = data_in & 0x0F;	// 历史字节长度
		if(data_in & 0x10) //DATA_IN4)								// TA1
		{
			rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);	
			if((rstdata[PsamInfo.nBytesReceived-1] != 0x11)&&(rstdata[PsamInfo.nBytesReceived-1] != 0x01))		// 为9000bps，无需pps
			{
				PSAM_PARA[psam].TA = rstdata[PsamInfo.nBytesReceived-1];
			}
		}
		if(data_in & 0x20) //if(DATA_IN5)								// TB1
			rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);			
		if(data_in & 0x40) //if(DATA_IN6)								// TC1
			rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);			
		if(data_in & 0x80) // if(DATA_IN7)								// TD1
		{
			data_in= PSAM_Receive(psam);		
			rstdata[PsamInfo.nBytesReceived++] = data_in;		
			if(data_in & 0x01)		// T = 1
			{
				PSAM_PARA[psam].T01 = TRUE;	
				PsamInfo.nBytesToRec++;			// TCK校验字节
				
			}
			else
			{
				PSAM_PARA[psam].T01 = FALSE;					
			}

			if(data_in & 0x10)//if(DATA_IN4)								// TA2
			{
				rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);		
			}
			if(data_in & 0x20) //if(DATA_IN5)								// TB2
				rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);			
			if(data_in & 0x40) //if(DATA_IN6)								// TC2
				rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);			
			if(data_in & 0x80) //if(DATA_IN7)								// TD2
			{
				data_in= PSAM_Receive(psam);		
				rstdata[PsamInfo.nBytesReceived++] = data_in;		
				if(data_in & 0x10)//if(DATA_IN4)								// TA3
				{
					rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);			
				}
				if(data_in & 0x20) //if(DATA_IN5)								// TB3
					rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);			
				if(data_in & 0x40) //if(DATA_IN6)								// TC3
					rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);			
				if(data_in & 0x80) //if(DATA_IN7)								// TD3
				{
					data_in= PSAM_Receive(psam);		
					rstdata[PsamInfo.nBytesReceived++] = data_in;		
					if(data_in & 0x10)//if(DATA_IN4)								// TA4
					{
						rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);			
					}
					if(data_in & 0x20) //if(DATA_IN5)								// TB4
						rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);			
					if(data_in & 0x40) //if(DATA_IN6)								// TC4
						rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);			
					if(data_in & 0x80) //if(DATA_IN7)								// TD4
					{
						data_in= PSAM_Receive(psam);		
						rstdata[PsamInfo.nBytesReceived++] = data_in;		
					}

				}
			}
		}
		for(i=0; i< PsamInfo.nBytesToRec; i++)			// history byte
		{
			rstdata[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);	
			if(psam_timeoutflag == TRUE)					// timeout
			{			
				PsamInfo.status = PSAM_TIMEOUTERR;
				break;
			}
		}
		*rstlen = PsamInfo.nBytesReceived;
	}
	else
	{
		if(psam_timeoutflag == 1)	// 接收超时，判断无psam卡
			PsamInfo.status = PSAM_NOT_EXIST;
	}

	if(PsamInfo.status == NO_ERROR)
	{
		PSAM_PARA[psam].RST_STATE = 1;		// psam 卡复位成功
	}
	if(PSAM_PARA[psam].TA)
	{
		if(PSAM_PARA[psam].TA == 0x95)
		{
			PSAM_PARA[psam].TA = 0x18;
		}
		USER_Delay1ms(10);						//经测试发现，PPS后不延时出错
		PSAM_PPS(psam, PSAM_PARA[psam].TA);
		USER_Delay1ms(10);						//经测试发现，PPS后不延时出错
	}
			
	return PsamInfo.status;
}


//////////////////////////////////////////////////////////////////////
//          P S A M    C A R D   C O M M A N D    S E N D    T = 0
///////////////////////////////////////////////////////////////////////
void PSAM_SendCmd(PSAM_TypeDef psam,uint8_t clen,uint8_t *csource)
{
	uint8_t  templen =0 ;
	uint8_t  i =0;

	PSAM_IO_Set_Output(psam);
	for (templen=0 ;templen<clen;templen++)
	{
		TIM_Cmd(PSAM_TIMX,ENABLE);
		for(i=0; i<3;i++)
		{
			PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;		// clear the flag
			PSAM_TIMX->CCR1 = (PSAM_PARA[psam].BAUD);   
			PSAM_TIMX->CNT = 0; //PSAM_TIMX->CCR1-Baud_9600S;			
			while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
		}
		PSAM_Send(psam, csource[templen]); 
	}
}

u8 PSAM_T0Cmd(PSAM_TypeDef psam, u8 cCmdLen, u8 *cCMD,u8 *cReplyLen, u8 * cReplay)
{ 
	u8 status = NO_ERROR;
	static u8 ctemp;
	u8 tempbuf[5];

	if  (cCmdLen>=5){ 	
		PSAM_SendCmd(psam,5,cCMD); // head lable,5 bytes
		PSAM_IO_Set_Input(psam);
		ctemp = PSAM_Receive(psam);
		if (psam_timeoutflag == 0){
			PsamInfo.nBytesReceived++ ;
			if (ctemp ==cCMD[1]){  	/* =INS    */ 
				if (cCmdLen == 5){ 	/* receive datas */   
					PsamInfo.nBytesToRec = cCMD[4]+2 ;
					PsamInfo.nBytesReceived=0 ;
					while(!(psam_timeoutflag||PsamInfo.cmdcomplete)){
						cReplay[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);
						if (PsamInfo.nBytesReceived>=PsamInfo.nBytesToRec)
							PsamInfo.cmdcomplete=1 ;       
					}
					*cReplyLen = PsamInfo.nBytesReceived;
				}else{
					PSAM_SendCmd(psam,cCMD[4],&cCMD[5]);
					PSAM_IO_Set_Input(psam);
					ctemp = PSAM_Receive(psam);
					if (psam_timeoutflag == 0){
						if(0x61 == ctemp ){          
							ctemp = PSAM_Receive(psam);
							if(psam_timeoutflag == 1){
								return status = PSAM_TIMEOUTERR;
							}
							PsamInfrst(PsamInfo) ; 			//reset infomation initialization
							PsamInfo.nBytesToRec = ctemp + 2 ;
							tempbuf[0]=0 ;
							tempbuf[1]=0xC0 ;
							tempbuf[2]=0 ;
							tempbuf[3]=0 ;
							tempbuf[4]=ctemp ;
							PSAM_Delayms(3);
							PSAM_SendCmd(psam,5,tempbuf); 	//get response

							PSAM_IO_Set_Input(psam);				
							ctemp = PSAM_Receive(psam);
							if(psam_timeoutflag == 1){
								return status = PSAM_TIMEOUTERR;
							}
							while(!(psam_timeoutflag||PsamInfo.cmdcomplete)){
								cReplay[PsamInfo.nBytesReceived++]=PSAM_Receive(psam);
								if  (PsamInfo.nBytesReceived>=PsamInfo.nBytesToRec){
									PsamInfo.cmdcomplete=1 ;      
									*cReplyLen = PsamInfo.nBytesReceived;
								}
							}
						}else{							/* receive sw1 sw2 */
							cReplay[1] = PSAM_Receive(psam);
							cReplay[0] = ctemp;
							*cReplyLen = 2;
							if(psam_timeoutflag == 1){
								status = PSAM_TIMEOUTERR;
							}
							PsamInfo.nBytesReceived=2 ;              
						}
					}
				}  
			}else{			/* receive sw1 sw2  */
				cReplay[PsamInfo.nBytesReceived++] = PSAM_Receive(psam);;
				cReplay[0] = ctemp;
				*cReplyLen = 2;
				if(psam_timeoutflag == 1){
					status = PSAM_TIMEOUTERR;
				}
			}
		}else{
			status = PSAM_TIMEOUTERR;
		}
	}else{
		status = PSAM_CODE_ERR ;
	}		
  return status ;
}

//////////////////////////////////////////////////////////////////////
//          P S A M    C A R D   C O M M A N D   T = 1
/////////////////////////////////////////////////////////////////////// 
uint8_t PSAM_APDU(PSAM_TypeDef psam, uint8_t cCmdLen, uint8_t *cCMD,uint8_t *cReplyLen, uint8_t * cReplay)
{
	uint8_t  status=0;
	volatile uint8_t  tempbuf[3];
	uint8_t  i;

//	if(PSAM_PARA[psam].RST_STATE == 0)
//		return PSAM_NOT_RESET;
	
	PsamInfrst(PsamInfo) ; //reset infomation initialization
	// T = 1
	if (PSAM_PARA[psam].T01 != 0x00){
		cCRCcheck = 0;
		PsamInfo.nBytesToSend=cCmdLen +4 ;
		tempbuf[0] = 0x00; 
		tempbuf[2] = cCmdLen; 
		cCRCcheck = cCmdLen;

		if (PSAM_PARA[psam].SendOrder == 0){
			tempbuf[1] = 0; 
			PSAM_PARA[psam].SendOrder = 1;
		}else{
			tempbuf[1] =0x40 ;
			cCRCcheck ^= tempbuf[1];	
			PSAM_PARA[psam].SendOrder = 0;
		} 
		for (i =0; i< cCmdLen ; i++ ){
			cCRCcheck ^= cCMD[i];
		}
		cCMD[i] = cCRCcheck;
		PSAM_IO_Set_Output(psam);	
		while (PsamInfo.nBytesSent<PsamInfo.nBytesToSend){ 
			for(i=0; i<1;i++){
				PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;		// clear the flag
				PSAM_TIMX->CCR1 = PSAM_PARA[psam].BAUD;
				PSAM_TIMX->CNT = 0; //PSAM_TIMX->CCR1-Baud_9600S;			
				while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
			}
			if(PsamInfo.nBytesSent < 3){
				PSAM_Send(psam, tempbuf[PsamInfo.nBytesSent]);
			}else if((PsamInfo.nBytesSent >= 3) &&(PsamInfo.nBytesSent < PsamInfo.nBytesToSend-1)){
				PSAM_Send(psam, cCMD[PsamInfo.nBytesSent -3]);			
			}else if (PsamInfo.nBytesSent==(PsamInfo.nBytesToSend-1)){
				PSAM_Send(psam, cCRCcheck);							
			}				
			PsamInfo.nBytesSent++ ;
		}
		cCRCcheck = 0;
		PSAM_IO_Set_Input(psam);
		for(i =0; ((i<3) && (!psam_timeoutflag)); i++){
			tempbuf[PsamInfo.nBytesReceived]=PSAM_Receive(psam);
			cCRCcheck ^= tempbuf[PsamInfo.nBytesReceived] ;
			PsamInfo.nBytesReceived++;
		}
		*cReplyLen = tempbuf[2];
		PsamInfo.nBytesToRec=tempbuf[PsamInfo.nBytesReceived-1]+4 ;
		while (!(PsamInfo.cmdcomplete || psam_timeoutflag)){
			if (PsamInfo.nBytesReceived< PsamInfo.nBytesToRec){
				cReplay[PsamInfo.nBytesReceived -3 ]=PSAM_Receive(psam);
				cCRCcheck ^= cReplay[PsamInfo.nBytesReceived-3] ;
				PsamInfo.nBytesReceived++;
			}else if ((PsamInfo.nBytesToRec!=0)&&(PsamInfo.nBytesToRec==PsamInfo.nBytesReceived)){
				PsamInfo.cmdcomplete=1 ;
				if (cCRCcheck != 0){                
					PsamInfo.status=PSAM_CRCERR ; 
				}
			}
		}
		if (psam_timeoutflag==1) {
			psam_timeoutflag =0 ;
			PsamInfo.status=PSAM_TIMEOUTERR ;
		} 
		status=PsamInfo.status;
	}else if (PSAM_PARA[psam].T01 == FALSE){ 
		status = PSAM_T0Cmd(psam,cCmdLen, cCMD, cReplyLen, cReplay) ; 
		//*cReplyLen = 2;
		PsamInfo.cmdcomplete=0 ;
		if (psam_timeoutflag==1){
			status = PSAM_TIMEOUTERR ;   
			psam_timeoutflag=0 ;
		}
	}else{
		status=PSAM_CODE_ERR ;
	}
	return status;
}

////////////////////////////////////////////////////////////////////////
////          P S A M    C A R D   C O M M A N D   T = 1
///////////////////////////////////////////////////////////////////////// 
//uint8_t PSAM_APDU(PSAM_TypeDef psam, uint8_t cCmdLen, uint8_t *cCMD,uint8_t *cReplyLen, uint8_t * cReplay)
//{
//	uint8_t  status=0 ;
//	vuint8_t  tempbuf[3];
//	uint8_t  i;

//	if(PSAM_PARA[psam].RST_STATE == 0)
//		return PSAM_NOT_RESET;
//	
//	PsamInfrst(PsamInfo) ; //reset infomation initialization
//	// T = 1
//	if (PSAM_PARA[psam].T01 != 0x00){
//		cCRCcheck = 0;
//		PsamInfo.nBytesToSend=cCmdLen +4 ;
//		tempbuf[0] = 0x00; 
//		tempbuf[2] = cCmdLen; 
//		cCRCcheck = cCmdLen;

//		if (PSAM_PARA[psam].SendOrder == 0){
//			tempbuf[1] = 0; 
//			PSAM_PARA[psam].SendOrder = 1;
//		}else{
//			tempbuf[1] =0x40 ;
//			cCRCcheck ^= tempbuf[1];	
//			PSAM_PARA[psam].SendOrder = 0;
//		} 
//		for (i =0; i< cCmdLen ; i++ ){
//			cCRCcheck ^= cCMD[i];
//		}
//		cCMD[i] = cCRCcheck;
//		PSAM_IO_Set_Output(psam);	
//		while (PsamInfo.nBytesSent<PsamInfo.nBytesToSend){ 
//			for(i=0; i<1;i++){
//				PSAM_TIMX->SR = (uint16_t)~TIM_FLAG_CC1;		// clear the flag
//				PSAM_TIMX->CCR1 = PSAM_PARA[psam].BAUD;
//				PSAM_TIMX->CNT = 0; //PSAM_TIMX->CCR1-Baud_9600S;			
//				while((PSAM_TIMX->SR & TIM_FLAG_CC1) == (uint16_t)RESET);
//			}
//			if(PsamInfo.nBytesSent < 3){
//				PSAM_Send(psam, tempbuf[PsamInfo.nBytesSent]);
//			}else if((PsamInfo.nBytesSent >= 3) &&(PsamInfo.nBytesSent < PsamInfo.nBytesToSend-1)){
//				PSAM_Send(psam, cCMD[PsamInfo.nBytesSent -3]);			
//			}else if (PsamInfo.nBytesSent==(PsamInfo.nBytesToSend-1)){
//				PSAM_Send(psam, cCRCcheck);							
//			}				
//			PsamInfo.nBytesSent++ ;
//		}
//		cCRCcheck = 0;
//		PSAM_IO_Set_Input(psam);
//		for(i =0; ((i<3) && (!psam_timeoutflag)); i++){
//			tempbuf[PsamInfo.nBytesReceived]=PSAM_Receive(psam);
//			cCRCcheck ^= tempbuf[PsamInfo.nBytesReceived] ;
//			PsamInfo.nBytesReceived++;
//		}
//		*cReplyLen = tempbuf[2];
//		PsamInfo.nBytesToRec=tempbuf[PsamInfo.nBytesReceived-1]+4 ;
//		while (!(PsamInfo.cmdcomplete || psam_timeoutflag)){
//			if (PsamInfo.nBytesReceived< PsamInfo.nBytesToRec){
//				cReplay[PsamInfo.nBytesReceived -3 ]=PSAM_Receive(psam);
//				cCRCcheck ^= cReplay[PsamInfo.nBytesReceived-3] ;
//				PsamInfo.nBytesReceived++;
//			}else if ((PsamInfo.nBytesToRec!=0)&&(PsamInfo.nBytesToRec==PsamInfo.nBytesReceived)){
//				PsamInfo.cmdcomplete=1 ;
//				if (cCRCcheck != 0){                
//					PsamInfo.status=PSAM_CRCERR ; 
//				}
//			}
//		}
//		if (psam_timeoutflag==1) {
//			psam_timeoutflag =0 ;
//			PsamInfo.status=PSAM_TIMEOUTERR ;
//		} 
//		status=PsamInfo.status;
//	}else if (PSAM_PARA[psam].T01 == FALSE){ 
//		status = PSAM_T0Cmd(psam,cCmdLen, cCMD, cReplyLen, cReplay) ; 
//		//*cReplyLen = 2;
//		PsamInfo.cmdcomplete=0 ;
//		if (psam_timeoutflag==1){
//			status = PSAM_TIMEOUTERR ;   
//			psam_timeoutflag=0 ;
//		}
//	}else{
//		status=PSAM_CODE_ERR ;
//	}
//	return status;
//}

uint8_t  PSAM_PPS(PSAM_TypeDef psam, uint8_t cTA )
{
	uint8_t  cBuf[4];
	uint8_t  cRbuf[4];
	uint8_t  i;

#if defined(CQPSAM)&&(CQPSAM==1)
	if(psam ==0)					//卡槽0重庆(重庆PSAM自动38400)
	{
		PSAM_PARA[psam].BAUD = Baud_38400;
		PSAM_PARA[psam].BAUDS = Baud_38400S;	
		return TRUE;; 
	}
#endif
	
	cBuf[0] = 0xFF;				
	cBuf[1] = 0x10;		// T =0;
	cBuf[2] = cTA;
	
	cBuf[3] = 0;

	for(i =0; i<3;i++)
	{
		cBuf[3] ^= cBuf[i];
	}

	PSAM_SendCmd(psam,4,cBuf); // pps 4 bytes

	PSAM_IO_Set_Input(psam);
	cRbuf[0] = PSAM_Receive(psam);
	if (psam_timeoutflag == 0)		
	{
		for(i= 1;i<4;i++)
		{
			cRbuf[i] = PSAM_Receive(psam);
		}
	}else{
		return FALSE;  // 超时
	}

	if(*(unsigned long *)cBuf == *(unsigned long *)cRbuf)
	{
		if(cBuf[2] == 0x18)   // 115200
		{
			PSAM_PARA[psam].BAUD = Baud_115200;
			PSAM_PARA[psam].BAUDS = Baud_115200S;	
		}
		else if(cBuf[2] == 0x13)
		{
			PSAM_PARA[psam].BAUD = Baud_38400;
			PSAM_PARA[psam].BAUDS = Baud_38400S;	
		}

		return TRUE;; 
	}
	else{
		return FALSE;
	}
}

