#include "stm32f10x.h"

unsigned long Bk5823TX_Reg0_16[]=
{

/*低位在前，高位在后*///(信道1-->5.84\5.80(5.805)  ;信道0-->5.83\5.79(5.785)
/*低位在前，高位在后*/
/*低位在前，高位在后*/
0xC2BE5557,					//0		5.795		57 55 BE C2        0xA2E45558 //5.805  5855e4a2   ;0XE2985556  //5.785  565598e2	
0x404E3BED,					//1		5.83G 	ED 3B 4E 40
//0x40 683bee				//1		5.84G 	ee 3b 68 40

0x2000F009,					//2
0x603B0031,					//3
	//---0x3F0F0C38,				//4
	0x3F0F1C38,				//4 reg4.<20> = 1 decode data
0x30D7F202,					//5
	0x30303D02,//---0x30303D82,				//6	030H,030H,03DH,082H
	0x00000092,				//7
	
0x00000000,					//8
0x1F500418,					//	9	
0xFF010000,					//	10
0x04143041,					//	11
	0x10800140,				// 
0x20000280,					//	13
0xCD4C5555,					//	14
//---0xfd93620f			//	15
//---0xf193620f			//	test mode normal
0xf1A3620f					//	rx 512 ; tx 256
};


unsigned char  Bk5823TX_Reg17[]=
{
// 0x00,0x00,0x00,0x20,0x40,0x00,0x85,0x48,0x31,0x1A,0x8D,0x46,0xA3,0xD1
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x28,0x44,0x82,0xC5
// 	0x00,0x04,0x04,0x88,0x84,0x62,0xC1,0x68,0x38,0x7E,0xBE,0xE7,0xDF,0xFF
};

//调制深度;测试达到92%左右
unsigned char Bk5823TX_Reg18[]=
{
// 0x00,0x82,0x20,0x10,0x04,0xFE,0x20,0x44,0x10,0xC4,0x0F
	//0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
	//db	  041H,010H,004H,0C1H,0FFH,0FFH,0FFH,01FH,004H,001H,00CH
	0x41,0x10,0x04,0xC1,0xFF,0xFF,0xFF,0x1F,0x04,0x01,0x0C
};

void bk5823_delay(void)
{
		int	 i;
		for(i=0;i<1000;i++);
}

uint8_t BK5823TX_SPI_RW(uint8_t _UINT8)
{
    uint8_t bit_ctr;
	//OS_CPU_SR cpu_sr;
	
	//OS_ENTER_CRITICAL();
    for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // output 8-bit
    {
        if(_UINT8 & 0x80)
            BK_TX_MOSI_High ;         // output 'UINT8', MSB to MOSI
        else
            BK_TX_MOSI_Low ;         // output 'UINT8', MSB to MOSI

        _UINT8 = (_UINT8 << 1);           // shift next bit into MSB..
        BK_TX_SCK_High;                      // Set SCK high..
        _UINT8 |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);                 // capture current MISO bit
        BK_TX_SCK_Low;                      // ..then set SCK low again
    }
	//OS_EXIT_CRITICAL();
    return(_UINT8);                     // return read UINT8
}
/**************************************************
Function: SPI_Write_Buf();
Description:
  Writes contents of buffer '*pBuf' to BK5822
  Typically used to write TX payload, Rx/Tx address
**************************************************/
void BK5823TX_SPI_Write(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8)
{
    uint8_t UINT8_ctr;	
	
    BK_TX_CSN_Low;                   								// Set CSN low, init SPI tranaction
    BK5823TX_SPI_RW(reg);    									// Select register to write to and read status UINT8
    for(UINT8_ctr=0; UINT8_ctr<_UINT8; UINT8_ctr++) // then write all UINT8 in buffer(*pBuf)
    {
        BK5823TX_SPI_RW(*pBuf++);
    }
    BK_TX_CSN_High;                 // Set CSN high again
	
    return;
}	

