#include "stm32f10x.h"
#include "shell_port.h"

uint8_t AntSwitchState = 0;					//Ĭ�Ϲر����߷���
uint8_t AntSwitchCloseFlag = 0;				//���߹رձ�־λ,RSU��ǰ�����ڽ��׻��ʶ����ʱ,�յ��˹ر���������,�ɽ�����ɺ��ٹر�����.
uint8_t CurrentObuId[4] = {0};
//uint8_t SuccessObuId[5][4] = {0};
uint8_t SuccIndex = 4;
uint8_t BstFlag = 0;
uint8_t chanCmd = 0;    					// ͨ��ָ��
uint8_t RsuIdFlag = 0;
uint8_t SL1102_RSSI = 0;
uint8_t RSU_CurrentMode = RSU_IDLE;
uint8_t B3_C1Flag = 0;

uint8_t HeartFlag = 0;
//uint8_t ExAuthFlag = 0;

uint8_t GetSecureFlag = 0;
uint8_t BstBecanIdFlag = 0;
uint8_t PC_ReceivedFlag = 0;
//uint8_t EnterB6_Count = 0;


const uint8_t HeartTAB[8] = {0x88,0xB2,0x00 ,0x00 ,0x00 ,0x00 ,0x80};
SuccessObuInfoDef SuccessObuInfoStructure[SUCC_MAX];

void RSU_IdleMode(void)
{
	SLTX_PWR_LOW;
	
}

void TX_CtrlOpen(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9 );
}

void TX_CtrlClose(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);
}

void RSU_SendHeartFrame(void)
{
	RSU_USART_SendFunction((uint8_t *)HeartTAB,7);
}

void RSU_SendB2Frame(void)
{	
	RSU_ObuInfoStructure.ErrorCode = 0;
	RSU_DataFrameCount = sizeof(RSU_ObuInfoStructure);	
	memcpy(RSU_ObuInfoStructure.ObuId,OBU_VST_Info.MacAdrr,4);
	memcpy(RSU_ObuInfoStructure.Issuer,OBU_VST_Info.EsamSysInfoIssueFlag,26);
//	RSU_ObuInfoStructure.SL1102_RSSI = SL1102_RSSI;
	
	if((OBU_VST_Info.Player[8] & 0x20) == 0x20)				//��ICC
	{
		memcpy(&RSU_ObuInfoStructure.EquitmentCV,&OBU_VST_Info.EquipmentInfo,3);
		memcpy(&RSU_ObuInfoStructure.CPUCardID,&OBU_VST_Info.IccSysInfoCarNum,10);
	}
	else													//��ICC
	{
		memcpy(&RSU_ObuInfoStructure.EquitmentCV,&OBU_VST_Info.IccSysInfoIssueFlag[3],3);		//û��ICCʱ
		memset(&RSU_ObuInfoStructure.CPUCardID,0,10);		//û��ICCʱ
	}
	
//	memset(&RSU_ObuInfoStructure.nMark,0xFF,9);
	
	RSU_ObuInfoStructure.RSCTL = 0x18;
	
	RSU_USART_SendFunction((uint8_t *)&RSU_ObuInfoStructure,RSU_DataFrameCount);
}


void RSU_SendB3Frame(void)
{
	memcpy(RSU_CarInfoStructure.ObuId,OBU_VST_Info.MacAdrr,4);
	
//	if(GetSecureFlag == 1)
//	{
//		memset(RSU_CarInfoStructure.Vehicle_info,0,59);
//		memcpy(RSU_CarInfoStructure.Vehicle_info,PSAM_CarDedes32_rs.CarInfo + 9,16);
//	}
//	else
//	{
//		memcpy(RSU_CarInfoStructure.Vehicle_info,PSAM_CarDedes_rs.CarInfo + 9,59);
//	}
	
	memcpy(RSU_CarInfoStructure.Vehicle_info,PSAM_CarDedes_rs.CarInfo + 9,79);
	
	RSU_DataFrameCount = sizeof(RSU_CarInfoStructure);
	
	RSU_CarInfoStructure.RSCTL = 0x28;
	
	RSU_USART_SendFunction((uint8_t *)&RSU_CarInfoStructure,RSU_DataFrameCount);
	
	memset(PSAM_CarDedes_rs.CarInfo + 9,0,79);
	
	if(RSU_CarInfoStructure.ErrorCode != 0x00)
	{
		OBU_BST_Info[11]++;
		TickCount = 0;
	}
}

