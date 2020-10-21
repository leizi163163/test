#include "stm32f10x.h"

extern unsigned char PcSn;

uint8_t PSAM_ResetFlag = 1;					//5个PSAM卡的复位成功标志
uint8_t PSAM_AuthFlag = 0;				//PSAM卡认证标志	0需要认证,  1已经认证成功
uint8_t RSU_DataFrameCount = 0;

PC_InitDef 				PC_InitStructure = {
	0x80,	0xC0,	0x5D,0x0D,0x98,0x84,	0x20,0x19,0x06,0x22,0x10,0x55,0x00,
	0x03,0x0A,0x1F,0x00,
};

PC_ContinueDef 			PC_ContinueStructure;
PC_StopDef				PC_StopStructure;
PC_SwitchDef			PC_SwitchStructure;
PC_PurchaseDef			PC_PurchaseStructure;
PC_PassMoneyDef			PC_PassMoneyStructure;
PC_GetTACDef			PC_GetTACStructure;
PC_PsamAuthInitDef		PC_PsamAuthInitStructure;
PC_PsamAuthDef			PC_PsamAuthStructure;

PC_SetRsuIdDef			PC_SetRsuIdStructure;


//康定出口ID	02ECC33C	0x02,0xEC,0xC3,0x3C,				0x46,0x02,0x00,0x18,
//康定入口ID	02FB6DDE	0x02,0xFB,0x6D,0xDE,
RSU_StatusDef 			RSU_StatusStructure = {
	0x08,	0xB0,	0x00,	
	0x01,0x05,0x00,		0x11,0x22,0x33,0x44,0x55,0x66,
	0x00,	0x46,0x01,0x20,0x01,	0x22,0x01,
	0x00,0x00,0x00,0x00,0x00
};
RSU_ObuInfoDef 			RSU_ObuInfoStructure = {
	0x18,0xB2,	0x00
};
RSU_CarInfoDef			RSU_CarInfoStructure = {
	0x28,0xB3,	0x00
};
RSU_IccInfoDef 			RSU_IccInfoStructure = {
	0x48,0xB4,	0x00
};
RSU_TransInfoDef 		RSU_TransInfoStructure = {
	0x58,0xB5,	0x00
};

RSU_PassMoneyDef		RSU_PassMoneyStructure = {
	0x68,0xB9,	0x00
};

RSU_PsamAuthInitDef		RSU_PsamAuthInitStructure = {
	0x78,0xBD,	0x00
};

RSU_PsamAuthDef			RSU_PsamAuthStructure = {
	0x88,0xBF,	0x00
};


OBU_FalseListDef		OBU_FalseListStructure[10] = {0};
uint8_t OBU_FalseListAddr = 0;

uint8_t SendBuf[256] = {0xFF,0xFF,0x00};
uint8_t SendBufCount = 0;

UnixTimeDef UnixTimeUnion;

void RSU_USART_SendFunction(uint8_t *buf,uint8_t buf_len)
{
	uint8_t j;
	uint8_t bcc = 0;
		
	SendBufCount = 2;				//去掉帧头FFFF
	SendBuf[0] = 0xFF;
	SendBuf[1] = 0xFF;
	
	for(j = 0; j < buf_len; j++)
	{
		bcc ^= buf[j];
		if(buf[j] == 0xFF)
		{
			SendBuf[SendBufCount++] = 0xFE;
			SendBuf[SendBufCount++] = 0x01;
		}
		else if(buf[j] == 0xFE)
		{
			SendBuf[SendBufCount++] = 0xFE;
			SendBuf[SendBufCount++] = 0x00;
		}
		else
		{
			SendBuf[SendBufCount++] = buf[j];
		}
	}
	//bcc
	if(bcc == 0xFF)
	{
		SendBuf[SendBufCount++] = 0xFE;
		SendBuf[SendBufCount++] = 0x01;
	}
	else if(bcc == 0xFE)
	{
		SendBuf[SendBufCount++] = 0xFE;
		SendBuf[SendBufCount++] = 0x00;
	}
	else
	{
		SendBuf[SendBufCount++] = bcc;
	}
	//FF
	SendBuf[SendBufCount++] = 0xFF;
		
	while(DMA1_Channel4->CNDTR != 0);
	DMA_ClearFlag(DMA1_FLAG_TC4);
	
	DMA_Cmd (DMA1_Channel4,DISABLE);
	DMA1_Channel4->CMAR = (uint32_t)SendBuf;
	DMA1_Channel4->CNDTR = SendBufCount;
	DMA_Cmd(DMA1_Channel4, ENABLE);
	
	log_hex("Rsu Tx: ",SendBuf,SendBufCount);
}

