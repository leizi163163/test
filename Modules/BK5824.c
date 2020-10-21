#include "BK5823Code.h"
#include "BK5824.h"
uint32_t Bk5824_Reg0_16[] = 
{
////RSU TX 5824
////0~8
//0x800FFF71,			//0x806FFF7B,//0
//0x806FFF1B,
0x804FFF71,
0x00F20F76,			//0x00B2CE4C,//1
0x56002060,//2
0xFA130000,			//0xCA1F0000,//3
	
0x5008FB65,//4
0x55959C76,//5
0xab4ae675,//6
0x70B61380,//7
0x80DD1D14,//8

//9~15
0x1f70DC18,//9
0x00000000,//10
0x22000000,//11
0x20000280,//12
0x30c03400,//13
0xCD4C5555,//14
0xF3273200 //15
};
uint8_t  BK5824_Reg17[]=
{
	 0x00,0x01,0x48,0x84,0x52,0x01,0x85,0x60,0x31,0x5A,0x0D,0x97,0xD3,0xED
};
//uint8_t BK5824_Reg18[]=
//{
//	0x38,0x8E,0xE3,0x00,0x00,0x00,0x00,0x00,0xE0,0x38,0x0E
//	//0xFF,0xFF,0xff,0x00,0x00,0x00,0x00,0x00,0xfc,0xff,0x0f
//};
uint8_t BK5824_Reg18[]=
{
//	0x38,0x8E,0xE3,0x00,0x00,0x00,0x00,0x00,0xE0,0x38,0x0E			//-->38
	0x3C,0xCF,0xF3,0x00,0x00,0x00,0x00,0x00,0xF0,0x3C,0x0F				//-->3C
//	0xFF,0xFF,0xff,0x00,0x00,0x00,0x00,0x00,0xfc,0xff,0x0f        //-->3F
};
//初始化5824寄存器
void BK5824_AutoASKReg()
{
	
	char i,j;
	uint8_t WriteArr[24];
	for(i=0;i<=8;i++)//reverse
	{
		for(j=0;j<4;j++)
			WriteArr[j]=(Bk5824_Reg0_16[i]>>(8*(j) ) )&0xff;
		
		BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	}
	
	for(i=9;i<=15;i++)
	{
		for(j=0;j<4;j++)
			WriteArr[j]=(Bk5824_Reg0_16[i]>>(8*(3-j) ) )&0xff;
		
		BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	}
	
	i=0;
	WriteArr[i++]=0x00;
	WriteArr[i++]=0x00;
	WriteArr[i++]=0x00;
	WriteArr[i++]=0x00;
	BK5823TX_SPI_Write((BKWRITE_REG|17),&(WriteArr[0]),4);	
	
	BK5823TX_SPI_Write((BKWRITE_REG|17),&(BK5824_Reg17[0]),14);
	
	BK5823TX_SPI_Write((BKWRITE_REG|18),&(BK5824_Reg18[0]),11);	
	
	i=0;
	WriteArr[i++]=0x00;
	WriteArr[i++]=0x00;
	WriteArr[i++]=0x00;
	WriteArr[i++]=0x00;
	BK5823TX_SPI_Write((BKWRITE_REG|19),&(WriteArr[0]),4);
	
	i = 0;
	WriteArr[i++]=0x00;
	WriteArr[i++]=0x00;
	WriteArr[i++]=0x00;
	WriteArr[i++]=0x00;
	BK5823TX_SPI_Write((BKWRITE_REG|20),&(WriteArr[0]),4);	

	i = 0;
	WriteArr[i++]=0x76;
	WriteArr[i++]=0x00;
	WriteArr[i++]=0x55;
	WriteArr[i++]=0x55;
	BK5823TX_SPI_Write((BKWRITE_REG|24),&(WriteArr[0]),4);	

}
//设置发送信道
void BK5824_CHANGECHANNEL(uint8_t in_u8_Channel)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 5;
	if(in_u8_Channel)
	{
		WriteArr[0] = 0x76;
		WriteArr[1] = 0xD0;
		WriteArr[2] = 0xAA;
		WriteArr[3] = 0xAB; 
	}
	else
	{
		WriteArr[0] = 0x76;
		WriteArr[1] = 0x9C;
		WriteArr[2] = 0x95;
		WriteArr[3] = 0x55;
	}
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
}
//设置发射功率
void BK5824_ConfigPower(uint8_t in_u8_Level)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	if(in_u8_Level > 15)
	{
		in_u8_Level = 15;
	}
	
	i = 9;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
	//reg9 1~3
	WriteArr[0] &= 0xE1;
	WriteArr[0] |= (in_u8_Level&0x0F)<<1;
	
 	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
}
//BK5824微调延时
void bk5824_rst_delay(void)
{
	unsigned int i = 1000000;
	while(i--)
		;
}
void BK5824_zb_en(uint8_t b_enable)
{
	//0-正常模式 1-载波模式
	uint8_t i,j;
	uint8_t WriteArr[4];
		//set REG1< 7 > 1 or 0
		i = 1;
		for(j=0; j<4; j++)
			WriteArr[j]=(Bk5824_Reg0_16[i]>>(8*(j) ) )&0xff;//reverse tx
		if(b_enable)
		{
			WriteArr[3]|=0x80;//tx single carry wave enable. 1: enable.
		}
		else
		{
			WriteArr[3]&=0x7F;//tx single carry wave enable. 0: disenable.
		}
		BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
		
		i = 15;
		BK5823TX_SPI_Read((i),&(WriteArr[0]),4); 
		if(b_enable)
		{
			WriteArr[0] &= 0xF3;
			WriteArr[0] |= (1<<2);
		}
		else
		{
			WriteArr[0] &= 0xF3;
		}
		BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
}
void BK5824_all0_pn9_mode(unsigned char _all0_pn9)
{
	//0-all0模式 1-pn9模式
	uint8_t i,j;
	uint8_t WriteArr[4];
	
	//set REG1< 7 > 0
	i = 1;
	for(j=0; j<4; j++)
		WriteArr[j]=(Bk5824_Reg0_16[i]>>(8*(j) ) )&0xff;//reverse tx
	WriteArr[3]&=0x7F;//tx single carry wave enable. 0: disenable.
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	
	i = 15;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4); 
	if(_all0_pn9)//reg15 <3:2> = 1 PN9码发送
	{
		WriteArr[0] &= 0xF3;
		WriteArr[0] |= 0x04;
	}
	else//reg15 <3:2> = 2 all0码发送
	{
		WriteArr[0] &= 0xF3;
		WriteArr[0] |= 0x08;
	}
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
	
	/*
	i = 15;//all1 reg15<3:2>=2,reg15<12:11>=0
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4); 
	WriteArr[0] &= 0xF3;
	WriteArr[0] |= 0x08;
	WriteArr[1] &= 0xE7;
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
	*/
}
void BK5824BstMode(void)
{
	//设置BK5824位于常发BST模式
	u32 timeoutTx;
//	u8 writereg[4]={0};
	unsigned char _bk5823Irq = 1;
	unsigned char _bst[] = {0xFF,0xFF,0xFF,0xFF,0x50,0x03,0x91,0xC0,0x00,0x00,0x00,0x42,0x5C,0x9D,0xBC,0x6F,0x01,0x01,0x41,0xA5,0x29,0x20,0x2C,0x00,0x2B,0x00};
	BK5824_Rx_En(0);//设置空闲
	BK5824_14K_Enable(1);//打开14K方波使能
	ClearBk5824Irq();//清中断
	while(1)
	{
		ClearBk5824Irq();
		_bk5823Irq = 1;
		BK5823TX_SPI_Write(((BKWRITE_REG|16)), (unsigned char*)_bst, sizeof(_bst));//装载BST
		//USER_Delay1us(60);
		TX_CtrlOpen();					//打开放大器
		timeoutTx=0xffffffff;
		while(timeoutTx--)
		{
			_bk5823Irq = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
			if(!_bk5823Irq)//发送完成
			{
				TX_CtrlClose();			//关放大器
				break;
			}
		}
		TX_CtrlClose();
		USER_Delay1ms(10);
		if(ReceiveOverFlag == 1)
			break;
	}
	BK5824_14K_Enable(0);
}
//设置工作模式：0-正常模式 1-载波模式 2-发送全0码 3-发送PN9码 4-BST模式
void BK5824ModeSet(uint8_t _BK5824Mode)
{
	switch(_BK5824Mode)
	{
		case 0:
			BK5824_zb_en(0);//正常工作模式
			break;
		case 1:
			BK5824_zb_en(1);//载波模式
			break;
		case 2:
			BK5824_all0_pn9_mode(0);
			break;
		case 3:
			BK5824_all0_pn9_mode(1);
			break;
		case 4:
			BK5824BstMode();
		default:
			break;
	}
		
}
//设置5824接收使能
void BK5824_Rx_En(uint8_t b_enable)
{	
	uint8_t i;
	uint8_t WriteArr[4];
	
	//set REG15<1> 1 or 0
	i = 15;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4);
	if(b_enable)
	{
		WriteArr[0] |= 0x02;
 	}
	else
	{
		WriteArr[0] &= 0xfd;
 	}
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}
//设置5824波特率
void BK5824_Baudrate(void)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 15;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4); 
	WriteArr[1] &= 0xE7;//256K

	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
}
//设置BK5824为空闲状态
void BK5824PowerUp(void)
{
	uint8_t i;
	uint8_t WriteArr[4];
	USER_Delay1ms(1);
	i = 9;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4); 
	WriteArr[0]  |= 0x01;
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
	USER_Delay1ms(1);
}
//BK5824初始化
void BK5824Init(void)
{
	bk5824_rst_delay();//延时，等待5824稳定工作
	BK5823TX_PinIni();
	BK5823TX_SPI_Init();
	BK_TX_CSN_High;//CSN=1;			// Spi disable
	BK_TX_SCK_Low;//SCK=0;			// Spi clock line init high
	
	USER_Delay1ms(4);
	BK5824_AutoASKReg();//配置5824寄存器 reg0~8为高字节在前、低字节在后 reg9~15为低字节在前、高字节在后 每个寄存器的byet中bit均为高字节在前、低字节在后
	BK5824_CHANGECHANNEL(PC_InitStructure.PLLChannelID);//设置信道0:5830 1:5840 
	BK5824_ConfigPower(15);//设置功率
	//BK5824_CHANGECHANNEL(0);
	//BK5824_ConfigPower(7);
	BK5824_Rx_En(0);
}
//设置5824 14K方波使能
void BK5824_14K_Enable(unsigned char enable)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 15;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4); 
	if(enable)
	{		
		WriteArr[0] |=0xc0;	//13	
		WriteArr[1] |=0x03;	//12 	11
		WriteArr[1] |=0x04;	//10	14k		打开14K方波功能
	}
	else
		WriteArr[1] &=0xFb; 
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
}
//清BK5824中断
void ClearBk5824Irq(void)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 15;
	BK5823TX_SPI_Read((i),&(WriteArr[0]),4); 
	WriteArr[3] |= 0x07;
	BK5823TX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
}

