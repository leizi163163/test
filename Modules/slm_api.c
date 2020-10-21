/*
*************************************************************
 Copyright (C), 2016-2020 smartlinkMicro. Co., Ltd.  All rights reserved.
 Filename:
 Complier:    ARM-MDK 5.1.2
 Target OS:   NONE
 Target HW:  SL1101
 Author:        htyi
 Version :     V1.0.0
 Date:          2017.6.16
 Description:
 History:
     <author>     <time>     <version >   <desc>

*************************************************************
*/

#include    <stdlib.h>
#include    <math.h>
#include    <string.h>
#include    <stdarg.h>
#include "stm32f10x.h"
#include "BK5824.h"

typedef unsigned char uint8;

//_read_reg  read_reg = DEV_ReadSR;
//_write_reg  write_reg = DEV_WriteSR;
//_read_buf   read_buf = DEV_SPI_Read;
//_write_buf  write_buf = DEV_SPI_Write;

#define VERSION "V1.062"

/*
*  sensitivity_cfg; reg: 0x47 , index:0~13  for -87~-60 dbm
*/
#if 0
const S_GLOBAL_CFG sensitivity_cfg[] =
{
    {0 , 0x47, 0x19},   // -85dbm
    {1 , 0x47, 0x18},   // -84dbm
    {2 , 0x47, 0x17},   // -82dbm
    {3 , 0x47, 0x16},   // -80dbm
    {4 , 0x47, 0x15},   // -78dbm
    {5 , 0x47, 0x14},   // -76dbm
    {6 , 0x47, 0x13},   // -74dbm
    {7 , 0x47, 0x12},   // -72dbm
    {8 , 0x47, 0x11},   // -71dbm
    {9 , 0x47, 0x10},   // -68dbm
    {10, 0x47, 0xF },   // -66dbm
    {11, 0x47, 0xE },   // -65dbm
    {12, 0x47, 0xD },   // -63dbm
    {13, 0x47, 0xC }    // -60dbm
};
const  UINT32_T SEN_CFG_SIZE = ARRAY_SIZE(sensitivity_cfg);
const  UINT32_T SEN_MAX_INDEX = SEN_CFG_SIZE;
#endif
#if 1
#define SEN_CFG_ARRAY_LEN (2)
const S_GLOBAL_CFG sensitivity_cfg[] =
{
    {0, 0x47, 0x19}, {0, 0x2a, 0xd6},    // -85dbm
    {1, 0x47, 0x18}, {1, 0x2a, 0xd8},    // -84dbm
    {2, 0x47, 0x17}, {2, 0x2a, 0xd7},    // -82dbm
    {3, 0x47, 0x16}, {3, 0x2a, 0xd6},    // -80dbm
    {4, 0x47, 0x15}, {4, 0x2a, 0xd5},    // -78dbm
    {5, 0x47, 0x14}, {5, 0x2a, 0xd4},    // -76dbm
    {6, 0x47, 0x13}, {6, 0x2a, 0xd3},    // -74dbm
    {7, 0x47, 0x12}, {7, 0x2a, 0xd2},    // -72dbm
    {8, 0x47, 0x11}, {8, 0x2a, 0xd1},    // -71dbm
    {9, 0x47, 0x10}, {9, 0x2a, 0xd0},    // -68dbm
    {10, 0x47, 0xF }, {10, 0x2a, 0xdF},  // -66dbm
    {11, 0x47, 0xE }, {11, 0x2a, 0xdE},  // -65dbm
    {12, 0x47, 0xD }, {12, 0x2a, 0xdD},  // -63dbm
    {13, 0x47, 0xC }, {13, 0x2a, 0xdC}   // -60dbm
};
const  UINT32_T SEN_CFG_SIZE = ARRAY_SIZE(sensitivity_cfg);
const  UINT32_T SEN_MAX_INDEX = SEN_CFG_SIZE / SEN_CFG_ARRAY_LEN;
#endif
/*
*  wkup sensitivity cfg;  index:0~4  for hight, midhight, mid, midlow, low
*/
const S_GLOBAL_CFG wkupSen_cfg[] =
{
    {0, 0x6f, 0x44}, {0, 0x7d, 0x40}, {0, 0x1a, 0x0e}, {0, 0x1b, 0x1d}, {0, 0x1c, 0x2f}, // hight
    {1, 0x6f, 0x44}, {1, 0x7d, 0x80}, {1, 0x1a, 0x0e}, {1, 0x1b, 0x1d}, {1, 0x1c, 0x2f}, // midhight
    {2, 0x6f, 0x48}, {2, 0x7d, 0x00}, {2, 0x1a, 0x0e}, {2, 0x1b, 0x1d}, {2, 0x1c, 0x2f}, // mid
    {3, 0x6f, 0x48}, {3, 0x7d, 0x40}, {3, 0x1a, 0x0e}, {3, 0x1b, 0x1d}, {3, 0x1c, 0x2f}, // midlow
    {4, 0x6f, 0x48}, {4, 0x7d, 0x80}, {4, 0x1a, 0x0e}, {4, 0x1b, 0x1d}, {4, 0x1c, 0x2f}  // low
};
const  UINT32_T WKUP_SEN_CFG_SIZE    =  ARRAY_SIZE(wkupSen_cfg);
const  UINT32_T WKUP_SEN_MAX_INDEX = WKUP_SEN_CFG_SIZE / 5;

