#include "stm32f10x.h"

unsigned char Dsrc_NFlag=0x77;
unsigned char DsrcRxLen=0;
unsigned char DsrcReCount=70;
unsigned char NowCount=0;
unsigned char DsrcOverTime_0=15;

uint8_t ReceiveVstBuf[128] = {0};
uint8_t GetRand8Buf[32] = {0};


STRUCT_OBU_VST_Info  OBU_VST_Info = {0};

uint16_t OffsetTemp = 0;


//#define CLEAR_MARK_INFO

uint8_t Dsrc_Trans_Func(unsigned char DsrcBstFlag,unsigned char *txBuff,unsigned char txLen,unsigned char *rxBuff,unsigned char rxLen,unsigned char ReCount,unsigned char *Count,unsigned char OverTime)
{
//	BK5824ModeSet(0);
//	SLTX_PWR_LOW;
	
	switch(DsrcBstFlag)
	{
		case	BstType:
			BstFlag = 1;
			break;
		case	GetSectureType:
		case	TransferCharnnelType:
		case	SetMMIType:
			if(Dsrc_NFlag == 0x77)
			{
				Dsrc_NFlag = 0xF7;
			}
			else
			{
				Dsrc_NFlag = 0x77;
			}
			txBuff[5] = Dsrc_NFlag;
			chanCmd = 1;
			break;
		case	EvenReportType:

			break;
		default	:
			BstFlag = 0;
			chanCmd = 0;
			

			break;
	}
	
	DsrcRxLen = 0;				//接收数据长度清0
	*Count = 100;				//剩余发送次数赋值100,避免为0
	
	DsrcRxLen=SLTransFunc(txBuff,txLen,rxBuff,&DsrcRxLen,ReCount,Count,OverTime);	//返回接收长度
	log_hex("DsrcTx: ",txBuff,txLen);
	log_hex("DsrcRx: ",rxBuff,DsrcRxLen);
	log_hex("Count: ",Count,1);
	
	BstFlag = 0;
	chanCmd = 0;
	
	
	if((DsrcRxLen > 8)&&(*Count >= 1))
	{
//		BK5824ModeSet(2);
//		SLTX_PWR_HIGH;
		return DsrcSecussed;
	}
	else
	{
		return DsrcError;
	}
}



//=============================================
//DSRC BST:读取ESAM系统信息文件和ICC0015基本信息文件
//=============================================
//unsigned char OBU_BST_Info[]={0xff,0xff,0xff,0xff,0x50,0x03,0x91,0xC0,0x46,0x00,0x00,0x05,0x5C,0xD5,0x5E,0x9D,0x00,0x01,0x41,0x81,0x29,0x20,0x2C,0x00,0x2B,0x00};
unsigned char OBU_BST_Info[]={0xff,0xff,0xff,0xff,0x50,0x03,0x91,0xC0,0x46,0x00,0x00,0x05,0x5C,0xD5,0x5E,0x9D,0x00,0x01,0x41,0x87,0x29,0x20,0x25,0x00,0x2B,0x00};
//08284F76 
//C00391D00001C10120 
//27 D5E3BDAD33010001 00 10 3301081501184815 20181106 20191231 01 FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF 
//28 D5E3BDAD33010001 16 41 3301 1311230200062825 2017113020280525 D5E35A383838383800000000 00 000D
//08284F76
//30
//20
//01