void RSU_SendB4Frame(void)
{	
	memcpy(RSU_IccInfoStructure.ObuId,OBU_VST_Info.MacAdrr,4);
	
	memcpy(RSU_IccInfoStructure.CardRestMoney,Transfer_ICC_Money_Read0019_rs.ICC_Money,4);				//4
	memcpy(RSU_IccInfoStructure.IssuerInfo,OBU_VST_Info.IccSysInfoIssueFlag,43);						//50
	memcpy(RSU_IccInfoStructure.LastStation,Transfer_ICC_Money_Read0019_rs.ICC_0019,43);				//63
	
	RSU_IccInfoStructure.RSCTL = 0X48;
	if(PC_InitStructure.EF04_Opt == 1)
	{
		memcpy(RSU_IccInfoStructure.EF04_Off,PC_InitStructure.EF04_Off,2);
		memcpy(RSU_IccInfoStructure.EF04_Len,PC_InitStructure.EF04_Len,2);
//		memcpy(RSU_IccInfoStructure.EF04_Info,Transfer_ESAM_ReadEF04_rs.ReadEF04Data,Transfer_ESAM_ReadEF04_rs.ReadEF04Header_Len - 2);
		if(PC_InitStructure.LaneMode == ETC_ENTRY)				//���
		{
			memset(RSU_IccInfoStructure.EF04_Info,0,PC_InitStructure.EF04_Len[1]);
		}
		else if(PC_InitStructure.LaneMode == ETC_EXIT)			//����
		{
//			memcpy(RSU_IccInfoStructure.EF04_Info,Transfer_ESAM_ReadEF04_rs.ReadEF04Data,PC_InitStructure.EF04_Len[1]);
			memcpy(RSU_IccInfoStructure.EF04_Info,Transfer_SelectReadEsamEF04_rs.SelectReadEF04_Data + Transfer_SelectReadEsamEF04_rs.SelectEF04_Len + 1,PC_InitStructure.EF04_Len[1]);
		}
		RSU_DataFrameCount = 103 + PC_InitStructure.EF04_Len[1];
	}
	else
	{
		RSU_IccInfoStructure.ReadEf04Status = 1;
		RSU_DataFrameCount = 99;
	}
	
	RSU_USART_SendFunction((uint8_t *)&RSU_IccInfoStructure,RSU_DataFrameCount);
	
	memset(Transfer_ICC_Money_Read0019_rs.ICC_0019,0,43);
	
	if(RSU_IccInfoStructure.ErrorCode != 0x00)
	{
		OBU_BST_Info[11]++;
		TickCount = 0;
	}
}

