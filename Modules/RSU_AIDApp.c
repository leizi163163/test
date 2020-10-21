#include "stm32f2xx.h"

typedef unsigned          char uint8_t;

/*
Ӧ�ú�����
					AID1LLBstFunction(void);//AID=1ģʽ�£���·��ʶ��BST
					AID2LLBstFunction(void);//AID=2ģʽ�£���·��ʶ��BST
					
					AID1LLObuProcFunction(void);//OBU��·��ʶ�������̣��������£�
						AID1LLObuGetSecureFunction(void);//OBU��·��ʶGetSecure
						AID1LLObuTrans1Function(void);//OBU��·��ʶͨ������1
						AID1LLObuTrans2Function(void);//OBU��·��ʶͨ������2
						AID1LLObuSetMMIFunction(void);//OBU��·��ʶSetMMI
						
					AID1LLCpcProcFunction(void);//Cpc��·��ʶ�������̣��������£�
						AID1LLCpcTrans1Function(void);//Cpc��·��ʶͨ������
						
					AID1GBBstFunction(void);//AID=1ģʽ�£��㲥��ʶ��BST
					AID2GBBstFunction(void);//AID=2ģʽ�£��㲥��ʶ��BST
					
					AIDEvenReportFunction(void);//��ʶRSU���ߵ��ͷ���·EvenReport
						
						
Ӧ�ò�����
		AID=1��·��ʶ������
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
					
		AID=1�㲥��ʶ������
					AID1GBBstStruct	AID1GBBst={0xff,0xff,0xff,0xff,0x50,0x03,0x91,0xc0,0xff,0x01,0x02,0x03,0xff,0x01,0x02,0x03,0x00,0x01,0x41,0x87,0x29,0x60,0x19,0x08,0x03,0x00,0x2b,0x00};
					AID1GBObuVstStruct	AID1GBObuVst={0};
					AID1GBCpcVstStruct	AID1GBCpcVst={0};
					
		AID=2��·��ʶ������
					AID2LLBstStruct	AID2LLBst={0xff,0xff,0xff,0xff,0x50,0x03,0x91,0xc0,0xff,0x01,0x02,0x03,0xff,0x01,0x02,0x03,0x00,0x01,0x42,0x87,0x31,0xe8,0x1e,0x00,0x07,0x00,0x2b,0x00,0x07,0x00,0x00,0x00};
					AID2LLVstStruct	AID2LLVst={0};
					
		AID=2�㲥��ʶ������
					AID2GBBstStruct	AID2GBBst={0xff,0xff,0xff,0xff,0x50,0x03,0x91,0xc0,0xff,0x01,0x02,0x03,0xff,0x01,0x02,0x03,0x00,0x01,0x42,0x87,0x31,0xf8,0x1e,0x00,0x07,0x00,0x2b,0x00,0x07,0x07,0x01,0x00,0x00,0x00};
					AID2GBVstStruct	AID2GBVst={0};
						
*/

AID2GBBstStruct	AID2GBBst=
{
	0xff,0xff,0xff,0xff,				//ff ff ff ff:�㲥��ַ
	0x50,                               //50��Mac���������С�����LPDU������㲥��Ϣ��Ѱ����ר����·
	0x03,                               //03��LLC��������ȷ���������ӷ�ʽ
	0x91,                               //91������ͷ���޷ֶ�
	0xc0,                               //c0��BST��־
	0xff,0x01,0x02,0x03,                //ff 01 02 03��RSU ID
	0x5C,0x1A,0xDB,0x80,                //5C 1A DB 80��Unixʱ��		2018-12-20 8:00:00
	0x00,                               //00���ŵ��㲥
	0x01,                               //01��Ӧ��������1��Ӧ��
	0x42,                               //42��Dsrc-did����ʾ��Parameter���ڡ�����չ��AID=2����ʶ�㣩
	0x87,                               //87��Container���ڡ�RSU֧�ֵĿ�Ƭ����ģʽ
	0x29,                               //31��Container Type
	0xf8,                               //F8��OBU/CPC����ȡEF04/ef03����ȡ0015/ef01����ȡ0008/ef02��Я���㲥��ʶ��Я��ACK	
	0x1e,                               //1E��ϵͳ��Ϣ�ļ�����
	0x00,                               //00:	Ԥ���ļ�ƫ���� OBU:EF04  CPC:ef03
	0x07,                               //07��Ԥ���ļ�����
	0x00,                               //00:	Ԥ���ļ�ƫ���� OBU:0015  CPC:ef01
	0x2b,                               //2B��Ԥ���ļ�����
	0x00,                               //00:	Ԥ���ļ�ƫ���� OBU:0008  CPC:ef02
	0x07,                               //07��Ԥ���ļ�����
	0x01,0x01,                          	//07 01��·����Ϣ
	0x00,0x00,                          	//00 00:��һ����֡��С�ظ���������ʱ�䣨��λ��s��,��Op5ȷ���Ƿ����
	0x00,                               //00��ProfileList
};