uint8_t OBU_Bst_func(void)
{
	uint8_t status = 1;
	
	BK5824_ConfigPower(PC_InitStructure.TxPower);
	SLRXrxSen_set(13);

	OBU_BST_Info[11]++;
//	Get_unixtime(OBU_BST_Info + 12);
	OBU_BST_Info[12] = UnixTimeUnion.UnixTimeTab[3];
	OBU_BST_Info[13] = UnixTimeUnion.UnixTimeTab[2];
	OBU_BST_Info[14] = UnixTimeUnion.UnixTimeTab[1];
	OBU_BST_Info[15] = UnixTimeUnion.UnixTimeTab[0];
//	log_hex("UnixTime: ",OBU_BST_Info + 12,4);
//	memcpy(OBU_BST_Info + 12,PC_InitStructure.UnixTime,4);

	DsrcReCount = 20;
	status = Dsrc_Trans_Func(BstType,OBU_BST_Info,sizeof(OBU_BST_Info),OBU_VST_Info.MacAdrr,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
//	if((status == 0) && (OBU_VST_Info.MacAdrr[0] < 0xA0))
	if(status == 0)
	{
		return 0;
	}
	else
	{
		return 2;
	}
		
//	memset(OBU_VST_Info.IccSysInfoCarNum,0,12);
//	if(DsrcRxLen != 0)
//	return status;
}

unsigned char OBU_GetSecure16_rq[]={1,1,1,1,0x40,0x77,0x91,0x05,0x01,0x00,0x14,0x80,0x01,0x00,0x00,0x10,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x00,0x00};

typedef struct
{
	unsigned char MacAdrr[4];
	unsigned char Player[8];
	unsigned char GetSecureCarDes[32];
	unsigned char GetSecureDes[9];
}STRUCT_OBU_GetSecure16_rs;
STRUCT_OBU_GetSecure16_rs  OBU_GetSecure16_rs = {0};


//=============================================
//DSRC GetSecure:获取车辆信息密文
//=============================================

unsigned char OBU_GetSecure_rq[]={1,1,1,1,0x40,0x77,0x91,0x05,0x01,0x00,0x14,0x80,0x01,0x00,0x00,0x4F,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x00,0x00};

typedef struct
{
	unsigned char MacAdrr[4];
	unsigned char Player[8];
	unsigned char GetSecureCarDes[88];
	unsigned char GetSecureDes[9];
}STRUCT_OBU_GetSecure_rs;
STRUCT_OBU_GetSecure_rs  OBU_GetSecure_rs = {0};

//uint8_t OBU_GetSecure_func(void)
//{
//	uint8_t status = 1;
//	
//	memcpy(OBU_GetSecure16_rq,OBU_VST_Info.MacAdrr,4);
//	DsrcReCount = DSRC_B6_COUNT;
//	OBU_GetSecure16_rs.Player[0] = 0;
//	status = Dsrc_Trans_Func(GetSectureType,OBU_GetSecure16_rq,sizeof(OBU_GetSecure16_rq),OBU_GetSecure16_rs.MacAdrr,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
//	if(status == 0)
//	{
//		if(OBU_GetSecure16_rs.Player[0] == 0xE0)
//		{
//			GetSecureFlag = 1;
//			return 0;
//		}
//		else
//		{
//			status = Dsrc_Trans_Func(GetSectureType,OBU_GetSecure16_rq,sizeof(OBU_GetSecure16_rq),OBU_GetSecure16_rs.MacAdrr,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
//			if(OBU_GetSecure16_rs.Player[0] == 0xE0)
//			{
//				GetSecureFlag = 1;
//				return 0;
//			}
//			return 1 ;
//		}
//		
//		
//	}
//	else
//	{
//		return 1;
//	}
//}

uint8_t OBU_GetSecure_func(void)
{
	uint8_t status = 1;
	
	memcpy(OBU_GetSecure_rq,OBU_VST_Info.MacAdrr,4);
	DsrcReCount = DSRC_B6_COUNT;
	OBU_GetSecure_rs.Player[0] = 0;
	status = Dsrc_Trans_Func(GetSectureType,OBU_GetSecure_rq,sizeof(OBU_GetSecure_rq),OBU_GetSecure_rs.MacAdrr,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
	if(status == 0)
	{
		if(OBU_GetSecure_rs.Player[0] == 0xE0)
		{
			GetSecureFlag = 1;
			return 0;
		}
		else
		{
			status = Dsrc_Trans_Func(GetSectureType,OBU_GetSecure_rq,sizeof(OBU_GetSecure_rq),OBU_GetSecure_rs.MacAdrr,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
			if(OBU_GetSecure_rs.Player[0] == 0xE0)
			{
				GetSecureFlag = 1;
				return 0;
			}
			return 1 ;
		}
	}
	else
	{
		return 1;
	}
}
//=============================================
//PSAM:密钥离散并解密车辆信息文件
//=============================================
unsigned char PSAM_Select3F00[7]={0x00,0xa4,0x00,0x00,0x02,0x3f,0x00};
unsigned char PSAM_SelectDF01[7]={0x00,0xa4,0x00,0x00,0x02,0xdf,0x01};
typedef struct
{
	unsigned char Delivery_Cmd[2];
	unsigned char Delivery_KeyUse;
	unsigned char Delivery_KeyFlag;
	unsigned char Delivery_DeliLevel;
	unsigned char Delivery_EsamCardSN[8];
	unsigned char Delivery_shengfen[8];
}STRUCT_PSAM_Car2rdDelivery_rq;
STRUCT_PSAM_Car2rdDelivery_rq PSAM_Car2rdDelivery_rq = {0x80,0x1a,0x59,0x03,0x10,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4};
typedef struct
{
	unsigned char sw[2];
}STRUCT_PSAM_Car2rdDelivery_rs;
STRUCT_PSAM_Car2rdDelivery_rs  PSAM_Car2rdDelivery_rs = {0};

typedef struct
{
	unsigned char CarDeDes_Cmd[2];
	unsigned char CarDeDes_p1;
	unsigned char CarDeDes_p2;
	unsigned char CarDeDes_Lc;
	unsigned char CarDeDes_CardDes[88];		//59密文和5字节80 00 00 00 00
}STRUCT_PSAM_CarDedes_rq;
STRUCT_PSAM_CarDedes_rq PSAM_CarDedes_rq = {0x80,0xfa,0x80,0x00,0x58,0x00};

STRUCT_PSAM_CarDedes_rs  PSAM_CarDedes_rs = {0};

typedef struct
{
	unsigned char CarDeDes_Cmd[2];
	unsigned char CarDeDes_p1;
	unsigned char CarDeDes_p2;
	unsigned char CarDeDes_Lc;
	unsigned char CarDeDes_CardDes[32];		//59密文和5字节80 00 00 00 00
}STRUCT_PSAM_CarDedes32_rq;
STRUCT_PSAM_CarDedes32_rq PSAM_CarDedes32_rq = {0x80,0xfa,0x80,0x00,0x20,0x00};

STRUCT_PSAM_CarDedes32_rs  PSAM_CarDedes32_rs = {0};

uint8_t PSAM_CarDedes_func(void)
{
	unsigned char PsamRxLen=0;
	
//	SELECT_PSAM2();										//选择PSAM1,系统启动后,先复位PSAM1,后复位PSAM2,当前选择的为PSAM2
//	
	
	memcpy(PSAM_Car2rdDelivery_rq.Delivery_EsamCardSN,OBU_VST_Info.EsamSysInfoSN,sizeof(OBU_VST_Info.EsamSysInfoSN));
//	memcpy(PSAM_Car2rdDelivery_rq.Delivery_shengfen,PC_ContinueStructure.ObuDivFactor,8);
	memcpy(PSAM_Car2rdDelivery_rq.Delivery_shengfen,OBU_VST_Info.EsamSysInfoIssueFlag,4);
	memcpy(PSAM_Car2rdDelivery_rq.Delivery_shengfen + 4,OBU_VST_Info.EsamSysInfoIssueFlag,4);
	
	PSAM_Car2rdDelivery_rs.sw[0] = 0;
	log_hex("PSAM_Car2rdDelivery_rq: ",(uint8_t *)&PSAM_Car2rdDelivery_rq,sizeof(PSAM_Car2rdDelivery_rq));
	PSAM_APDU(PSAM2,sizeof(PSAM_Car2rdDelivery_rq),PSAM_Car2rdDelivery_rq.Delivery_Cmd, &PsamRxLen,PSAM_Car2rdDelivery_rs.sw);	//APDU命令
	log_hex("PSAM_Car2rdDelivery_rs: ",(uint8_t *)&PSAM_Car2rdDelivery_rs,PsamRxLen);
	if(PSAM_Car2rdDelivery_rs.sw[0] == 0x90)
	{
//		return 0;
	}
	else
	{
		if((PSAM_Car2rdDelivery_rs.sw[0] == 0x69) && (PSAM_Car2rdDelivery_rs.sw[1] == 0x85))
		{
			return 5;
		}
		return 1;
	}

//	memcpy(PSAM_CarDedes32_rq.CarDeDes_CardDes,OBU_GetSecure16_rs.GetSecureCarDes,sizeof(OBU_GetSecure16_rs.GetSecureCarDes));	
//	PSAM_CarDedes32_rs.sw[0] = 0;
//	memset(&PSAM_CarDedes32_rs,0,sizeof(PSAM_CarDedes32_rs));
//	log_hex("PSAM_CarDedes32_rq: ",(uint8_t *)&PSAM_CarDedes32_rq,sizeof(PSAM_CarDedes32_rq));
//	PSAM_APDU(PSAM2,sizeof(PSAM_CarDedes32_rq),PSAM_CarDedes32_rq.CarDeDes_Cmd, &PsamRxLen,PSAM_CarDedes32_rs.CarInfo);	//APDU命令
//	log_hex("PSAM_CarDedes32_rs: ",(uint8_t *)&PSAM_CarDedes32_rs,PsamRxLen);
//	if(PSAM_CarDedes32_rs.sw[0] == 0x90)
//	{
//		return 0;
//	}
//	else
//	{
//		return 2;
//	}
	
	memcpy(PSAM_CarDedes_rq.CarDeDes_CardDes,OBU_GetSecure_rs.GetSecureCarDes,sizeof(OBU_GetSecure_rs.GetSecureCarDes));	
	PSAM_CarDedes_rs.sw[0] = 0;
	memset(&PSAM_CarDedes_rs,0,sizeof(PSAM_CarDedes_rs));
	log_hex("PSAM_CarDedes_rq: ",(uint8_t *)&PSAM_CarDedes_rq,sizeof(PSAM_CarDedes_rq));
	PSAM_APDU(PSAM2,sizeof(PSAM_CarDedes_rq),PSAM_CarDedes_rq.CarDeDes_Cmd, &PsamRxLen,PSAM_CarDedes_rs.CarInfo);	//APDU命令
	log_hex("PSAM_CarDedes_rs: ",(uint8_t *)&PSAM_CarDedes_rs,PsamRxLen);
	if(PSAM_CarDedes_rs.sw[0] == 0x90)
	{
		return 0;
	}
	else
	{
		return 2;
	}
}

unsigned char Transfer_SelectEsamDF01_EF04_rq[] = {1,2,3,4,0x40,0xf7,0x91,0x05,0x01,0x03,0x18,0x02,0x02,
																0x07,0x00,0xA4,0x00,0x00,0x02,0xDF,0x01,
																0x07,0x00,0xA4,0x00,0x00,0x02,0xEF,0x04};
unsigned char Transfer_SelectEsamDF01_EF04_rs[128];

uint8_t SelectEsamDF01_EF04_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	memcpy(Transfer_SelectEsamDF01_EF04_rq,OBU_VST_Info.MacAdrr,4);
	DsrcReCount = DSRC_B6_COUNT;
//	memset(Transfer_SelectEsamDF01_EF04_rs,0,128);
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_SelectEsamDF01_EF04_rq,sizeof(Transfer_SelectEsamDF01_EF04_rq),Transfer_SelectEsamDF01_EF04_rs,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
	if(status == 0)
	{
//		if(Transfer_SelectEsamDF01_EF04_rs[DsrcRxLen - 3] == 0x90)
//		{
//			return 0;
//		}
//		else
//		{
//			status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_SelectEsamDF01_EF04_rq,sizeof(Transfer_SelectEsamDF01_EF04_rq),Transfer_SelectEsamDF01_EF04_rs,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
//			if(status == 0)
//			{
//				return 0;
//			}
//			else
//			{
//				return 1;
//			}
//		}
		
		return 0;
	}
	else
	{
		return 1;
	}
}

unsigned char Transfer_SelectEsamEF04_rq[] = {1,2,3,4,0x40,0xf7,0x91,0x05,0x01,0x03,0x18,0x02,0x01,
																0x07,0x00,0xA4,0x00,0x00,0x02,0xEF,0x04};
unsigned char Transfer_SelectEsamEF04_rs[128];

uint8_t SelectEsamEF04_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	memcpy(Transfer_SelectEsamEF04_rq,OBU_VST_Info.MacAdrr,4);
	DsrcReCount = DSRC_B6_COUNT;
//	memset(Transfer_SelectEsamEF04_rs,0,128);
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_SelectEsamEF04_rq,sizeof(Transfer_SelectEsamEF04_rq),Transfer_SelectEsamEF04_rs,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
	if(status == 0)
	{	
		return 0;
	}
	else
	{
		return 1;
	}
}

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;//02
	unsigned char SelectEF04_Len;//05
	unsigned char SelectEF04_APDU[7];
	unsigned char ReadEF04_Len;//05
	unsigned char ReadEF04_CMD[2];//00b0
	unsigned char ReadEF04_p1;//84
	unsigned char ReadEF04_p2;//00
	unsigned char ReadEF04_Lc;//读15条记录 0x27   9+30=39
}STRUCT_Transfer_SelectReadEsamEF04_rq;

STRUCT_Transfer_SelectReadEsamEF04_rq Transfer_SelectReadEsamEF04_rq = {1,2,3,4,0x40,0xf7,0x91,0x05,0x01,0x03,0x18,0x02,0x02,
																0x07,0x00,0xA4,0x00,0x00,0x02,0xEF,0x04,
																0x05,0x00,0xb0,0x01,0x3A,0x5B};
STRUCT_Transfer_SelectReadEsamEF04_rs Transfer_SelectReadEsamEF04_rs;
																
uint8_t SelectReadEsamEF04_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	
	memcpy(Transfer_SelectReadEsamEF04_rq.OBUID,OBU_VST_Info.MacAdrr,4);
	Transfer_SelectReadEsamEF04_rq.ReadEF04_p1 = PC_InitStructure.EF04_Off[0];
	Transfer_SelectReadEsamEF04_rq.ReadEF04_p2 = PC_InitStructure.EF04_Off[1];
	Transfer_SelectReadEsamEF04_rq.ReadEF04_Lc = PC_InitStructure.EF04_Len[1];
	DsrcReCount = DSRC_B6_COUNT;
	memset(Transfer_SelectReadEsamEF04_rs.SelectReadEF04_Data,0,114);
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_SelectReadEsamEF04_rq.OBUID,sizeof(Transfer_SelectReadEsamEF04_rq),Transfer_SelectReadEsamEF04_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
	if(status == 0)
	{			
		if(Transfer_SelectReadEsamEF04_rs.SelectReadEF04_Data[DsrcRxLen - 14 - 3] == 0x90)				//14为头
		{		
			return 0;
		}
		else
		{
			return 1;
		}		
	}
	else
	{
		return 2;
	}
}