void RSU_SendB5Frame(void)
{
	memcpy(RSU_TransInfoStructure.ObuId,OBU_VST_Info.MacAdrr,4);
//	memcpy(RSU_TransInfoStructure.WrFileTime,OBU_BST_Info + 12,4);			//BST���Ѷ�ȡ�����µ�ʱ��
	memcpy(RSU_TransInfoStructure.WrFileTime,PC_PurchaseStructure.Station + 5,4);			//ֱ��ʹ��C6֡��0019�ļ�unixʱ��
	memcpy(RSU_TransInfoStructure.TransTime,PC_PurchaseStructure.PurchaseTime,7);
	RSU_TransInfoStructure.TransType = 0x09;
	if(PC_PurchaseStructure.TradeMode == 0x01)
	{
		memset(RSU_TransInfoStructure.TAC,0,4);
		memset(RSU_TransInfoStructure.IccPaySerial,0,2);
		memset(RSU_TransInfoStructure.PsamTransSerial,0,4);
		memset(RSU_TransInfoStructure.CardBalance,0,4);
	}
	else
	{
		memcpy(RSU_TransInfoStructure.TAC,Transfer_ICC_CappPurchase_GetMoney_rs.CappPurchase_Tac,4);
		memcpy(RSU_TransInfoStructure.IccPaySerial,Transfer_ICC_CappPurchaseInit_rs.CappInit_PurchaseSN,2);
		memcpy(RSU_TransInfoStructure.PsamTransSerial,PSAM_ICC_CappPurseMac1_rs.CappMac1_EquimPurseSN,4);
		memcpy(RSU_TransInfoStructure.CardBalance,Transfer_ICC_CappPurchase_GetMoney_rs.GetMoney_Money,4);
	}
	
	if(RSU_StatusStructure.PsamAuthStatus == 1)							//PSAM���Ƿ���Ҫ��Ȩ
	{
		RSU_TransInfoStructure.KeyType = 4;						//PSAM����Ҫ��Ȩ����ΪSM4----4
	}
	else
	{
		RSU_TransInfoStructure.KeyType = 0;						//PSAM������Ҫ��Ȩ����Ϊ3DES----0
	}
	
//	RSU_TransInfoStructure.SL1102_RSSI = SL1102_RSSI;
	
	RSU_DataFrameCount = sizeof(RSU_TransInfoStructure);
	RSU_TransInfoStructure.RSCTL = 0x58;
	RSU_USART_SendFunction((uint8_t *)&RSU_TransInfoStructure,RSU_DataFrameCount);
	
//	memset(Transfer_ICC_CappPurchase_GetMoney_rs.CappPurchase_Tac,0,4);
//	memset(Transfer_ICC_CappPurchaseInit_rs.CappInit_PurchaseSN,0,2);
//	memset(PSAM_ICC_CappPurseMac1_rs.CappMac1_EquimPurseSN,0,4);
//	memset(Transfer_ICC_CappPurchase_GetMoney_rs.GetMoney_Money,0,4);
	
	if(RSU_TransInfoStructure.ErrorCode != 0x00)
	{
		OBU_BST_Info[11]++;
		TickCount = 0;
	}
}

void RSU_SendB9Frame(void)
{
	memcpy(RSU_PassMoneyStructure.ObuId,OBU_VST_Info.MacAdrr,4);
	memcpy(RSU_PassMoneyStructure.EF04_Info,Transfer_ESAM_ReadWriteEF04_rs.ReadWriteEF04Data,PC_PassMoneyStructure.EF04_Len[1]);
	
	RSU_DataFrameCount = 7 + PC_PassMoneyStructure.EF04_Len[1];
	RSU_PassMoneyStructure.RSCTL = 0x68;
	RSU_USART_SendFunction((uint8_t *)&RSU_PassMoneyStructure,RSU_DataFrameCount);
}

uint32_t LedCount = 0;

void RSU_B2Mode(void)
{		
	if((OBU_BST_Info[11] != RSU_StatusStructure.RsuId[2]) && (TickCount > 1200))
	{
		OBU_BST_Info[11] = RSU_StatusStructure.RsuId[2];
	}
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)==0) 				//��ⰴť����,Ϊ0���ʾ�ɿ���ť
	{
		RedLedL;
		GreLedH;
		if(OBU_Bst_func() == DsrcSecussed)
		{		
			RSU_SendB2Frame();		
			RSU_CurrentMode = RSU_B2_C1;
		}
	}
	else
	{
		GreLedL;
		
		LedCount++;
		if(LedCount < 100000)
		{
			RedLedH;	
		}
		else if((LedCount >= 100000) && (LedCount < 200000))
		{
			RedLedL;
		}
		else
		{
			LedCount = 0;
		}	
	}
	
}

