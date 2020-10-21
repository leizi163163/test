#ifndef _RSU_AID2_H_
#define _RSU_AID2_H_

#include "stm32f2xx.h"


//��������VST
#define		AID2_GB_MODE		0x00	
#define		AID2_LL_MODE		0x01


//AID=2ģʽ�£�OBU��CPC����·��ʶ����
/*
1��RSU����AID=2�Ĺ㲥BST
2��RSU�յ�VST�����MAC��ַ������OBU����CPC���б𷽷�ΪMacAddr[0]:00~FF=>OBU  A0~FF=>CPC
3���ֱ����OBU����CPC����·��ʶ����
4����·��ʶ������AID=1ģʽ����ͬ

��ע��AID=1��AID=2����·��ʶ������һ���ģ�ֻ����BST��VST�����ݸ�ʽ���в�ͬ������������ɺ�ͨ���������̣�AID=1��AID=2�ֱ����OBU��CPC��������һ����
�ܽ᣺
			a����ʶRSU����·��ʶ�����������̣�OBU���̺�CPC����
			b��AID=1��AID=2����·������һ����
			c����ʶRUS����4��BST��AID=1�㲥BST��AID=1��·BST��AID=2�㲥BST��AID=2��·BST
			d����ͬ�Ĺ㲥BST����Ҫ�ж�VST��OBU����CPC���Դ�����ͬ�����ݽ���
*/

//AID=2ģʽ�£�OBU��CPC���㲥��ʶ����
/*
1��RSU����AID=2�Ĺ㲥BST
2���յ�CPC����OBU��VST
3������EvenReport
*/

////////////////////////////////////OBU��CPC���㲥��ʶ��BST��VST���ݸ�ʽ////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];					//ff ff ff ff:�㲥��ַ
	uint8_t	MacControl;					//50��Mac���������С�����LPDU������㲥��Ϣ��Ѱ����ר����·
	uint8_t	LLCControl;					//03��LLC��������ȷ���������ӷ�ʽ
	uint8_t	SegmentHead;				//91������ͷ���޷ֶ�
	uint8_t	BstFlag;					//c0��BST��־
	uint8_t	RsuBeaconID[4];				//ff 01 02 03��RSU ID
	uint8_t	UnixTime[4];				//ff 01 02 03��Unixʱ��
	uint8_t	Channel;					//00���ŵ��㲥
	uint8_t	MandApplication;			//01��Ӧ��������1��Ӧ��
	uint8_t	DsrcDidAid;					//42��Dsrc-did����ʾ��Parameter���ڡ�����չ��AID=2����ʶ�㣩
	uint8_t	AppPara;					//87��Container���ڡ�RSU֧�ֵĿ�Ƭ����ģʽ
	uint8_t	Container;					//31��Container Type
	uint8_t	PrefetchControl;			//F8��OBU/CPC����ȡEF04/ef03����ȡ0015/ef01����ȡ0008/ef02��Я���㲥��ʶ��Я��ACK	
	uint8_t	SysInfoFlieLength;			//1E��ϵͳ��Ϣ�ļ�����
	uint8_t	Op1Offset;					//00:	Ԥ���ļ�ƫ���� OBU:EF04  CPC:ef03
	uint8_t	Op1Length;					//07��Ԥ���ļ�����
	uint8_t	Op2Offset;					//00:	Ԥ���ļ�ƫ���� OBU:0015  CPC:ef01
	uint8_t	Op2Length;					//2B��Ԥ���ļ�����
	uint8_t	Op3Offset;					//00:	Ԥ���ļ�ƫ���� OBU:0008  CPC:ef02
	uint8_t	Op3Length;					//07��Ԥ���ļ�����
	uint8_t 	RouteInfo[2];					//07 01��·����Ϣ
	uint8_t	AckPara[2];					//00 00:��һ����֡��С�ظ���������ʱ�䣨��λ��s��,��Op5ȷ���Ƿ����
	uint8_t	ProfileList;				//00��ProfileList
}AID2GBBstStruct;


typedef struct
{
	uint8_t	MacAddr[4];					//01 02 03 04:OBU/CPC MAC   MacAddr[0]:00~FF=>OBU  A0~FF=>CPC
	uint8_t	MacControl;					//C0
	uint8_t	LLCControl;					//03
	uint8_t	SegmentHead;				//91
	uint8_t	VstFlag;						//D0
	uint8_t	Channel;						//00:�ŵ���
	uint8_t	ApplicationList;		//01:Ӧ��
	uint8_t	DsrcDidAid;					//C2��C1��ETC C2����ʶ��
	uint8_t	DsrcDid;						//01
	
	uint8_t	InfoPara;						//F0������ϵͳ��Ϣ�ļ�������EF04/ef03������0015/ef01������0008/ef02����Я�������
	uint8_t	SysInfoLength;			//1E
	uint8_t	SysInfoData[30];		//OBU/CPC��SysInfoFile�ļ�����ǰ30�ֽ�
	uint8_t	Op1Length;					//07			//EF04/ef03����
	uint8_t	Op1InfoData[7];			//7Data		//
	uint8_t	Op2Length;					//2B			//0015/ef01���ݳ���
	uint8_t	Op2InfoData[43];		//41Data	//
	uint8_t	Op3Length;					//07			//0008/ef02���ݳ���
	uint8_t	Op3InfoData0[7];		//7Data	  //
	uint8_t	EquipmentInfo;			//�豸��Ϣ�����ͺͰ汾�� ���λΪ1��OBU 0:CPC
	uint8_t	CpcStatus0;					//Cpc״̬����ص���
	uint8_t	CpcStatus1;					//��ж״̬
}AID2GBVstStruct;//����Vst��Ϣ


