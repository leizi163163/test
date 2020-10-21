#include "stm32f10x.h"

u8 BST_flag = 0;
u8 OBU_id[4]={0x00};		//OBU_ID


void bk5823_delay(void)
{
		int	 i;
		for(i=0;i<1000;i++);
}

/**************************************************
Function: BK5823_Init
Paramter
	in_u8_channel
						= 1 tx 5.83
						= 0 tx 5.84
	in_u8_Level
						=0~7,tx power level
return value
	= 0 		,ok
	= other ,fail
**************************************************/
uint8_t BK5823_Init(uint8_t in_u8_channel,uint8_t in_u8_Level)
{
	uint8_t m_i_rv=0;
	m_i_rv = BK5823TX_Init(in_u8_channel,in_u8_Level);
	
			DsrcTxPowerPinEN(0);
//	m_i_rv += BK5823RX_Init();
	return m_i_rv;
}

u8 BK5823_RXTX( u8 in_u8_IS14k, u8 *in_u8_TXbuf,
							u8 in_u8_TXlen,u8 *out_u8_rvbuf,
							u8 in_u8_TxCount,u8 *out_u8_ResCount,
							u8 in_u8_overtime )
{
	u8 i;
	u8 j;
	u8	m_i_rv = 0;
	u8 writereg[4]={0};
	u16 timeoutTx;
	u8 WriteArr[4];
	u8 m_i_len;
	u8 channel = 0;
	u8 nPos;    // n位
	u8 carry14kFlag = 1;
	u8 config5823 = 0;	
	u8 TIM_flag=0;
	

	nPos = in_u8_TXbuf[5]&0x80;
	
	j = 16;
	
	channel = in_u8_TXbuf[16]&0x0f;
	
	for(i=0;i<in_u8_TxCount;i++){
	 {
		BK5823RX_Rx_En(0);
		BK5823RX_ClrRX();
		
		DsrcRxPowerPinEN(0);
		DsrcTxPowerPinEN(1);	
		
		if (bstCmd==1)		//发BST命令
		{
			if (carry14kFlag==1){
					if (config5823==0){			//没有收到VST
						config5823 = 1;
//						if(i > (in_u8_TxCount - 30))		//最后30次加大唤醒功率
//						{
//							BK5823TX_ConfigPower(CommF0.TransPowerSet + 2);
//						}
						BK5823TX_14K(1);
						__nop();__nop();__nop();__nop();__nop();
					}
				}else{
					if (config5823==0){				//收到VST
						config5823 = 1;
						BK5823TX_14K(0);
						__nop();__nop();__nop();__nop();__nop();
					}
				}
			}
			BK5823TX_SPI_Write((BKWRITE_REG|j),&(in_u8_TXbuf[0]),in_u8_TXlen);		// //(16寄存器:TX and RX FIFO)
			timeoutTx = 0x3ff;    //等待5823发送TXfifo发送完毕
			while (timeoutTx--){
//				u8 i;
				BK5823TX_SPI_Read((6),&(writereg[0]),4);
				if (!(writereg[0]&0x20))		//TXFIFO为空，发送完毕
					break;
			}

//			if(i > (in_u8_TxCount - 30))
//			{
//				BK5823TX_ConfigPower(CommF0.TransPowerSet);
//			}
			
			if (writereg[0]&0x20)	//TXFIFO不为空，结束本次循环
				continue;
			DsrcTxPowerPinEN(0);
			DsrcRxPowerPinEN(1);
			{ 		
				if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11) == 0)		//BK5823_IRQ_TX检测是否收到数据
				{
					BK5823RX_ClrRX();   //已接收到数据，先清，防止收到上一次的数据
				}
				BK5823RX_Rx_En(1);//开接收使能
				BK5823RX_ClrRX();
				TIM5823->SR = (u16)~TIM_FLAG_CC1;		// clear the flag
				TIM5823->CNT = 0; 
				TIM5823->CCR1 = in_u8_overtime-1;

				TIM_flag=((TIM5823->SR & TIM_FLAG_CC1));
				while(TIM_flag == (uint16_t)RESET){
					TIM_flag=((TIM5823->SR & TIM_FLAG_CC1));
					if (GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)==0)
						break;	
				}				
				if (GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11) )
					continue;
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
					}
				}
				else
					BK5823RX_ClrRX();
			}
			if(m_i_rv>0){
				if (bstCmd == 1){
					u8 chan;
					chan = (out_u8_rvbuf[8]&0x0f);
					if (chan != channel)
						continue;
				}else if(chanCmd == 1){
					u8 nPosReverse;
					nPosReverse = (out_u8_rvbuf[5]&0x80);
					if (nPos == nPosReverse)
						continue;
				}
			}
			
			if(m_i_rv>0){
				if(bstCmd == 1){
					carry14kFlag=0;
					if (carry14kFlag==1){
						config5823 = 0;
						carry14kFlag = 0;
						in_u8_TxCount = 50;
						continue;
					}else{
						*out_u8_ResCount = (in_u8_TxCount -i);
						BST_flag = 0;
						for (i=0; i<4; i++)           //保存OBU_ID
							OBU_id[i] = out_u8_rvbuf[i];
						return m_i_rv;
					}
				}else if (chanCmd==1){
					u8 recOBUID[4] = {0};
					u8 j;
					
					for (j=0; j<4; j++)
						recOBUID[j] = out_u8_rvbuf[j];
					
					for(j=0; j<4; j++){
						if (OBU_id[j]!=recOBUID[j])
							break;
					}
					if (j==4){
					}else{
						continue;
					}
					
					*out_u8_ResCount = (in_u8_TxCount -i);
					BST_flag = 0;
					return m_i_rv;
				}
			}
		}
	}
	*out_u8_ResCount = (in_u8_TxCount -i);
	return m_i_rv;
}