uint8_t RSU_B3Mode(void)
{
	uint8_t dsrc_flag = 0;
	
	BK5824_ConfigPower(15);
	SLRXrxSen_set(0);
	
	log_title("Enter B3");
/*******************************	B3	*************************************/
	RSU_CarInfoStructure.ErrorCode = 0x00;	
	if(OBU_GetSecure_func() != DsrcSecussed)
	{
		log_title("ERR///////////////////////");
		RSU_CarInfoStructure.ErrorCode = 0x04;						//��DSRC���ݷ���
		RSU_SendB3Frame();
		RSU_CurrentMode = RSU_B3_C1;
		return 1;
	}
	dsrc_flag = PSAM_CarDedes_func();
	if(dsrc_flag != DsrcSecussed)
	{
		log_title("ERR///////////////////////");
		if(dsrc_flag == 5)
		{
			log_title("PSAM��δ��Ȩ");
			RSU_CarInfoStructure.ErrorCode = 0x2E;					//PSAM��δ��Ȩ
		}
		else
		{
			RSU_CarInfoStructure.ErrorCode = 0x06;						//���ܳ�����Ϣʧ��
		}
		RSU_SendB3Frame();
		RSU_CurrentMode = RSU_B3_C1;
		return 2;
	}
	
	RSU_SendB3Frame();
	RSU_CurrentMode = RSU_B3_C1;										//B3��ȷ,ֱ�ӽ���B4
	return 0;	
}

uint8_t RSU_B4Mode(void)
{
	uint8_t dsrc_flag = 0;
	
	log_title("Enter B4");
	RSU_IccInfoStructure.ErrorCode = 0x00;
/*******************************	B4	*************************************/		
	dsrc_flag = ICC_Money_Read0019_func();
	if(dsrc_flag != DsrcSecussed)
	{
		log_title("ERR///////////////////////");
		if(dsrc_flag == 2)
		{
			RSU_IccInfoStructure.ErrorCode = 0x08;						//��DSRC���ݷ���
		}
		else
		{
			RSU_IccInfoStructure.ErrorCode = 0x09;						//��0019ʧ��
			RSU_IccInfoStructure.ReadIccStatus = 1;
		}
		RSU_SendB4Frame();
		RSU_CurrentMode = RSU_B4_C6;
		return 3;
	}
		
	RSU_IccInfoStructure.ReadIccStatus = 0;
	
/*******************************EF04*************************************/
	RSU_IccInfoStructure.ReadEf04Status = 1;
	if(PC_InitStructure.EF04_Opt == 1)
	{
		//��EF04
		dsrc_flag = SelectReadEsamEF04_func();
		if(dsrc_flag != DsrcSecussed)
		{
			log_title("ERR///////////////////////");
			if(dsrc_flag == 2)
			{
				RSU_IccInfoStructure.ErrorCode = 0x20;					//��ȡEF04�����ݷ���
			}
			else
			{
				RSU_IccInfoStructure.ErrorCode = 0x21;					//��ȡEF04ʧ��
			}
			RSU_IccInfoStructure.ReadEf04Status = 1;					//��EF04ʧ��Ϊ1
			RSU_SendB4Frame();
			RSU_CurrentMode = RSU_B4_C6;
			return 4;
		}
		RSU_IccInfoStructure.ReadEf04Status = 0;						//��EF04�ɹ�Ϊ0
	}
	
	RSU_SendB4Frame();
	RSU_CurrentMode = RSU_B4_C6;
	return 0;
}