AID2GBVstStruct	AID2GBVst={0};	

AID2LLBstStruct	AID2LLBst=
{
	0xff,0xff,0xff,0xff,			//ff ff ff ff:�㲥��ַ
	0x50,                           //50��Mac���������С�����LPDU������㲥��Ϣ��Ѱ����ר����·
	0x03,                           //03��LLC��������ȷ���������ӷ�ʽ
	0x91,                           //91������ͷ���޷ֶ�
	0xc0,                           //c0��BST��־
	0xff,0x01,0x02,0x03,            //ff 01 02 03��RSU ID
	0xff,0x01,0x02,0x03,            //ff 01 02 03��Unixʱ��
	0x00,                           //00���ŵ�
	0x01,                           //01��Ӧ��������1��Ӧ��
	0x42,                           //42��Dsrc-did����ʾ��Parameter���ڡ�����չ��AID=2����ʶ�㣩
	0x87,                           //87��Container���ڡ�RSU֧�ֵĿ�Ƭ����ģʽ
	0x29,                           //31��Container Type
	0xe8,                           //E8��OBU/CPC����ȡEF04/ef03����ȡ0015/ef01����ȡ0008/ef02����Я���㲥��ʶ��Я��ACK	
	0x1e,                           //1E��ϵͳ��Ϣ�ļ�����
	0x00,                           //00:	Ԥ���ļ�ƫ���� OBU:EF04  CPC:ef03
	0x07,                           //07��Ԥ���ļ�����
	0x00,                           //00:	Ԥ���ļ�ƫ���� OBU:0015  CPC:ef01
	0x2b,                           //2B��Ԥ���ļ�����
	0x00,                           //00:	Ԥ���ļ�ƫ���� OBU:0008  CPC:ef02
	0x07,                           //07��Ԥ���ļ�����
	0x00,0x00,                      //00 00:��һ����֡��С�ظ���������ʱ�䣨��λ��s��,��Op5ȷ���Ƿ����
	0x00                           //00��ProfileList
};
AID2LLVstStruct	AID2LLVst={0};

AID2LLCpcTransRqStruct AID2LLCpcTransRq = 
{
	0x01,0x02,0x03,0x04,							//01 02 03 04:OBU��ַ
	0x40,                                           //40��Mac���������С�����LPDU��ר����·
	0x77,                                           //77��LLC����������3 ������תλ
	                           
	0x91,                                           //91������ͷ��PDU=0010
	0x05,                                           //05��Action.req
	0x01,                            				//01:	DID
	0x03,				                            //03��TransferChannel
	0x18,                                           //18����������18
	0x00,                                           //00��·��ʶ��ͨ��0x00
	0x01,                                           //01��APDUָ���������ֵȡ1
	0x13,                                           //13��ָ��1�ĳ��ȣ�ȡֵ0x13
	0x00,                                           //16�ֽڣ�����ʹ��SM4���ⲿ��֤��Կ���ⲿ��֤����Կ����ID��ɢ��ó�������������м��ܼ���
													//2�ֽڣ�·����Ϣ
													//00:����������ʱ��������ָʾCPC�����α�ʶ�ɹ��󣬿��Բ��ٱ����ѵ�ʱ����0Ϊ�����ߣ�1~254Ϊ������1~254min��255Ϊ�����ٻ��ѡ�													
};

AID2LLCpcTransRsStruct AID2LLCpcTransRs = {0};
	
uint8_t LLRouteInfo[2] = {0x01,0x01};
	
//////AID=2ģʽ�£��㲥��ʶ��BST
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
////		memcpy((uint8_t*)&AID2GBVst,DsrcBuff.RxData,DsrcBuff.RxDataLen);//��ȡVst��Ϣ
////		if(AID2GBVst.Op1InfoData[2] != AID2GBBst.RouteInfo[0] || AID2GBVst.Op1InfoData[3] != AID2GBBst.RouteInfo[1])
////		{
////			//�����ʶ���ɹ�����δ���
////		}
////		EvenReportFunction(AID2_GB_MODE);								//��·�ͷ�
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
		memcpy((uint8_t*)&Vst,DsrcBuff.RxData,DsrcBuff.RxDataLen);//��ȡVst��Ϣ
		memcpy((uint8_t*)&EsamEF01,Vst.SysInfoData,sizeof(Vst.SysInfoData));//��ȡESAM EF01ϵͳ������Ϣ�ļ�
		if(Vst.InfoPara==0x00)//OBU�޿�������û�з���GbIccInfoData
		{
//			memcpy(Vst.MacId,&Vst.GbIccInfoContainer,7);
//			memset(&Vst.GbIccInfoContainer,0xFF,1);
//			memset(Vst.GbIccInfoData,(uint8_t)"No Icc Card",11);
		}
		else
		{
			memcpy(&Icc0015,Vst.GbIccInfoData,sizeof(Vst.GbIccInfoData));//��ȡ�û���ICC	0015���л�������
		}
		return DsrcStatus;
	}
	return	DsrcError;
}