//=============================================
//DSRC ICC和ESAM:读取ICC余额并选ESAM_DF01目录
//08284F76 40F7910501031801 0105805C000204 910501031802 010700A4000002DF01
//=============================================
//unsigned char Transfer_IccMoney_EsamDf01_rq[] = {1,2,3,4,0x40,0xf7,0x91,0x05,0x01,0x03,0x18,0x01,0x01,0x05,0x80,0x5C,0x00,0x02,0x04,
//																	0x91,0x05,0x01,0x03,0x18,0x02,0x01,0x07,0x00,0xA4,0x00,0x00,0x02,0xDF,0x01};

unsigned char Transfer_IccMoney_EsamDF01_EF04_rq[] = {1,2,3,4,0x40,0xf7,0x91,0x05,0x01,0x03,0x18,0x01,0x01,0x05,0x80,0x5C,0x00,0x02,0x04,
																	0x91,0x05,0x01,0x03,0x18,0x02,0x02,0x07,0x00,0xA4,0x00,0x00,0x02,0xDF,0x01,
																										0x07,0x00,0xA4,0x00,0x00,0x02,0xEF,0x04};
STRUCT_Transfer_IccMoney_EsamDF01_EF04_rs	Transfer_IccMoney_EsamDF01_EF04_rs={0};	

uint8_t ICCMoney_EsamDF01_EF04_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	memcpy(Transfer_IccMoney_EsamDF01_EF04_rq,OBU_VST_Info.MacAdrr,4);
	DsrcReCount = DSRC_B6_COUNT;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_IccMoney_EsamDF01_EF04_rq,sizeof(Transfer_IccMoney_EsamDF01_EF04_rq),Transfer_IccMoney_EsamDF01_EF04_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
	return status;
}

//=============================================
//DSRC ESAM:读取EF04文件前9+30个字节并读取107开始60个字节 共15条记录和文件头
//=============================================
typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;//02
	unsigned char ReadEF04Header_Len;//05
	unsigned char ReadEF04Header_CMD[2];//00b0
	unsigned char ReadEF04Header_p1;//84
	unsigned char ReadEF04Header_p2;//00
	unsigned char ReadEF04Header_Lc;//读15条记录 0x27   9+30=39
}STRUCT_Transfer_ESAM_ReadEF04_rq;
STRUCT_Transfer_ESAM_ReadEF04_rq Transfer_ESAM_ReadEF04_rq = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x02,
																	0x01,0x05,0x00,0xb0,0x01,0x3A,0x5B};

STRUCT_Transfer_ESAM_ReadEF04_rs	Transfer_ESAM_ReadEF04_rs={0};

uint8_t ESAM_ReadEF04_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	memcpy(Transfer_ESAM_ReadEF04_rq.OBUID,OBU_VST_Info.MacAdrr,4);
	Transfer_ESAM_ReadEF04_rq.ReadEF04Header_p1 = PC_InitStructure.EF04_Off[0];
	Transfer_ESAM_ReadEF04_rq.ReadEF04Header_p2 = PC_InitStructure.EF04_Off[1];
	Transfer_ESAM_ReadEF04_rq.ReadEF04Header_Lc = PC_InitStructure.EF04_Len[1];
	DsrcReCount = DSRC_B6_COUNT;
	memset(Transfer_ESAM_ReadEF04_rs.ReadEF04Data,0,98);
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ESAM_ReadEF04_rq.OBUID,sizeof(Transfer_ESAM_ReadEF04_rq),Transfer_ESAM_ReadEF04_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
	if(status == 0)
	{
		if(Transfer_ESAM_ReadEF04_rs.ReadEF04Header_Len > 98)					//防止数组ReadEF04Data越界
		{
			return 1;
		}
		if(Transfer_ESAM_ReadEF04_rs.ReadEF04Data[Transfer_ESAM_ReadEF04_rs.ReadEF04Header_Len - 2] == 0x90)
		{		
			return 0;
		}
		else
		{
			return 1;
		}		
	}
	else
	{
		return 2;
	}
}

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;//02
	unsigned char ReadEF04_Len;//05
	unsigned char ReadEF04_CMD[2];//00b0
	unsigned char ReadEF04_p1;//84
	unsigned char ReadEF04_p2;//00
	unsigned char ReadEF04_Lc;//读15条记录 0x27   9+30=39
	
	unsigned char WriteEF04_Head_Len;//05
	unsigned char WriteEF04_Head_CMD[2];//00b0
	unsigned char WriteEF04_Head_p1;//84
	unsigned char WriteEF04_Head_p2;//00
	unsigned char WriteEF04_Head_Lc;//读15条记录 0x27   9+30=39
	unsigned char WriteEF04_Head_Data[3];
	
	unsigned char WriteEF04_Tail_Len;//05
	unsigned char WriteEF04_Tail_CMD[2];//00b0
	unsigned char WriteEF04_Tail_p1;//84
	unsigned char WriteEF04_Tail_p2;//00
	unsigned char WriteEF04_Tail_Lc;//读15条记录 0x27   9+30=39
	unsigned char WriteEF04_Tail_Data[80];
}STRUCT_Transfer_ESAM_ReadWriteEF04_rq;
STRUCT_Transfer_ESAM_ReadWriteEF04_rq Transfer_ESAM_ReadWriteEF04_rq = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x02,
																	0x03,0x05,0x00,0xb0,0x01,0x3A,0x5B,
																		 0x08,0x00,0xD6,0x01,0x3A,0x03,0,0,0,
																		 0x55,0x00,0xD6,0x01,0x95,0x50,0};

STRUCT_Transfer_ESAM_ReadWriteEF04_rs	Transfer_ESAM_ReadWriteEF04_rs={0};

uint8_t ESAM_ReadWriteEF04_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	memcpy(Transfer_ESAM_ReadWriteEF04_rq.OBUID,OBU_VST_Info.MacAdrr,4);
	Transfer_ESAM_ReadWriteEF04_rq.ReadEF04_p1 = PC_PassMoneyStructure.EF04_Off[0];
	Transfer_ESAM_ReadWriteEF04_rq.ReadEF04_p2 = PC_PassMoneyStructure.EF04_Off[1];
	Transfer_ESAM_ReadWriteEF04_rq.ReadEF04_Lc = PC_PassMoneyStructure.EF04_Len[1];
	DsrcReCount = DSRC_B6_COUNT;
	memset(Transfer_ESAM_ReadWriteEF04_rs.ReadWriteEF04Data,0,98);
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ESAM_ReadWriteEF04_rq.OBUID,sizeof(Transfer_ESAM_ReadWriteEF04_rq),Transfer_ESAM_ReadWriteEF04_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
	if(status == 0)
	{
		if(Transfer_ESAM_ReadWriteEF04_rs.ReadEF04_Len > (98 - 4))					//防止数组ReadEF04Data越界
		{
			return 1;
		}
		if((Transfer_ESAM_ReadWriteEF04_rs.ReadWriteEF04Data[Transfer_ESAM_ReadWriteEF04_rs.ReadEF04_Len - 2] == 0x90) && 		//读SW
			(Transfer_ESAM_ReadWriteEF04_rs.ReadWriteEF04Data[Transfer_ESAM_ReadWriteEF04_rs.ReadEF04_Len + 1] == 0x90) &&		//写SW
			(Transfer_ESAM_ReadWriteEF04_rs.ReadWriteEF04Data[Transfer_ESAM_ReadWriteEF04_rs.ReadEF04_Len + 4] == 0x90))		//写SW
		{		
			return 0;
		}
		else
		{
			return 1;
		}		
	}
	else
	{
		return 2;
	}
}

//=============================================
//DSRC ESAM:写EF04文件头并写路径信息并写标识时间
//=============================================
typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;//02
	unsigned char WriteEF04_Len;//0E
	unsigned char WriteEF04_CMD[2];//00d6
	unsigned char WriteEF04_p1;//84
	unsigned char WriteEF04_p2;//00
	unsigned char WriteEF04_Lc;//09
	unsigned char WriteEF04_Data[98];
}STRUCT_Transfer_ESAM_WriteEF04_rq;
//STRUCT_Transfer_ESAM_WriteEF04_rq Transfer_ESAM_WriteEF04_rq = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x02,
//																	0x03,0x0e,0x00,0xd6,0x84,0x00,0x09,1,2,3,4,4,4,4,5,5,
//																	     0x07,0x00,0xd6,0x84,1,0x02,2,2,
//																	     0x09,0x00,0xd6,0x84,1,0x04,2,2,2,2};

STRUCT_Transfer_ESAM_WriteEF04_rq Transfer_ESAM_WriteEF04_rq = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x02,
																	0x01,0x60,0x00,0xd6,0x00,0x00,0x5B,0};

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char WriteEF04_Len;
	unsigned char WriteEF04sw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ESAM_WriteEF04_rs;
STRUCT_Transfer_ESAM_WriteEF04_rs	Transfer_ESAM_WriteEF04_rs={0};	

uint8_t ESAM_WriteEF04_func(uint8_t write_p1,uint8_t write_p2,uint8_t *data_buf,uint8_t write_len)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 50;
	memcpy(Transfer_ESAM_WriteEF04_rq.OBUID,OBU_VST_Info.MacAdrr,4);
