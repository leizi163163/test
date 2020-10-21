#include "stm32f2xx.h"

typedef unsigned          char uint8_t;

/*
应用函数：
					AID1LLBstFunction(void);//AID=1模式下，链路标识的BST
					AID2LLBstFunction(void);//AID=2模式下，链路标识的BST
					
					AID1LLObuProcFunction(void);//OBU链路标识处理流程，包含以下：
						AID1LLObuGetSecureFunction(void);//OBU链路标识GetSecure
						AID1LLObuTrans1Function(void);//OBU链路标识通道命令1
						AID1LLObuTrans2Function(void);//OBU链路标识通道命令2
						AID1LLObuSetMMIFunction(void);//OBU链路标识SetMMI
						
					AID1LLCpcProcFunction(void);//Cpc链路标识处理流程，包含以下：
						AID1LLCpcTrans1Function(void);//Cpc链路标识通道命令
						
					AID1GBBstFunction(void);//AID=1模式下，广播标识的BST
					AID2GBBstFunction(void);//AID=2模式下，广播标识的BST
					
					AIDEvenReportFunction(void);//标识RSU天线的释放链路EvenReport
						
						
应用参数：
		AID=1链路标识参数：
					AID1LLBstStruct	AID1LLBst={0xff,0xff,0xff,0xff,0x50,0x03,0x91,0xc0,0xff,0x01,0x02,0x03,0xff,0x01,0x02,0x03,0x00,0x01,0x41,0x87,0x29,0x30,0x1a,0x00,0x29,0x00,0x07,0x00};
					AID1LLObuVstStruct	AID1LLObuVst={0};
					AID1LLCpcVstStruct	AID1LLCpcVst={0};
					
					AID1LLGetSectureRqStruct 	AID1LLGetSectureRq={0x01,0x02,0x03,0x04,0x40,0x77,0x91,0x05,0x01,0x00,0x14,0x80,0x01,0x00,0x00,0x10,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0x00,0x00};
					AID1LLGetSectureRsStruct 	AID1LLGetSectureRs={0};
					AID1LLObuTrans1RqStruct		AID1LLObuTrans1Rq={0x01,0x02,0x03,0x04,0x40,0xf7,0x91,0x05,0x01,0x03,0x18,0x02,0x01,0x05,0x00,0xb0,0x84,0x00,0x09,0x91,0x05,0x01,0x03,0x18,0x01,0x02,0x05,0x00,0xb0,0x88,0x00,0x03,0x05,0x00,0x84,0x00,0x00,0x08};
					AID1LLObuTrans1RsStruct		AID1LLObuTrans1Rs={0};
					AID1LLObuTrans2RqStruct		AID1LLObuTrans2Rq={0x01,0x02,0x03,0x04,0x40,0x77,0x91,0x05,0x01,0x03,0x18,0x02,0x02,0x0e,0x00,0xd6,0x84,0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0b,0x00,0xd6,0x84,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x91,0x05,0x01,0x03,0x18,0x01,0x03,0x0d,0x00,0x82,0x00,0x01,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0xd6,0x88,0x00,0x03,0x00,0x00,0x00,0x07,0x00,0xd6,0x88,0x00,0x02,0x00,0x00};
					AID1LLObuTrans2RsStruct		AID1LLObuTrans2Rs={0};
					AID1LLObuSetMMIRqStruct		AID1LLObuSetMMIRq={0x01,0x02,0x03,0x04,0x40,0xf7,0x91,0x05,0x01,0x04,0x1a,0x00};
					AID1LLObuSetMMIRsStruct		AID1LLObuSetMMIRs={0};
					
					AID1LLCpcTransRqStruct		AID1LLCpcTransRq={0x01,0x02,0x03,0x04,0x40,0x77,0x91,0x05,0x01,0x03,0x18,0x00,0x01,0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x07,0x00};
					AID1LLCpcTransRsStruct		AID1LLCpcTransRs={0};
					
		AID=1广播标识参数：
					AID1GBBstStruct	AID1GBBst={0xff,0xff,0xff,0xff,0x50,0x03,0x91,0xc0,0xff,0x01,0x02,0x03,0xff,0x01,0x02,0x03,0x00,0x01,0x41,0x87,0x29,0x60,0x19,0x08,0x03,0x00,0x2b,0x00};
					AID1GBObuVstStruct	AID1GBObuVst={0};
					AID1GBCpcVstStruct	AID1GBCpcVst={0};
					
		AID=2链路标识参数：
					AID2LLBstStruct	AID2LLBst={0xff,0xff,0xff,0xff,0x50,0x03,0x91,0xc0,0xff,0x01,0x02,0x03,0xff,0x01,0x02,0x03,0x00,0x01,0x42,0x87,0x31,0xe8,0x1e,0x00,0x07,0x00,0x2b,0x00,0x07,0x00,0x00,0x00};
					AID2LLVstStruct	AID2LLVst={0};
					
		AID=2广播标识参数：
					AID2GBBstStruct	AID2GBBst={0xff,0xff,0xff,0xff,0x50,0x03,0x91,0xc0,0xff,0x01,0x02,0x03,0xff,0x01,0x02,0x03,0x00,0x01,0x42,0x87,0x31,0xf8,0x1e,0x00,0x07,0x00,0x2b,0x00,0x07,0x07,0x01,0x00,0x00,0x00};
					AID2GBVstStruct	AID2GBVst={0};
						
*/

