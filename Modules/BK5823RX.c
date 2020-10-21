#include "stm32f10x.h"


unsigned long BK5823RX_Reg0_16[]=
{
/*低位在前，高位在后*/
/*低位在前，高位在后*/
/*低位在前，高位在后*/
0xC2BE5557,					//0	5.795		57 55 BE C2
//	0xA2E45558,					//5.805		信道1
//0XE2985556,							//5.785		信道0
0x404E3BED,					//1	5.83G 	ED 3B 4E 40
0x2000F009,					//2
0x603B0031,					//3
	//---0x3F0F0C38,				//4
	0x3F0F1C38,				//4 reg4.<20> = 1 decode data	
0x30D7F202,					//5
	0x30303D02,//---0x30303D82,				//6	030H,030H,03DH,082H
	0x00000092,				//7
0x00000000,					//8
	0x1F500418,					//9	
0xFF010000,					//10
0x04143041,					//11
	0x10800140,
0x20000280,					//13
0xCD4C5555,					//14
//---0xfd93620f			//15
//---0xf193620f			//test mode normal
0xf1A3620f					//rx 512 ; tx 256
};


// 0x00,0x00,0x00,0x20,0x40,0x00,0x85,0x48,0x31,0x1A,0x8D,0x46,0xA3,0xD1		//高灵敏度