//	Transfer_ESAM_WriteEF04_rq.WriteEF04_Len = PC_PurchaseStructure.EF04_Len[1] + 5;
//	Transfer_ESAM_WriteEF04_rq.WriteEF04_p1 = PC_PurchaseStructure.EF04_Off[0];
//	Transfer_ESAM_WriteEF04_rq.WriteEF04_p2 = PC_PurchaseStructure.EF04_Off[1];
//	Transfer_ESAM_WriteEF04_rq.WriteEF04_Lc = PC_PurchaseStructure.EF04_Len[1];
//	memcpy(Transfer_ESAM_WriteEF04_rq.WriteEF04_Data,PC_PurchaseStructure.Ef04Info,PC_PurchaseStructure.EF04_Len[1]);
	
	Transfer_ESAM_WriteEF04_rq.WriteEF04_Len = write_len + 5;
	Transfer_ESAM_WriteEF04_rq.WriteEF04_p1 = write_p1;
	Transfer_ESAM_WriteEF04_rq.WriteEF04_p2 = write_p2;
	Transfer_ESAM_WriteEF04_rq.WriteEF04_Lc = write_len;
	memcpy(Transfer_ESAM_WriteEF04_rq.WriteEF04_Data,data_buf,write_len);
	
	Transfer_ESAM_WriteEF04_rs.WriteEF04sw[0] = 0;
	DsrcReCount = DSRC_B6_COUNT;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ESAM_WriteEF04_rq.OBUID,19 + write_len,Transfer_ESAM_WriteEF04_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	

	if(status == 0)
	{
		if(Transfer_ESAM_WriteEF04_rs.WriteEF04sw[0] == 0x90)
		{		
			return 0;
		}
		else
		{
			return 1;
		}		
	}
	else
	{
		return 2;
	}
//	return status;
}


typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;//02
	unsigned char SelectEF04_Len;//05
	unsigned char SelectEF04_APDU[7];
	unsigned char WriteEF04_Len;//0E
	unsigned char WriteEF04_CMD[2];//00d6
	unsigned char WriteEF04_p1;//84
	unsigned char WriteEF04_p2;//00
	unsigned char WriteEF04_Lc;//09
	unsigned char WriteEF04_Data[98];
}STRUCT_Transfer_ESAM_SelectWriteEF04_rq;
//STRUCT_Transfer_ESAM_WriteEF04_rq Transfer_ESAM_WriteEF04_rq = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x02,
//																	0x03,0x0e,0x00,0xd6,0x84,0x00,0x09,1,2,3,4,4,4,4,5,5,
//																	     0x07,0x00,0xd6,0x84,1,0x02,2,2,
//																	     0x09,0x00,0xd6,0x84,1,0x04,2,2,2,2};

STRUCT_Transfer_ESAM_SelectWriteEF04_rq Transfer_ESAM_SelectWriteEF04_rq = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x02,
																	0x02, 0x07,0x00,0xA4,0x00,0x00,0x02,0xEF,0x04,
																		  0x60,0x00,0xd6,0x00,0x00,0x5B,0};

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char SelectEF04_Len;
	unsigned char SelectWriteEF04_Data[64];
}STRUCT_Transfer_ESAM_SelectWriteEF04_rs;
STRUCT_Transfer_ESAM_SelectWriteEF04_rs	Transfer_ESAM_SelectWriteEF04_rs={0};	

uint8_t ESAM_SelectWriteEF04_func(uint8_t write_p1,uint8_t write_p2,uint8_t *data_buf,uint8_t write_len)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 50;
	memcpy(Transfer_ESAM_SelectWriteEF04_rq.OBUID,OBU_VST_Info.MacAdrr,4);
//	Transfer_ESAM_WriteEF04_rq.WriteEF04_Len = PC_PurchaseStructure.EF04_Len[1] + 5;
//	Transfer_ESAM_WriteEF04_rq.WriteEF04_p1 = PC_PurchaseStructure.EF04_Off[0];
//	Transfer_ESAM_WriteEF04_rq.WriteEF04_p2 = PC_PurchaseStructure.EF04_Off[1];
//	Transfer_ESAM_WriteEF04_rq.WriteEF04_Lc = PC_PurchaseStructure.EF04_Len[1];
//	memcpy(Transfer_ESAM_WriteEF04_rq.WriteEF04_Data,PC_PurchaseStructure.Ef04Info,PC_PurchaseStructure.EF04_Len[1]);
	
	Transfer_ESAM_SelectWriteEF04_rq.WriteEF04_Len = write_len + 5;
	Transfer_ESAM_SelectWriteEF04_rq.WriteEF04_p1 = write_p1;
	Transfer_ESAM_SelectWriteEF04_rq.WriteEF04_p2 = write_p2;
	Transfer_ESAM_SelectWriteEF04_rq.WriteEF04_Lc = write_len;
	memcpy(Transfer_ESAM_SelectWriteEF04_rq.WriteEF04_Data,data_buf,write_len);
	
	memset(Transfer_ESAM_SelectWriteEF04_rs.SelectWriteEF04_Data,0,64);
	DsrcReCount = DSRC_B6_COUNT;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ESAM_SelectWriteEF04_rq.OBUID,27 + write_len,Transfer_ESAM_SelectWriteEF04_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	

	if(status == 0)
	{
		if(Transfer_ESAM_SelectWriteEF04_rs.SelectWriteEF04_Data[DsrcRxLen - 14 - 3] == 0x90)
		{		
			return 0;
		}
		else
		{
			return 1;
		}		
	}
	else
	{
		return 2;
	}
//	return status;
}

//清空EF04文件的406---485，共80字节
uint8_t ESAM_WriteEF04_406_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 50;
	memcpy(Transfer_ESAM_WriteEF04_rq.OBUID,OBU_VST_Info.MacAdrr,4);
	Transfer_ESAM_WriteEF04_rq.WriteEF04_Len = 80 + 5;
	Transfer_ESAM_WriteEF04_rq.WriteEF04_p1 = 0x01;
	Transfer_ESAM_WriteEF04_rq.WriteEF04_p2 = 0x95;
	Transfer_ESAM_WriteEF04_rq.WriteEF04_Lc = 80;
	memset(Transfer_ESAM_WriteEF04_rq.WriteEF04_Data,0,80);
	
	Transfer_ESAM_WriteEF04_rs.WriteEF04sw[0] = 0;
	DsrcReCount = DSRC_B6_COUNT;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ESAM_WriteEF04_rq.OBUID,19 + 80,Transfer_ESAM_WriteEF04_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	

	if(status == 0)
	{
		if(Transfer_ESAM_WriteEF04_rs.WriteEF04sw[0] == 0x90)
		{		
			return 0;
		}
		else
		{
			return 1;
		}		
	}
	else
	{
		return 2;
	}
//	return status;
}

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;//01
	unsigned char ClearEF04_Head_Len;//0E
	unsigned char ClearEF04_Head_CMD[2];//00d6
	unsigned char ClearEF04_Head_p1;//84
	unsigned char ClearEF04_Head_p2;//00
	unsigned char ClearEF04_Head_Lc;//09
	unsigned char ClearEF04_Head_Data[3];
	
	unsigned char ClearEF04_Tail_Len;//0E
	unsigned char ClearEF04_Tail_CMD[2];//00d6
	unsigned char ClearEF04_Tail_p1;//84
	unsigned char ClearEF04_Tail_p2;//00
	unsigned char ClearEF04_Tail_Lc;//09
	unsigned char ClearEF04_Tail_Data[80];
	
}STRUCT_Transfer_ESAM_ClearEF04_rq;

STRUCT_Transfer_ESAM_ClearEF04_rq Transfer_ESAM_ClearEF04_rq = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x02,
																	0x02,0x08,0x00,0xD6,0x01,0x3A,0x03,0,0,0,
																		 0x55,0x00,0xD6,0x01,0x95,0x50,0};

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ClearEF04_Head_Len;
	unsigned char ClearEF04_Head_sw[2];
	unsigned char ClearEF04_Tail_Len;
	unsigned char ClearEF04_Tail_sw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ESAM_ClearEF04_rs;

STRUCT_Transfer_ESAM_ClearEF04_rs	Transfer_ESAM_ClearEF04_rs={0};

uint8_t ESAM_ClearEF04_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 50;
	memcpy(Transfer_ESAM_ClearEF04_rq.OBUID,OBU_VST_Info.MacAdrr,4);
	
	Transfer_ESAM_ClearEF04_rs.ClearEF04_Head_sw[0] = 0;
	Transfer_ESAM_ClearEF04_rs.ClearEF04_Tail_sw[0] = 0;
	DsrcReCount = DSRC_B6_COUNT;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ESAM_ClearEF04_rq.OBUID,sizeof(Transfer_ESAM_ClearEF04_rq),Transfer_ESAM_ClearEF04_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);	
	
	if(status == 0)
	{
		if((Transfer_ESAM_ClearEF04_rs.ClearEF04_Head_sw[0] == 0x90) && (Transfer_ESAM_ClearEF04_rs.ClearEF04_Tail_sw[0] == 0x90))
		{		
			return 0;
		}
		else
		{
			return 1;
		}		
	}
	else
	{
		return 2;
	}
//	return status;
}

//=============================================
//DSRC ICC:读取0008文件前69个字节，并取随机数用于外部认证
//=============================================
unsigned char Transfer_ICC_Read0008_Rand8_rq[]={0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x01,0x02,0x05,0x00,0xB0,0x88,0x00,0x23,0x05,0x00,0x84,0x00,0x00,0x08};

STRUCT_Transfer_ICC_Read0008_Rand8_rs	Transfer_ICC_Read0008_Rand8_rs={0};	

////最大支持37个标识点信息,与EF04短文件标识符操作相匹配
//uint8_t ICC_Read0008_Rand8_func(void)
//{
//	uint8_t status = 1;
//	
////	DsrcOverTime_0 = 15;
//	memcpy(Transfer_ICC_Read0008_Rand8_rq,OBU_VST_Info.MacAdrr,4);
//	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_Read0008_Rand8_rq,sizeof(Transfer_ICC_Read0008_Rand8_rq),Transfer_ICC_Read0008_Rand8_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
//	if(Transfer_ICC_Read0008_Rand8_rs.ICC0008[0] > 50)
//	{
//		Transfer_ICC_Read0008_Rand8_rs.ICC0008[0] = 0;
//	}
//	return status;
//}