uint8_t RSU_B5Mode(void)
{
	uint8_t dsrc_flag = 0;
	
	log_title("Enter B5");	
	RSU_TransInfoStructure.ErrorCode = 0x00;
	
	if(PC_InitStructure.LaneMode == ETC_ENTRY)
	{
		//�����дEF04,������
//		if(SelectEsamDF01_EF04_func() != DsrcSecussed)				//ѡ��ESAM��DF01Ŀ¼
//		{
//			log_title("ERR///////////////////////");
//			RSU_TransInfoStructure.ErrorCode = 0x52;					//�Զ���Ĵ�����
//			RSU_SendB5Frame();
//			RSU_CurrentMode = RSU_B5_C1;
//			return 3;
//		}
		
		RSU_TransInfoStructure.EF04InfoWriteStatus = 0x00;
		if((PC_PurchaseStructure.TradeMode == 0x01) || (PC_PurchaseStructure.TradeMode == 0x02))
		{
			dsrc_flag = ESAM_SelectWriteEF04_func(PC_PurchaseStructure.EF04_Off[0],PC_PurchaseStructure.EF04_Off[1],PC_PurchaseStructure.Ef04Info,PC_PurchaseStructure.EF04_Len[1]);
			if(dsrc_flag != DsrcSecussed)
			{
				log_title("ERR///////////////////////");
				if(dsrc_flag == 2)
				{
					RSU_TransInfoStructure.ErrorCode = 0x22;						//��DSRC���ݷ���
				}
				else
				{
					RSU_TransInfoStructure.ErrorCode = 0x23;						//��EF04ʧ��
				}
				RSU_TransInfoStructure.EF04InfoWriteStatus = 0x01;
				RSU_SendB5Frame();
				RSU_CurrentMode = RSU_B5_C1;
				return 3;
			}				
		}
		
		if((PC_PurchaseStructure.TradeMode == 0x00) || (PC_PurchaseStructure.TradeMode == 0x02))
		{
			dsrc_flag = ICC_CappPurchaseInit_func();
			if(dsrc_flag != DsrcSecussed)
			{
				log_title("ERR///////////////////////");
				if(dsrc_flag == 2)
				{
					RSU_TransInfoStructure.ErrorCode = 0x10;						//��ICC,��DSRC���ݷ���
				}
				else
				{
					RSU_TransInfoStructure.ErrorCode = 0x11;
				}
				
		//		SetMMI_func();								//���۽��׳ɹ����,������SetMMI
				RSU_SendB5Frame();
				RSU_CurrentMode = RSU_B5_C1;
				return 1;
			}
			if(PSAM_ICC_CappPurseMac1_func() != DsrcSecussed)
			{
				log_title("ERR///////////////////////");
				RSU_TransInfoStructure.ErrorCode = 0x13;						//MAC1���㷵��״̬��!=9000
				
		//		SetMMI_func();								//���۽��׳ɹ����,������SetMMI
				RSU_SendB5Frame();
				RSU_CurrentMode = RSU_B5_C1;
				return 3;
			}
			dsrc_flag = ICC_CappPurchase_func();
			if(dsrc_flag != DsrcSecussed)
			{
				log_title("ERR///////////////////////");
				if(dsrc_flag == 2)
				{
					RSU_TransInfoStructure.ErrorCode = 0x16;						//��ICC,��DSRC���ݷ���
				}
				else
				{
					RSU_TransInfoStructure.ErrorCode = 0x17;						//�������ѷ���״̬��!=9000
				}
				
		//		SetMMI_func();								//���۽��׳ɹ����,������SetMMI
				RSU_SendB5Frame();
				RSU_CurrentMode = RSU_B5_C1;
				return 4;
			}
			
			if(PSAM_CheckMac2_func() != DsrcSecussed)
			{
				log_title("ERR///////////////////////");
				log_title("-------------MAC2У��ʧ��--------------");
				RSU_TransInfoStructure.ErrorCode = 0x19;						//MAC2У�鷵��״̬��!=9000
				RSU_SendB5Frame();
				RSU_CurrentMode = RSU_B5_C1;
				return 3;
			}		
		}
	}
	else
	{
		if((PC_PurchaseStructure.TradeMode == 0x00) || (PC_PurchaseStructure.TradeMode == 0x03))
		{
			dsrc_flag = ICC_CappPurchaseInit_func();
			if(dsrc_flag != DsrcSecussed)
			{
				log_title("ERR///////////////////////");
				if(dsrc_flag == 2)
				{
					RSU_TransInfoStructure.ErrorCode = 0x10;						//��ICC,��DSRC���ݷ���
				}
				else
				{
					RSU_TransInfoStructure.ErrorCode = 0x11;
				}
				
		//		SetMMI_func();								//���۽��׳ɹ����,������SetMMI
				RSU_SendB5Frame();
				RSU_CurrentMode = RSU_B5_C1;
				return 1;
			}
			if(PSAM_ICC_CappPurseMac1_func() != DsrcSecussed)
			{
				log_title("ERR///////////////////////");
				RSU_TransInfoStructure.ErrorCode = 0x13;						//MAC1���㷵��״̬��!=9000
				
		//		SetMMI_func();								//���۽��׳ɹ����,������SetMMI
				RSU_SendB5Frame();
				RSU_CurrentMode = RSU_B5_C1;
				return 3;
			}
			dsrc_flag = ICC_CappPurchase_func();
			if(dsrc_flag != DsrcSecussed)
			{
				log_title("ERR///////////////////////");
				if(dsrc_flag == 2)
				{
					RSU_TransInfoStructure.ErrorCode = 0x16;						//��ICC,��DSRC���ݷ���
				}
				else
				{
					RSU_TransInfoStructure.ErrorCode = 0x17;						//�������ѷ���״̬��!=9000
				}
				
		//		SetMMI_func();								//���۽��׳ɹ����,������SetMMI
				RSU_SendB5Frame();
				RSU_CurrentMode = RSU_B5_C1;
				return 4;
			}
			
			if(PSAM_CheckMac2_func() != DsrcSecussed)
			{
				log_title("ERR///////////////////////");
				log_title("-------------MAC2У��ʧ��--------------");
				RSU_TransInfoStructure.ErrorCode = 0x19;						//MAC2У�鷵��״̬��!=9000
				RSU_SendB5Frame();
				RSU_CurrentMode = RSU_B5_C1;
				return 3;
			}		
		}
		
		RSU_TransInfoStructure.EF04InfoWriteStatus = 0x00;
		if((PC_PurchaseStructure.TradeMode == 0x01) || (PC_PurchaseStructure.TradeMode == 0x03))
		{
			dsrc_flag = ESAM_SelectWriteEF04_func(PC_PurchaseStructure.EF04_Off[0],PC_PurchaseStructure.EF04_Off[1],PC_PurchaseStructure.Ef04Info,PC_PurchaseStructure.EF04_Len[1]);
			if(dsrc_flag != DsrcSecussed)
			{
				log_title("ERR///////////////////////");
				if(dsrc_flag == 2)
				{
					RSU_TransInfoStructure.ErrorCode = 0x22;						//��DSRC���ݷ���
				}
				else
				{
					RSU_TransInfoStructure.ErrorCode = 0x23;						//��EF04ʧ��
				}
				RSU_TransInfoStructure.EF04InfoWriteStatus = 0x01;
				RSU_SendB5Frame();
				RSU_CurrentMode = RSU_B5_C1;
				return 3;
			}				
		}
		
	}
	
	log_title("Send B5");
	RSU_SendB5Frame();
	
	if(PC_InitStructure.LaneMode == ETC_EXIT)
	{
		RSU_CurrentMode = RSU_B5_C9;
	}
	else
	{
		//��ڣ���EF04��411--430����20�ֽ�
//		ESAM_WriteEF04_406_func();
		
		SetMMI_func();								//����SetMMI
		EventReport_func();
		OBU_BST_Info[11] = RSU_StatusStructure.RsuId[2];
		SuccIndex++;
		if(SuccIndex >= SUCC_MAX)
		{
			SuccIndex = 0;
		}
		memcpy(SuccessObuInfoStructure[SuccIndex].ObuId,OBU_VST_Info.MacAdrr,4);
		memcpy(SuccessObuInfoStructure[SuccIndex].UnixTime,OBU_BST_Info + 12,4);
		RSU_CurrentMode = RSU_B5_C1;
	}
	
	return 0;
}

