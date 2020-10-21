#ifndef _RSU_AID1_H_
#define _RSU_AID1_H_

#include "stm32f2xx.h"

//AID=1	ETC������ʶ
//AID=2	��·�м�RSU��ʶ



//AID=1ģʽ�£�OBU��CPC����·��ʶ����
/*
1��RSU����AID=1�Ĺ㲥BST
2��RSU�յ�VST�����MAC��ַ������OBU����CPC���б𷽷�ΪMacAddr[0]:00~FF=>OBU  A0~FF=>CPC
3���ֱ����OBU����CPC����·��ʶ����

OBU��·��ʶ���̣�
1��PSAMȡ8λ�����
2������GetSecure,����Я��RSU��PSAM�����ɵ������
3��OBU����GetSecure
4�����͵�һ��͸��������а�������ָ�ȡESAM��EF04�ļ���ǰ9���ֽڡ�ȡICC��0008�ļ���ǰ3���ֽڡ�ȡICC����8λ�����
5��OBU���ص�һ��͸����Ӧ��RSU�ж�����ָ����Ӧ��״̬���Ƿ�Ϊ����
6����ȡESAM��EF04�ļ�ǰ9�ֽ���Ϣ��������·����Ϣ��EF04�ļ��е�ƫ����
7����PSAM����֯��ICC���ⲿ��֤������
8����ȡICC��0008�ļ�ǰ3�ֽ���Ϣ��������·����Ϣ��0008�ļ��е�ƫ����
9�����͵ڶ���͸��������а�������ָ�����ESAM��EF04�ļ�ǰ9�ֽ���Ϣ������EF04·����Ϣ���踽��ƫ����������ICC�����ⲿ��֤�������ICC��0008�ļ�ǰ3�ֽ���Ϣ������0008·����Ϣ���踽��ƫ������
10��OBU���صڶ���͸����Ӧ��RSU�ж�����ָ����Ӧ��״̬���Ƿ�Ϊ����
11��SetMMI
12��Evenreport

CPC��·��ʶ���̣�
1��ͨ��PSAM��ʹ��SM4���ⲿ��֤��Կ���ⲿ��֤����Կ����ID��ɢ��ó�������������м��ܼ��㣬��ȡ16λ����
2������͸��������а������ⲿ��֤�����ġ�·����Ϣ������ʱ��
3��CPC����͸����Ӧ��RSU�жϱ�ʶ�Ƿ�ɹ�
4��EvenReport
*/

////////////////////////////////////OBU��CPC����BST��VST���ݸ�ʽ////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];					//ff ff ff ff:�㲥��ַ
	uint8_t	MacControl;					//50��Mac���������С�����LPDU������㲥��Ϣ��Ѱ����ר����·
	uint8_t	LLCControl;					//03��LLC��������ȷ���������ӷ�ʽ
	uint8_t	SegmentHead;				//91������ͷ���޷ֶ�
	uint8_t	BstFlag;						//c0��BST��־
	uint8_t	RsuBeaconID[4];			//ff 01 02 03��RSU ID
	uint8_t	UnixTime[4];				//ff 01 02 03��Unixʱ��
	uint8_t	Channel;						//00���ŵ�
	uint8_t	MandApplication;		//01��Ӧ��������1��Ӧ��
	uint8_t	DsrcDidAid;					//41��Dsrc-did����ʾ��Parameter���ڡ�����չ��AID=1��ETC��
	uint8_t	AppPara;						//87��Container���ڡ�RSU֧�ֵĿ�Ƭ����ģʽ
	uint8_t	Container;					//29��Container Type
	uint8_t	PrefetchControl;		//30����·��ʶ��Ԥ����Ϣ
	uint8_t	SysInfoFlieLength;	//1A��ϵͳ��Ϣ�ļ�����
	uint8_t	Op3Offset;			//00:	Ԥ���ļ�ƫ���� OBU:0015  CPC:EF01
	uint8_t	Op3Length;			//29��Ԥ���ļ�����
	uint8_t	Op4Offset;			//00:	Ԥ���ļ�ƫ���� OBU:0019  CPC:EF02
	uint8_t	Op4Length;			//07��Ԥ���ļ�����
	uint8_t	ProfileList;				//00��ProfileList
}AID1LLBstStruct;