unsigned char  BK5823RX_Reg17[]=
{
//		0x00,0x00,0x00,0x20,0x40,0x00,0x85,0x48,0x31,0x1A,0x8D,0x46,0xA3,0xD1	//高灵敏度
//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x28,0x44,0x82,0xC5    // 监听单元的配置	低灵敏度
//		0x00,0x04,0x04,0x88,0x84,0x62,0xC1,0x68,0x38,0x7E,0xBE,0xE7,0xDF,0xFF   // 天线的配置
//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x28,0x14,0x0A,0x85   // OBU的配置
	
	//		08	02	00	00	00	00	00	00	00	00	00	00	00	00	00	00		//6m
	//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10
	
	//		40	08	02	00	00	00	00	00	00	00	00	00	00	00	00	00		//10m
	//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x20,0x80
	
	//		42	40	08	02	00	00	00	00	00	00	00	00	00	00	00	00		//16m
			0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x40,0x00,0x85
	
	//		48	42	40	08	02	00	00	00	00	00	00	00	00	00	00	00		//23m
	//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x80,0x00,0x0A,0x91
	
	//20190227配置低中高
	//低：4A 48 42 40 8 2 0 0 0 0 0 0 0 0 0 0	： 	9522140100800000000000000000
//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0x14,0x22,0x95
	//低：60 48 42 40 8 2 0 0 0 0 0 0 0 0 0 0 ：	C122140100800000000000000000
//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0x14,0x22,0xc1
	//中：6A 68 62 60 48 42 40 8 2 0 0 0 0 0 0 0:	D5A3160910A00804000000000000
//		0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0xa0,0x10,0X09,0x16,0xa3,0xd5
	//中：54 52 50 4A 48 42 40 8 2 0 0 0 0 0 0 0:	A94A84A910A00804000000000000
//		0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x08,0xA0,0x10,0xA9,0x84,0x4A,0xA9
	//高：76 74 72 70 6A 68 62 60 48 42 40 8 2 0 0 0:	EDD3970D5A3160910A0080400000
//		0x00,0x00,0x40,0x80,0x00,0x0A,0x91,0x60,0x31,0x5A,0x0D,0x97,0xD3,0xED
	//高：76 74 6E 56 54 52 50 4A 48 42 40 8 2 0 0 0：EDD3756A94A84A910A0080400000
//		0x00,0x00,0x40,0x80,0x00,0x0A,0x91,0x4A,0xA8,0x94,0x6A,0x75,0xD3,0xED

	//20190227,校准
//	0x02,0x81,0x40,0x20,0x10,0x08,0x04,0x02,0x81,0x40,0x20,0x10,0x08,0x04	//2H	06(不交易)	*
//	0x08,0x04,0x02,0x81,0x40,0x20,0x10,0x08,0x04,0x02,0x81,0x40,0x20,0x10	//8H	06	*
//	0x0A,0x85,0x42,0xA1,0x50,0x28,0x14,0x0A,0x85,0x42,0xA1,0x50,0x28,0x14	//AH	0a	0c
//	0x20,0x10,0x08,0x04,0x02,0x81,0x40,0x20,0x10,0x08,0x04,0x02,0x81,0x40	//20H 0a 0c 
//	0x40,0x20,0x10,0x08,0x04,0x02,0x81,0x40,0x20,0x10,0x08,0x04,0x02,0x81	//40H	0a 0c	*
//	0x22,0x91,0x48,0x24,0x12,0x89,0x44,0x22,0x91,0x48,0x24,0x12,0x89,0x44	//22h	0e
//	0x42,0xA1,0x50,0x28,0x14,0x0A,0x85,0x42,0xA1,0x50,0x28,0x14,0x0A,0x85	//42H	0e 0c 10	*
//	0xC3,0xE1,0x70,0x38,0x1C,0x0E,0x87,0xC3,0xE1,0x70,0x38,0x1C,0x0E,0x87	//43H	0e
//	0xa3,0xd1,0x68,0x34,0x1a,0x8d,0x46,0xa3,0xd1,0x68,0x34,0x1a,0x8d,0x46	//23H 0e
//	0x9b,0xcd,0x66,0xb3,0xd9,0x6c,0x36,0x9b,0xcd,0x66,0xb3,0xd9,0x6c,0x36	//1bH 10 11
//	0x1A,0x8D,0x46,0xA3,0xD1,0x68,0x34,0x1A,0x8D,0x46,0xA3,0xD1,0x68,0x34	//1AH	11
//	0x93,0xC9,0x64,0x32,0x99,0x4C,0x26,0x93,0xC9,0x64,0x32,0x99,0x4C,0x26	//13H	11
//	0x48,0x24,0x12,0x89,0x44,0x22,0x91,0x48,0x24,0x12,0x89,0x44,0x22,0x91	//48H	11 12	*
//	0xC9,0x64,0x32,0x99,0x4C,0x26,0x93,0xC9,0x64,0x32,0x99,0x4C,0x26,0x93	//49H	11 12
//	0xa9,0x54,0x2a,0x95,0x4a,0xa5,0x52,0xa9,0x54,0x2a,0x95,0x4a,0xa5,0x52	//29H 11
//	0xa5,0x52,0xa9,0x54,0x2a,0x95,0x4a,0xa5,0x52,0xa9,0x54,0x2a,0x95,0x4a	//25H 11 12
//	0x4A,0xA5,0x52,0xA9,0x54,0x2A,0x95,0x4A,0xA5,0x52,0xA9,0x54,0x2A,0x95	//4AH	13 14 15 16	*
//	0x95,0x4A,0xA5,0x52,0xA9,0x54,0x2A,0x95,0x4A,0xA5,0x52,0xA9,0x54,0x2A	//15H	14 15 13 
//	0x2A,0x95,0x4A,0xA5,0x52,0xA9,0x54,0x2A,0x95,0x4A,0xA5,0x52,0xA9,0x54	//2AH	16 15 17
//	0x46,0xA3,0xD1,0x68,0x34,0x1A,0x8D,0x46,0xA3,0xD1,0x68,0x34,0x1A,0x8D	//46H	16 15 17
//	0x26,0x93,0xc9,0x64,0x32,0x99,0x4c,0x26,0x93,0xc9,0x64,0x32,0x99,0x4c	//26H 16
//	0x60,0x30,0x18,0x0C,0x06,0x83,0xC1,0x60,0x30,0x18,0x0C,0x06,0x83,0xC1	//60H	17 	*
//	0xab,0xd5,0x6a,0xb5,0x5a,0xad,0x56,0xab,0xd5,0x6a,0xb5,0x5a,0xad,0x56	//2bH 15 16
//	0xC7,0xE3,0xF1,0x78,0x3C,0x1E,0x8F,0xC7,0xE3,0xF1,0x78,0x3C,0x1E,0x8F	//47H	15 16
//	0x16,0x8B,0xC5,0x62,0xB1,0x58,0x2C,0x16,0x8B,0xC5,0x62,0xB1,0x58,0x2C	//16H	1a 19 1b 1c
//	0xB9,0x5C,0x2E,0x97,0xCB,0xE5,0x72,0xB9,0x5C,0x2E,0x97,0xCB,0xE5,0x72	//39H	1a 1b 1c
//	0x50,0x28,0x14,0x0A,0x85,0x42,0xA1,0x50,0x28,0x14,0x0A,0x85,0x42,0xA1	//50H	1a 1b 1c	*
//	0x62,0xB1,0x58,0x2C,0x16,0x8B,0xC5,0x62,0xB1,0x58,0x2C,0x16,0x8B,0xC5	//62H	1d 1c	*
//	0xB1,0x58,0x2C,0x16,0x8B,0xC5,0x62,0xB1,0x58,0x2C,0x16,0x8B,0xC5,0x62	//31H 1b
//	0x52,0xA9,0x54,0x2A,0x95,0x4A,0xA5,0x52,0xA9,0x54,0x2A,0x95,0x4A,0xA5	//52H	1f 1e	*
//	0xaf,0xd7,0xeb,0xf5,0x7a,0xbd,0x5e,0xaf,0xd7,0xeb,0xf5,0x7a,0xbd,0x5e	//2fH 1f 20 
//	0x3A,0x9D,0x4E,0xA7,0xD3,0xE9,0x74,0x3A,0x9D,0x4E,0xA7,0xD3,0xE9,0x74	//3AH	1f 20 
//	0x68,0x34,0x1A,0x8D,0x46,0xA3,0xD1,0x68,0x34,0x1A,0x8D,0x46,0xA3,0xD1	//68H	20 21
//	0xB3,0xD9,0x6C,0x36,0x9B,0xCD,0x66,0xB3,0xD9,0x6C,0x36,0x9B,0xCD,0x66	//33H	1f 20
//	0x68,0x34,0x1A,0x8D,0x46,0xA3,0xD1,0x68,0x34,0x1A,0x8D,0x46,0xA3,0xD1	//68H	20	*
//	0x54,0x2A,0x95,0x4A,0xA5,0x52,0xA9,0x54,0x2A,0x95,0x4A,0xA5,0x52,0xA9	//54H	23 22 24	*
//	0x3C,0x1E,0x8F,0xC7,0xE3,0xF1,0x78,0x3C,0x1E,0x8F,0xC7,0xE3,0xF1,0x78	//3cH	23 24 
//	0x6A,0xB5,0x5A,0xAD,0x56,0xAB,0xD5,0x6A,0xB5,0x5A,0xAD,0x56,0xAB,0xD5	//6AH	24 	*
//	0x56,0xAB,0xD5,0x6A,0xB5,0x5A,0xAD,0x56,0xAB,0xD5,0x6A,0xB5,0x5A,0xAD	//56H	27 28	*
//	0x70,0x38,0x1C,0x0E,0x87,0xC3,0xE1,0x70,0x38,0x1C,0x0E,0x87,0xC3,0xE1	//70H	28 29	*
//	0xB7,0xDB,0xED,0x76,0xBB,0xDD,0x6E,0xB7,0xDB,0xED,0x76,0xBB,0xDD,0x6E	//3AH	27 28
//	0xB7,0xDB,0xED,0x76,0xBB,0xDD,0x6E,0xB7,0xDB,0xED,0x76,0xBB,0xDD,0x6E	//37H	28
//	0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0x7E,0xBF,0xDF,0xEF,0xF7,0xFB,0xFD,0x7E	//3fH 27 28
//	0x6E,0xB7,0xDB,0xED,0x76,0xBB,0xDD,0x6E,0xB7,0xDB,0xED,0x76,0xBB,0xDD	//6EH	2a	*
//	0x72,0xB9,0x5C,0x2E,0x97,0xCB,0xE5,0x72,0xB9,0x5C,0x2E,0x97,0xCB,0xE5	//72H	2a	*
//	0x74,0x3A,0x9D,0x4E,0xA7,0xD3,0xE9,0x74,0x3A,0x9D,0x4E,0xA7,0xD3,0xE9	//74H	2b	*
//	0x76,0xBB,0xDD,0x6E,0xB7,0xDB,0xED,0x76,0xBB,0xDD,0x6E,0xB7,0xDB,0xED	//76H	2b	*

};