/*
*  Tx_Pow_cfg ;  index: 0~26;   -16dbm~10dbm
*/
const S_GLOBAL_CFG Tx_Pow_cfg[] =
{
    {0,  0x64, 0x30}, { 0, 0x71, 0x00}, { 0, 0x6B, 0x00}, { 0, 0x69, 0x04},  // -20dbm
    {1,  0x64, 0x30}, { 1, 0x71, 0x00}, { 1, 0x6B, 0x20}, { 1, 0x69, 0x04},  // -19dbm
    {2,  0x64, 0x30}, { 2, 0x71, 0x00}, { 2, 0x6B, 0x40}, { 2, 0x69, 0x04},  // -18dbm
    {3,  0x64, 0x30}, { 3, 0x71, 0x00}, { 3, 0x6B, 0x60}, { 3, 0x69, 0x04},  // -17dbm
    {4,  0x64, 0x30}, { 4, 0x71, 0x00}, { 4, 0x6B, 0x00}, { 4, 0x69, 0x05},  // -16dbm
    {5,  0x64, 0x00}, { 5, 0x71, 0x00}, { 5, 0x6B, 0x20}, { 5, 0x69, 0x05},  // -15dbm
    {6,  0x64, 0x30}, { 6, 0x71, 0x00}, { 6, 0x6B, 0x00}, { 6, 0x69, 0x06},  // -14dbm
    {7,  0x64, 0x30}, { 7, 0x71, 0x00}, { 7, 0x6B, 0x20}, { 7, 0x69, 0x06},  // -13m
    {8,  0x64, 0x30}, { 8, 0x71, 0x00}, { 8, 0x6B, 0x40}, { 8, 0x69, 0x06},  // -12dbm
    {9,  0x64, 0x30}, { 9, 0x71, 0x00}, { 9, 0x6B, 0x60}, { 9, 0x69, 0x06},  // -11dbm
    {10, 0x64, 0x30}, {10, 0x71, 0x00}, {10, 0x6B, 0x00}, {10, 0x69, 0x07},  // -10dbm
    {11, 0x64, 0x30}, {11, 0x71, 0x00}, {11, 0x6B, 0x20}, {11, 0x69, 0x07},  //  -9dbm
    {12, 0x64, 0x30}, {12, 0x71, 0x00}, {12, 0x6B, 0x40}, {12, 0x69, 0x07},  //  -8dbm
    {13, 0x64, 0x30}, {13, 0x71, 0x00}, {13, 0x6B, 0x60}, {13, 0x69, 0x07},  //  -7dbm
    {14, 0x64, 0x30}, {14, 0x71, 0x00}, {14, 0x6B, 0x80}, {14, 0x69, 0x07},  //  -6dbm
    {15, 0x64, 0x30}, {15, 0x71, 0x00}, {15, 0x6B, 0xA0}, {15, 0x69, 0x07},  //  -5dbm
    {16, 0x64, 0x30}, {16, 0x71, 0x00}, {16, 0x6B, 0xC0}, {16, 0x69, 0x07},  //  -4dbm
    {17, 0x64, 0x30}, {17, 0x71, 0x00}, {17, 0x6B, 0xC1}, {17, 0x69, 0x07},  //  -3dbm
    {18, 0x64, 0x30}, {18, 0x71, 0x00}, {18, 0x6B, 0xC2}, {18, 0x69, 0x07},  //  -2dbm
    {19, 0x64, 0x30}, {19, 0x71, 0x20}, {19, 0x6B, 0xC0}, {19, 0x69, 0x07},  //  -1dbm
    {20, 0x64, 0x30}, {20, 0x71, 0x20}, {20, 0x6B, 0xC1}, {20, 0x69, 0x07},  //  00dbm
    {21, 0x64, 0x30}, {21, 0x71, 0x20}, {21, 0x6B, 0xC2}, {21, 0x69, 0x07},  //  01dbm
    {22, 0x64, 0x30}, {22, 0x71, 0x20}, {22, 0x6B, 0xC3}, {22, 0x69, 0x07},  //  02dbm
    {23, 0x64, 0x30}, {23, 0x71, 0x20}, {23, 0x6B, 0xF3}, {23, 0x69, 0x07},  //  03dbm
    {24, 0x64, 0x30}, {24, 0x71, 0x60}, {24, 0x6B, 0xF0}, {24, 0x69, 0x07},  //  04dbm
    {25, 0x64, 0x30}, {25, 0x71, 0x60}, {25, 0x6B, 0xF3}, {25, 0x69, 0x07},  //  05dbm
    {26, 0x64, 0x34}, {26, 0x71, 0x60}, {26, 0x6B, 0xF3}, {26, 0x69, 0x07}   //  06dbm
};
const  UINT32_T TX_POW_CFG_SIZE    =  ARRAY_SIZE(Tx_Pow_cfg);
const  UINT32_T TX_POW_MAX_INDEX    = TX_POW_CFG_SIZE / 4;