/******************************************************************************************/

//AID=2ģʽ�£���·��ʶ��BST
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
		memcpy((uint8_t*)&AID2LLVst,DsrcBuff.RxData,DsrcBuff.RxDataLen);	//��ȡVst��Ϣ
		return DsrcStatus;
	}
	return	DsrcError;
}

uint8_t	AID2LLCpcInitForDesFunction(uint8_t P1,uint8_t P2)
{
	InitForDescrypt.P1=P1;
	InitForDescrypt.P2=P2;
	memcpy(InitForDescrypt.ContractSn,AID2LLVst.SysInfoData + 8,8);
	if((P1&0xf0)==0x20)//��Կһ����ɢ
	{
		InitForDescrypt.Lc=0x08;
		memcpy(Psam2.Cmd,&InitForDescrypt,sizeof(InitForDescrypt)-8);//����PSAM����Կ��ɢʱֻ��Ҫ��Ƭ�ڲ����к�
		Psam2.CmdLen=sizeof(InitForDescrypt)-8;
	}
	else
	{
		InitForDescrypt.Lc=0x10;
		memcpy(InitForDescrypt.IssuerFlag1,AID2LLVst.SysInfoData,4);
		memcpy(InitForDescrypt.IssuerFlag2,AID2LLVst.SysInfoData,4);
		memcpy(Psam2.Cmd,&InitForDescrypt,sizeof(InitForDescrypt));//������ɢ��Կ��Ҫ���ӷ��з���ʶ
		Psam2.CmdLen=sizeof(InitForDescrypt);
	}
	PSAMTransferAPDU(&Psam2);
	if(Psam2.CosSw.result!=CosRspOK)
		return	PsamErr;
	return	DsrcSecussed;
}

uint8_t	AID2LLCpcDesFunction(void)
{
	uint8_t RandomBuf[16] = {0};	//���Թ����з��֣�����±�memcpy��ǰ8�ֽڸ�ֵ����˾�ֻ�����8�ֽ�Ϊ0��
									//���û���±�memcpy��䣬��˾����16�ֽھ�Ϊ0��Ӧ����Keil����Ż��Ľ����

//	memset(RandomBuf,0,16);
//	memset(&Psam2.CmdLen,0,sizeof(Psam2) - 1);
	memcpy(RandomBuf,AID2LLVst.Random8,8);
	memcpy(PsamDes.IccRnd8,RandomBuf,16);//����Des�õ�Icc�����
	//װ��APDU
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
	
	
	memcpy(AID2LLCpcTransRq.MacAddr,AID2LLVst.MacAddr,4);//��ȡOBU MacAddr
	
	memcpy(AID2LLCpcTransRq.L1Apdu1Data,&PsamDesInfo.DesInfo,16);
	LLRouteInfo[0]++;
	LLRouteInfo[1]++;
	memcpy(AID2LLCpcTransRq.RouteInfo,LLRouteInfo,2);//��ȡOBU MacAddr
	//װ��Dsrc�������ݻ�����
	DsrcBuff.TxDataLen=sizeof(AID2LLCpcTransRq);
	memcpy(DsrcBuff.TxData,&AID2LLCpcTransRq,DsrcBuff.TxDataLen);
	//����Dsrc
	DsrcStatus=DsrcTransferFunction(TransferCharnnelType);
	//����Dsrc��������
	if(DsrcStatus==DsrcSecussed)
	{
		memcpy(&AID2LLCpcTransRs,DsrcBuff.RxData,DsrcBuff.RxDataLen);//ȡ��������
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
		EvenReportFunction(AID2_LL_MODE);								//��·�ͷ�
		return 5;
		
//		PsamSelectDirc(SelectDF01,SelectDF01Len);
//		AID2LLCpcInitForDesFunction(0x48,0x44);
//		AID2LLCpcDesFunction();
//		AID2LLCpcTrans1Function();
	}
	return 0;
}


//��·�ͷ�
/*****************************************************************************************/
void EvenReportFunction(uint8_t AID_Mode)
{
	switch(AID_Mode)
	{
		case AID2_GB_MODE:
			memcpy(EventReportRq.MacAddr,AID2GBVst.MacAddr,4);//��ȡOBU MacAddr
			break;
		case AID2_LL_MODE:
			memcpy(EventReportRq.MacAddr,AID2LLVst.MacAddr,4);//��ȡOBU MacAddr
			break;
		
		default:
			break;
	}
//	memcpy(EventReportRq.MacAddr,AID2GBVst.MacAddr,4);//��ȡOBU MacAddr
	DsrcBuff.TxDataLen=10;
	memcpy(DsrcBuff.TxData,(uint8_t*)&EventReportRq,DsrcBuff.TxDataLen);
	DsrcTransferFunction(EvenReportType);
}
/*****************************************************************************************/