unsigned char  BK5823RX_Reg17_High[]=
{
		0x00,0x00,0x40,0x80,0x00,0x0A,0x91,0x4A,0xA8,0x94,0x6A,0x75,0xD3,0xED	//高灵敏度
//		0x00,0x00,0x00,0x20,0x40,0x00,0x85,0x48,0x31,0x1A,0x8D,0x46,0xA3,0xD1	//高灵敏度
//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x28,0x44,0x82,0xC5    // 监听单元的配置	低灵敏度
//		0x00,0x04,0x04,0x88,0x84,0x62,0xC1,0x68,0x38,0x7E,0xBE,0xE7,0xDF,0xFF   // 天线的配置
//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x28,0x14,0x0A,0x85   // OBU的配置
	
};

unsigned char  BK5823RX_Reg17_Low[]=
{
//		0x00,0x00,0x00,0x20,0x40,0x00,0x85,0x48,0x31,0x1A,0x8D,0x46,0xA3,0xD1	//高灵敏度
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x28,0x44,0x82,0xC5    // 监听单元的配置	低灵敏度
//		0x00,0x04,0x04,0x88,0x84,0x62,0xC1,0x68,0x38,0x7E,0xBE,0xE7,0xDF,0xFF   // 天线的配置
//		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x28,0x14,0x0A,0x85   // OBU的配置
	
};


