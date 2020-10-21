#include "stm32f10x.h"

//Operate PSAM Cos Init
ApduStruct	Psam2={0};

//Psam	Operate COS
uint8_t	Select3F00[Select3F00Len]={0x00,0xA4,0x00,0x00,0x02,0x3F,0x00};
uint8_t	SelectDF01[SelectDF01Len]={0x00,0xA4,0x00,0x00,0x02,0xDF,0x01};
uint8_t	ReadPsam0015[5]={0x00,0xb0,0x95,0x00,0x0E};
uint8_t	ReadPsamAreaCode[5]={0x00,0xb0,0x97,0x09,0x08};
uint8_t	ReadGBTerminalID[ReadGBTerminalIDLen]={0x00,0xb0,0x96,0x00,0x06};
uint8_t	GetRandom8[GetRandom8Len]={0x00,0x84,0x00,0x00,0x08};

Psam0015Struct		Psam0015Structure[2] = {0};
uint8_t PSAM_SerialNum[2][6] = {0};							//2个PSAM卡的终端序列号
uint8_t PSAM_AreaCode[8] = {0};
uint8_t PSAM_RandData8[8] = {0};
uint8_t PSAM_ExternalAuth[13] = {0x00,0x82,0x00,0x41,0x08,0x00};


void	PsamParameterInit(void)
{
	Psam2.num=1;//卡槽2
}

uint8_t	PsamSelectDirc(uint8_t	*Dirc,uint8_t	Len)
{
	memcpy(Psam2.Cmd,Dirc,Len);
	Psam2.CmdLen=Len;
	if(PSAMTransferAPDU(&Psam2)!=CosStateOK)
	{
		return	PsamErr;
	}
		
	return	DsrcSecussed;
}

uint8_t	PSAMTransferAPDU(ApduStruct*	Psam)
{
	uint8_t	status=1;
	Psam->DataLen = 0;
	status=PSAM_APDU((PSAM_TypeDef)(Psam->num),Psam->CmdLen,Psam->Cmd, &Psam->DataLen,Psam->Data);	//APDU命令
	log_hex("PsamTX: ",Psam->Cmd,Psam->CmdLen);
	log_hex("PsamRX: ",Psam->Data,Psam->DataLen);
	if(!status)
	{
		status=CosStateOK;
		Psam->CosSw.SW[0]=Psam->Data[Psam->DataLen-2];
		Psam->CosSw.SW[1]=Psam->Data[Psam->DataLen-1];
		if(Psam->CosSw.SW[0] != 0x90)
		{
			status=CosStateErr;
		}
	}
	else if(status == PSAM_TIMEOUTERR)
	{
		PSAM_ResetFlag = PSAMReset(&Psam2);
		
		if(PSAM_ResetFlag == 0)
		{
			memcpy(Psam2.Cmd,Select3F00,Select3F00Len);			//选择PSAM卡3F00目录
			Psam2.CmdLen=Select3F00Len;
			PSAMTransferAPDU(&Psam2);
			
			memcpy(Psam2.Cmd,SelectDF01,SelectDF01Len);			//选择PSAM卡DF01目录
			Psam2.CmdLen=SelectDF01Len;
			PSAMTransferAPDU(&Psam2);
		}
		
		status=CosStateErr;
		Psam->CosSw.result=CosRspErr;
	}
	return	status;
}

uint8_t	PSAMReset(ApduStruct*	Psam	)
{
	uint8_t	status=1;
	Psam->DataLen = 0;
	status=PSAM_RestCard((PSAM_TypeDef)(Psam->num),Psam->Data,&Psam->DataLen);		//复位PSAM
	if(!status)
	{
		Psam->CosSw.result=CosRspOK;
	}
	else
	{
		Psam->CosSw.result=CosRspErr;
	}
	
	return	status;
}