//最大支持37个标识点信息,与EF04短文件标识符操作相匹配
uint8_t ICC_Read0008_Rand8_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	memcpy(Transfer_ICC_Read0008_Rand8_rq,OBU_VST_Info.MacAdrr,4);
	DsrcReCount = DSRC_B6_COUNT;
	Transfer_ICC_Read0008_Rand8_rs.ICC0008sw[0] = 0;
	Transfer_ICC_Read0008_Rand8_rs.Rand8sw[0] = 0;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_Read0008_Rand8_rq,sizeof(Transfer_ICC_Read0008_Rand8_rq),Transfer_ICC_Read0008_Rand8_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
	if(status == 0)
	{
		if((Transfer_ICC_Read0008_Rand8_rs.ICC0008sw[0] == 0x90) && (Transfer_ICC_Read0008_Rand8_rs.Rand8sw[0] == 0x90))
		{
			if(Transfer_ICC_Read0008_Rand8_rs.ICC0008[0] > 16)
			{
				Transfer_ICC_Read0008_Rand8_rs.ICC0008[0] = 0;
			}
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 2;
	}
}

//=============================================
//PSAM卡 :更新0008文件前，密钥离散和DES计算
//=============================================
typedef struct
{
	unsigned char Des_Init_cmd[2];
	unsigned char Des_Init_Key_User;
	unsigned char Des_Init_Key_Flag;
	unsigned char Des_Init_Lc;
	unsigned char Des_Init_CardID[8];
	unsigned char Des_Init_Cord[8];
}STRUCT_PSAM_ICC2rdDelivery_rq;
STRUCT_PSAM_ICC2rdDelivery_rq PSAM_ICCWrite0008Delivery_rq = {0x80,0x1a,0x48,0x01,0x10,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4};
typedef struct
{
	unsigned char sw[2];
}STRUCT_PSAM_ICC2rdDelivery_rs;
STRUCT_PSAM_ICC2rdDelivery_rs  PSAM_ICCWrite0008Delivery_rs = {0};

typedef struct
{
	unsigned char Des_cmd[2];
	unsigned char Des_func;
	unsigned char Des_p2;
	unsigned char Des_Lc;
	unsigned char Des_IccRnd8[8];
}STRUCT_PSAM_ICCDes_rq;
STRUCT_PSAM_ICCDes_rq PSAM_ICCWrite0008Des_rq = {0x80,0xfa,0x00,0x00,0x08,1,1,1,1,1,1,1,1};
typedef struct
{
	unsigned char Des_Data[8];
	unsigned char Des_sw[2];
}STRUCT_PSAM_ICCDes_rs;
STRUCT_PSAM_ICCDes_rs  PSAM_ICCWrite0008Des_rs = {0};

uint8_t PSAM_ICCWrite0008Des_func(void)
{
	unsigned char PsamRxLen=0;
	memcpy(PSAM_ICCWrite0008Delivery_rq.Des_Init_CardID,OBU_VST_Info.IccSysInfoCardSN,8);
	memcpy(PSAM_ICCWrite0008Delivery_rq.Des_Init_Cord,OBU_VST_Info.IccSysInfoIssueFlag,4);
	memcpy(PSAM_ICCWrite0008Delivery_rq.Des_Init_Cord+4,OBU_VST_Info.IccSysInfoIssueFlag,4);
	
	PSAM_ICCWrite0008Delivery_rs.sw[0] = 0;
	PSAM_APDU(PSAM2,sizeof(PSAM_Car2rdDelivery_rq),PSAM_ICCWrite0008Delivery_rq.Des_Init_cmd, &PsamRxLen,PSAM_ICCWrite0008Delivery_rs.sw);	//APDU命令
	
	if(PSAM_ICCWrite0008Delivery_rs.sw[0] == 0x90)
	{
//		return 0;
	}
	else
	{
		return 1;
	}	
	
	memcpy(PSAM_ICCWrite0008Des_rq.Des_IccRnd8,Transfer_ICC_Read0008_Rand8_rs.Rand8,8);
	
	PSAM_ICCWrite0008Des_rs.Des_sw[0] = 0;
	PSAM_APDU(PSAM2,sizeof(PSAM_ICCWrite0008Des_rq),PSAM_ICCWrite0008Des_rq.Des_cmd, &PsamRxLen,PSAM_ICCWrite0008Des_rs.Des_Data);	//APDU命令
	
	if(PSAM_ICCWrite0008Des_rs.Des_sw[0] == 0x90)
	{
		return 0;
	}
	else
	{
		return 2;
	}
}

//=============================================
//DSRC ICC卡:外部认证并写0008
//=============================================
//typedef struct
//{
//	unsigned char OBUID[4];
//	unsigned char Player[8];
//	unsigned char Apdu_Num;
//	unsigned char ExAuth_Len;
//	unsigned char ExAuth_CMD[2];
//	unsigned char ExAuth_p1;
//	unsigned char ExAuth_KeyFlag;
//	unsigned char ExAuth_Lc;
//	unsigned char ExAuth_Des8[8];
//	unsigned char Write0008_Header_Len;
//	unsigned char Write0008_Header_CMD[2];
//	unsigned char Write0008_Header_p1;
//	unsigned char Write0008_Header_p2;
//	unsigned char Write0008_Header_Lc;
//	unsigned char Write0008_Header_Data[3];
//	unsigned char Write0008_Len;
//	unsigned char Write0008_CMD[2];
//	unsigned char Write0008_p1;
//	unsigned char Write0008_p2;
//	unsigned char Write0008_Lc;
//	unsigned char Write0008_Data[4];
//}STRUCT_Transfer_ICC_ExAuth_Write0008_rq;

//STRUCT_Transfer_ICC_ExAuth_Write0008_rq Transfer_ICC_ExAuth_Write0008_rq={0x01,0x02,0x03,0x04,0x40,0x77,0x91,0x05,0x01,0x03,0x18,0x01,
//																			0x03,0x0d,0x00,0x82,0x00,0x01,0x08,1,2,3,4,5,6,7,8,
//																				 0x08,0x00,0xd6,0x88,0x00,0x03,3,2,1,
//																				 0x09,0x00,0xd6,0x88,0x00,4,1,2,3,4};


//STRUCT_Transfer_ICC_ExAuth_Write0008_rs	Transfer_ICC_ExAuth_Write0008_rs={0};

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ExAuth_Len;
	unsigned char ExAuth_CMD[2];
	unsigned char ExAuth_p1;
	unsigned char ExAuth_KeyFlag;
	unsigned char ExAuth_Lc;
	unsigned char ExAuth_Des8[8];
}STRUCT_Transfer_ICC_ExAuth_rq;

STRUCT_Transfer_ICC_ExAuth_rq Transfer_ICC_ExAuth_rq={0x01,0x02,0x03,0x04,0x40,0x77,0x91,0x05,0x01,0x03,0x18,0x01,
																			0x01,0x0d,0x00,0x82,0x00,0x01,0x08,1,2,3,4,5,6,7,8};


STRUCT_Transfer_ICC_ExAuth_rs	Transfer_ICC_ExAuth_rs={0};

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char Write0008_Header_Len;
	unsigned char Write0008_Header_CMD[2];
	unsigned char Write0008_Header_p1;
	unsigned char Write0008_Header_p2;
	unsigned char Write0008_Header_Lc;
	unsigned char Write0008_Header_Data[3];
	unsigned char Write0008_Len;
	unsigned char Write0008_CMD[2];
	unsigned char Write0008_p1;
	unsigned char Write0008_p2;
	unsigned char Write0008_Lc;
	unsigned char Write0008_Data[4];
}STRUCT_Transfer_ICC_Write0008_rq;

STRUCT_Transfer_ICC_Write0008_rq Transfer_ICC_Write0008_rq={0x01,0x02,0x03,0x04,0x40,0x77,0x91,0x05,0x01,0x03,0x18,0x01,
																			0x02,0x08,0x00,0xd6,0x88,0x00,0x03,3,2,1,
																				 0x09,0x00,0xd6,0x88,0x00,4,1,2,3,4};


STRUCT_Transfer_ICC_Write0008_rs	Transfer_ICC_Write0008_rs={0};