//调制深度;测试达到92%左右
unsigned char BK5823RX_Reg18[]=
{
// 0x00,0x82,0x20,0x10,0x04,0xFE,0x20,0x44,0x10,0xC4,0x0F
	//0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
	//db	  041H,010H,004H,0C1H,0FFH,0FFH,0FFH,01FH,004H,001H,00CH
	0x41,0x10,0x04,0xC1,0xFF,0xFF,0xFF,0x1F,0x04,0x01,0x0C
};


/************************BK5823发射IO配置********************************/
/*--
	IRQ		= e11	
	DIO		= e12
--*/ 
void  BK5823RX_PinIni(void)
{  	
	
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
}
uint8_t BK5823RX_SPI_RW(uint8_t _UINT8)
{
    uint8_t bit_ctr;
    for(bit_ctr=0;bit_ctr<8;bit_ctr++)   // output 8-bit
    {
        if(_UINT8 & 0x80)
            BK_RX_MOSI_High ;         // output 'UINT8', MSB to MOSI
        else
            BK_RX_MOSI_Low ;         // output 'UINT8', MSB to MOSI

        _UINT8 = (_UINT8 << 1);           // shift next bit into MSB..
        BK_RX_SCK_High;                      // Set SCK high..
        _UINT8 |= GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7);                 // capture current MISO bit
        BK_RX_SCK_Low;                      // ..then set SCK low again
    }

    return(_UINT8);                     // return read UINT8
}

/**************************************************
Function: SPI_Write_Buf();
Description:
  Writes contents of buffer '*pBuf' to BK5822
  Typically used to write TX payload, Rx/Tx address
**************************************************/
void BK5823RX_SPI_Write(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8)
{

    uint8_t UINT8_ctr;

    BK_RX_CSN_Low;                   // Set CSN low, init SPI tranaction
    BK5823RX_SPI_RW(reg);    // Select register to write to and read status UINT8
    for(UINT8_ctr=0; UINT8_ctr<_UINT8; UINT8_ctr++) // then write all UINT8 in buffer(*pBuf)
    {
        BK5823RX_SPI_RW(*pBuf++);
    }

    BK_RX_CSN_High;                 // Set CSN high again

    return;		
}	

/************************BK5823接收IO配置********************************/
void  BK5823RX_SPI_Init(void)
{  	
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

//	//PE7-->MISO 输入   PE8-->MOSI  PE9-->CLK  PE10-->CS输出		PE11-->IRQ输入	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);	
//	
//	BK_TX_CSN_High;//CSN=1;			// Spi disable
//	BK_TX_SCK_Low;//SCK=0;			// Spi clock line init high
}