typedef struct
{
	uint8_t	MacAddr[4];					//01 02 03 04:OBU/CPC MAC   MacAddr[0]:00~FF=>OBU  A0~FF=>CPC
	uint8_t	MacControl;					//C0
	uint8_t	LLCControl;					//03
	uint8_t	SegmentHead;				//91
	uint8_t	VstFlag;						//D0
	uint8_t	Channel;						//00:�ŵ���
	uint8_t	ApplicationList;		//01:Ӧ��
	uint8_t	DsrcDidAid;					//C1/C2��C1��OBU�ظ� C2��CPC�ظ�
	uint8_t	DsrcDid;						//01
	
	uint8_t	InfoPara;						//20
	uint8_t	SysInfoContainer;		//27
	uint8_t	SysInfoData[26];		//OBU/CPC��SysInfoFile�ļ�����ǰ26�ֽ�
	uint8_t	GbIccInfoContainer;	//28
	uint8_t	GbIccInfoData[41];	//41Data	//0015�ļ���Ϣ
	uint8_t	GbIccInfoData0[7];	//7Data	  //0019�ļ���Ϣ
	uint8_t	MacId[4];						//OBU MacId
	uint8_t	EquipmentInfo;			//�豸��Ϣ�����ͺͰ汾��
	uint8_t	ObuStatus;					//OBU״̬��IC����OBU�Ƿ�������OBU�Ƿ�𶯡�OBU����Ƿ�����
	uint8_t	EquipmentStatus;		//��ж״̬
}AID1LLObuVstStruct;//Obu ����Vst��Ϣ

typedef struct
{
	uint8_t	MacAddr[4];					//01 02 03 04:OBU/CPC MAC   MacAddr[0]:00~FF=>OBU  A0~FF=>CPC
	uint8_t	MacControl;					//C0
	uint8_t	LLCControl;					//03
	uint8_t	SegmentHead;				//91
	uint8_t	VstFlag;						//D0
	uint8_t	Channel;						//00:�ŵ���
	uint8_t	ApplicationList;		//01:Ӧ��
	uint8_t	DsrcDidAid;					//C1/C2��C1��OBU�ظ� C2��CPC�ظ�
	uint8_t	DsrcDid;						//01
	
	uint8_t	InfoPara;						//B8
	uint8_t	SysInfoLength;			//1A
	uint8_t	SysInfoData[26];		//OBU/CPC��SysInfoFile�ļ�����ǰ26�ֽ�
	uint8_t	Op3Length;					//29			//EF01���ݳ���
	uint8_t	Op3InfoData[41];		//41Data	//EF01�ļ���Ϣ
	uint8_t	Op4Length;					//07			//EF01���ݳ���
	uint8_t	Op4InfoData0[7];		//7Data	  //0019�ļ���Ϣ
	uint8_t	Random8[8];					//������ɿ�Ƭ�������ڶ����ߵ���֤
	uint8_t	EquipmentInfo;			//�豸��Ϣ�����ͺͰ汾��
	uint8_t	CpcStatus0;					//Cpc״̬����ص���
	uint8_t	CpcStatus1;					//��ж״̬
}AID1LLCpcVstStruct;//Cpc ����Vst��Ϣ