//uint8_t ICC_ExAuth_Write0008_func(void)
//{
//	uint8_t status = 1;
//	uint8_t m;
//	
//	for(m = 0; m < 8; m++)
//	{
//		if(memcmp(OBU_FalseListStructure[m].ObuId,OBU_VST_Info.MacAdrr,4) == 0)
//		{
//			return 1;
//		}
//	}
//	
////	DsrcOverTime_0 = 15;
//	memcpy(Transfer_ICC_ExAuth_Write0008_rq.OBUID,OBU_VST_Info.MacAdrr,4);
//	memcpy(Transfer_ICC_ExAuth_Write0008_rq.ExAuth_Des8,PSAM_ICCWrite0008Des_rs.Des_Data,8);
//	
//#ifndef CLEAR_MARK_INFO
//	Transfer_ICC_ExAuth_Write0008_rq.Write0008_Header_Data[0] = Transfer_ICC_Read0008_Rand8_rs.ICC0008[0] + 2;
//	if(Transfer_ICC_ExAuth_Write0008_rq.Write0008_Header_Data[0] > 50)
//	{
//		return 1;
////		Transfer_ICC_ExAuth_Write0008_rq.Write0008_Header_Data[0] = 1;
//	}
//#else
//	Transfer_ICC_ExAuth_Write0008_rq.Write0008_Header_Data[0] = 1;
//#endif
//	
//	memcpy(Transfer_ICC_ExAuth_Write0008_rq.Write0008_Header_Data + 1,PC_InitStructure.nFlagID,2);
//	Transfer_ICC_ExAuth_Write0008_rq.Write0008_p2 = (Transfer_ICC_ExAuth_Write0008_rq.Write0008_Header_Data[0] - 2) * 2 + 3;
////	memcpy(Transfer_ICC_ExAuth_Write0008_rq.Write0008_Data,PC_InitStructure.FlagID,2);
//	memcpy(Transfer_ICC_ExAuth_Write0008_rq.Write0008_Data,PC_InitStructure.FlagID,4);			//0008文件写4字节路径
//	
//	Transfer_ICC_ExAuth_Write0008_rs.ExAuthsw[0] = 0x00 ;
//	Transfer_ICC_ExAuth_Write0008_rs.Write0008_Headersw[0] = 0x00;
//	Transfer_ICC_ExAuth_Write0008_rs.Write0008sw[0] = 0x00;
//	DsrcReCount = DSRC_B6_COUNT;
//	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_ExAuth_Write0008_rq.OBUID,sizeof(Transfer_ICC_ExAuth_Write0008_rq),Transfer_ICC_ExAuth_Write0008_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
//	
//	if(status == 0)
//	{
//		if((Transfer_ICC_ExAuth_Write0008_rs.ExAuthsw[0] == 0x90) && (Transfer_ICC_ExAuth_Write0008_rs.Write0008_Headersw[0] == 0x90) && (Transfer_ICC_ExAuth_Write0008_rs.Write0008sw[0] == 0x90))
//		{
//			return 0;
//		}
//		else
//		{
//			return 1;
//		}
//	}
//	else
//	{
//		return 1;
//	}	
//}

uint8_t ICC_ExAuth_func(void)
{
	uint8_t status = 1;
	uint8_t m;
	
	for(m = 0; m < 8; m++)
	{
		if(memcmp(OBU_FalseListStructure[m].ObuId,OBU_VST_Info.MacAdrr,4) == 0)
		{
			return 1;
		}
//		log_title("OBU_FalseList1");
	}

	memcpy(Transfer_ICC_ExAuth_rq.OBUID,OBU_VST_Info.MacAdrr,4);
	memcpy(Transfer_ICC_ExAuth_rq.ExAuth_Des8,PSAM_ICCWrite0008Des_rs.Des_Data,8);
	
	Transfer_ICC_ExAuth_rs.ExAuthsw[0] = 0x00 ;

	DsrcReCount = DSRC_B6_COUNT;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_ExAuth_rq.OBUID,sizeof(Transfer_ICC_ExAuth_rq),Transfer_ICC_ExAuth_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
	
	if(Transfer_ICC_ExAuth_rs.ExAuthsw[0] == 0x90)
	{
		return status;
	}
	else
	{
		return 1;
	}
}

uint8_t ICC_Write0008_func(void)
{
//	uint8_t status = 1;
//	
//	memcpy(Transfer_ICC_Write0008_rq.OBUID,OBU_VST_Info.MacAdrr,4);
//	
//	Transfer_ICC_Write0008_rq.Write0008_Header_Data[0] = Transfer_ICC_Read0008_Rand8_rs.ICC0008[0] + 2;
//	if(Transfer_ICC_Write0008_rq.Write0008_Header_Data[0] > 50)
//	{
//		return 1;
//	}
//	
//	memcpy(Transfer_ICC_Write0008_rq.Write0008_Header_Data + 1,PC_InitStructure.nFlagID,2);
//	Transfer_ICC_Write0008_rq.Write0008_p2 = (Transfer_ICC_Write0008_rq.Write0008_Header_Data[0] - 2) * 2 + 3;
//	memcpy(Transfer_ICC_Write0008_rq.Write0008_Data,PC_InitStructure.FlagID,4);			//0008文件写4字节路径
//	
//	Transfer_ICC_Write0008_rs.Write0008_Headersw[0] = 0x00;
//	Transfer_ICC_Write0008_rs.Write0008sw[0] = 0x00;
//	DsrcReCount = DSRC_B6_COUNT;
//	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_Write0008_rq.OBUID,sizeof(Transfer_ICC_Write0008_rq),Transfer_ICC_Write0008_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
//	
//	if((Transfer_ICC_Write0008_rs.Write0008_Headersw[0] == 0x90) && (Transfer_ICC_Write0008_rs.Write0008sw[0] == 0x90))
//	{
//		return status;
//	}
//	else
//	{
//		return 1;
//	}
	return 0;
}

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ICC_Select1001_Len;
	unsigned char ICC_Select1001[64];
}STRUCT_Transfer_ICC_Select1001_rs;

unsigned char Transfer_ICC_Select1001_rq[] = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x01,0x02,0x05,0x00,0xB2,0x01,0xCC,0x2B,0x05,0x00,0xB0,0x8C,0x00,0x40};
STRUCT_Transfer_ICC_Select1001_rs Transfer_ICC_Select1001_rs = {0};

uint8_t ICC_Select1001_func(void)
{
	
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	memcpy(Transfer_ICC_Select1001_rq,OBU_VST_Info.MacAdrr,4);
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_Select1001_rq,sizeof(Transfer_ICC_Select1001_rq),Transfer_ICC_Select1001_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
	
	return status;
}


unsigned char Transfer_ICC_Money_Read0019_0015_rq[] = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x01,0x03,0x05,0x80,0x5C,0x00,0x02,0x04,0x05,0x00,0xB2,0x01,0xCC,0x2B,0x05,0x00,0xB0,0x95,0x2B,0x07};
STRUCT_Transfer_ICC_Money_Read0019_0015_rs Transfer_ICC_Money_Read0019_0015_rs = {0};

unsigned char Transfer_ICC_Money_Read0019_rq[] = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x01,0x02,0x05,0x80,0x5C,0x00,0x02,0x04,0x05,0x00,0xB2,0x01,0xCC,0x2B};
STRUCT_Transfer_ICC_Money_Read0019_rs Transfer_ICC_Money_Read0019_rs = {0};

//unsigned char Transfer_ICC_Read0019_rq[] = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x01,0x01,0x05,0x00,0xB2,0x01,0xCC,0x2B};
//STRUCT_Transfer_ICC_Read0019_rs Transfer_ICC_Read0019_rs = {0};

//unsigned char Transfer_ICC_Read000C_rq[] = {0x01,0x02,0x03,0x04,0x40,0xF7,0x91,0x05,0x01,0x03,0x18,0x01,0x01,0x05,0x00,0xB0,0x8C,0x00,0x40};
//STRUCT_Transfer_ICC_Read000C_rs Transfer_ICC_Read000C_rs = {0};

uint8_t ICC_Money_Read0019_0015_func(void)
{
	
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	memcpy(Transfer_ICC_Money_Read0019_0015_rq,OBU_VST_Info.MacAdrr,4);
	DsrcReCount = DSRC_B6_COUNT;
	Transfer_ICC_Money_Read0019_0015_rs.ICC_MoneySw[0] = 0;
	Transfer_ICC_Money_Read0019_0015_rs.ICC_0019sw[0] = 0;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_Money_Read0019_0015_rq,sizeof(Transfer_ICC_Money_Read0019_0015_rq),Transfer_ICC_Money_Read0019_0015_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
	
	if(status == 0)
	{
		if((Transfer_ICC_Money_Read0019_0015_rs.ICC_MoneySw[0] == 0x90) && (Transfer_ICC_Money_Read0019_0015_rs.ICC_0019sw[0] == 0x90))
		{
			return 0;
		}
		else
		{
			return 1;
		}		
	}
	else
	{
		return 2;
	}
}

uint8_t ICC_Money_Read0019_func(void)
{
	
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	memcpy(Transfer_ICC_Money_Read0019_rq,OBU_VST_Info.MacAdrr,4);
	DsrcReCount = DSRC_B6_COUNT;
	Transfer_ICC_Money_Read0019_rs.ICC_MoneySw[0] = 0;
	Transfer_ICC_Money_Read0019_rs.ICC_0019sw[0] = 0;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_Money_Read0019_rq,sizeof(Transfer_ICC_Money_Read0019_rq),Transfer_ICC_Money_Read0019_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
	
	if(status == 0)
	{
		if((Transfer_ICC_Money_Read0019_rs.ICC_MoneySw[0] == 0x90) && (Transfer_ICC_Money_Read0019_rs.ICC_0019sw[0] == 0x90))
		{
			return 0;
		}
		else
		{
			return 1;
		}		
	}
	else
	{
		return 2;
	}
}

//uint8_t ICC_Read0019_func(void)
//{
//	
//	uint8_t status = 1;
//	
////	DsrcOverTime_0 = 15;
//	memcpy(Transfer_ICC_Read0019_rq,OBU_VST_Info.MacAdrr,4);
//	DsrcReCount = DSRC_B6_COUNT;
//	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_Read0019_rq,sizeof(Transfer_ICC_Read0019_rq),Transfer_ICC_Read0019_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
//	
//	return status;
//}

//uint8_t ICC_Read000C_func(void)
//{
//	
//	uint8_t status = 1;
//	
////	DsrcOverTime_0 = 15;
//	memcpy(Transfer_ICC_Read000C_rq,OBU_VST_Info.MacAdrr,4);
//	DsrcReCount = DSRC_B6_COUNT;
//	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_Read000C_rq,sizeof(Transfer_ICC_Read000C_rq),Transfer_ICC_Read000C_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
//	
//	return status;
//}