/**************************************************
//
**************************************************/
uint8_t BK5823RX_SPI_Read(uint8_t reg, uint8_t *pBuf, uint8_t _UINT8)
{
    uint8_t status,UINT8_ctr;

    BK_RX_CSN_Low;                            // Set CSN low, init SPI tranaction
    status = BK5823RX_SPI_RW(reg);               // Select register to write to and read status UINT8
		for(UINT8_ctr=0;UINT8_ctr<_UINT8;UINT8_ctr++)
        pBuf[UINT8_ctr] = BK5823RX_SPI_RW(0);    // Perform SPI_RW to read UINT8 from BK5822
    BK_RX_CSN_High;                           // Set CSN high again

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
void BK5823RX_AutoASKReg(void)
{
	
	char i,j;
	unsigned char WriteArr[4];

	for(i=0;i<=8;i++)//reverse
	{
		//g_reg_0_16[i]=RegArr[i];

		for(j=0;j<4;j++)
			WriteArr[j]=(BK5823RX_Reg0_16[i]>>(8*(j) ) )&0xff;

		BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	}
	
	for(i=9;i<=15;i++)
	{
		//g_reg_0_16[i]=RegArr[i];
		for(j=0;j<4;j++)
			WriteArr[j]=(BK5823RX_Reg0_16[i]>>(8*(3-j) ) )&0xff;

		BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	}
	
	BK5823RX_SPI_Write((BKWRITE_REG|17),&(BK5823RX_Reg17[0]),14);
	BK5823RX_SPI_Write((BKWRITE_REG|18),&(BK5823RX_Reg18[0]),11);	

}

void BK5823RX_AGC_Config(uint8_t agc_config)
{
	if(agc_config == 1)
	{
		BK5823RX_SPI_Write((BKWRITE_REG|17),&(BK5823RX_Reg17_High[0]),14);
	}
	else
	{
		BK5823RX_SPI_Write((BKWRITE_REG|17),&(BK5823RX_Reg17_Low[0]),14);
	}
}
/**************************************************
Function: set 5823 tx en
description:
	set REG4<1,0>  1 or 0
	=	1	 enable
	= 0  disenable
Pwd_tx.1	TX Enable. “1h”: Normal “0h”: Used CW mode	“1h”: Normal 
Pwd_rx.0	=1 package receiving ;=0 receive data stream output from pin23 DIO 
**************************************************/
void BK5823RX_TX_En(uint8_t b_enable)
{
	uint8_t i,j;
	uint8_t WriteArr[4];
	
	//set REG4< 1 > 1 or 0
	i = 4;
	for(j=0; j<4; j++)
		WriteArr[j]=(BK5823RX_Reg0_16[i]>>(8*(j) ) )&0xff;//reverse tx
	
//  	if(!b_enable)
//  	{
// 		WriteArr[3]|=0x02;
// 		WriteArr[3]|=0x01;
//  	}
//  	else
//  	{
//  		WriteArr[3]&=0xfd;
//  		WriteArr[3]&=0xfe;
//  	}
 		WriteArr[3]|=0x02;
 		WriteArr[3]|=0x01;
		//WriteArr[3]&=0xfe;
		//WriteArr[3]&=0xfd;
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}
/**************************************************
Function: reset 5823
description:
	set REG15<0> 0 -> 1
**************************************************/
void	BK5823RX_RST(void)
{
	
	uint8_t m_i_reg15=15;
	uint8_t	j;
	uint8_t WriteArr[4];
	
	//set REG15<0> 1 or 0
	for(j=0;j<4;j++)
		WriteArr[j]=(BK5823RX_Reg0_16[m_i_reg15]>>(8*(3-j)))&0xff;
	
	//set 0
	WriteArr[0]&=0xfe;
	BK5823RX_SPI_Write((BKWRITE_REG|m_i_reg15),&(WriteArr[0]),4);		
	
	bk5823_delay();
	bk5823_delay();	
	bk5823_delay();	
	
	//set	1
	WriteArr[0]|= 0x01;
	BK5823RX_SPI_Write((BKWRITE_REG|m_i_reg15),&(WriteArr[0]),4);
	
}


/**************************************************
Function: Bk5823 TX Frequency Correct
description:
	reg5<26> 1->0
**************************************************/
void	BK5823RX_FreCorrect(void)
{
	
	uint8_t m_i_reg5=5;
	uint8_t	j;
	uint8_t WriteArr[4];
	
	//set REG5<0> 1 or 0
	for(j=0;j<4;j++)
		WriteArr[j]=(BK5823RX_Reg0_16[m_i_reg5]>>(8*j))&0xff;
	
	//set	1
	WriteArr[0] |= 0x04;
	BK5823RX_SPI_Write((BKWRITE_REG|m_i_reg5),&(WriteArr[0]),4);
	
	//set 0
	WriteArr[0] &= 0xfb;
	BK5823RX_SPI_Write((BKWRITE_REG|m_i_reg5),&(WriteArr[0]),4);
	
}

//reg9.<18,17> -> 1,0
void BK5823RX_SpeedAGC(void)
{
	uint8_t i,j;
	uint8_t WriteArr[4];
	
	//set REG15<1> 1 or 0
	i = 9;
	for(j=0;j<4;j++)
		WriteArr[j]=(BK5823RX_Reg0_16[i]>>(8*(3-j) ) )&0xff;
	
	WriteArr[2] |= 0x04;
	WriteArr[2] &= 0xfd;
	
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}

uint8_t BK5823TimFlag = 0;  // 默认关5823定时器
/**************************************************
Function: BK5823_Rx_En();
	set REG15<1> 1 or 0
Description:
**************************************************/
void BK5823RX_Rx_En(uint8_t b_enable)
{	
	uint8_t i;
	uint8_t WriteArr[4];
	
	//set REG15<1> 1 or 0
	i = 15;
// 	for(j=0;j<4;j++)
// 		WriteArr[j]=(BK5823RX_Reg0_16[i]>>(8*(3-j) ) )&0xff;

	BK5823RX_SPI_Read((i),&(WriteArr[0]),4);	
	
	if(b_enable)
	{
		BK5823TimFlag = 1;
		WriteArr[0] |= 0x02;
 		TIM_Cmd(TIM5823,ENABLE);
	}
	else
	{
		BK5823TimFlag = 0;
		WriteArr[0] &= 0xfd;
 		TIM_Cmd(TIM5823,DISABLE);	
	}
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}

/**************************************************
Function: BK5823RX_RXTx_Reset
description:
	reg6<31> 
						= 1 reset
						= 0 normal
**************************************************/
void BK5823RX_RXTx_Reset(uint8_t b_enable)
{
	
	uint8_t i,j;
	uint8_t WriteArr[4];
	
	//set REG4< 1 > 1 or 0
	i = 6;
	for(j=0; j<4; j++)
		WriteArr[j]=(BK5823RX_Reg0_16[i]>>(8*(j) ) )&0xff;//reverse tx
	
	if(b_enable)
	{
		WriteArr[0]|=0x80;
	}
	else
	{
		WriteArr[0]&=0x7f;
	}

	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}
/**************************************************
Function: BK5823RX_14K
description:
	reg15<10> 
						= 1 dis
						= 0 enable 14K
**************************************************/
void		BK5823RX_14K(uint8_t b_enable)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i=15;
	BK5823RX_SPI_Read((i),&(WriteArr[0]),4);
	
	//WriteArr[1] &=0xFb;
	//BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	
	if(b_enable)
	{
		WriteArr[1] |=0x20;	//13	
		WriteArr[1] &=0xe7;	//12 	11
		WriteArr[1] |=0x04;	//10	14k
		
	}
	else
		WriteArr[1] &=0xFb;
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
}