AID2GBBstStruct	AID2GBBst=
{
	0xff,0xff,0xff,0xff,				//ff ff ff ff:广播地址
	0x50,                               //50：Mac控制域；下行、存在LPDU、命令、广播信息并寻求建立专用链路
	0x03,                               //03：LLC控制域；无确认无链连接方式
	0x91,                               //91：段字头；无分段
	0xc0,                               //c0：BST标志
	0xff,0x01,0x02,0x03,                //ff 01 02 03：RSU ID
	0x5C,0x1A,0xDB,0x80,                //5C 1A DB 80：Unix时间		2018-12-20 8:00:00
	0x00,                               //00：信道广播
	0x01,                               //01：应用数量：1个应用
	0x42,                               //42：Dsrc-did不显示、Parameter存在、无扩展，AID=2（标识点）
	0x87,                               //87：Container存在、RSU支持的卡片交易模式
	0x29,                               //31：Container Type
	0xf8,                               //F8：OBU/CPC：读取EF04/ef03、读取0015/ef01、读取0008/ef02、携带广播标识、携带ACK	
	0x1e,                               //1E：系统信息文件长度
	0x00,                               //00:	预读文件偏移量 OBU:EF04  CPC:ef03
	0x07,                               //07：预读文件长度
	0x00,                               //00:	预读文件偏移量 OBU:0015  CPC:ef01
	0x2b,                               //2B：预读文件长度
	0x00,                               //00:	预读文件偏移量 OBU:0008  CPC:ef02
	0x07,                               //07：预读文件长度
	0x01,0x01,                          	//07 01：路径信息
	0x00,0x00,                          	//00 00:单一下行帧最小回复次数休眠时间（单位：s）,由Op5确定是否存在
	0x00,                               //00：ProfileList
};

AID2GBVstStruct	AID2GBVst={0};	