void RSU_Frame98Fuction(void)
{
	RSU_StatusStructure.RsuStatus = 0x00;
	
	PSAM_ResetFlag = PSAMReset(&Psam2);
	if(PSAM_ResetFlag == 0)
	{
		if(USER_ReadPSAM(PSAM_SLOT_1) == 0)
		{
			memcpy(RSU_StatusStructure.PsamTerminal,PSAM_SerialNum[PSAM_SLOT_1],6);
			RSU_StatusStructure.PsamVer = Psam0015Structure[PSAM_SLOT_1].PsamVersion;

			RSU_StatusStructure.PsamChannelId = 1;
			
			if((RSU_StatusStructure.PsamVer & 0x0F) >= 0x05)
			{
//				RSU_StatusStructure.PsamAuthStatus = PSAM_AuthFlag;
				RSU_StatusStructure.PsamAuthStatus = 0x01;
			}
			else
			{
				RSU_StatusStructure.PsamAuthStatus = 0x00;
			}		
		}
		else
		{
			RSU_StatusStructure.RsuStatus = 0x03;
		}
	}
	else
	{
		RSU_StatusStructure.RsuStatus = 0x01;
	}
	RSU_StatusStructure.RSCTL = 0x98;
	RSU_USART_SendFunction((uint8_t *)&RSU_StatusStructure,sizeof(RSU_StatusStructure));
}

void USER_SetUnixTime(void)
{
	UnixTimeCount = 0;
	UnixTimeUnion.UnixTime = (PC_InitStructure.UnixTime[0] << 24) | (PC_InitStructure.UnixTime[1] << 16) | 
								(PC_InitStructure.UnixTime[2] << 8) | PC_InitStructure.UnixTime[3];
}

void PC_InitFunction(void)
{	
	RSU_StatusStructure.RsuStatus = 0x00;

	memcpy(OBU_BST_Info + 8,&RSU_StatusStructure.RsuManuId,4);				//更改BST中RsuId
	USER_SetUnixTime();														//UNIX时间不准，用这个函数代替
	OBU_BST_Info[16] = PC_InitStructure.PLLChannelID;
	
	BK5824_CHANGECHANNEL(PC_InitStructure.PLLChannelID);
	SL1102RX_Init();								//有些天线只配置信道号，配置不成功

//	PC_InitStructure.TxPower = (uint8_t)(PC_InitStructure.TxPower / 2);
	//BK5823TX_ConfigPower(PC_InitStructure.TxPower);
	if(PC_InitStructure.TxPower > 15)
	{
		PC_InitStructure.TxPower = 15;
	}
	BK5824_ConfigPower(PC_InitStructure.TxPower);
//	BK5824_ConfigPower(6);
	
	PSAM_ResetFlag = PSAMReset(&Psam2);
	if(PSAM_ResetFlag == 0)
	{
		if(USER_ReadPSAM(PSAM_SLOT_1) == 0)
		{
			memcpy(RSU_StatusStructure.PsamTerminal,PSAM_SerialNum[PSAM_SLOT_1],6);
			RSU_StatusStructure.PsamVer = Psam0015Structure[PSAM_SLOT_1].PsamVersion;

			RSU_StatusStructure.PsamChannelId = 1;
			
			if((RSU_StatusStructure.PsamVer & 0x0F) >= 0x05)
			{
//				RSU_StatusStructure.PsamAuthStatus = PSAM_AuthFlag;
				RSU_StatusStructure.PsamAuthStatus = 0x01;
			}
			else
			{
				RSU_StatusStructure.PsamAuthStatus = 0x00;
			}		
		}
		else
		{
			RSU_StatusStructure.RsuStatus = 0x03;
		}
	}
	else
	{
		RSU_StatusStructure.RsuStatus = 0x01;
	}
	RSU_StatusStructure.RSCTL = 0x08;
	RSU_USART_SendFunction((uint8_t *)&RSU_StatusStructure,sizeof(RSU_StatusStructure));
	
	if(RSU_StatusStructure.RsuStatus == 0x00)
	{
		RSU_CurrentMode = RSU_B0_C1;
		memcpy(Psam2.Cmd,Select3F00,Select3F00Len);			//选择PSAM卡3F00目录
		Psam2.CmdLen=Select3F00Len;
		PSAMTransferAPDU(&Psam2);
		
		memcpy(Psam2.Cmd,SelectDF01,SelectDF01Len);			//选择PSAM卡DF01目录
		Psam2.CmdLen=SelectDF01Len;
		PSAMTransferAPDU(&Psam2);
	}
	else
	{
		RSU_CurrentMode = RSU_IDLE;
//		RSU_CurrentMode = RSU_B2;
		
	}

}