const S_GLOBAL_CFG Regs_cfg[] =
{
    {0, 0x65, 0x00},//analog test system config 
//    {1, 0x67, 0xCE},//analog test user config: close RXBPF;config rxBPF BW {1, 0x67, 0x51},			//CC
    {2, 0x68, 0x0c},
    {3, 0x6d, 0xe0},//config LNA gain;colse LNA
    {4, 0x70, 0x09},
    // next is rsu config
    {5, 0x38, 0x07},
    {6, 0x3b, 0x80},
    {7, 0x3c, 0x04}
};
const  UINT32_T REGS_CFG_SIZE    =  ARRAY_SIZE(Regs_cfg);


const S_GLOBAL_CFG Tx_mod_cfg[] =
{
    {0, 0x3f, 0x3f}, {0, 0x40, 0x02}, {0, 0x71, 0x60}, {0, 0x68, 0x80}, {0, 0x69, 0x07}, {0, 0x6b, 0x08},  // 90%
    {1, 0x3f, 0x3f}, {1, 0x40, 0x06}, {1, 0x71, 0x60}, {1, 0x68, 0x80}, {1, 0x69, 0x07}, {1, 0x6b, 0x08},  // 80%
    {2, 0x3f, 0x3f}, {2, 0x40, 0x0a}, {2, 0x71, 0x60}, {2, 0x68, 0x80}, {2, 0x69, 0x07}, {2, 0x6b, 0x08},  // 70%
    {3, 0x3f, 0x3f}, {3, 0x40, 0x0e}, {3, 0x71, 0x60}, {3, 0x68, 0x80}, {3, 0x69, 0x07}, {3, 0x6b, 0x08},  // 60%
    {4, 0x3f, 0x3f}, {4, 0x40, 0x12}, {4, 0x71, 0x60}, {4, 0x68, 0x80}, {4, 0x69, 0x07}, {4, 0x6b, 0x08}  // 50%
};
const  UINT32_T TX_MOD_CFG_SIZE    =  ARRAY_SIZE(Tx_mod_cfg);
const  UINT32_T TX_MOD_MAX_INDEX = TX_MOD_CFG_SIZE / 6;


void DelayBK5823_14K(u32 i)
{
	while(i--)
		continue;
}


void SLdelay_us(void)
{
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();
}

void SLdelay_1us(u8 num)
{
		while(num--)
			SLdelay_us();
}
void SLdelay_10us_(void)
{
	SLdelay_us();SLdelay_us();SLdelay_us();SLdelay_us();SLdelay_us();
	SLdelay_us();SLdelay_us();SLdelay_us();SLdelay_us();SLdelay_us();
	SLdelay_us();
	__nop();__nop();__nop();__nop();__nop();	__nop();__nop();__nop();__nop();__nop(); __nop();__nop();__nop();__nop();  
	__nop();__nop();__nop();__nop();__nop();	__nop();__nop();__nop();__nop();__nop(); __nop();__nop();__nop();__nop();  
	__nop();__nop();__nop();__nop();__nop();	__nop();__nop();__nop();__nop();__nop(); __nop();__nop();__nop();__nop();  
	__nop();__nop();__nop();__nop();__nop();	__nop();__nop();__nop();__nop();__nop(); __nop();__nop();__nop();__nop();  
	__nop();__nop();__nop();__nop();__nop();	__nop();__nop();__nop();__nop();__nop(); __nop();__nop();__nop();__nop();  
}

void SLdelay_10us(u8 num)
{
		while(num--)
			SLdelay_10us_();
}

void SLdelay_1ms_(void)
{
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
	SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();SLdelay_10us_();
}
void SLdelay_1ms(u8 num)
{
	while(num--)
		SLdelay_1ms_();
}

/*******************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************/
/**************************************************************************SL1102发送 API配置***********************************************************************/
/*******************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************/

INT8 SLTXtxPower_cur_idx = 0;
u8 SLTXRegValue=0;
volatile UINT32_T MAX_LEN = 0;

u8 SLTXRFTxFunc(u8 *rftx_buf,u32 rftx_len)
{
	u32 timeoutTx;
	//u8 writereg[4]={0};
	unsigned char _bk5823Irq = 1;
	if(rftx_len==0)
	{
		return 1;
	}
	if (BstFlag == 1)
	{
		BK5824_14K_Enable(1);
	}	
	else
	{
		BK5824_14K_Enable(0);
	}
	ClearBk5824Irq();
	_bk5823Irq = 1;
	BK5823TX_SPI_Write14k((BKWRITE_REG|16),&(rftx_buf[0]),rftx_len);
	timeoutTx = 0xffffffff;    //等待5823发送TXfifo发送完毕
//	log_title("timeoutTx = 0xffffffff;    //等待5823发送TXfifo发送完毕");
	while (timeoutTx--)
	{
		//BK5823TX_SPI_Read((6),&(writereg[0]),4);
		#if 0
		if (!(writereg[0]&0x20))		//TXFIFO为空，发送完毕
			break;
		#endif
		#if 1
		_bk5823Irq = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
			if(!_bk5823Irq)//发送完成
			{
				TX_CtrlClose();						//关放大器
				break;
			}
		#endif
//			return 0;
	}
	//ClearBk5824Irq();
	#if 0
	if (writereg[0]&0x20)	//TXFIFO不为空，结束本次循环
		return 1;
	#endif
	#if 1
	if (_bk5823Irq)	//TX packet is not finished
		return 1;
	#endif
//	hlll = SLRXread_reg(IRQ_STAT);
	return 0;
}