AID2LLBstStruct	AID2LLBst=
{
	0xff,0xff,0xff,0xff,			//ff ff ff ff:广播地址
	0x50,                           //50：Mac控制域；下行、存在LPDU、命令、广播信息并寻求建立专用链路
	0x03,                           //03：LLC控制域；无确认无链连接方式
	0x91,                           //91：段字头；无分段
	0xc0,                           //c0：BST标志
	0xff,0x01,0x02,0x03,            //ff 01 02 03：RSU ID
	0xff,0x01,0x02,0x03,            //ff 01 02 03：Unix时间
	0x00,                           //00：信道
	0x01,                           //01：应用数量：1个应用
	0x42,                           //42：Dsrc-did不显示、Parameter存在、无扩展，AID=2（标识点）
	0x87,                           //87：Container存在、RSU支持的卡片交易模式
	0x29,                           //31：Container Type
	0xe8,                           //E8：OBU/CPC：读取EF04/ef03、读取0015/ef01、读取0008/ef02、不携带广播标识、携带ACK	
	0x1e,                           //1E：系统信息文件长度
	0x00,                           //00:	预读文件偏移量 OBU:EF04  CPC:ef03
	0x07,                           //07：预读文件长度
	0x00,                           //00:	预读文件偏移量 OBU:0015  CPC:ef01
	0x2b,                           //2B：预读文件长度
	0x00,                           //00:	预读文件偏移量 OBU:0008  CPC:ef02
	0x07,                           //07：预读文件长度
	0x00,0x00,                      //00 00:单一下行帧最小回复次数休眠时间（单位：s）,由Op5确定是否存在
	0x00                           //00：ProfileList
};
AID2LLVstStruct	AID2LLVst={0};

AID2LLCpcTransRqStruct AID2LLCpcTransRq = 
{
	0x01,0x02,0x03,0x04,							//01 02 03 04:OBU地址
	0x40,                                           //40：Mac控制域；下行、存在LPDU、专用链路
	0x77,                                           //77：LLC控制域；类型3 包含反转位
	                           
	0x91,                                           //91：段字头；PDU=0010
	0x05,                                           //05：Action.req
	0x01,                            				//01:	DID
	0x03,				                            //03：TransferChannel
	0x18,                                           //18：容器类型18
	0x00,                                           //00：路径识别通道0x00
	0x01,                                           //01：APDU指令的数量，值取1
	0x13,                                           //13：指令1的长度，取值0x13
	0x00,                                           //16字节，天线使用SM4、外部认证密钥（外部认证根密钥经卡ID分散后得出）对随机数进行加密计算
													//2字节，路径信息
													//00:“建议休眠时长”用于指示CPC卡本次标识成功后，可以不再被唤醒的时长。0为不休眠，1~254为可休眠1~254min。255为无需再唤醒。													
};

AID2LLCpcTransRsStruct AID2LLCpcTransRs = {0};
	
uint8_t LLRouteInfo[2] = {0x01,0x01};
	
//////AID=2模式下，广播标识的BST
////uint8_t AID2GBBstFunction(void)		
////{
////	uint8_t	DsrcStatus=DsrcNull;

////	Get_unixtime(AID2GBBst.UnixTime);

//////	if(CommF0.RSUIDMode)
//////	{
//////		AID2GBBst.RsuBeaconID[3]++;
//////	}	
////	AID2GBBst.RsuBeaconID[3]++;
////	AID2GBBst.RouteInfo[0]++;
////	AID2GBBst.RouteInfo[1]++;
////	DsrcBuff.TxDataLen=sizeof(AID2GBBst);
////	memcpy(DsrcBuff.TxData,(uint8_t*)&AID2GBBst,DsrcBuff.TxDataLen);
////	DsrcStatus=DsrcTransferFunction(BstType);
////	if(DsrcStatus==DsrcSecussed)
////	{
////		memcpy((uint8_t*)&AID2GBVst,DsrcBuff.RxData,DsrcBuff.RxDataLen);//获取Vst信息
////		if(AID2GBVst.Op1InfoData[2] != AID2GBBst.RouteInfo[0] || AID2GBVst.Op1InfoData[3] != AID2GBBst.RouteInfo[1])
////		{
////			//如果标识不成功，如何处理
////		}
////		EvenReportFunction(AID2_GB_MODE);								//链路释放
////		return DsrcStatus;
////	}
////	return	DsrcError;
////}