/**************************************************
Function: BK5823RX_WakeUPEnd
description:
	reg9<10>  0->1->0
	&
	close	pwr
**************************************************/
void	BK5823RX_WakeUPEnd(void)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 9;
	BK5823RX_SPI_Read((i),&(WriteArr[0]),4);
	
	WriteArr[1] &= 0xFb;
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
	
	WriteArr[1] |= 0x04;
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
	
	WriteArr[1] &= 0xFb;
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
	
	WriteArr[0] &= 0xFE;
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}
/**************************************************
Function: BK5823RX_WakeUPEnd
description:
	reg9<0>  
		=1 idle
		=0 sleep
**************************************************/
void	BK5823RX_Pwr(uint8_t b_enable)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i=9;
	BK5823RX_SPI_Read((i),&(WriteArr[0]),4);
	
	if (b_enable)
		WriteArr[0] |=0x01;
	else
		WriteArr[0] &=0xFE;
	
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	
}

uint8_t	BK5823RX_CheckReg9To18(void)
{
	uint8_t i,j,k,u;
	uint8_t WriteArr[4];
	
	//compare reg9~reg14
	for(i=9;i<15;i++)
	{
			BK5823RX_SPI_Read((i),&(WriteArr[0]),4);
			for(j=0;j<4;j++)
			{
					k = (BK5823RX_Reg0_16[i]>>((3-j)*8))&0xFF   ;
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

//void BK5823_SetReceiveGain(void)
//{
//	uint8_t WriteArr[4];
//	
//	WriteArr[0] = 0x38;
//	WriteArr[1] = (0x10 | (CommF0.ReceiveGain >> 4)) & 0xF7;				//&0xF7,确保19位agcen为0
//	WriteArr[2] = 0x0F | (CommF0.ReceiveGain << 4);
//	WriteArr[3] = 0x3F;
//	
//	BK5823RX_SPI_Write((BKWRITE_REG|4),&(WriteArr[0]),4);
//}

uint8_t		BK5823RX_Init(void)
{
	uint8_t m_i_rv;	
	//CLR	Pin_5823BK_SCLK
	//SETB	Pin_5823BK_CS	
	
	BK5823RX_PinIni();
	BK5823RX_SPI_Init();
	BK_RX_CSN_High;//CSN=1;			// Spi disable
	BK_RX_SCK_Low;//SCK=0;			// Spi clock line init high
	
	BK5823RX_RST();
	BK5823RX_AutoASKReg();
//	BK5823_SetReceiveGain();
	//BK5823RX_RXTx_Reset(0);//normal
	BK5823RX_FreCorrect();
	m_i_rv = BK5823RX_CheckReg9To18();		
	
	BK5823RX_Tim_Config();
// 	BK5823RX_Sleep();
	return m_i_rv;
}

void		BK5823RX_CHANGECHANNEL(uint8_t in_u8_Channel)
{
	
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 0;
	
	if (in_u8_Channel)			//5855e4a2  5.805
	{
		WriteArr[0] =0x58;
		WriteArr[1] =0x55;
		WriteArr[2] =0xe4;
		WriteArr[3] =0xa2;				

		
	}
	else									//565598e2    5.785
	{		
		
		WriteArr[0] =0x56;
		WriteArr[1] =0x55;
		WriteArr[2] =0x98;
		WriteArr[3] =0xe2;
	}
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);	

}


uint8_t		BK5823RX_02Init(uint8_t in_u8_channel)
{
	uint8_t m_i_rv;	
	//CLR	Pin_5823BK_SCLK
	//SETB	Pin_5823BK_CS	
	
	BK5823RX_PinIni();
	BK5823RX_SPI_Init();
	BK_RX_CSN_High;//CSN=1;			// Spi disable
	BK_RX_SCK_Low;//SCK=0;			// Spi clock line init high
	
	BK5823RX_RST();
	BK5823RX_AutoASKReg();
	//BK5823RX_RXTx_Reset(0);//normal
	BK5823RX_FreCorrect();
	m_i_rv = BK5823RX_CheckReg9To18();		
BK5823RX_CHANGECHANNEL(in_u8_channel);	
	BK5823RX_Tim_Config();
 	BK5823RX_Sleep();
	return m_i_rv;
}

void	BK5823RX_Sleep(void)
{			
		BK5823RX_Rx_En(0);
		BK5823RX_TX_En(0);
		BK5823RX_RST();
		bk5823_delay();	
		//设置快速AGC
		//reg9.<18,17> -> 10
// 		BK5823RX_SpeedAGC();
		BK5823RX_RST();
		bk5823_delay();
		BK5823RX_RST();
		bk5823_delay();	
		//BK5823RX_RXTx_Reset(0);//normal
		BK5823RX_WakeUPEnd();
		BK5823RX_Pwr(0);
}


 
void		BK5823RX_BST(void)
{
	uint8_t i,j;
	uint8_t WriteArr[30];
	
	i=16;
	j=0;
//FF,FF,FF,FF,50,03,91,C0,11,23,01,01,54,E6,AB,A1,00,01,01,00 
	WriteArr[j++] =0xff;
	WriteArr[j++] =0xff;
	WriteArr[j++] =0xff;
	WriteArr[j++] =0xff;
	WriteArr[j++] =0x50;
	WriteArr[j++] =0x03;
	WriteArr[j++] =0x91;
	WriteArr[j++] =0xc0;
	WriteArr[j++] =0x11;
	WriteArr[j++] =0x23;
	WriteArr[j++] =0x01;
	WriteArr[j++] =0x01;
	WriteArr[j++] =0x54;
	WriteArr[j++] =0xe6;
	WriteArr[j++] =0xab;
	WriteArr[j++] =0xa1;
	WriteArr[j++] =0x00;
	WriteArr[j++] =0x01;
	WriteArr[j++] =0x01;
	WriteArr[j++] =0x00;
 
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),j);	
}