////////////////////////////////////OBU��CPC����·��ʶ��BST��VST���ݸ�ʽ////////////////////////////
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
	uint8_t	DsrcDidAid;					//42��Dsrc-did����ʾ��Parameter���ڡ�����չ��AID=2����ʶ�㣩
	uint8_t	AppPara;						//87��Container���ڡ�RSU֧�ֵĿ�Ƭ����ģʽ
	uint8_t	Container;					//31��Container Type
	uint8_t	PrefetchControl;		//E8��OBU/CPC����ȡEF04/ef03����ȡ0015/ef01����ȡ0008/ef02����Я���㲥��ʶ��Я��ACK	
	uint8_t	SysInfoFlieLength;	//1E��ϵͳ��Ϣ�ļ�����
	uint8_t	Op1Offset;					//00:	Ԥ���ļ�ƫ���� OBU:EF04  CPC:ef03
	uint8_t	Op1Length;					//07��Ԥ���ļ�����
	uint8_t	Op2Offset;					//00:	Ԥ���ļ�ƫ���� OBU:0015  CPC:ef01
	uint8_t	Op2Length;					//2B��Ԥ���ļ�����
	uint8_t	Op3Offset;					//00:	Ԥ���ļ�ƫ���� OBU:0008  CPC:ef02
	uint8_t	Op3Length;					//07��Ԥ���ļ�����
	uint8_t	AckPara[2];					//00 00:��һ����֡��С�ظ���������ʱ�䣨��λ��s��,��Op5ȷ���Ƿ����
	uint8_t	ProfileList;				//00��ProfileList
}AID2LLBstStruct;

typedef struct
{
	uint8_t	MacAddr[4];					//01 02 03 04:OBU/CPC MAC   MacAddr[0]:00~FF=>OBU  A0~FF=>CPC
	uint8_t	MacControl;					//C0
	uint8_t	LLCControl;					//03
	uint8_t	SegmentHead;				//91
	uint8_t	VstFlag;						//D0
	uint8_t	Channel;						//00:�ŵ���
	uint8_t	ApplicationList;		//01:Ӧ��
	uint8_t	DsrcDidAid;					//C1/C2��C1��ETC C2����ʶ��
	uint8_t	DsrcDid;						//01
	
	uint8_t	InfoPara;						//F8������ϵͳ��Ϣ�ļ�������EF04/ef03������0015/ef01������0008/ef02��Я�������
	uint8_t	SysInfoLength;			//1E
	uint8_t	SysInfoData[30];		//OBU/CPC��SysInfoFile�ļ�����ǰ30�ֽ�
	uint8_t	Op1Length;					//07			//EF04/ef03����
	uint8_t	Op1InfoData[7];			//7Data		//
	uint8_t	Op2Length;					//2B			//0015/ef01���ݳ���
	uint8_t	Op2InfoData[43];		//41Data	//
	uint8_t	Op3Length;					//07			//0008/ef02���ݳ���
	uint8_t	Op3InfoData0[7];		//7Data	  //
	uint8_t	Random8[8];					//������ɿ�Ƭ�������ڶ����ߵ���֤
	uint8_t	EquipmentInfo;			//�豸��Ϣ�����ͺͰ汾�� ���λΪ1��OBU 0:CPC
	uint8_t	CpcStatus0;					//Cpc״̬����ص���
	uint8_t	CpcStatus1;					//��ж״̬
}AID2LLVstStruct;//����Vst��Ϣ


////////////////////////////////////CPC��·��ʶ͸�����ݸ�ʽ��1��͸�����ݣ�////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU��ַ
	uint8_t	MacControl;				//40��Mac���������С�����LPDU��ר����·
	uint8_t	LLCControl;				//77��LLC����������3 ������תλ
	
	uint8_t	SegmentHead1;			//91������ͷ��PDU=0010
	uint8_t	ActionRq1;				//05��Action.req
	uint8_t	DsrcDid1;				//01:	DID
	uint8_t	ActionType1;			//03��TransferChannel
	uint8_t	ChannelRq1;				//18����������18
	uint8_t	ChannelID1;				//00��·��ʶ��ͨ��0x00
	uint8_t	APDUList1;				//01��APDUָ���������ֵȡ1
	uint8_t	L1Apdu1Len;				//13��ָ��1�ĳ��ȣ�ȡֵ0x13
	uint8_t	L1Apdu1Data[16];		//����ʹ��SM4���ⲿ��֤��Կ���ⲿ��֤����Կ����ID��ɢ��ó�������������м��ܼ���
	uint8_t	RouteInfo[2];			//·����Ϣ
	uint8_t	SleepInterval;			//00:����������ʱ��������ָʾCPC�����α�ʶ�ɹ��󣬿��Բ��ٱ����ѵ�ʱ����0Ϊ�����ߣ�1~254Ϊ������1~254min��255Ϊ�����ٻ��ѡ�
}AID2LLCpcTransRqStruct;			//Cpc��·��ʶ͸������

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
}AID2LLCpcTransRsStruct;	//Cpc��·��ʶ͸����Ӧ

////////////////////////////////////OBU��CPC��ʶEvenReport���ݸ�ʽ////////////////////////////
typedef	struct
{
	uint8_t	MacAddr[4];										//01 02 03 04
	uint8_t	MacControl;										//40
	uint8_t	LLCControl;										//03
	uint8_t	SegmentHead;									//91
	uint8_t	EventReportRq;								//60
	uint8_t	DsrcDid;											//00
	uint8_t	EventType;										//00
}AID2LLObuEvenReportRqStruct;				//EventReport




uint8_t AID2GBBstFunction(void);

uint8_t AID2GBBstFunction(void);

uint8_t AID2LLBstFunction(void);

uint8_t AID2LLCpcProcFunction(void);

void EvenReportFunction(uint8_t AID_Mode);
	


#endif