////////////////////////////////////OBU��·��ʶGetSecture���ݸ�ʽ////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU��ַ
	uint8_t	MacControl;				//40��Mac���������С�����LPDU��ר����·
	uint8_t	LLCControl;				//77��LLC����������3 ������תλ
	uint8_t	SegmentHead;			//91������ͷ��PDU=0010
	uint8_t	ActionRq;					//05��Action.req
	uint8_t	DsrcDid;					//01:	DID
	uint8_t	ActionType;				//00��Getseure
	uint8_t	Contain;					//14����������20
	uint8_t	Fill;							//80��keyedForEncrypt��������
	uint8_t	FID;							//01��ETCӦ�ó�����Ϣ�ļ�
	uint8_t Offset1;					//00 00������������ʼλ��
	uint8_t	Offset2;
	uint8_t	Length;						//10���������ݳ���length
	uint8_t	Random[8];				//F1 F2 F3 F4 F5 F6 F7 F8��Rsu���������
	uint8_t	AuthenKeyId;			//00����Ϣ������Կ����Կ��ʶ
	uint8_t	EncryptKeyId;			//00:	������Կ����Կ��ʶ
}AID1LLGetSectureRqStruct;//GetSecture����
typedef	struct
{
	uint8_t	MacAddr[4];				//01 02 03 04
	uint8_t	MacControl;				//E0
	uint8_t	LLCControl;				//f7
	uint8_t	LLCState;					//00
	
	uint8_t	SegmentHead;			//91
	uint8_t	ActionRs;					//18
	uint8_t	DsrcDid;					//01
	uint8_t	GetSecureRs;			//15
	uint8_t	Fid;							//01
	uint8_t	CartInfo[16];			//16Data
	uint8_t	Authenticator[8];	//8Data
	uint8_t	ReturnStatus;			//00
}AID1LLGetSectureRsStruct;//GetSecture����
////////////////////////////////////OBU��·��ʶ͸�����ݸ�ʽ��2��͸�����ݣ�////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU��ַ
	uint8_t	MacControl;				//40��Mac���������С�����LPDU��ר����·
	uint8_t	LLCControl;				//F7��LLC����������3 ������תλ
	
	uint8_t	SegmentHead1;			//91������ͷ��PDU=0010
	uint8_t	ActionRq1;				//05��Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ActionType1;			//03��TransferChannel
	uint8_t	ChannelRq1;				//18����������18
	uint8_t	ChannelID1;				//02��Esamͨ��0x02
	uint8_t	APDUList1;				//01��APDUָ���������ֵȡ1
	uint8_t	L1Apdu1Len;				//05��ָ��1�ĳ��ȣ�ȡֵ0x05
	uint8_t	L1Apdu1Data[5];		//00 b0 84 00 09
	
	uint8_t	SegmentHead2;			//91������ͷ��PDU=0010
	uint8_t	ActionRq2;				//05��Action.req
	uint8_t	DsrcDid2;					//01:	DID
	uint8_t	ActionType2;			//03��TransferChannel
	uint8_t	ChannelRq2;				//18����������18
	uint8_t	ChannelID2;				//01��Iccͨ��0x01
	uint8_t	APDUList2;				//02��APDUָ���������ֵȡ2
	uint8_t	L2Apdu1Len;				//05��ָ��2�ĳ��ȣ�ȡֵ0x05
	uint8_t	L2Apdu1Data[5];		//00 b0 88 00 03
	uint8_t	L2Apdu2Len;				//05��ָ��2�ĳ��ȣ�ȡֵ0x05
	uint8_t	L2Apdu2Data[5];		//00 84 00 00 08
}AID1LLObuTrans1RqStruct;	//Obu��·��ʶ��һ��͸������
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU��ַ
	uint8_t	MacControl;				//E0��Mac���������С�����LPDU��ר����·
	uint8_t	LLCControl;				//77��LLC����������3 ������תλ
	uint8_t	LLCState;					//00
	
	uint8_t	SegmentHead1;			//91������ͷ��PDU=0010
	uint8_t	ActionRs1;				//18��Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ChannelRs1;				//19����������18
	uint8_t	ChannelID1;				//02��Esamͨ��0x02
	uint8_t	APDUList1;				//01��APDUָ���������ֵȡ1
	uint8_t	L1Apdu1Len;				//0B������1�ĳ��ȣ�ȡֵ0x0B
	uint8_t	L1Apdu1Data[9];		//9data:EF04����
	uint16_t	L1SW1;					//״̬�֣�9000��ʾOK
	uint8_t	L1ResturnStatus;	//00:00��ʾִ�гɹ�
	
	uint8_t	SegmentHead2;			//91������ͷ��PDU=0010
	uint8_t	ActionRs2;				//18��Action.req
	uint8_t	DsrcDid2;					//01:	DID
	uint8_t	ChannelRs2;				//19����������18
	uint8_t	ChannelID2;				//01��Iccͨ��0x01
	uint8_t	APDUList2;				//02��APDU���ݵ�������ֵȡ2
	uint8_t	L2Apdu1Len;				//05������2�ĳ��ȣ�ȡֵ0x05
	uint8_t	L2Apdu1Data[3];		//3data:0008����
	uint16_t	L2SW1;					//״̬�֣�9000��ʾOK
	uint8_t	L2Apdu2Len;				//0a������3�ĳ��ȣ�ȡֵ0x0a
	uint8_t	L2Apdu2Data[8];		//8data:8�ֽ������
	uint16_t	L2SW2;					//״̬�֣�9000��ʾOK
	uint8_t	L2ResturnStatus;	//00:00��ʾִ�гɹ�
}AID1LLObuTrans1RsStruct;	//Obu��·��ʶ��һ��͸����Ӧ

typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU��ַ
	uint8_t	MacControl;				//40��Mac���������С�����LPDU��ר����·
	uint8_t	LLCControl;				//77��LLC����������3 ������תλ
	
	uint8_t	SegmentHead1;			//91������ͷ��PDU=0010
	uint8_t	ActionRq1;				//05��Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ActionType1;			//03��TransferChannel
	uint8_t	ChannelRq1;				//18����������18
	uint8_t	ChannelID1;				//02��Esamͨ��0x02
	uint8_t	APDUList1;				//02��APDUָ���������ֵȡ1
	uint8_t	L1Apdu1Len;				//0E��ָ��1�ĳ��ȣ�ȡֵ0x0E
	uint8_t	L1Apdu1Data[14];	//00 D6 84 00 09 + EF04ǰ9�ֽ�����
	uint8_t	L1Apdu2Len;				//0B��ָ��2�ĳ��ȣ�ȡֵ0x0B
	uint8_t	L1Apdu2Data[11];	//00 D6 84 xx 09 + 6�ֽڣ�·����Ϣ2�ֽڣ�ʱ���4�ֽڣ�UNIXʱ�䣩//xxΪƫ����
	
	uint8_t	SegmentHead2;			//91������ͷ��PDU=0010
	uint8_t	ActionRq2;				//05��Action.req
	uint8_t	DsrcDid2;					//01:	DID
	uint8_t	ActionType2;			//03��TransferChannel
	uint8_t	ChannelRq2;				//18����������18
	uint8_t	ChannelID2;				//01��Iccͨ��0x01
	uint8_t	APDUList2;				//03��APDUָ���������ֵȡ3
	uint8_t	L2Apdu1Len;				//0D��ָ��1�ĳ��ȣ�ȡֵ0x0D
	uint8_t	L2Apdu1Data[13];	//00 82 00 01 08 + 8�ֽ���֤���ݣ��ⲿ��֤��
	uint8_t	L2Apdu2Len;				//08��ָ��2�ĳ��ȣ�ȡֵ0x08
	uint8_t	L2Apdu2Data[8];		//00 D6 88 00 03 + 08�ļ�ǰ3�ֽ�
	uint8_t	L2Apdu3Len;				//07��ָ��3�ĳ��ȣ�ȡֵ0x07
	uint8_t	L2Apdu3Data[7];		//00 D6 88 xx 02 + 2�ֽ�·����Ϣ //xxΪƫ����
}AID1LLObuTrans2RqStruct;	//Obu��·��ʶ�ڶ���͸������
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU��ַ
	uint8_t	MacControl;				//E0��Mac���������С�����LPDU��ר����·
	uint8_t	LLCControl;				//77��LLC����������3 ������תλ
	uint8_t	LLCState;					//00
	
	uint8_t	SegmentHead1;			//91������ͷ��PDU=0010
	uint8_t	ActionRs1;				//18��Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ChannelRs1;				//19����������18
	uint8_t	ChannelID1;				//02��Esamͨ��0x02
	uint8_t	APDUList1;				//02��APDUָ���������ֵȡ2
	uint8_t	L1Apdu1Len;				//02������1�ĳ��ȣ�ȡֵ0x02
	uint16_t	L1SW1;					//״̬�֣�9000��ʾOK
	uint8_t	L1Apdu2Len;				//02������1�ĳ��ȣ�ȡֵ0x02
	uint16_t	L1SW2;					//״̬�֣�9000��ʾOK
	uint8_t	L1ResturnStatus;	//00:00��ʾִ�гɹ�
	
	uint8_t	SegmentHead2;			//91������ͷ��PDU=0010
	uint8_t	ActionRs2;				//18��Action.req
	uint8_t	DsrcDid2;					//01:	DID
	uint8_t	ChannelRs2;				//19����������18
	uint8_t	ChannelID2;				//01��Iccͨ��0x01
	uint8_t	APDUList2;				//03��APDU���ݵ�������ֵȡ3
	uint8_t	L2Apdu1Len;				//02������1�ĳ��ȣ�ȡֵ0x02
	uint16_t	L2SW1;					//״̬�֣�9000��ʾOK
	uint8_t	L2Apdu2Len;				//02������2�ĳ��ȣ�ȡֵ0x02
	uint16_t	L2SW2;					//״̬�֣�9000��ʾOK
	uint8_t	L2Apdu3Len;				//02������3�ĳ��ȣ�ȡֵ0x02
	uint16_t	L2SW3;					//״̬�֣�9000��ʾOK
	uint8_t	L2ResturnStatus;	//00:00��ʾִ�гɹ�
}AID1LLObuTrans2RsStruct;	//Obu��·��ʶ�ڶ���͸����Ӧ