u8 BK5823_SendFunc(u8 *rftx_buf,u32 rftx_len)
{
	u16 timeoutTx;
	u8 writereg[4]={0};

	if(rftx_len==0)
	{
		return 1;
	}	
	
	BK5823TX_SPI_Write14k((BKWRITE_REG|16),&(rftx_buf[0]),rftx_len);
	
	timeoutTx = 0xffff;    //等待5823发送TXfifo发送完毕
	while (timeoutTx--)
	{
		BK5823TX_SPI_Read((6),&(writereg[0]),4);
		if (!(writereg[0]&0x20))		//TXFIFO为空，发送完毕
			break;
//			return 0;
	}

	if (writereg[0]&0x20)	//TXFIFO不为空，结束本次循环
		return 1;
	
//	hlll = SLRXread_reg(IRQ_STAT);
	return 0;
}


/*******************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************/
/**************************************************************************SL1102接收 API配置***********************************************************************/
/*******************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************/
_SLRXread_reg  		SLRXread_reg = 		SL1102RX_ReadSR;
_SLRXwrite_reg  	SLRXwrite_reg = SL1102RX_WriteSR;
_SLRXread_buf   	SLRXread_buf = 	SL1102RX_SPI_Read;
_SLRXwrite_buf  	SLRXwrite_buf = SL1102RX_SPI_Write;
INT8 SLRXtxPower_cur_idx = 0;
volatile UINT32_T ARRAY_CFG_LEN = 0;
#if 0
static void  SLRXcfg_common(UINT8_T index, S_GLOBAL_CFG *pcfg)
{
    //S_GLOBAL_CFG *pcfg;
    volatile INT32 i = 0;
    //pcfg = &cfg;
    for(i = 0; i < MAX_LEN; i++)
    {
        if(index == pcfg->index)
        {
            SL1102RX_WriteSR(pcfg->addr, pcfg->val);
        }
        pcfg++;
    }
}
#endif
#if 1
static void  SLRXcfg_common(UINT8_T index, S_GLOBAL_CFG *pcfg)
{
    //S_GLOBAL_CFG *pcfg;
    volatile INT32 i = 0;
    //pcfg = &cfg;
    for(i = 0; i < MAX_LEN; i++)
    {
        if(index == pcfg->index)
        {
            for(i = 0; i < ARRAY_CFG_LEN; i++)
            {
                SL1102RX_WriteSR(pcfg->addr, pcfg->val);
                pcfg++;
            }
            break;
        }
        pcfg += ARRAY_CFG_LEN;
    }
}
#endif
/**
  * @brief  : init the rf hardware.  m
  * @param: NONE
*/
u8 statusregvalue=0;
void SLRXrf_init(void)
{
//    SL1102RX_WriteSR(VCO_CFG1, 0x35);
//		SL1102RX_WriteSR(VCO_CFG1, 0x36);
//	while(1)
	{
	SL1102RX_WriteSR(VCO_CFG1, 0x35);
	statusregvalue = 0 ;
	statusregvalue = SL1102RX_ReadSR(VCO_CFG1) ;
	
//	log_hex("VCO_CFG1:",&statusregvalue,1);
	USER_Delay1ms(2);
	
	SL1102RX_WriteSR(VCO_CFG1, 0x35);
	statusregvalue = 0 ;
	statusregvalue = SL1102RX_ReadSR(VCO_CFG1) ;
//	log_hex("VCO_CFG1:",&statusregvalue,1);
	USER_Delay1ms(20);
//	while(1);
	}

}
/**
  * @brief   : SLRXtxPower_set .
  * @param:  index:0~26  for -16~10 dbm    ,mush to be set, because 0x64[0:4]bit about the lo_isel
  *
  */
INT32 SLRXtxPower_set(UINT8_T index)
{
    if(index >= TX_POW_MAX_INDEX)
    {
        return FAIL;
    }

    MAX_LEN  = TX_POW_CFG_SIZE;
    SLRXcfg_common(index, (S_GLOBAL_CFG *)Tx_Pow_cfg);

    return SUCCESS;
}
/**
  * @brief   : SLRXtxmod_idx_set .   set  the  modulation of tx.
  * @param:  index:0~4  for 90%~50% dbm
  * @return:
  */
INT32 SLRXtxmod_idx_set(UINT8_T index)
{
    if(index >= TX_MOD_MAX_INDEX)
    {
        return FAIL;
    }

    MAX_LEN  = TX_MOD_CFG_SIZE;
		

    SLRXcfg_common(index, (S_GLOBAL_CFG *)Tx_mod_cfg);

    return SUCCESS;
}
/**
  * @brief   : SLRXrxSen_set .   set  the  sensitivity of rx.
  * @param:  index:0~13  for -87~-60 dbm
  * @return:
  */