uint8_t AID2GBBstFunction(void)		
{
	uint8_t	DsrcStatus=DsrcNull;
	
	Get_unixtime(Bst.UnixTime);

	Bst.RsuBeaconID[3]++;

	DsrcBuff.TxDataLen=sizeof(Bst);
	memcpy(DsrcBuff.TxData,(uint8_t*)&Bst,DsrcBuff.TxDataLen);
	DsrcStatus=SLDsrcTransferFunction(BstType);
	if(DsrcStatus==DsrcSecussed)
	{
		memcpy((uint8_t*)&Vst,DsrcBuff.RxData,DsrcBuff.RxDataLen);//获取Vst信息
		memcpy((uint8_t*)&EsamEF01,Vst.SysInfoData,sizeof(Vst.SysInfoData));//获取ESAM EF01系统基本信息文件
		if(Vst.InfoPara==0x00)//OBU无卡，所以没有返回GbIccInfoData
		{
//			memcpy(Vst.MacId,&Vst.GbIccInfoContainer,7);
//			memset(&Vst.GbIccInfoContainer,0xFF,1);
//			memset(Vst.GbIccInfoData,(uint8_t)"No Icc Card",11);
		}
		else
		{
			memcpy(&Icc0015,Vst.GbIccInfoData,sizeof(Vst.GbIccInfoData));//获取用户卡ICC	0015发行基本数据
		}
		return DsrcStatus;
	}
	return	DsrcError;
}

/******************************************************************************************/

//AID=2模式下，链路标识的BST
/******************************************************************************************/
uint8_t AID2LLBstFunction(void)		
{
	uint8_t	DsrcStatus=DsrcNull;

	Get_unixtime(AID2LLBst.UnixTime);

//	if(CommF0.RSUIDMode)
	{
		AID2LLBst.RsuBeaconID[3]++;
	}	

	DsrcBuff.TxDataLen=sizeof(AID2LLBst);
	memcpy(DsrcBuff.TxData,(uint8_t*)&AID2LLBst,DsrcBuff.TxDataLen);
	DsrcStatus=DsrcTransferFunction(BstType);
	if(DsrcStatus==DsrcSecussed)
	{
		memcpy((uint8_t*)&AID2LLVst,DsrcBuff.RxData,DsrcBuff.RxDataLen);	//获取Vst信息
		return DsrcStatus;
	}
	return	DsrcError;
}

uint8_t	AID2LLCpcInitForDesFunction(uint8_t P1,uint8_t P2)
{
	InitForDescrypt.P1=P1;
	InitForDescrypt.P2=P2;
	memcpy(InitForDescrypt.ContractSn,AID2LLVst.SysInfoData + 8,8);
	if((P1&0xf0)==0x20)//密钥一级分散
	{
		InitForDescrypt.Lc=0x08;
		memcpy(Psam2.Cmd,&InitForDescrypt,sizeof(InitForDescrypt)-8);//华软PSAM卡密钥分散时只需要卡片内部序列号
		Psam2.CmdLen=sizeof(InitForDescrypt)-8;
	}
	else
	{
		InitForDescrypt.Lc=0x10;
		memcpy(InitForDescrypt.IssuerFlag1,AID2LLVst.SysInfoData,4);
		memcpy(InitForDescrypt.IssuerFlag2,AID2LLVst.SysInfoData,4);
		memcpy(Psam2.Cmd,&InitForDescrypt,sizeof(InitForDescrypt));//二级分散密钥需要增加发行方标识
		Psam2.CmdLen=sizeof(InitForDescrypt);
	}
	PSAMTransferAPDU(&Psam2);
	if(Psam2.CosSw.result!=CosRspOK)
		return	PsamErr;
	return	DsrcSecussed;
}

uint8_t	AID2LLCpcDesFunction(void)
{
	uint8_t RandomBuf[16] = {0};	//调试过程中发现，如果下边memcpy对前8字节赋值，则此句只会令后8字节为0，
									//如果没有下边memcpy语句，则此句会令16字节均为0。应该是Keil语句优化的结果。

//	memset(RandomBuf,0,16);
//	memset(&Psam2.CmdLen,0,sizeof(Psam2) - 1);
	memcpy(RandomBuf,AID2LLVst.Random8,8);
	memcpy(PsamDes.IccRnd8,RandomBuf,16);//计算Des用的Icc随机数
	//装载APDU
	Psam2.CmdLen=21;
	memcpy(Psam2.Cmd,&PsamDes,Psam2.CmdLen);
	PSAMTransferAPDU(&Psam2);
	if(Psam2.CosSw.result!=CosRspOK)
		return	PsamErr;
	memcpy(&PsamDesInfo,Psam2.Data,Psam2.DataLen);
	return	DsrcSecussed;
	
	
}