//=============================================
//DSRC ICC卡:消费初始化并更新复合应用数据缓存
//=============================================
typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char CappInit_Len;
	unsigned char CappInit_CMD[2];
	unsigned char CappInit_p1;
	unsigned char CappInit_p2;
	unsigned char CappInit_Lc;
	unsigned char CappInit_KeyFlag;
	unsigned char CappInit_Money[4];
	unsigned char CappInit_EquimNum[6];
	unsigned char UpdataCapp_Len;
	unsigned char UpdataCapp_CMD[2];
	unsigned char UpdataCapp_CappFlag;
	unsigned char UpdataCapp_FileFlag;
	unsigned char UpdataCapp_Lc;
	unsigned char UpdataCapp_Data[43];
}STRUCT_Transfer_ICC_CappPurchaseInit_rq;

STRUCT_Transfer_ICC_CappPurchaseInit_rq Transfer_ICC_CappPurchaseInit_rq={0x01,0x02,0x03,0x04,0x40,0x77,0x91,0x05,0x01,0x03,0x18,0x01,
																			0x02,0x10,0x80,0x50,0x03,0x02,0x0b,0x01,0,0,0,0,1,1,1,1,1,1,
																				 0x30,0x80,0xdc,0xaa,0xc8,0x2B,0xAA,0x29,0x00,40};

STRUCT_Transfer_ICC_CappPurchaseInit_rs	Transfer_ICC_CappPurchaseInit_rs={0};

uint8_t ICC_CappPurchaseInit_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 15;
	memcpy(Transfer_ICC_CappPurchaseInit_rq.OBUID,OBU_VST_Info.MacAdrr,4);
	memcpy(Transfer_ICC_CappPurchaseInit_rq.CappInit_Money,PC_PurchaseStructure.ConsumeMoney,4);
	memcpy(Transfer_ICC_CappPurchaseInit_rq.CappInit_EquimNum,RSU_StatusStructure.PsamTerminal,6);
	memcpy(Transfer_ICC_CappPurchaseInit_rq.UpdataCapp_Data + 3,PC_PurchaseStructure.Station,40);
	
	Transfer_ICC_CappPurchaseInit_rs.CappInitsw[0] = 0;
	Transfer_ICC_CappPurchaseInit_rs.UpdataCappsw[0] = 0;
	DsrcReCount = DSRC_B5_COUNT;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_CappPurchaseInit_rq.OBUID,sizeof(Transfer_ICC_CappPurchaseInit_rq),Transfer_ICC_CappPurchaseInit_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
	if(status == 0)
	{
		if(Transfer_ICC_CappPurchaseInit_rs.CappInitsw[0] == 0x90 && Transfer_ICC_CappPurchaseInit_rs.UpdataCappsw[0] == 0x90)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 2;
	}
		
//	return status;
}

//=============================================
//PSAM卡 :复合消费前，计算MAC1
//=============================================
typedef struct
{
	unsigned char CappMac1_cmd[2];
	unsigned char CappMac1_p1;
	unsigned char CappMac1_p2;
	unsigned char CappMac1_Lc;
	unsigned char CappMac1_Rand4[4];
	unsigned char CappMac1_PurchaseSN[2];
	unsigned char CappMac1_Money[4];
	unsigned char CappMac1_PurseType;
	unsigned char CappMac1_PurseDate[4];
	unsigned char CappMac1_PurseTime[3];
	unsigned char CappMac1_KeyVer;
	unsigned char CappMac1_KeyFlag;
	unsigned char CappMac1_CardID[8];
	unsigned char CappMac1_Core[8];
}STRUCT_PSAM_ICC_CappPurseMac1_rq;
STRUCT_PSAM_ICC_CappPurseMac1_rq	PSAM_ICC_CappPurseMac1_rq={0x80,0x70,0x00,0x00,0x24,
																				1,2,3,4,
																				1,1,
																				2,2,2,2,
																				3,
																				4,4,4,4,5,5,5,
																				6,
																				7,
																				8,8,8,8,8,8,8,8,
																				9,9,9,9,9,9,9,9};


STRUCT_PSAM_ICC_CappPurseMac1_rs	PSAM_ICC_CappPurseMac1_rs={0};	

uint8_t PSAM_ICC_CappPurseMac1_func(void)
{
	unsigned char PsamRxLen=0;

	memcpy(PSAM_ICC_CappPurseMac1_rq.CappMac1_Rand4,Transfer_ICC_CappPurchaseInit_rs.CappInit_Rand4,4);
	memcpy(PSAM_ICC_CappPurseMac1_rq.CappMac1_PurchaseSN,Transfer_ICC_CappPurchaseInit_rs.CappInit_PurchaseSN,2);
	memcpy(PSAM_ICC_CappPurseMac1_rq.CappMac1_Money,PC_PurchaseStructure.ConsumeMoney,4);
	PSAM_ICC_CappPurseMac1_rq.CappMac1_PurseType = 0x09;//交易类型，日志中复合消费为01
	memcpy(PSAM_ICC_CappPurseMac1_rq.CappMac1_PurseDate,PC_PurchaseStructure.PurchaseTime,7);
	PSAM_ICC_CappPurseMac1_rq.CappMac1_KeyVer = Transfer_ICC_CappPurchaseInit_rs.CappInit_KeyVer;
	PSAM_ICC_CappPurseMac1_rq.CappMac1_KeyFlag = Transfer_ICC_CappPurchaseInit_rs.CappInit_KeyDesFlag;
	memcpy(PSAM_ICC_CappPurseMac1_rq.CappMac1_CardID,OBU_VST_Info.IccSysInfoCardSN,8);
	memcpy(PSAM_ICC_CappPurseMac1_rq.CappMac1_Core,OBU_VST_Info.IccSysInfoIssueFlag,4);
	memcpy(PSAM_ICC_CappPurseMac1_rq.CappMac1_Core+4,OBU_VST_Info.IccSysInfoIssueFlag,4);
	
	PSAM_ICC_CappPurseMac1_rs.CappMac1_sw[0] = 0;
//	PSAM_ICC_CappPurseMac1_rs.CappMac1_sw[1] = 0;
	log_hex("Mac1_rq: ",(uint8_t *)&PSAM_ICC_CappPurseMac1_rq,sizeof(PSAM_ICC_CappPurseMac1_rq));
	PSAM_APDU(PSAM2,sizeof(PSAM_ICC_CappPurseMac1_rq),PSAM_ICC_CappPurseMac1_rq.CappMac1_cmd, &PsamRxLen,PSAM_ICC_CappPurseMac1_rs.CappMac1_EquimPurseSN);	//APDU命令
	log_hex("Mac1_rs: ",(uint8_t *)&PSAM_ICC_CappPurseMac1_rs,PsamRxLen);
	if(PSAM_ICC_CappPurseMac1_rs.CappMac1_sw[0] == 0x90)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char GetTAC_Len;
	unsigned char GetTAC_CMD[2];		//805A
	unsigned char GetTAC_p1;			//00
	unsigned char GetTAC_p2;			//09
	unsigned char GetTAC_Lc;			//02
	unsigned char GetTAC_PurchaseSN[2];
}STRUCT_Transfer_ICC_GetTAC_rq;

STRUCT_Transfer_ICC_GetTAC_rq Transfer_ICC_GetTAC_rq={0x01,0x02,0x03,0x04,0x40,0x77,0x91,0x05,0x01,0x03,0x18,0x01,
																			0x01,0x07,0x80,0x5A,0x00,0x09,0x02,1,1};

STRUCT_Transfer_ICC_GetTAC_rs	Transfer_ICC_GetTAC_rs={0};

uint8_t ICC_GetTac_func(void)
{
	uint8_t status = 1;
	uint16_t temp16;
	
//	memcpy(Transfer_ICC_GetTAC_rq.OBUID,OBU_VST_Info.MacAdrr,4);
	memcpy(Transfer_ICC_GetTAC_rq.OBUID,PC_GetTACStructure.ObuId,4);
	
	temp16 = Transfer_ICC_CappPurchaseInit_rs.CappInit_PurchaseSN[0] * 256 + Transfer_ICC_CappPurchaseInit_rs.CappInit_PurchaseSN[1] + 1;
	Transfer_ICC_GetTAC_rq.GetTAC_PurchaseSN[0] = temp16 / 256;
	Transfer_ICC_GetTAC_rq.GetTAC_PurchaseSN[1] = temp16 % 256;
	
	
//	memcpy(Transfer_ICC_GetTAC_rq.GetTAC_PurchaseSN,Transfer_ICC_CappPurchaseInit_rs.CappInit_PurchaseSN,2);
	
	Transfer_ICC_GetTAC_rs.GetTACsw[0] = 0x00 ;
	DsrcReCount = DSRC_B5_COUNT;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_GetTAC_rq.OBUID,sizeof(Transfer_ICC_GetTAC_rq),Transfer_ICC_GetTAC_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
	if(status == 0)
	{
		if(Transfer_ICC_GetTAC_rs.GetTACsw[0] == 0x90)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 2;
	}
}

//=============================================
//用户卡 :复合消费并取余额
//=============================================
typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char CappPurchase_Len;
	unsigned char CappPurchase_CMD[2];
	unsigned char CappPurchase_p1;
	unsigned char CappPurchase_p2;
	unsigned char CappPurchase_Lc;
	unsigned char CappPurchase_EquimPurseSN[4];
	unsigned char CappPurchase_PurseDate[4];
	unsigned char CappPurchase_PurseTime[3];
	unsigned char CappPurchase_Mac1[4];
	unsigned char GetMoney_Len;
	unsigned char GetMoney_CMD[2];
	unsigned char GetMoney_p1;
	unsigned char GetMoney_p2;
	unsigned char GetMoney_Le;
}STRUCT_Transfer_ICC_CappPurchase_GetMoney_rq;