void PC_ContinueFunction(uint8_t current_mode)
{
	switch(current_mode)
	{
		case RSU_B0_C1:
		case RSU_BF_C1:
			RSU_CurrentMode = RSU_B2;
			break;
		
		case RSU_B2_C1:
			RSU_CurrentMode = RSU_B3;
			break;
		case RSU_B3_C1:
			RSU_CurrentMode = RSU_B4;
//			B3_C1Flag = 1;
			break;
		case RSU_B5_C9:									//出口模式，有时车道不回C9直接回C1
			SetMMI_func();								//发送SetMMI
			EventReport_func();
			OBU_BST_Info[11] = RSU_StatusStructure.RsuId[2];
			SuccIndex++;
			if(SuccIndex >= SUCC_MAX)
			{
				SuccIndex = 0;
			}
			memcpy(SuccessObuInfoStructure[SuccIndex].ObuId,OBU_VST_Info.MacAdrr,4);
			memcpy(SuccessObuInfoStructure[SuccIndex].UnixTime,OBU_BST_Info + 12,4);
			RSU_CurrentMode = RSU_B2;
			break;
		case RSU_B5_C1:									//入口模式，上传B5后，车道直接回C1
		case RSU_B9_C1:									//出口模式，上传B9后，车道回C1
			RSU_CurrentMode = RSU_B2;
			break;
		case RSU_B6_ERR:
		case RSU_IDLE:
			RSU_CurrentMode = RSU_B2;
			break;
		
		default:
			break;	
	}		
}

//uint8_t SL1102_ErrorFlag = 0;

void PC_StopFunction(void)
{	
	switch(PC_StopStructure.StopType)
	{
		case RE_FIND_OBU:
			EventReport_func();
			RSU_CurrentMode = RSU_B2;				
			break;
		
		default:
			break;
	}
}

void PC_SwitchFunction(void)
{
	AntSwitchState = PC_SwitchStructure.ControlType;			//天线开关状态
	switch(AntSwitchState)
	{
		case 0:
			RSU_CurrentMode = RSU_IDLE;
			log_title("RSU Close");
			break;
		case 1:
//			if((RSU_CurrentMode == RSU_B2_C1) || (RSU_CurrentMode == RSU_B5_C1) || (RSU_CurrentMode == RSU_B6_C6))
			if(RSU_CurrentMode == RSU_IDLE)
			{
				RSU_CurrentMode = RSU_B2;
				log_title("RSU Open");
			}
			break;	
		default:
			break;		
	}
}

void PC_PurchaseFunction(void)
{
	if(RSU_CurrentMode == RSU_B4_C6)				//20190626
	{
		RSU_CurrentMode = RSU_B5;
	}
}