INT32 SLRXrxSen_set(UINT8_T index)
{
    if(index >= SEN_MAX_INDEX)
    {
        return FAIL;
    }
    MAX_LEN  = SEN_CFG_SIZE;

    SLRXcfg_common(index, (S_GLOBAL_CFG *)sensitivity_cfg);

    return SUCCESS;
}

//INT32 SLRXrxSen_set(UINT8_T index)
//{
//    if(index >= SEN_MAX_INDEX)
//    {
//        return FAIL;
//    }
//    MAX_LEN  = SEN_MAX_INDEX;
//    ARRAY_CFG_LEN = SEN_CFG_ARRAY_LEN;

//    SL1102RX_WriteSR(0x2a, 0xd6);
//    SLRXcfg_common(index, (S_GLOBAL_CFG *)sensitivity_cfg);

//    return SUCCESS;
//}

/**
  * @brief   : SLRXwkupSen_set .   set  the  sensitivity of wakeup.
  * @param:  index:0~3  for hight, mid, midlow, low
  * @return:
  */
INT32 SLRXwkupSen_set(UINT8_T index)
{
    if(index >= WKUP_SEN_MAX_INDEX)
    {
        return FAIL;
    }

    MAX_LEN  = WKUP_SEN_CFG_SIZE;

    SLRXcfg_common(index, (S_GLOBAL_CFG *)wkupSen_cfg);

    return SUCCESS;
}
/**
  * @brief  SLRXwkupMode_set  set the wakeup mode and wakeup time
  * @param
  *              mode: 0 wait the ce, 1(default) auto wkup when recive 14K square wave
  *              Twkup_ms: 1~f  for 0.5~7.5ms,   0.5ms/step, default 0.5ms
  *
  */
INT32 SLRXwkupMode_set(UINT8_T mode , UINT8_T Twkup_ms)
{
    UINT8_T setval = 0;
    UINT8_T modeval = 0;
    modeval = SLRXread_reg(WKUP_CTRL0);

    if(mode == 0)
    {
        modeval &= ~(modeval & 0x80);
    }
    else
    {
        modeval |= (modeval & 0x80);
    }
    SL1102RX_WriteSR(WKUP_CTRL0, modeval);


    if(Twkup_ms > 0xf)
    {
        return FAIL;
    }
    setval = SLRXread_reg(ONRST_TIMER);
    setval = ((setval & 0xf0) | Twkup_ms);
    SL1102RX_WriteSR(ONRST_TIMER, setval);
    return SUCCESS;
}
u8 slregsvalue1=0;
void SLRXtxFreq_set(UINT16_T freq)
{
    F64 div;
    UINT16_T div_int;
    UINT16_T div_frac;

    div = (F64)(freq  / (32.768 * 4));
    div_int = (UINT16_T)div;
    div_frac = (UINT16_T) ((div - div_int) * pow(2, 16));

    /* cfg int */
    SL1102RX_WriteSR(PLL_CTRL0, div_int);
	slregsvalue1=SLRXread_reg(PLL_CTRL0);
    /* cfg frac */
    SL1102RX_WriteSR(PLL_CTRL1, (div_frac & 0xff00) >> 8 );
	slregsvalue1=SLRXread_reg(PLL_CTRL1);
    SL1102RX_WriteSR(PLL_CTRL2, div_frac & 0xff);
	slregsvalue1=SLRXread_reg(PLL_CTRL2);

}

void SLRXrxFreq_set(UINT16_T freq)
{
    F64 div;
    UINT16_T div_int;
    UINT16_T div_frac;

    div = (F64)(freq  / (32.768 * 4));
    div_int = (UINT16_T)div;
    div_frac = (UINT16_T) ((div - div_int) * pow(2, 16));

    /* cfg int */
    SL1102RX_WriteSR(PLL_CTRL3, div_int);
    /* cfg frac */
    SL1102RX_WriteSR(PLL_CTRL4, (div_frac & 0xff00) >> 8 );
    SL1102RX_WriteSR(PLL_CTRL5, div_frac & 0xff);


}

INT32 SLRXrxBitrate_set(UINT16_T  kbps)
{
    UINT8_T setval = 0;
    setval = SLRXread_reg(RX_SYS_CTRL1);
    if(kbps == 256)
    {
        setval = (setval & 0xfc);
    }
    else if(kbps == 512)
    {
        setval = ((setval & 0xfc) | 0x01);
    }
    else if(kbps == 1024)
    {
        setval = ((setval & 0xfc) | 0x02);
    }
    else
    {
        return FAIL;
    }

    SL1102RX_WriteSR(RX_SYS_CTRL1, setval);
    return SUCCESS;
}

INT32 SLRXtxBitrate_set(UINT16_T  kbps)
{
    UINT8_T setval = 0;
    setval = SLRXread_reg(TXMODM_CTRL0);


    if(kbps == 256)
    {
        setval = (setval & 0xfc);
    }
    else if(kbps == 512)
    {
        setval = ((setval & 0xfc) | 0x01);
    }

    else if(kbps == 1024)
    {
        setval = ((setval & 0xfc) | 0x02);
    }
    else
    {
        return FAIL;
    }

    SL1102RX_WriteSR(TXMODM_CTRL0, setval);
    return SUCCESS;
}
/**
  * @brief  set the crc mode
  * @param  crc_inv_en: 0 not invert ; 1(default) invert
  */