STRUCT_Transfer_ICC_CappPurchase_GetMoney_rq Transfer_ICC_CappPurchase_GetMoney_rq={0x01,0x02,0x03,0x04,0x40,0x77,0x91,0x05,0x01,0x03,0x18,0x01,
																			0x02,0x14,0x80,0x54,0x01,0x00,0x0f,0,0,0,0,1,1,1,1,2,2,2,3,3,3,3,
																				 0x05,0x80,0x5c,0x00,0x02,0x04};

STRUCT_Transfer_ICC_CappPurchase_GetMoney_rs	Transfer_ICC_CappPurchase_GetMoney_rs={0};

uint8_t ICC_CappPurchase_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 30;
	memcpy(Transfer_ICC_CappPurchase_GetMoney_rq.OBUID,OBU_VST_Info.MacAdrr,4);
	memcpy(Transfer_ICC_CappPurchase_GetMoney_rq.CappPurchase_EquimPurseSN,PSAM_ICC_CappPurseMac1_rs.CappMac1_EquimPurseSN,4);
	memcpy(Transfer_ICC_CappPurchase_GetMoney_rq.CappPurchase_PurseDate,PC_PurchaseStructure.PurchaseTime,7);
	memcpy(Transfer_ICC_CappPurchase_GetMoney_rq.CappPurchase_Mac1,PSAM_ICC_CappPurseMac1_rs.CappMac1_Mac1,4);
	
	Transfer_ICC_CappPurchase_GetMoney_rs.CappPurchasesw[0] = 0x00 ;
//	memset(&Transfer_ICC_CappPurchase_GetMoney_rs,0,sizeof(Transfer_ICC_CappPurchase_GetMoney_rs));
	DsrcReCount = DSRC_B5_COUNT;
	status = Dsrc_Trans_Func(TransferCharnnelType,Transfer_ICC_CappPurchase_GetMoney_rq.OBUID,sizeof(Transfer_ICC_CappPurchase_GetMoney_rq),Transfer_ICC_CappPurchase_GetMoney_rs.OBUID,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
	if(status == 0)
	{
		if(Transfer_ICC_CappPurchase_GetMoney_rs.CappPurchasesw[0] != 0x90)
		{
			return 1 ;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 2;
	}
	
//	return status;
}


//=============================================
//PSAM卡 :校验MAC2
//=============================================
typedef struct
{
	unsigned char CheckMac2_cmd[2];
	unsigned char CheckMac2_p1;
	unsigned char CheckMac2_p2;
	unsigned char CheckMac2_Lc;
	unsigned char CheckMac2_Data[4];
}STRUCT_PSAM_CheckMac2_rq;
STRUCT_PSAM_CheckMac2_rq	PSAM_CheckMac2_rq={0x80,0x72,0x00,0x00,0x04,
																	1,2,3,4};


typedef struct
{
	unsigned char CheckMac2_sw[2];
}STRUCT_PSAM_CheckMac2_rs;
STRUCT_PSAM_CheckMac2_rs	PSAM_CheckMac2_rs={0};	

uint8_t PSAM_CheckMac2_func(void)
{
	unsigned char PsamRxLen=0;

	memcpy(PSAM_CheckMac2_rq.CheckMac2_Data,Transfer_ICC_CappPurchase_GetMoney_rs.CappPurchase_Mac2,4);
	
	PSAM_CheckMac2_rs.CheckMac2_sw[0] = 0;
	log_hex("CheckMac2_rq: ",(uint8_t *)&PSAM_CheckMac2_rq,sizeof(PSAM_CheckMac2_rq));
	PSAM_APDU(PSAM2,sizeof(PSAM_CheckMac2_rq),PSAM_CheckMac2_rq.CheckMac2_cmd, &PsamRxLen,PSAM_CheckMac2_rs.CheckMac2_sw);	//APDU命令
	log_hex("CheckMac2_rs: ",(uint8_t *)&PSAM_CheckMac2_rs,PsamRxLen);
	if(PSAM_CheckMac2_rs.CheckMac2_sw[0] == 0x90)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}



uint8_t SC_NewCardTab[] = {0x00,0x0A,0xFF,0xF5};
uint8_t SC_OldCardTab[] = {0x00,0x03,0xFF,0xFC};
uint8_t SC_CardTab_rq[] = {0x01,0x02,0x03,0x04,0x40,0x77,0x91,0x05,0x01,0x03,0x18,0x01,0x01,0x0A,0x00,0xD6,0x84,0x00,0x05,0x11,0x22,0x33,0x44,0x55};
uint8_t SC_CardTab_rs[32];
	
//uint8_t SC_ICC_WriteBin_func(void)
//{
//	uint8_t status = 1;

//	memcpy(SC_CardTab_rq,OBU_VST_Info.MacAdrr,4);
//	SC_CardTab_rq[19] = PC_InitStructure.ICK04file[0];
//	
//	if((OBU_VST_Info.IccSysInfoVer & 0xF0) == 0x40)					//新卡
//	{
//		SC_CardTab_rq[16] = 0x8B;
//		memcpy(SC_CardTab_rq + 20,SC_NewCardTab,4);
//	}
//	else
//	{
//		SC_CardTab_rq[16] = 0x84;
//		memcpy(SC_CardTab_rq + 20,SC_OldCardTab,4);
//	}

//	DsrcReCount = DSRC_B5_COUNT;
//	status = Dsrc_Trans_Func(TransferCharnnelType,SC_CardTab_rq,sizeof(SC_CardTab_rq),SC_CardTab_rs,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
//	if(status == 0)
//	{
//		return 0;
//	}
//	else
//	{
//		return 2;
//	}
//	
////	return status;
//}

unsigned char SetMMI_rq[]={0x0C,0x00,0x5C,0x63,0x40,0xF7,0x91,0x05,0x01,0x04,0x1A,0x00};
//unsigned char SetMMI_rq[]={0x0C,0x00,0x5C,0x63,0x40,0xF7,0x91,0x05,0x01,0x04,0x1A,0x00};
unsigned char SetMMI_rs[32]={0};
uint8_t SetMMI_func(void)
{
	uint8_t status = 1;
	
//	DsrcOverTime_0 = 10;
	memcpy(SetMMI_rq,OBU_VST_Info.MacAdrr,4);
	if(RSU_TransInfoStructure.ErrorCode == 0)
	{
		SetMMI_rq[11] = 0;
	}
	else
	{
		SetMMI_rq[11] = 1;
	}
	DsrcReCount = 5;
	status = Dsrc_Trans_Func(SetMMIType,SetMMI_rq,sizeof(SetMMI_rq),SetMMI_rs,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
	return status;
}
unsigned char EventReport_rq[]={0x0C,0x00,0x5C,0x63,0x40,0x03,0x91,0x60,0x00,0x00};
unsigned char EventReport_rs[32] = {0};

//uint8_t EventReport_func(void)
//{	
//	uint8_t status = 1;
//	
//	memcpy(EventReport_rq,OBU_VST_Info.MacAdrr,4);
//	DsrcReCount = 15;
//	
//	status = Dsrc_Trans_Func(EvenReportType,EventReport_rq,sizeof(EventReport_rq),EventReport_rs,DsrcRxLen,DsrcReCount,&NowCount,DsrcOverTime_0);
//	return status;	
//}

uint8_t EventReport_func(void)
{	
	uint8_t ii;
	
//	BK5824ModeSet(0);
//	SLTX_PWR_LOW;
	
	memcpy(EventReport_rq,OBU_VST_Info.MacAdrr,4);
//	log_hex("EventReport_rq: ",EventReport_rq,sizeof(EventReport_rq));

	for(ii = 0; ii < 5; ii++)
	{
		SLTXRFTxFunc(EventReport_rq,10);
		USER_Delay1ms(1);
		log_title("EvenT");
	}
	
	return 0;
}

uint8_t SuccessEventReport_func(uint8_t *obuid)
{	
	uint8_t ii;
	
//	BK5824ModeSet(0);
//	SLTX_PWR_LOW;
	
	memcpy(EventReport_rq,obuid,4);
	log_hex("EventReport_rq: ",EventReport_rq,sizeof(EventReport_rq));

	for(ii = 0; ii < 5; ii++)
	{
		SLTXRFTxFunc(EventReport_rq,10);
		USER_Delay1ms(1);
		log_title("EvenT");
	}

	return 0;
}



void Dsrc_Procol_Test_func(void)
{
//	uint8_t status=0;
//	unsigned char psamRstData[128]={0};
//	unsigned char *psamRstLen=0;
//	
//	status=PSAM_RestCard(PSAM1,psamRstData,psamRstLen);		//复位PSAM
//	log_title("start Test...");
//	OBU_Bst_func();
//	OBU_GetSecure_func();
//	PSAM_CarDedes_func();
//	return;
//	ICCMoney_EsamDf01_func();
//	ICCMoney_EsamDf01_func();
//	ESAM_ReadEF04_func();
//	ESAM_WriteEF04_func();
//	ICC_Read0008_Rand8_func();
//	PSAM_ICCWrite0008Des_func();
//	ICC_ExAuth_Write0008_func();
//	ICC_CappPurchaseInit_func();
//	ICC_CappPurchase_func();
//	PSAM_ICC_CappPurseMac1_func();
//	ICC_CappPurchase_func();
//	SetMMI_func();
//	EventReport_func();
}