void BK5823TX_SPI_Write14k(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8)
{
    uint8_t UINT8_ctr;	
	
    BK_TX_CSN_Low;                   								// Set CSN low, init SPI tranaction
    BK5823TX_SPI_RW(reg);    									// Select register to write to and read status UINT8
    for(UINT8_ctr=0; UINT8_ctr<_UINT8; UINT8_ctr++) // then write all UINT8 in buffer(*pBuf)
    {
        BK5823TX_SPI_RW(*pBuf++);
    }
    BK_TX_CSN_High;                 // Set CSN high again
	
    return;
}

/************************BK5823发射IO配置********************************/
void  BK5823TX_SPI_Init(void)
{  	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	//PC10-->MISO 输入   PD2-->MOSI  PC11-->CLK  PC12-->CS输出		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	BK_TX_CSN_High;//CSN=1;			// Spi disable
	BK_TX_SCK_Low;//SCK=0;			// Spi clock line init high
}

/**************************************************
//
//
//
**************************************************/
uint8_t BK5823TX_SPI_Read(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8)
{
    uint8_t status,UINT8_ctr;

    BK_TX_CSN_Low;                            // Set CSN low, init SPI tranaction
    status = BK5823TX_SPI_RW(reg);               // Select register to write to and read status UINT8
	for(UINT8_ctr=0;UINT8_ctr<_UINT8;UINT8_ctr++)
    	pBuf[UINT8_ctr] = BK5823TX_SPI_RW(0);    // Perform SPI_RW to read UINT8 from BK5822
    BK_TX_CSN_High;                           // Set CSN high again

    return(status);                    // return BK5822 status UINT8
}

/*******************************************************************************
  * @函数名称	AutoASKReg
  * @函数说明   初始化BK5823
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
//RSU
void BK5823TX_AutoASKReg(void)
{
	
	char i,j;
	unsigned char WriteArr[4];

	for(i=0;i<=8;i++)//reverse
	{
		//g_reg_0_16[i]=RegArr[i];

		for(j=0;j<4;j++)
			WriteArr[j]=(Bk5823TX_Reg0_16[i]>>(8*(j) ) )&0xff;

		BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	}
	
	for(i=9;i<=15;i++)
	{
		//g_reg_0_16[i]=RegArr[i];
		for(j=0;j<4;j++)
			WriteArr[j]=(Bk5823TX_Reg0_16[i]>>(8*(3-j) ) )&0xff;

		BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	}
	
	BK5823TX_SPI_Write((BKWRITE_REG|17),&(Bk5823TX_Reg17[0]),14);
	BK5823TX_SPI_Write((BKWRITE_REG|18),&(Bk5823TX_Reg18[0]),11);	

}
/**************************************************
Function: set 5823 tx by dio
description:
	set REG4<1,0>  1 or 0
	=	1	 enable
	= 0  disenable
Pwd_tx.1	TX Enable. “1h”: Normal “0h”: Used CW mode	“1h”: Normal 
Pwd_rx.0	=1 package receiving ;=0 receive data stream output from pin23 DIO 
**************************************************/
void BK5823TX_TX_En(uint8_t b_enable)
{
	uint8_t i,j;
	uint8_t WriteArr[4];
	
	//set REG4< 1 > 1 or 0
	i = 4;
	for(j=0; j<4; j++)
		WriteArr[j]=(Bk5823TX_Reg0_16[i]>>(8*(j) ) )&0xff;//reverse tx
	
 	if(!b_enable)
 	{
		WriteArr[3]|=0x02;
		WriteArr[3]|=0x01;
 	}
 	else
 	{
 		WriteArr[3]&=0xfd;
 		WriteArr[3]&=0xfe;
 	}
	
//  		WriteArr[3]|=0x02;
//  		WriteArr[3]|=0x01;

	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	i=15;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
	
	if(!b_enable)
	{
		WriteArr[1] &=0xF3;	 

	}
	else
	{
		WriteArr[1] &=0xF3; 
		WriteArr[0] |=0x0c;	 

	} 
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);		
	
	
}
/*----------------
	singal carrier test
-------------------*/
void BK5823TX_Test_Singalcarrier(uint8_t in_u8_channel,uint8_t in_u8_Level)
{	
	BK5823TX_Init(in_u8_channel,in_u8_Level);
	BK5823TX_Pwr(1);
	BK5823TX_TX_En(1);
	
	BK_TX_DIO_High;
	//BK_TX_DIO_Low;
	
//	while(1);
}


