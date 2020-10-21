#ifndef _RSU_AID2_H_
#define _RSU_AID2_H_

#include "stm32f2xx.h"


//用于区分VST
#define		AID2_GB_MODE		0x00	
#define		AID2_LL_MODE		0x01


//AID=2模式下，OBU和CPC卡链路标识流程
/*
1，RSU发送AID=2的广播BST
2，RSU收到VST后根据MAC地址区分是OBU还是CPC，判别方法为MacAddr[0]:00~FF=>OBU  A0~FF=>CPC
3，分别进入OBU或者CPC的链路标识流程
4，链路标识流程与AID=1模式下相同

备注：AID=1和AID=2的链路标识流程是一样的，只是在BST和VST的数据格式上有不同解析，解析完成后，通道命令流程，AID=1和AID=2分别对于OBU和CPC的流程是一样的
总结：
			a，标识RSU的链路标识中有两个流程：OBU流程和CPC流程
			b，AID=1和AID=2的链路流程是一样的
			c，标识RUS中有4中BST：AID=1广播BST、AID=1链路BST、AID=2广播BST、AID=2链路BST
			d，不同的广播BST，均要判断VST是OBU还是CPC，以此做不同的数据解析
*/

//AID=2模式下，OBU和CPC卡广播标识流程
/*
1，RSU发送AID=2的广播BST
2，收到CPC或者OBU的VST
3，发送EvenReport
*/

////////////////////////////////////OBU、CPC卡广播标识的BST和VST数据格式////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];					//ff ff ff ff:广播地址
	uint8_t	MacControl;					//50：Mac控制域；下行、存在LPDU、命令、广播信息并寻求建立专用链路
	uint8_t	LLCControl;					//03：LLC控制域；无确认无链连接方式
	uint8_t	SegmentHead;				//91：段字头；无分段
	uint8_t	BstFlag;					//c0：BST标志
	uint8_t	RsuBeaconID[4];				//ff 01 02 03：RSU ID
	uint8_t	UnixTime[4];				//ff 01 02 03：Unix时间
	uint8_t	Channel;					//00：信道广播
	uint8_t	MandApplication;			//01：应用数量：1个应用
	uint8_t	DsrcDidAid;					//42：Dsrc-did不显示、Parameter存在、无扩展，AID=2（标识点）
	uint8_t	AppPara;					//87：Container存在、RSU支持的卡片交易模式
	uint8_t	Container;					//31：Container Type
	uint8_t	PrefetchControl;			//F8：OBU/CPC：读取EF04/ef03、读取0015/ef01、读取0008/ef02、携带广播标识、携带ACK	
	uint8_t	SysInfoFlieLength;			//1E：系统信息文件长度
	uint8_t	Op1Offset;					//00:	预读文件偏移量 OBU:EF04  CPC:ef03
	uint8_t	Op1Length;					//07：预读文件长度
	uint8_t	Op2Offset;					//00:	预读文件偏移量 OBU:0015  CPC:ef01
	uint8_t	Op2Length;					//2B：预读文件长度
	uint8_t	Op3Offset;					//00:	预读文件偏移量 OBU:0008  CPC:ef02
	uint8_t	Op3Length;					//07：预读文件长度
	uint8_t 	RouteInfo[2];					//07 01：路径信息
	uint8_t	AckPara[2];					//00 00:单一下行帧最小回复次数休眠时间（单位：s）,由Op5确定是否存在
	uint8_t	ProfileList;				//00：ProfileList
}AID2GBBstStruct;


typedef struct
{
	uint8_t	MacAddr[4];					//01 02 03 04:OBU/CPC MAC   MacAddr[0]:00~FF=>OBU  A0~FF=>CPC
	uint8_t	MacControl;					//C0
	uint8_t	LLCControl;					//03
	uint8_t	SegmentHead;				//91
	uint8_t	VstFlag;						//D0
	uint8_t	Channel;						//00:信道号
	uint8_t	ApplicationList;		//01:应用
	uint8_t	DsrcDidAid;					//C2：C1：ETC C2：标识点
	uint8_t	DsrcDid;						//01
	
	uint8_t	InfoPara;						//F0：存在系统信息文件、存在EF04/ef03、存在0015/ef01、存在0008/ef02、不携带随机数
	uint8_t	SysInfoLength;			//1E
	uint8_t	SysInfoData[30];		//OBU/CPC中SysInfoFile文件内容前30字节
	uint8_t	Op1Length;					//07			//EF04/ef03数据
	uint8_t	Op1InfoData[7];			//7Data		//
	uint8_t	Op2Length;					//2B			//0015/ef01数据长度
	uint8_t	Op2InfoData[43];		//41Data	//
	uint8_t	Op3Length;					//07			//0008/ef02数据长度
	uint8_t	Op3InfoData0[7];		//7Data	  //
	uint8_t	EquipmentInfo;			//设备信息（类型和版本） 最高位为1：OBU 0:CPC
	uint8_t	CpcStatus0;					//Cpc状态：电池电量
	uint8_t	CpcStatus1;					//拆卸状态
}AID2GBVstStruct;//返回Vst信息