void SLRXcrcMode_set(UINT8_T crc_inv_en)
{
    UINT8_T setval = 0;
    setval =  SLRXread_reg(RX_AFE_CFG0);

    if(crc_inv_en == 0)
    {
        setval &= ~(setval & 0x80);
    }
    else
    {
        setval |= (setval & 0x80);
    }
    SL1102RX_WriteSR(RX_AFE_CFG0, setval);
}

/**
  * @brief  set the pin mux
  * @param  pinmux: 0: no interrupt ; 1 pin_10->interrupt; 2  pin_12->interrupt
  */

void SLRXpinMux_init(UINT8_T pinmux)
{
    UINT8_T pinval;

    pinval = (pinmux << 4);
		SL1102RX_WriteSR(MFIO_CFG, pinval); 	// 	set pin multiplexing: pinval=0x20:DATACLK->interrupt(PC0);TXDATA->txdata/rxdata;RXDATA->dataclk-out
																	//	set pin multiplexing : pinval=0x00:dataclk->dataclk-out;   txdata->txdata     rxdata->rxdata
//    PIN_MUX_Config(pinmux);//MCU pin config
}
void SLRXrxtx_switch(UINT8_T sel)
{
    if(sel == 0)
    {
        SL1102RX_WriteSR(SYS_CNTRL0, 0x0);
		TIM_Cmd(TIM4,DISABLE);
    }
    else  if(sel == 1)
    {
        SL1102RX_WriteSR(0x64, 0x30);
        SL1102RX_WriteSR(0x65, 0x00);
        SL1102RX_WriteSR(0x6e, 0x40);

        if(SLRXtxPower_cur_idx == 26)
        {
            SL1102RX_WriteSR(0x64, 0x34);
        }
        SL1102RX_WriteSR(SYS_CNTRL0, 0x01);
    }
    else  if(sel == 2)
    {
        SL1102RX_WriteSR(0x64, 0x35);
        SL1102RX_WriteSR(0x65, 0x5a);
        SL1102RX_WriteSR(0x6e, 0x6b);
        if(SLRXtxPower_cur_idx == 26 )
        {
            SL1102RX_WriteSR(0x64, 0x40);
        }
//        if(rx_current_flag == 1)
//        {
//            SL1102RX_WriteSR(0x64, 0x40);
//            SL1102RX_WriteSR(0x65, 0x00);
//            SL1102RX_WriteSR(0x6e, 0x5b);
//        }

        SL1102RX_WriteSR(SYS_CNTRL0, 0x12);
		TIM_Cmd(TIM4,ENABLE);
    }
}
u8 slregsvalue1;
void SLRXclean_stat(void)
{
    SL1102RX_WriteSR(RXFIFO_D1, 0x10);
    SL1102RX_WriteSR(TXFIFO_D1, 0x10);
    SL1102RX_WriteSR (IRQ_EN, 0xf0);
		slregsvalue1=SLRXread_reg(IRQ_EN);
    SL1102RX_WriteSR (IRQ_STAT, 0xff);
		slregsvalue1=SLRXread_reg(IRQ_STAT);
    SLRXrxtx_switch(RX_EN); // enable rx default
}
void SLRXregs_init(void)
{
    S_GLOBAL_CFG *pcfg;
    volatile INT32 i = 0;

    pcfg = (S_GLOBAL_CFG*) Regs_cfg;
    for(i = 0; i < REGS_CFG_SIZE; i++)
    {
        SL1102RX_WriteSR(pcfg->addr, pcfg->val);
        pcfg++;
    }
}
/**
  * @brief  3.3V circuit config, all the config will becom the default value after reset the sl1101, (sleepMode_en(1) -->sleepMode_en(0))
  * @param  NONE
  */
void SL1102RX_chip_init_3v3(void)
{
    /* init RF, pin mux */
    SLRXrf_init(); //压控振荡器配置
	
		SLRXpinMux_init(2);//config pins of sl1102 and MCU:dataclk Txdata Rxdata:
    SLRXregs_init();

    /* seting the tx power, rx sensitivity,wakup  sensitivity and so on
         and now seting those config by default val */
//    SLRXtxPower_set(TX_POWER_DEFAULT);//set tx power 0dbm
//    SLRXtxPower_cur_idx = TX_POWER_DEFAULT;   // for the max txpower

//    SLRXtxmod_idx_set(TX_MOD_IDX_DEFAULT);   // 90% set the deep of moudle

    SLRXrxSen_set(RX_SENSITIVITY_DEFAULT);//13
//		SLRXrxSen_set(0);//13

//		SLRXtxFreq_set(5830);//set tx freq:channel 1(5830);channel 2(5840) (00H;channel 1   01H;channel 2)

//		SLRXrxFreq_set(5795);//set rx freq:channel 1(5790);channel 2(5800)
//	SLRXrxFreq_set(5795);//set rx freq:channel 1(5790);channel 2(5800)
	
	if(PC_InitStructure.PLLChannelID == 0)
	{
		log_title("信道0");
		SLRXrxFreq_set(5795);
	}
	else
	{
		log_title("信道1");
		SLRXrxFreq_set(5805);
	}

//    SLRXtxBitrate_set(256);

    SLRXrxBitrate_set(512);

    SLRXcrcMode_set(CRC_INV_DEFAULT);
		SL1102RX_WriteSR(0x28, 0x39);
//		SL1102RX_WriteSR(0x4D, 0x11);
//    /* clean rxtx FIFO ,irq status; and enable irq */
    SLRXclean_stat();
		
}
/**
  * @brief  1.8V circuit config, all config will keep until the power off .
  * @param  NONE
  */