/******************************************************************************************/
uint8_t AID2LLCpcTrans1Function(void)
{
	uint8_t	DsrcStatus=DsrcNull;
	
	
	memcpy(AID2LLCpcTransRq.MacAddr,AID2LLVst.MacAddr,4);//获取OBU MacAddr
	
	memcpy(AID2LLCpcTransRq.L1Apdu1Data,&PsamDesInfo.DesInfo,16);
	LLRouteInfo[0]++;
	LLRouteInfo[1]++;
	memcpy(AID2LLCpcTransRq.RouteInfo,LLRouteInfo,2);//获取OBU MacAddr
	//装载Dsrc发送数据缓冲区
	DsrcBuff.TxDataLen=sizeof(AID2LLCpcTransRq);
	memcpy(DsrcBuff.TxData,&AID2LLCpcTransRq,DsrcBuff.TxDataLen);
	//发送Dsrc
	DsrcStatus=DsrcTransferFunction(TransferCharnnelType);
	//处理Dsrc返回数据
	if(DsrcStatus==DsrcSecussed)
	{
		memcpy(&AID2LLCpcTransRs,DsrcBuff.RxData,DsrcBuff.RxDataLen);//取返回数据
		return	DsrcSecussed;
//		memcpy(&EsamEF1DInfo,TransferRs.TransferRsData,DsrcBuff.RxDataLen-TransferRsHeadLen);
//		if((EsamEF1DInfo.EF1DBuff[EsamEF1DInfo.EF1DLen-2]==0x90)&&(EsamEF1DInfo.EF1DBuff[EsamEF1DInfo.EF1DLen-1]==0x00))
//		{
//			memcpy(EsamEF1DInfo.EF1DData,EsamEF1DInfo.EF1DBuff,EsamEF1DInfo.EF1DLen-TransferSWLen);
//			return	DsrcSecussed;
//		}
	}
//	log_title("TransferEsamRq1 DsrcErr");
	return	DsrcError;
}

uint8_t AID2LLCpcProcFunction(void)
{
//	uint8_t status = 0;
	
	if(AID2LLBstFunction() == DsrcSecussed)
	{
		if(PsamSelectDirc(SelectDF01,SelectDF01Len) != DsrcSecussed)
		{
			return 1;
		}
		if(AID2LLCpcInitForDesFunction(0x48,0x44) != DsrcSecussed)
		{
			return 2;
		}
		if(AID2LLCpcDesFunction() != DsrcSecussed)
		{
			return 3;
		}
		if(AID2LLCpcTrans1Function() != DsrcSecussed)
		{
			return 4;
		}
		EvenReportFunction(AID2_LL_MODE);								//链路释放
		return 5;
		
//		PsamSelectDirc(SelectDF01,SelectDF01Len);
//		AID2LLCpcInitForDesFunction(0x48,0x44);
//		AID2LLCpcDesFunction();
//		AID2LLCpcTrans1Function();
	}
	return 0;
}


//链路释放
/*****************************************************************************************/
void EvenReportFunction(uint8_t AID_Mode)
{
	switch(AID_Mode)
	{
		case AID2_GB_MODE:
			memcpy(EventReportRq.MacAddr,AID2GBVst.MacAddr,4);//获取OBU MacAddr
			break;
		case AID2_LL_MODE:
			memcpy(EventReportRq.MacAddr,AID2LLVst.MacAddr,4);//获取OBU MacAddr
			break;
		
		default:
			break;
	}
//	memcpy(EventReportRq.MacAddr,AID2GBVst.MacAddr,4);//获取OBU MacAddr
	DsrcBuff.TxDataLen=10;
	memcpy(DsrcBuff.TxData,(uint8_t*)&EventReportRq,DsrcBuff.TxDataLen);
	DsrcTransferFunction(EvenReportType);
}
/*****************************************************************************************/