/*******************************************************************************
*
*函数原型：USER_ReadPSAM(uint8_t psam_num);
*入口参数：PSAM卡号
*出口参数：操作正常返回0,异常返回1
*函数功能：进入3F00,读0015,0016
*
*******************************************************************************/
uint8_t USER_ReadPSAM(uint8_t psam_num)
{
	if(psam_num >= 2)
	{
		return 1;
	}
	memcpy(Psam2.Cmd,Select3F00,Select3F00Len);
	Psam2.CmdLen=Select3F00Len;
	PSAMTransferAPDU(&Psam2);
	if(Psam2.CosSw.result==CosRspOK)
	{
		memcpy(Psam2.Cmd,ReadPsam0015,5);
		Psam2.CmdLen = 5;
		PSAMTransferAPDU(&Psam2);
		if(Psam2.CosSw.result == CosRspOK)
		{
			memcpy(Psam0015Structure[psam_num].PsamNo,Psam2.Data,Psam2.DataLen-2);
			
			memcpy(Psam2.Cmd,ReadGBTerminalID,ReadGBTerminalIDLen);
			Psam2.CmdLen=ReadGBTerminalIDLen;
			PSAMTransferAPDU(&Psam2);
			if(Psam2.CosSw.result==CosRspOK)
			{
				memcpy(PSAM_SerialNum[psam_num],Psam2.Data,Psam2.DataLen-2);
				return 0;
			}			
		}	
	}
	return 1;
}

/*******************************************************************************
*
*USER_ReadPsamAreaCode(uint8_t psam_num);
*入口参数：PSAM卡号
*出口参数：操作正常返回0,异常返回1
*函数功能：进入DF01,读0017
*
*******************************************************************************/
uint8_t USER_ReadPsamAreaCode(uint8_t psam_num)
{
	if(psam_num >= 2)
	{
		return 1;
	}
	memcpy(Psam2.Cmd,SelectDF01,SelectDF01Len);
	Psam2.CmdLen=SelectDF01Len;
	PSAMTransferAPDU(&Psam2);
	if(Psam2.CosSw.result==CosRspOK)
	{
		memcpy(Psam2.Cmd,ReadPsamAreaCode,5);
		Psam2.CmdLen = 5;
		PSAMTransferAPDU(&Psam2);
		if(Psam2.CosSw.result == CosRspOK)
		{
			memcpy(PSAM_AreaCode,Psam2.Data,Psam2.DataLen-2);
			return 0;	
		}	
	}
	return 1;
}

/*******************************************************************************
*
*USER_ReadPsamRand8(uint8_t psam_num);
*入口参数：PSAM卡号
*出口参数：操作正常返回0,异常返回1
*函数功能：进入3F00,取8字节随机数
*
*******************************************************************************/
uint8_t USER_ReadPsamRand8(uint8_t psam_num)
{
	if(psam_num >= 2)
	{
		return 1;
	}
	memcpy(Psam2.Cmd,Select3F00,Select3F00Len);
	Psam2.CmdLen=Select3F00Len;
	PSAMTransferAPDU(&Psam2);
	if(Psam2.CosSw.result==CosRspOK)
	{
		memcpy(Psam2.Cmd,GetRandom8,5);
		Psam2.CmdLen = 5;
		PSAMTransferAPDU(&Psam2);
		if(Psam2.CosSw.result == CosRspOK)
		{
			memcpy(PSAM_RandData8,Psam2.Data,Psam2.DataLen-2);
			return 0;	
		}	
	}
	return 1;
}

uint8_t USER_AuthFunction(uint8_t psam_num)
{
	if(psam_num >= 2)
	{
		return 1;
	}
	
	memcpy(Psam2.Cmd,Select3F00,Select3F00Len);
	Psam2.CmdLen=Select3F00Len;
	PSAMTransferAPDU(&Psam2);
	if(Psam2.CosSw.result==CosRspOK)
	{
		memcpy(Psam2.Cmd,GetRandom8,5);
		Psam2.CmdLen = 5;
		PSAMTransferAPDU(&Psam2);
		if(Psam2.CosSw.result == CosRspOK)
		{
			memcpy(PSAM_RandData8,Psam2.Data,Psam2.DataLen-2);
			return 0;	
		}	
	}
	return 1;
}