void SL1102RX_chip_init_1v8(void)
{

    SLRXwkupMode_set(WAKUP_MODE_DEFAULT, WAKUP_TIME_DEFAULT);
    SLRXwkupSen_set(WAKUP_SENSITIVITY_DEFAULT);
}
void SL1102RX_chip_init(void)
{
    SL1102RX_chip_init_3v3();
    SL1102RX_chip_init_1v8();
}
INT32 SLRXget_rxfifo_len()
{
    /*  can use  the rx list  up to your need */
    return ((SLRXread_reg(RXFIFO_STA1) & 0x1) << 7 | SLRXread_reg(RXFIFO_STA0));
}
INT32 SLRXget_txfifo_len()
{
    UINT16_T ret = 0;
    /*  can use  the tx list  up to your need */
    ret = ((SLRXread_reg(TXFIFO_STA1) & 0x1) << 7 | SLRXread_reg(TXFIFO_STA0));
    ret = TX_FIFO_LEN - ret;
    return ret;
}
INT32 SLRXfill_txfifo(UINT8_T *pdata, UINT32_T *pdatalen)
{
    INT32 fifo_cnt = 0;

    fifo_cnt = SLRXget_txfifo_len();
    if(fifo_cnt == 0 || *pdatalen == 0)
    {
        return FAIL;
    }
    SLRXrxtx_switch(RX_OFF);
    if(*pdatalen > fifo_cnt)
    {
        SLRXwrite_buf(TX_FIFO, pdata, fifo_cnt);
        *pdatalen -= fifo_cnt;
    }
    else
    {
        SLRXwrite_buf(TX_FIFO, pdata, *pdatalen);
        *pdatalen = 0;
//        tx_end   = 1;
    }
    return SUCCESS;
}
/**
  * @brief  rx the data.
  * @param  pdata: the buffer tobe store the data of receive
  *               plength : the length of data
  */

u8 SLRXrx(UINT8_T *pdata , u8 *plength)
{
    INT32 rdlen = -1;
    volatile INT32   i = 0;

    rdlen = SLRXget_rxfifo_len();
    if(rdlen == 0)
    {
        return 1;
    }

//    SLRXread_buf(RX_FIFO, pdata, (UINT32_T)rdlen);
		SL1102RX_SPI_Read(RX_FIFO, pdata, (UINT32_T)rdlen);

    *plength = (UINT32_T) rdlen;
    return 0;
}
/**
  * @brief   just An example of the sending process . no caller
  * @param  pdata: poit to the data be sending
  *               length : the length of data
  */
void SLRXtx(UINT8_T *pdata , UINT32_T length)
{
    SLRXrxtx_switch(RX_OFF);
    SL1102RX_WriteSR(IRQ_STAT, 0xff);
    /*send the vst.txt*/
    if(SLRXfill_txfifo(pdata, &length) == FAIL)
    {
        return;
    }
    SLRXrxtx_switch(TX_EN);
    while(!(SLRXread_reg(IRQ_STAT) & 0x80));

    SL1102RX_WriteSR(IRQ_STAT, 0xff);
}
#define TIM4_CCR1_Value 2240
//u8 SLRXRFRxFunc(u8 *rfrx_buf,u8* rfrx_len,u8 rfrx_overtime)
//{
//	u8 irqrxflag=0;
//	irqrxflag=0;
////	SLRXrxtx_switch(RX_EN);
//	SLRXwrite_reg(IRQ_STAT,SLRXread_reg(IRQ_STAT));
//	SL1102RX_WriteSR(TXFIFO_D1, 0x10);
//	SL1102RX_WriteSR(RXFIFO_D1, 0x10);
//	SLRXrxtx_switch(RX_EN);

//	TIM4->SR = (u16)~TIM_FLAG_CC1;		// clear the flag
//	TIM4->CNT = 0; 
//	TIM4->CCR1 = rfrx_overtime-1;//TIM4_CCR1_Value=1ms
//	TIM_Cmd(TIM4,ENABLE);
//	while((!((TIM4->SR)&TIM_FLAG_CC1))&&(irqrxflag!=0x40))
//		irqrxflag=SLRXread_reg(IRQ_STAT);
//	
//	SLRXrxtx_switch(RX_OFF);
//	if(irqrxflag==0x40)
//	{
//		SLRXrx(rfrx_buf, rfrx_len);
//		SLRXwrite_reg(IRQ_STAT,SLRXread_reg(IRQ_STAT));
//		SLRXwrite_reg(RXFIFO_D1, 0x10);
//		return 0;
//	}
//	else 
//	{
//		SLRXwrite_reg(RXFIFO_D1, 0x10);
//		SLRXwrite_reg(IRQ_STAT,SLRXread_reg(IRQ_STAT));
//		return 1;
//	}
//}