////////////////////////////////////OBU��·��ʶSetMMI���ݸ�ʽ////////////////////////////
typedef	struct
{
	uint8_t	MacAddr[4];										//01 02 03 04
	uint8_t	MacControl;										//40
	uint8_t	LLCControl;										//f7
	uint8_t	SegmentHead;									//91
	uint8_t	ActionRq;											//05
	uint8_t	DsrcDid;											//01
	uint8_t	ActionType;										//04
	uint8_t	Container;										//1A
	uint8_t	AccessParameter;							//00:OK 01:NOK
}AID1LLObuSetMMIRqStruct;//Obu��·��ʶSetMMI����
typedef	struct
{
	uint8_t	MacAddr[4];										//01 02 03 04
	uint8_t	MacControl;										//e0
	uint8_t	LLCControl;										//77
	uint8_t	LLCState;											//00
	uint8_t	SegmentHead;									//91
	uint8_t	ActionRs;											//18
	uint8_t	DsrcDid;											//01
	uint8_t	Container;										//1B
	uint8_t	Ret;													//00
}AID1LLObuSetMMIRsStruct;//Obu��·��ʶSetMMI����

////////////////////////////////////CPC��·��ʶ͸�����ݸ�ʽ��1��͸�����ݣ�////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU��ַ
	uint8_t	MacControl;				//40��Mac���������С�����LPDU��ר����·
	uint8_t	LLCControl;				//77��LLC����������3 ������תλ
	
	uint8_t	SegmentHead1;			//91������ͷ��PDU=0010
	uint8_t	ActionRq1;				//05��Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ActionType1;			//03��TransferChannel
	uint8_t	ChannelRq1;				//18����������18
	uint8_t	ChannelID1;				//00��·��ʶ��ͨ��0x00
	uint8_t	APDUList1;				//01��APDUָ���������ֵȡ1
	uint8_t	L1Apdu1Len;				//13��ָ��1�ĳ��ȣ�ȡֵ0x13
	uint8_t	L1Apdu1Data[16];	//����ʹ��SM4���ⲿ��֤��Կ���ⲿ��֤����Կ����ID��ɢ��ó�������������м��ܼ���
	uint8_t	RouteInfo[2];			//·����Ϣ
	uint8_t	SleepInterval;		//00:����������ʱ��������ָʾCPC�����α�ʶ�ɹ��󣬿��Բ��ٱ����ѵ�ʱ����0Ϊ�����ߣ�1~254Ϊ������1~254min��255Ϊ�����ٻ��ѡ�
}AID1LLCpcTransRqStruct;	//Cpc��·��ʶ͸������
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU��ַ
	uint8_t	MacControl;				//E0��Mac���������С�����LPDU��ר����·
	uint8_t	LLCControl;				//F7��LLC����������3 ������תλ
	uint8_t	LLCState;					//00
	
	uint8_t	SegmentHead1;			//91������ͷ��PDU=0010
	uint8_t	ActionRs1;				//18��Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ChannelRs1;				//19����������18
	uint8_t	ChannelID1;				//00��·��ʶ��ͨ��0x00
	uint8_t	APDUList1;				//00��APDUָ���������ֵȡ0
	uint8_t	L1ResturnStatus;	//00:00��ʾִ�гɹ�
}AID1LLCpcTransRsStruct;	//Cpc��·��ʶ͸����Ӧ

////////////////////////////////////OBU��CPC��·��ʶEvenReport���ݸ�ʽ////////////////////////////
typedef	struct
{
	uint8_t	MacAddr[4];										//01 02 03 04
	uint8_t	MacControl;										//40
	uint8_t	LLCControl;										//03
	uint8_t	SegmentHead;									//91
	uint8_t	EventReportRq;								//60
	uint8_t	DsrcDid;											//00
	uint8_t	EventType;										//00
}AID1LLObuEvenReportRqStruct;//EventReport


#endif