void PC_PassMoneyFunction(void)
{
	if(RSU_CurrentMode == RSU_B5_C9)				//20190626
	{
		RSU_CurrentMode = RSU_B9;
	}
}

void PC_GetTACFunction(void)
{
	uint8_t dsrc_flag = 0;
	
	RSU_TransInfoStructure.ErrorCode = 0x00;
	
	dsrc_flag = ICC_GetTac_func();
	if(dsrc_flag != DsrcSecussed)
	{
		log_title("ERR///////////////////////");
		if(dsrc_flag == 2)
		{
			RSU_TransInfoStructure.ErrorCode = 0x35;						//无DSRC数据返回
		}
		else
		{
			//重取失败
			memset(RSU_TransInfoStructure.TAC,0,4);
			RSU_TransInfoStructure.ErrorCode = 0x36;						//重取TAC失败
		}
	}
	
	if(PC_GetTACStructure.OptMode == 1)
	{
		dsrc_flag = ESAM_WriteEF04_func(PC_GetTACStructure.EF04_Off[0],PC_GetTACStructure.EF04_Off[1],PC_GetTACStructure.Ef04Info,PC_GetTACStructure.EF04_Len[1]);
		if(dsrc_flag != DsrcSecussed)
		{
			log_title("ERR///////////////////////");
			if(dsrc_flag == 2)
			{
				RSU_TransInfoStructure.ErrorCode = 0x22;						//无DSRC数据返回
			}
			else
			{
				RSU_TransInfoStructure.ErrorCode = 0x23;						//重取TAC失败
			}
		}		
	}
	
	//重取成功
	memcpy(RSU_TransInfoStructure.TAC,Transfer_ICC_GetTAC_rs.GetTAC_Tac,4);
	
	RSU_DataFrameCount = sizeof(RSU_TransInfoStructure);
	RSU_TransInfoStructure.RSCTL = 0x68;
	RSU_USART_SendFunction((uint8_t *)&RSU_TransInfoStructure,RSU_DataFrameCount);
	
	if(PC_InitStructure.LaneMode == ETC_EXIT)
	{
		RSU_CurrentMode = RSU_B5_C9;
	}
	else
	{
		RSU_CurrentMode = RSU_B5_C1;
	}
	
}

void PC_SetRsuIdFunction(void)
{
	if(PC_SetRsuIdStructure.RsuID == 2)
	{
		RsuIdFlag = 1;
	}
	else
	{
		RsuIdFlag = 0;
	}
}

void PC_PsamAuthInitFunction(void)
{
	RSU_PsamAuthInitStructure.ErrorCode = 0x00;
	
	if(USER_ReadPsamAreaCode(PSAM_SLOT_1) == 0)
	{
		memcpy(RSU_PsamAuthInitStructure.PsamAreaCode,PSAM_AreaCode,4);
		if(USER_ReadPsamRand8(PSAM_SLOT_1) == 0)
		{
			memcpy(RSU_PsamAuthInitStructure.PsamRandCode,PSAM_RandData8,8);
		}
		else
		{
			RSU_PsamAuthInitStructure.ErrorCode = 0x01;
		}
	}
	else
	{
		RSU_PsamAuthInitStructure.ErrorCode = 0x02;
	}

	memcpy(RSU_PsamAuthInitStructure.PsamNo,Psam0015Structure[PSAM_SLOT_1].PsamNo,10);
	RSU_PsamAuthInitStructure.PsamVersion = Psam0015Structure[PSAM_SLOT_1].PsamVersion;
	
	RSU_PsamAuthInitStructure.RSCTL = 0x78;
	RSU_USART_SendFunction((uint8_t *)&RSU_PsamAuthInitStructure,sizeof(RSU_PsamAuthInitStructure));
}

