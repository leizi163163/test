#include "stm32f10x.h"

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int  u32;
extern void u1_send(u8* fmt,u32 len);


void SLRXauto_carrier_en(UINT8_T auto_en)
{

    if(auto_en == 1)
    {
        SLRXrxtx_switch(RX_OFF);
        SL1102RX_WriteSR(TXFIFO_D0, 0x01);
        SL1102RX_WriteSR(TX_FIFO, 0xff);
        SL1102RX_WriteSR(0xd , 0x01);
    }
    else
    {
        SL1102RX_WriteSR(TXFIFO_D0, 0x80);
        SL1102RX_WriteSR(TXFIFO_D1, 0x10);
        SL1102RX_WriteSR(0xd , 0x0);
    }
    SLRXrf_init();
}


u8 	CheckRxBuff[256];
u8 	CheckRxBuffLen;
u8 	SLDataBuf[128]={0};
u8 	SLDataBufLen=0;
u8	SLStatus=0;
UINT8_T   bst[28] = /*广播地址*/{			0xFF,0xFF,0xFF,0xFF,\
								/**/							0x50,0x03,0x91,0xC0,\
								/*RSU信标*/				0x02,0x50,0x68,0xDE,\
								/*UNIX时间*/			0x68,0xBC,0x27,0xF2,\
								/*42：标识点RSU*/	0x00,0x01,0x41,0x83,\
																	0x29,0x30,0x1A,0x00,0x29,0x00,0x07,0x00
                    };
u8 testbufusart[]={"\r\nOK"};
u8 bstbecon=0x00;

u8 SLTransFunc(	u8 *trans_buf,u8 trans_len,\
						u8 *trans_revbuf,u8 *trans_revlen,\
						u8 trans_counter,u8 *out_u8_ResCount,
						u8 trans_overtime)
{
	u8 trans_i,status;

	if(trans_len==0)
	{
		return 0;
	}
		
	trans_overtime = 15;
	for(trans_i=trans_counter;trans_i>0;trans_i--)
	{
		
		TX_CtrlOpen();
		USER_Delay1ms(1);
		status=SLTXRFTxFunc(trans_buf,trans_len);
		TX_CtrlClose();
		if(!status)
		{
			status=SLRXRFRxFunc(trans_revbuf,trans_revlen,trans_overtime);
		}
		else
		{
			continue;
		}
		if(!status)
		{
			*out_u8_ResCount = trans_i;
			return *trans_revlen;
		}
		else
		{
			if(chanCmd == 1)
			{
//				trans_overtime = 10;
				if(status == 5)
				{
					if(Dsrc_NFlag == 0x77)
					{
						Dsrc_NFlag = 0xF7;
					}
					else
					{
						Dsrc_NFlag = 0x77;
					}
					trans_buf[5] = 	Dsrc_NFlag;			
				}
			}
		}
	}
	*out_u8_ResCount = 0;
	return 0;
}




u8 SLRXCheckRxBuf[128]={0};
u8 SLRXCheckRxBufLen=0;
void SLRXCheckRx(void)
{
	u8 SLRXCheckRxFlag=0;
	SLRXCheckRxFlag=SLRXread_reg(IRQ_STAT);
	if(SLRXCheckRxFlag&&(SLRXCheckRxFlag!=0x40))
	{
//		Log("\r\nCPC STAT:",11,&SLRXCheckRxFlag,1);
		SLRXCheckRxFlag=0;
	}
	if(SLRXCheckRxFlag&0x40)
	{
		SLRXrx(SLRXCheckRxBuf, &SLRXCheckRxBufLen);
//		u1_send(SLRXCheckRxBuf,SLRXCheckRxBufLen);
//		Log("\r\nCPC DATA:",11,SLRXCheckRxBuf,SLRXCheckRxBufLen);
		memset(SLRXCheckRxBuf,0,SLRXCheckRxBufLen);
		SLRXwrite_reg(RXFIFO_D1, 0x10);
	}
	
	SLRXCheckRxFlag=0;
	SLRXwrite_reg(IRQ_STAT,SLRXread_reg(IRQ_STAT));
}