/**************************************************
Function: BK5823RX_Rx_ClrRX
description: clr rx and so on
	set reg15<27~24> 1111B
**************************************************/
void BK5823RX_ClrRX(void)
{
	uint8_t i;
	uint8_t WriteArr[4];
	
	i = 15;
	BK5823RX_SPI_Read((i),&(WriteArr[0]),4);
	
	WriteArr[3] |= 0x0F;
	BK5823RX_SPI_Write((BKWRITE_REG|i),&(WriteArr[0]),4);
}

/**************************************************
Function: BK5823RX_Rx
description: clr rx and so on
	set reg15<27~24> 1111B
Paramter
	out_u8_rvbuf			rx data
	in_u8_overtime		unit is ms
return value
	= 0 rx over time
  = other rx len
**************************************************/
uint8_t BK5823RX_Rx(uint8_t *out_u8_rvbuf,uint8_t in_u8_overtime)
{	
	
	uint8_t WriteArr[4];
	uint8_t m_i_rv=0;
	uint8_t m_i_len;
	
	//已接收到数据，先清，防止收到上一次的数据
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)== 0)
		BK5823RX_ClrRX();
	
	//开接收使能
	BK5823RX_Rx_En(1);

	//while(con--)//in_u8_overtime
	{
		TIM5823->SR = (uint16_t)~TIM_FLAG_CC1;		// clear the flag
		TIM5823->CNT = 0; 
		TIM5823->CCR1 = in_u8_overtime-1;
	    while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11) && ((TIM5823->SR & TIM_FLAG_CC1) == (uint16_t)RESET)  ){
	    }
		/*while(con--){
			if (!GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11))
				break;
			OSTimeDlyHMSM(0, 0, 0, 1);
	    }*/
		if (GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11) ){
			return 0;
			//break;
		}
		//=== 读接收标记寄存器 ===
		//reg6.2 =1 表示接收出错[解码出错，或CRC出错这一位都为置]
		//reg6.1 = 1 表示接收到数据
		BK5823RX_SPI_Read((6),&(WriteArr[0]),4);
		if(WriteArr[0]&04)
			BK5823RX_ClrRX();
		else if(WriteArr[0]&02){
			//close rx
			BK5823RX_Rx_En(0);
			//if rx len is zero ,again rx
			BK5823RX_SPI_Read((5),&(WriteArr[0]),4);
			m_i_len = WriteArr[0];
			if (m_i_len==0){
				BK5823RX_ClrRX();
			}
			else if(m_i_len>128){
				BK5823RX_ClrRX();
			}else{		
				BK5823RX_SPI_Read((16),out_u8_rvbuf,m_i_len);
				BK5823RX_ClrRX();
				m_i_rv = m_i_len;
				//break;
			}
		}
		else
			BK5823RX_ClrRX();
	}	
	return m_i_rv;
}