void BK5823TX_Singalcarrier(uint8_t in_u8_channel,uint8_t in_u8_Level)
{	
//	BK5823_TXPwr(in_u8_Level);
	BK5823TX_Init(in_u8_channel,7);
	BK5823TX_Pwr(1);
	BK5823TX_TX_En(1);
	
//	
//	
//	
//	BK_TX_DIO_High;
//	
//	while(1);
}

/**************************************************
Function: reset 5823
description:
	set REG15<0> 0 -> 1
**************************************************/
void	BK5823TX_RST(void)
{
	
	uint8_t m_i_reg15=15;
	uint8_t	j;
	uint8_t WriteArr[4];
	
	//set REG15<0> 1 or 0
	for(j=0;j<4;j++)
		WriteArr[j]=(Bk5823TX_Reg0_16[m_i_reg15]>>(8*(3-j)))&0xff;
	
	//set 0
	WriteArr[0]&=0xfe;
	BK5823TX_SPI_Write((BKWRITE_REG|m_i_reg15),&(WriteArr[0]),4);		
	
	bk5823_delay();
	bk5823_delay();	
	bk5823_delay();	
	
	//set	1
	WriteArr[0]|= 0x01;
	BK5823TX_SPI_Write((BKWRITE_REG|m_i_reg15),&(WriteArr[0]),4);
	
}


/**************************************************
Function: Bk5823 TX Frequency Correct
description:
	reg5<26> 1->0
**************************************************/
void	BK5823TX_FreCorrect(void)
{
	
	uint8_t m_i_reg5=5;
	uint8_t	j;
	uint8_t WriteArr[4];
	
	//set REG5<0> 1 or 0
	for(j=0;j<4;j++)
		WriteArr[j]=(Bk5823TX_Reg0_16[m_i_reg5]>>(8*j))&0xff;
	
	//set	1
	WriteArr[0] |= 0x04;
	BK5823TX_SPI_Write((BKWRITE_REG|m_i_reg5),&(WriteArr[0]),4);
	
	//set 0
	WriteArr[0] &= 0xfb;
	BK5823TX_SPI_Write((BKWRITE_REG|m_i_reg5),&(WriteArr[0]),4);
	
}

//reg9.<18,17> -> 1,0
void BK5823TX_SpeedAGC(void)
{
	uint8_t i,j;
	uint8_t WriteArr[4];
	
	//set REG15<1> 1 or 0
	i = 9;
	for(j=0;j<4;j++)
		WriteArr[j]=(Bk5823TX_Reg0_16[i]>>(8*(3-j) ) )&0xff;
	
	WriteArr[2] |= 0x04;
	WriteArr[2] &= 0xfd;
	
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}

uint8_t BK5823TimFlag_1 = 0;  // 默认关5823定时器
/**************************************************
Function: BK5823_Rx_En();
	set REG15<1> 1 or 0
Description:
**************************************************/
void BK5823TX_Rx_En(uint8_t b_enable)
{	
	uint8_t i;
	uint8_t WriteArr[4];
	
	//set REG15<1> 1 or 0
	i = 15;
//	for(j=0;j<4;j++)
//		WriteArr[j]=(Bk5823TX_Reg0_16[i]>>(8*(3-j) ) )&0xff;
	
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);	
	
	if(b_enable){
		BK5823TimFlag_1 = 1;
		WriteArr[0] |= 0x02;
		TIM_Cmd(TIM5823,ENABLE);
	}
	else{
		BK5823TimFlag_1 = 0;
		WriteArr[0] &= 0xfd;
		TIM_Cmd(TIM5823,DISABLE);
	}
	
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}

/**************************************************
Function: BK5823TX_RXTx_Reset
description:
	reg6<31> 
						= 1 reset
						= 0 normal
**************************************************/
void BK5823TX_RXTx_Reset(uint8_t b_enable)
{
	
	uint8_t i,j;
	uint8_t WriteArr[4];
	
	//set REG4< 1 > 1 or 0
	i = 6;
	for(j=0; j<4; j++)
		WriteArr[j]=(Bk5823TX_Reg0_16[i]>>(8*(j) ) )&0xff;//reverse tx
	
	if(b_enable)
	{
		WriteArr[0]|=0x80;
	}
	else
	{
		WriteArr[0]&=0x7f;
	}

	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}