u8 SLRXRFRxFunc(u8 *rfrx_buf,u8* rfrx_len,u8 rfrx_overtime)
{
	u8 irqrxflag = 0;
	uint8_t kk = 0;
	
	SLRXwrite_reg(IRQ_STAT,SLRXread_reg(IRQ_STAT));
//	SL1102RX_WriteSR(TXFIFO_D1, 0x10);
	SL1102RX_WriteSR(RXFIFO_D1, 0x10);
	SLRXrxtx_switch(RX_EN);

	TIM4->SR = (u16)~TIM_FLAG_CC1;					// clear the flag
	TIM4->CNT = 0; 
	TIM4->CCR1 = rfrx_overtime * 10 - 1;			//TIM4_CCR1_Value = 0.1ms
//	TIM4->CCR1 = rfrx_overtime - 1;
//	TIM_Cmd(TIM4,ENABLE);
//	while((!((TIM4->SR)&TIM_FLAG_CC1))&&(irqrxflag!=0x40))
//	log_title("A");
	while((!((TIM4->SR)&TIM_FLAG_CC1)))
	{
		irqrxflag=SLRXread_reg(IRQ_STAT);
//		log_hex("irq:",&irqrxflag,1);
		if(irqrxflag != 0x40)
		{
			if(irqrxflag == 0x00)
			{
				continue;
			}
			else
			{
				SLRXwrite_reg(RXFIFO_D1, 0x10);
				SLRXwrite_reg(IRQ_STAT,SLRXread_reg(IRQ_STAT));
			}
		}
		else
		{
			break;
		}			
	}
//	log_title("B");
	SLRXrxtx_switch(RX_OFF);
	if(irqrxflag==0x40)
	{
		SLRXrx(rfrx_buf, rfrx_len);
		SL1102_RSSI = SLRXread_reg(RX_RSSI);
		if(SL1102_RSSI < 15)
		{
			SL1102_RSSI += 0xF0;
		}
		SLRXwrite_reg(IRQ_STAT,SLRXread_reg(IRQ_STAT));
		SLRXwrite_reg(RXFIFO_D1, 0x10);
		if((rfrx_buf[0] < 0xA0) && (rfrx_buf[0] >= 1) && (*rfrx_len > 8))							//剔除CPC卡,只保留OBU
		{
			if(BstFlag == 1)
			{
				if(rfrx_buf[7] != 0xD0)
				{
					return 1;
				}
				
				for(kk = 0; kk < SUCC_MAX; kk++)
				{
					if(!(memcmp(SuccessObuInfoStructure[kk].ObuId,rfrx_buf,4)))
					{
						SuccessEventReport_func(rfrx_buf);
						log_hex("RsuId: ",OBU_BST_Info + 8,4);
						log_title("找到已交易成功的OBU");
						log_hex("SuccessObuId: ",rfrx_buf,4);
						return 1;
					}
					else
					{
						memcpy(CurrentObuId,rfrx_buf,4);
						log_hex("CurrentObuId: ",rfrx_buf,4);
						return 0;
					}
				}	
			}
			if(chanCmd == 1)
			{
				if(memcmp(CurrentObuId,rfrx_buf,4) == 0)	//查看链路层OBU号和VST中的是否一致,剔除杂乱的DSRC
				{
					if(rfrx_buf[4] == 0xE0)
					{
						return 0;
					}
					else
					{
						return 5;
					}		
				}
				else
				{
					return 1;
				}				
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}
	else 
	{
		SLRXwrite_reg(RXFIFO_D1, 0x10);
		SLRXwrite_reg(IRQ_STAT,SLRXread_reg(IRQ_STAT));
		return 1;
	}
}

void SLRXChannelSeclect(unsigned char _channel)
{
//	unsigned char _reg28info=0;
//	_reg28info = SLRXread_reg(0x28);
	if(_channel==0)//接收单信道：信道0
	{
		SLRXwrite_reg(0x28, 0x39);
		SLRXrxFreq_set(5795);
	}
	else if(_channel==1)//接收单信道：信道1
	{
		SLRXwrite_reg(0x28, 0x39);
		SLRXrxFreq_set(5805);
	}
	else if(_channel==2)//接收双信道
	{
		SLRXwrite_reg(0x28, 0x39);
		SLRXrxFreq_set(5795);
	}
//	SL1102RX_WriteSR (0x28, _reg28info);
}

//ITS测试函数
//BER测试
void SLRXbbp_rst(void)
{
    SLRXwrite_reg(SYS_CNTRL0, 0x0);
    SLRXwrite_reg(SW_RST, 2);
    SLRXwrite_reg(SW_RST, 3);
    SLRXwrite_reg(SYS_CNTRL0, 0x2);
}
void delay_BER(void)
{
	u32 i=8;
	while(i--);
}