//void BK5823RX_Tim_Config(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;// 定义用于初始化的结构体 
//	TIM_OCInitTypeDef  BK5823_OCInitStructure;
//	uint16_t BK5823_PrescalerValue = 60000-1;

////	TIM_InternalClockConfig(PSAM_TIMX);//    配置定时器3的内部时钟源(启用) 
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5823,ENABLE);// 使能TIM3定时器的时钟

//	// 定时器基本配置 
//	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 				//自动重装周期
//	TIM_TimeBaseStructure.TIM_Prescaler = BK5823_PrescalerValue; //设置分频数值

//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//时钟分频因子
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//计数方式
//	TIM_TimeBaseInit(TIM5823,&TIM_TimeBaseStructure);
//	
//	// Prescaler configuration
//	TIM_PrescalerConfig(TIM5823, BK5823_PrescalerValue, TIM_PSCReloadMode_Immediate);
//	
//	//Output Compare Timing Mode configuration: Channel1 
//	BK5823_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
//	BK5823_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;
//	BK5823_OCInitStructure.TIM_Pulse = 0;
//	BK5823_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OC1Init(TIM5823, &BK5823_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM5823, TIM_OCPreload_Disable);
//	
//	//	TIM_Cmd(PSAM_TIMX,ENABLE);
//  // 清除溢出中断标志 //
//  TIM_ClearFlag(TIM5823,TIM_FLAG_CC1);
//  
//	// 禁止ARR预装载缓冲器 //
//  TIM_ARRPreloadConfig(TIM5823,DISABLE);
//	
//  //preset comparer ,the initial value is 1
//	TIM5823->SR = (uint16_t)~TIM_FLAG_CC1;		// clear the flag
//	TIM5823->CNT = 0; //PSAM_TIMX->CCR1-Baud_9600S;
//	TIM5823->CCR1 = 2-1;
//	TIM_Cmd(TIM5823,DISABLE);	
//}


void BK5823RX_Tim_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM4_OCInitStructure;
  //NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

//  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
// 

  TIM_TimeBaseStructure.TIM_Period =65535;// 1000;//2000;//4000;//2000;           // 自动重装载寄存器周期的值(计数值) 
  TIM_TimeBaseStructure.TIM_Prescaler =288-1;//(32 - 1);// (18000 - 1);	//时钟预分频数 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;			//
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	
	// Prescaler configuration
//	TIM_PrescalerConfig(TIM4, (18000 - 1), TIM_PSCReloadMode_ Immediate);
	
	//Output Compare Timing Mode configuration: Channel1 
	TIM4_OCInitStructure.TIM_OCMode 		= TIM_OCMode_Timing;
	TIM4_OCInitStructure.TIM_OutputState  = TIM_OutputState_Disable;
	TIM4_OCInitStructure.TIM_Pulse = 0;
	TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM4_OCInitStructure);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);

	TIM_ClearFlag(TIM4,TIM_FLAG_CC1);
    TIM_ARRPreloadConfig(TIM4,DISABLE);
	
	// 清除溢出中断标志 //
//  TIM_ClearFlag(TIM4,TIM_FLAG_CC1);
  
	// 禁止ARR预装载缓冲器 //
//  TIM_ARRPreloadConfig(TIM4,DISABLE);
	
  //preset comparer ,the initial value is 1
//	TIM4->SR = (u16)~TIM_FLAG_CC1;		// clear the flag
//	TIM4->CNT = 0; //
//	TIM4->CCR1 = 2-1;
	TIM_Cmd(TIM4,DISABLE);	

}