/**************************************************
Function: BK5823TX_14K
description:
	reg15<10> 
						= 1 dis
						= 0 enable 14K
**************************************************/
void BK5823TX_14K(uint8_t b_enable)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i=15;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
	
	//WriteArr[1] &=0xFb;
	//BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	
	if(b_enable)
	{
		WriteArr[1] |=0x20;	//13	
		WriteArr[1] &=0xe7;	//12 	11
		WriteArr[1] |=0x04;	//10	14k
		
	}
	else
		WriteArr[1] &=0xFb;
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
}

/**************************************************
Function: BK5823TX_WakeUPEnd
description:
	reg9<10>  0->1->0
	&
	close	pwr
**************************************************/
void	BK5823TX_WakeUPEnd(void)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 9;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
	
	WriteArr[1] &= 0xFb;
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	
	WriteArr[1] |= 0x04;
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
	
	WriteArr[1] &= 0xFb;
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
	
	WriteArr[0] &= 0xFE;
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}
/**************************************************
Function: BK5823TX_WakeUPEnd
description:
	reg9<0>  
		=1 idle
		=0 sleep
**************************************************/
void	BK5823TX_Pwr(uint8_t b_enable)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 9;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
	
	if (b_enable)
		WriteArr[0] |= 0x01;
	else
		WriteArr[0] &= 0xFE;

	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
}

uint8_t	BK5823TX_CheckReg9To18(void)
{

	uint8_t i,j,k,u;
	uint8_t WriteArr[4];
	
	//compare reg9~reg14
	for(i=9;i<15;i++)
	{
			BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
			for(j=0;j<4;j++)
			{
					k = (Bk5823TX_Reg0_16[i]>>((3-j)*8))&0xFF   ;
					u= WriteArr[j];
					//if( WriteArr[j]!= k)
					if( u != k)
						break;
			}
			if(j!=4)break;
	}
	if(i==15)
		return 0;
	else
		return 1;
}

void BK5823TX_Tim_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;// 定义用于初始化的结构体 
	TIM_OCInitTypeDef  BK5823_OCInitStructure;
	uint16_t BK5823_PrescalerValue = 60000-1;

//	TIM_InternalClockConfig(PSAM_TIMX);//    配置定时器3的内部时钟源(启用) 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5823,ENABLE);// 使能TIM3定时器的时钟

	// 定时器基本配置 
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
	TIM_TimeBaseStructure.TIM_Prescaler = BK5823_PrescalerValue; 

	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5823,&TIM_TimeBaseStructure);
	
	// Prescaler configuration
	TIM_PrescalerConfig(TIM5823, BK5823_PrescalerValue, TIM_PSCReloadMode_Immediate);
	
	//Output Compare Timing Mode configuration: Channel1 
	BK5823_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	BK5823_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
	BK5823_OCInitStructure.TIM_Pulse = 0;
	BK5823_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM5823, &BK5823_OCInitStructure);
	TIM_OC1PreloadConfig(TIM5823, TIM_OCPreload_Disable);
	
	//	TIM_Cmd(PSAM_TIMX,ENABLE);
  // 清除溢出中断标志 //
  TIM_ClearFlag(TIM5823,TIM_FLAG_CC1);
  
	// 禁止ARR预装载缓冲器 //
  TIM_ARRPreloadConfig(TIM5823,DISABLE);
	
  //preset comparer ,the initial value is 1
	TIM5823->SR = (uint16_t)~TIM_FLAG_CC1;		// clear the flag
	TIM5823->CNT = 0; //PSAM_TIMX->CCR1-Baud_9600S;
	TIM5823->CCR1 = 2-1;
	TIM_Cmd(TIM5823,DISABLE);	
}