////////////////////////////////////OBU、CPC卡链路标识的BST和VST数据格式////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];					//ff ff ff ff:广播地址
	uint8_t	MacControl;					//50：Mac控制域；下行、存在LPDU、命令、广播信息并寻求建立专用链路
	uint8_t	LLCControl;					//03：LLC控制域；无确认无链连接方式
	uint8_t	SegmentHead;				//91：段字头；无分段
	uint8_t	BstFlag;						//c0：BST标志
	uint8_t	RsuBeaconID[4];			//ff 01 02 03：RSU ID
	uint8_t	UnixTime[4];				//ff 01 02 03：Unix时间
	uint8_t	Channel;						//00：信道
	uint8_t	MandApplication;		//01：应用数量：1个应用
	uint8_t	DsrcDidAid;					//42：Dsrc-did不显示、Parameter存在、无扩展，AID=2（标识点）
	uint8_t	AppPara;						//87：Container存在、RSU支持的卡片交易模式
	uint8_t	Container;					//31：Container Type
	uint8_t	PrefetchControl;		//E8：OBU/CPC：读取EF04/ef03、读取0015/ef01、读取0008/ef02、不携带广播标识、携带ACK	
	uint8_t	SysInfoFlieLength;	//1E：系统信息文件长度
	uint8_t	Op1Offset;					//00:	预读文件偏移量 OBU:EF04  CPC:ef03
	uint8_t	Op1Length;					//07：预读文件长度
	uint8_t	Op2Offset;					//00:	预读文件偏移量 OBU:0015  CPC:ef01
	uint8_t	Op2Length;					//2B：预读文件长度
	uint8_t	Op3Offset;					//00:	预读文件偏移量 OBU:0008  CPC:ef02
	uint8_t	Op3Length;					//07：预读文件长度
	uint8_t	AckPara[2];					//00 00:单一下行帧最小回复次数休眠时间（单位：s）,由Op5确定是否存在
	uint8_t	ProfileList;				//00：ProfileList
}AID2LLBstStruct;

typedef struct
{
	uint8_t	MacAddr[4];					//01 02 03 04:OBU/CPC MAC   MacAddr[0]:00~FF=>OBU  A0~FF=>CPC
	uint8_t	MacControl;					//C0
	uint8_t	LLCControl;					//03
	uint8_t	SegmentHead;				//91
	uint8_t	VstFlag;						//D0
	uint8_t	Channel;						//00:信道号
	uint8_t	ApplicationList;		//01:应用
	uint8_t	DsrcDidAid;					//C1/C2：C1：ETC C2：标识点
	uint8_t	DsrcDid;						//01
	
	uint8_t	InfoPara;						//F8：存在系统信息文件、存在EF04/ef03、存在0015/ef01、存在0008/ef02、携带随机数
	uint8_t	SysInfoLength;			//1E
	uint8_t	SysInfoData[30];		//OBU/CPC中SysInfoFile文件内容前30字节
	uint8_t	Op1Length;					//07			//EF04/ef03数据
	uint8_t	Op1InfoData[7];			//7Data		//
	uint8_t	Op2Length;					//2B			//0015/ef01数据长度
	uint8_t	Op2InfoData[43];		//41Data	//
	uint8_t	Op3Length;					//07			//0008/ef02数据长度
	uint8_t	Op3InfoData0[7];		//7Data	  //
	uint8_t	Random8[8];					//随机数由卡片生成用于对天线的认证
	uint8_t	EquipmentInfo;			//设备信息（类型和版本） 最高位为1：OBU 0:CPC
	uint8_t	CpcStatus0;					//Cpc状态：电池电量
	uint8_t	CpcStatus1;					//拆卸状态
}AID2LLVstStruct;//返回Vst信息


////////////////////////////////////CPC链路标识透传数据格式（1条透传数据）////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU地址
	uint8_t	MacControl;				//40：Mac控制域；下行、存在LPDU、专用链路
	uint8_t	LLCControl;				//77：LLC控制域；类型3 包含反转位
	
	uint8_t	SegmentHead1;			//91：段字头；PDU=0010
	uint8_t	ActionRq1;				//05：Action.req
	uint8_t	DsrcDid1;				//01:	DID
	uint8_t	ActionType1;			//03：TransferChannel
	uint8_t	ChannelRq1;				//18：容器类型18
	uint8_t	ChannelID1;				//00：路径识别通道0x00
	uint8_t	APDUList1;				//01：APDU指令的数量，值取1
	uint8_t	L1Apdu1Len;				//13：指令1的长度，取值0x13
	uint8_t	L1Apdu1Data[16];		//天线使用SM4、外部认证密钥（外部认证根密钥经卡ID分散后得出）对随机数进行加密计算
	uint8_t	RouteInfo[2];			//路径信息
	uint8_t	SleepInterval;			//00:“建议休眠时长”用于指示CPC卡本次标识成功后，可以不再被唤醒的时长。0为不休眠，1~254为可休眠1~254min。255为无需再唤醒。
}AID2LLCpcTransRqStruct;			//Cpc链路标识透传命令

typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU地址
	uint8_t	MacControl;				//E0：Mac控制域；下行、存在LPDU、专用链路
	uint8_t	LLCControl;				//F7：LLC控制域；类型3 包含反转位
	uint8_t	LLCState;					//00
	
	uint8_t	SegmentHead1;			//91：段字头；PDU=0010
	uint8_t	ActionRs1;				//18：Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ChannelRs1;				//19：容器类型18
	uint8_t	ChannelID1;				//00：路径识别通道0x00
	uint8_t	APDUList1;				//00：APDU指令的数量，值取0
	uint8_t	L1ResturnStatus;	//00:00表示执行成功
}AID2LLCpcTransRsStruct;	//Cpc链路标识透传响应

////////////////////////////////////OBU、CPC标识EvenReport数据格式////////////////////////////
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