void PC_PsamAuthFunction(void)
{
	RSU_PsamAuthStructure.ErrorCode = 0x00;
	
	memcpy(PSAM_ExternalAuth + 5,PC_PsamAuthStructure.PsamMacCode,8);
	
	memcpy(Psam2.Cmd,PSAM_ExternalAuth,13);
	Psam2.CmdLen=13;
	PSAMTransferAPDU(&Psam2);
	if(Psam2.CosSw.result==CosRspOK)
	{
		memcpy(RSU_PsamAuthStructure.PsamAuthSW,Psam2.CosSw.SW,2);
		PSAM_AuthFlag = 1;
	}
	else
	{
		RSU_PsamAuthStructure.ErrorCode = 0x01;
	}
		
	RSU_PsamAuthStructure.RSCTL = 0x88;
	RSU_USART_SendFunction((uint8_t *)&RSU_PsamAuthStructure,sizeof(RSU_PsamAuthStructure));
	
	RSU_CurrentMode = RSU_BF_C1;
	
	memcpy(Psam2.Cmd,SelectDF01,SelectDF01Len);			//选择PSAM卡DF01目录
	Psam2.CmdLen=SelectDF01Len;
	PSAMTransferAPDU(&Psam2);
}
//void PC_DataParse(void)
//{
//	uint8_t ii = 0;
//	
//	if((DataFrameBuff[0] == 0xFF) && (DataFrameBuff[1] == 0xFF) && (DataFrameBuff[DataFrameCount - 1] == 0xFF))
//	{
//		USART1_ReceiveBCC = 0;
//		for(ii = 2; ii < DataFrameCount - 1; ii++)
//		{
////			if(DataFrameBuff[ii] == 0xFF)
////			{
////				RevRepFlag = 1;
////				break;
////			}
//			if(DataFrameBuff[ii] == 0xFE)
//			{
//				USART1_ReceiveDataCount -= 1;
//				if(DataFrameBuff[ii + 1] == 0x00)
//				{
//					USART1_ReceiveBuff[ii] = 0xFE;
//				}
//				else if(USART1_ReceiveBuff[ii + 1] == 0x01)
//				{
//					USART1_ReceiveBuff[ii] = 0xFF;
//				}
//				else
//				{
//					USART1_ReceiveDataCount = 0;
//					ReceiveOverFlag = 0;
//					return;
//				}
//				USART1_ReceiveBCC ^= USART1_ReceiveBuff[ii];
//				memcpy(USART1_ReceiveBuff + ii + 1,USART1_ReceiveBuff + ii + 2,USART1_ReceiveDataCount - ii);		
//			}
//			else
//			{
//				USART1_ReceiveBCC ^= USART1_ReceiveBuff[ii];
//			}		
//		}
//		if(USART1_ReceiveBCC == 0)
//		{				
//			if(RevRepFlag == 1)
//			{
//				RevRepFlag = 0;
//				DataFrameCount = ii + 1;
//			}
//			else
//			{
//				DataFrameCount = USART1_ReceiveDataCount;
//			}
//			
//			memcpy(DataFrameBuff,USART1_ReceiveBuff,DataFrameCount);
//			ReceiveOverFlag = 1;
//		}
//	}
//}