//收发合为一体 初始化BK5823
uint8_t		BK5823TX_Init(uint8_t in_u8_channel,uint8_t in_u8_Level)
{
	
	uint8_t m_i_rv;	
	//CLR	Pin_5823BK_SCLK
	//SETB	Pin_5823BK_CS	
	BK5823TX_PinIni();
	BK5823TX_SPI_Init();
	
	BK_TX_CSN_High;//CSN=1;			// Spi disable
	BK_TX_SCK_Low;//SCK=0;			// Spi clock line init high
	
	BK5823TX_RST();
	BK5823TX_AutoASKReg();
	//BK5823TX_RXTx_Reset(0);//normal
	BK5823TX_FreCorrect();
	m_i_rv = BK5823TX_CheckReg9To18();
//	BK5823TX_Tim_Config();
	
	BK5823TX_CHANGECHANNEL(in_u8_channel);
	BK5823TX_ConfigPower(in_u8_Level);
//	BK5823TX_Sleep();			//HL
	return m_i_rv;
	
}

void	BK5823TX_Sleep(void)
{				
		BK5823TX_Rx_En(0);
 		BK5823TX_TX_En(0);
		BK5823TX_RST();
		bk5823_delay();	
		//设置快速AGC
		//reg9.<18,17> -> 10
		//BK5823TX_SpeedAGC();
		BK5823TX_RST();
		bk5823_delay();
		BK5823TX_RST();
		bk5823_delay();	
		//BK5823TX_RXTx_Reset(0);//normal
		BK5823TX_WakeUPEnd();
		BK5823TX_Pwr(0);
// 		//close power 
		
}


/************************BK5823发射IO配置********************************/
void  BK5823TX_PinIni(void)
{  		
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;		//PB7--DCK		PB5--DIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;		//Pb9---LEE_EN		Pb8--sky_EN
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	TX_CtrlClose();
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;					//IRQ -- PB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}



/**************************************************
Function: BK5823TX_CHANGECHANNEL
Paramter
	in_u8_Channel
						signal channel
						= 0  	5.83 G
						= 1		5.84 G
return value
 
**************************************************/
 
void BK5823TX_CHANGECHANNEL(uint8_t in_u8_Channel)
{
	
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 1;
	
	if (in_u8_Channel)
	{
		WriteArr[0] =0xEE;
		WriteArr[1] =0x3B;
		WriteArr[2] =0x68;//WriteArr[2] =0x68;
		WriteArr[3] =0x60;				

		
	}
	else
	{		
		
		WriteArr[0] =0xED;
		WriteArr[1] =0x3B;
		WriteArr[2] =0x4E;
		WriteArr[3] =0x40;
	}
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	

}


void BK5823TX_ConfigPower(uint8_t in_u8_Level)
{
	
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 9;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
	
	//reg9 1~3
	WriteArr[0] &= 0xF1;
 	WriteArr[0] |= (in_u8_Level&0x07)<<1;

 	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	

}


/**************************************************
Function: BK5823RX_Rx_ClrRX
description: clr rx and so on
	set reg15<27~24> 1111B
**************************************************/
void BK5823TX_ClrRX(void)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 15;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
	
	WriteArr[3] |= 0x0F;
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}


void	BK5823TX_Type(uint8_t TxMode)
{
	uint8_t i,j;
	uint8_t WriteArr[4];
	
	//set REG4< 1 > 1 or 0
	i = 4;
	for(j=0; j<4; j++)
		WriteArr[j]=(Bk5823TX_Reg0_16[i]>>(8*(j) ) )&0xff;//reverse tx
 	if(TxMode==NormalMode)
 	{
		WriteArr[3]|=0x03;//正常模式
		TX_CtrlClose();
 	}
 	else
 	{
 		WriteArr[3]&=0xfc;//used CW mode
		TX_CtrlOpen();
 	}

	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	for(j=0; j<4; j++)
		WriteArr[j]=(Bk5823TX_Reg0_16[i]>>(8*(j) ) )&0xff;//reverse tx

	i=15;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
	if(TxMode==NormalMode)
	{
		WriteArr[1] &=0xF3;	 
		WriteArr[0] &=0xF3;
	}
	else if(TxMode==CarriterMode)
	{
		WriteArr[1] &=0xF3; 
		WriteArr[0] |=0x0c;	 

	} 
	else if(TxMode==AllZeroMode)
	{
		WriteArr[1] &=0xF3; 
		WriteArr[0] &=0xFB;
		WriteArr[0] |=0x04;	
	}
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);		
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
}