uint8_t RSU_ClearMode(void)
{	
	log_title("Enter Clear");
	
	ESAM_ClearEF04_func();
	SetMMI_func();								//����SetMMI
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
	
	return 0;
}

uint8_t RSU_B9Mode(void)
{
	uint8_t dsrc_flag = 0;
	
	log_title("Enter B9");	
	RSU_PassMoneyStructure.ErrorCode = 0x00;
	
	if(PC_PassMoneyStructure.EF04_Len[1] == 0)
	{
		dsrc_flag = ESAM_ClearEF04_func();
		if(dsrc_flag != DsrcSecussed)
		{
			log_title("ERR///////////////////////");
			if(dsrc_flag == 2)
			{
				RSU_PassMoneyStructure.ErrorCode = 0x08;						//��DSRC���ݷ���
			}
			else
			{
				RSU_PassMoneyStructure.ErrorCode = 0x23;						//��EF04ʧ��
			}
		}
	}
	else
	{
		dsrc_flag = ESAM_ReadWriteEF04_func();
		if(dsrc_flag != DsrcSecussed)
		{
			log_title("ERR///////////////////////");
			if(dsrc_flag == 2)
			{
				RSU_PassMoneyStructure.ErrorCode = 0x08;						//��DSRC���ݷ���
			}
			else
			{
				RSU_PassMoneyStructure.ErrorCode = 0x23;						//��EF04ʧ��
			}
		}
	}
	
	log_title("Send B9");
	RSU_SendB9Frame();
	
	SetMMI_func();								//����SetMMI
	EventReport_func();
	OBU_BST_Info[11] = RSU_StatusStructure.RsuId[2];
	SuccIndex++;
	if(SuccIndex >= SUCC_MAX)
	{
		SuccIndex = 0;
	}
	memcpy(SuccessObuInfoStructure[SuccIndex].ObuId,OBU_VST_Info.MacAdrr,4);
	memcpy(SuccessObuInfoStructure[SuccIndex].UnixTime,OBU_BST_Info + 12,4);
	
//	RSU_CurrentMode = RSU_B9_C1;
	RSU_CurrentMode = RSU_B2;
	
	return 0;
}