uint8_t PC_DataParse(void)
{
	uint8_t temp = 0;
	uint8_t bcc_temp = 0;
	uint8_t data_len = 0;
	
	do
	{
		if(RX_RingBuf_ReadData(&temp) != 0)
		{
			return 1;
		}
	}
	while(temp != 0xFF);											//找到帧头的第一个FF
	DataFrameBuff[data_len] = temp;
	data_len++;
	
	RX_RingBuf_ReadData(&temp);										//找到帧头的第二个FF
	if(temp != 0xFF)
	{
		return 1;
	}
	DataFrameBuff[data_len] = temp;
	data_len++;
	
	for( ; ; )
	{
		if(RX_RingBuf_ReadData(&temp) != 0)
		{
			return 1;
		}
		
		if(temp != 0xFF)
		{
			if(temp == 0xFE)
			{
				if(RX_RingBuf_ReadData(&temp) != 0)
				{
					return 1;
				}
				
				if(temp == 0x00)
				{
					temp = 0xFE;
				}
				else if(temp == 0x01)
				{
					temp = 0xFF;
				}
				else
				{
					return 2;
				}	
			}	
			DataFrameBuff[data_len] = temp;
			data_len++;
			bcc_temp ^= temp;
		}
		else
		{
			DataFrameBuff[data_len] = temp;
			data_len++;
			
			if(data_len <= 6)										//加帧头字节数大于6才认为帧正确
			{
				log_title("错误数据帧");
				if(data_len == 3)
				{
					RX_RingBufStructure.Head -= 2;
					RX_RingBufStructure.Length += 2;
					log_title("调整缓冲区头部");
				}
				return 3;											//防止解析为FF FF FF这样的帧
			}
			else
			{
				break;												//解析出正确的数据帧，退出for
			}
		}
	}
	
	log_hex("Head: ",&RX_RingBufStructure.Head,1);
	log_hex("Tail: ",&RX_RingBufStructure.Tail,1);
	log_hex("Length: ",&RX_RingBufStructure.Length,1);
	log_hex("DataBuf: ",DataFrameBuff,data_len);
		
	if(bcc_temp == 0)
	{
		DataFrameCount = data_len;
		return 0;
	}
	else
	{
		return 4;
	}
}


void PC_DataProcess(void)
{
	uint8_t cmder;
	
	cmder = DataFrameBuff[3];
	
	switch(cmder)
	{
		case PC_INIT_RQ:
			memcpy(&PC_InitStructure,DataFrameBuff + 2,DataFrameCount - 3);
			PC_InitFunction();
			log_title(">>>>>>>>吉林手持机集成模式<<<<<<<<");
			break;			
		case PC_CONTINUE_RQ:
			memcpy(&PC_ContinueStructure,DataFrameBuff + 2,DataFrameCount - 3);
			PC_ContinueFunction(RSU_CurrentMode);
			memcpy(OBU_VST_Info.MacAdrr,PC_ContinueStructure.ObuId,4);
			break;			
		case PC_STOP_RQ:
			memcpy(&PC_StopStructure,DataFrameBuff + 2,DataFrameCount - 3);
			PC_StopFunction();
			break;			
		case PC_SWITCH_RQ:
			memcpy(&PC_SwitchStructure,DataFrameBuff + 2,DataFrameCount - 3);
			PC_SwitchFunction();
			break;			
		case PC_PURCHASE_RQ:
			memcpy(&PC_PurchaseStructure,DataFrameBuff + 2,DataFrameCount - 3);
//			PC_PurchaseStructure.ConsumeMoney[3] = 1;
//			memset(PC_PurchaseStructure.ConsumeMoney,0,4);			//测试中,先赋值为0
			PC_PurchaseFunction();
			break;	
		case PC_PASS_MONEY_RQ:
			memcpy(&PC_PassMoneyStructure,DataFrameBuff + 2,DataFrameCount - 3);
			PC_PassMoneyFunction();
			break;
		case PC_GET_TAC_RQ:	
			memcpy(&PC_GetTACStructure,DataFrameBuff + 2,DataFrameCount - 3);
			PC_GetTACFunction();
			break;		
		case PC_SET_RSU_ID_RQ:	
//			memcpy(&PC_SetRsuIdStructure,DataFrameBuff + 2,DataFrameCount - 3);
//			PC_SetRsuIdFunction();
			break;
		case PC_PSAM_AUTH_INIT_RQ:	
			memcpy(&PC_PsamAuthInitStructure,DataFrameBuff + 2,DataFrameCount - 3);
			PC_PsamAuthInitFunction();
			break;
		case PC_PSAM_AUTH_RQ:	
			memcpy(&PC_PsamAuthStructure,DataFrameBuff + 2,DataFrameCount - 3);
			PC_PsamAuthFunction();
			break;
		case PC_EVENT_REPORT_RQ:	
			EventReport_func();
			break;
			
		
		default:
			break;
	}	
}