uint8_t RSU_WorkMode(uint8_t current_mode)
{
	uint8_t status = RunError;
	
	switch(current_mode)
	{
		case RSU_IDLE:
			RSU_IdleMode();

			break;
		case RSU_B2:
			RSU_B2Mode();
			SysTickCount = 0;
			break;
		case RSU_B3:
			RSU_B3Mode();
			SysTickCount = 0;
			break;
		case RSU_B4:
			RSU_B4Mode();
			SysTickCount = 0;
			break;
		case RSU_B5:
			RSU_B5Mode();
			SysTickCount = 0;
			break;
		case RSU_B9:
			RSU_B9Mode();
			SysTickCount = 0;
			break;
		case RSU_B2_C1:
		case RSU_B3_C1:
		case RSU_BA_C1:
		case RSU_B5_C1:
		case RSU_B4_C6:
		case RSU_B5_C9:
		case RSU_B9_C1:
			if(SysTickCount > 10)				//1S��ʱ
			{
				SysTickCount = 0;
				RSU_CurrentMode = RSU_B2;
//				BK5824ModeSet(0);
//				SLTX_PWR_LOW;
				log_title("TimeOut!!");
			}			
			break;
		default:
			break;
	}
	return status;
}



void ClearSuccessObuId(void)
{
	uint8_t kk;
	uint32_t succ_time;
	uint32_t bst_time;
	
	bst_time = ((OBU_BST_Info[12] << 24) | (OBU_BST_Info[13] << 16) | (OBU_BST_Info[14] << 8) | (OBU_BST_Info[15]));
	
	for(kk = 0; kk < SUCC_MAX; kk++)
	{
		succ_time = ((SuccessObuInfoStructure[kk].UnixTime[0] << 24) | (SuccessObuInfoStructure[kk].UnixTime[1] << 16)
						| (SuccessObuInfoStructure[kk].UnixTime[2] << 8) | (SuccessObuInfoStructure[kk].UnixTime[3]));
		if((bst_time - succ_time) > 10)
		{
			memset(SuccessObuInfoStructure[kk].ObuId,0,4);
		}
	}
}


