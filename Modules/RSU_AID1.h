#ifndef _RSU_AID1_H_
#define _RSU_AID1_H_

#include "stm32f2xx.h"

//AID=1	ETC车道标识
//AID=2	道路中间RSU标识



//AID=1模式下，OBU和CPC卡链路标识流程
/*
1，RSU发送AID=1的广播BST
2，RSU收到VST后根据MAC地址区分是OBU还是CPC，判别方法为MacAddr[0]:00~FF=>OBU  A0~FF=>CPC
3，分别进入OBU或者CPC的链路标识流程

OBU链路标识流程：
1，PSAM取8位随机数
2，发送GetSecure,其中携带RSU从PSAM卡生成的随机数
3，OBU返回GetSecure
4，发送第一条透传命令，其中包括三条指令：取ESAM卡EF04文件的前9个字节、取ICC卡0008文件的前3个字节、取ICC卡的8位随机数
5，OBU返回第一条透传响应，RSU判断三条指令响应的状态字是否为正常
6，获取ESAM卡EF04文件前9字节信息，并计算路径信息在EF04文件中的偏移量
7，从PSAM卡组织对ICC卡外部认证的密文
8，获取ICC卡0008文件前3字节信息，并计算路径信息在0008文件中的偏移量
9，发送第二条透传命令，其中包括五条指令：更新ESAM卡EF04文件前9字节信息、更新EF04路径信息（需附带偏移量）、对ICC卡的外部认证命令、更新ICC卡0008文件前3字节信息、更新0008路径信息（需附带偏移量）
10，OBU返回第二条透传响应，RSU判断五条指令响应的状态字是否为正常
11，SetMMI
12，Evenreport

CPC链路标识流程：
1，通过PSAM卡使用SM4、外部认证密钥（外部认证根密钥经卡ID分散后得出）对随机数进行加密计算，获取16位密文
2，发送透传命令，其中包括：外部认证的密文、路径信息、休眠时间
3，CPC返回透传响应，RSU判断标识是否成功
4，EvenReport
*/

////////////////////////////////////OBU、CPC卡的BST和VST数据格式////////////////////////////
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
	uint8_t	DsrcDidAid;					//41：Dsrc-did不显示、Parameter存在、无扩展，AID=1（ETC）
	uint8_t	AppPara;						//87：Container存在、RSU支持的卡片交易模式
	uint8_t	Container;					//29：Container Type
	uint8_t	PrefetchControl;		//30：链路标识和预读信息
	uint8_t	SysInfoFlieLength;	//1A：系统信息文件长度
	uint8_t	Op3Offset;			//00:	预读文件偏移量 OBU:0015  CPC:EF01
	uint8_t	Op3Length;			//29：预读文件长度
	uint8_t	Op4Offset;			//00:	预读文件偏移量 OBU:0019  CPC:EF02
	uint8_t	Op4Length;			//07：预读文件长度
	uint8_t	ProfileList;				//00：ProfileList
}AID1LLBstStruct;

typedef struct
{
	uint8_t	MacAddr[4];					//01 02 03 04:OBU/CPC MAC   MacAddr[0]:00~FF=>OBU  A0~FF=>CPC
	uint8_t	MacControl;					//C0
	uint8_t	LLCControl;					//03
	uint8_t	SegmentHead;				//91
	uint8_t	VstFlag;						//D0
	uint8_t	Channel;						//00:信道号
	uint8_t	ApplicationList;		//01:应用
	uint8_t	DsrcDidAid;					//C1/C2：C1：OBU回复 C2：CPC回复
	uint8_t	DsrcDid;						//01
	
	uint8_t	InfoPara;						//20
	uint8_t	SysInfoContainer;		//27
	uint8_t	SysInfoData[26];		//OBU/CPC中SysInfoFile文件内容前26字节
	uint8_t	GbIccInfoContainer;	//28
	uint8_t	GbIccInfoData[41];	//41Data	//0015文件信息
	uint8_t	GbIccInfoData0[7];	//7Data	  //0019文件信息
	uint8_t	MacId[4];						//OBU MacId
	uint8_t	EquipmentInfo;			//设备信息（类型和版本）
	uint8_t	ObuStatus;					//OBU状态：IC卡、OBU是否锁定、OBU是否拆动、OBU电池是否正常
	uint8_t	EquipmentStatus;		//拆卸状态
}AID1LLObuVstStruct;//Obu 返回Vst信息

typedef struct
{
	uint8_t	MacAddr[4];					//01 02 03 04:OBU/CPC MAC   MacAddr[0]:00~FF=>OBU  A0~FF=>CPC
	uint8_t	MacControl;					//C0
	uint8_t	LLCControl;					//03
	uint8_t	SegmentHead;				//91
	uint8_t	VstFlag;						//D0
	uint8_t	Channel;						//00:信道号
	uint8_t	ApplicationList;		//01:应用
	uint8_t	DsrcDidAid;					//C1/C2：C1：OBU回复 C2：CPC回复
	uint8_t	DsrcDid;						//01
	
	uint8_t	InfoPara;						//B8
	uint8_t	SysInfoLength;			//1A
	uint8_t	SysInfoData[26];		//OBU/CPC中SysInfoFile文件内容前26字节
	uint8_t	Op3Length;					//29			//EF01数据长度
	uint8_t	Op3InfoData[41];		//41Data	//EF01文件信息
	uint8_t	Op4Length;					//07			//EF01数据长度
	uint8_t	Op4InfoData0[7];		//7Data	  //0019文件信息
	uint8_t	Random8[8];					//随机数由卡片生成用于对天线的认证
	uint8_t	EquipmentInfo;			//设备信息（类型和版本）
	uint8_t	CpcStatus0;					//Cpc状态：电池电量
	uint8_t	CpcStatus1;					//拆卸状态
}AID1LLCpcVstStruct;//Cpc 返回Vst信息

////////////////////////////////////OBU链路标识GetSecture数据格式////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU地址
	uint8_t	MacControl;				//40：Mac控制域；下行、存在LPDU、专用链路
	uint8_t	LLCControl;				//77：LLC控制域；类型3 包含反转位
	uint8_t	SegmentHead;			//91：段字头；PDU=0010
	uint8_t	ActionRq;					//05：Action.req
	uint8_t	DsrcDid;					//01:	DID
	uint8_t	ActionType;				//00：Getseure
	uint8_t	Contain;					//14：容器类型20
	uint8_t	Fill;							//80：keyedForEncrypt参数存在
	uint8_t	FID;							//01：ETC应用车辆信息文件
	uint8_t Offset1;					//00 00：操作数据起始位置
	uint8_t	Offset2;
	uint8_t	Length;						//10：操作数据长度length
	uint8_t	Random[8];				//F1 F2 F3 F4 F5 F6 F7 F8：Rsu产生随机数
	uint8_t	AuthenKeyId;			//00：信息鉴别密钥的密钥标识
	uint8_t	EncryptKeyId;			//00:	加密密钥的密钥标识
}AID1LLGetSectureRqStruct;//GetSecture请求
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
}AID1LLGetSectureRsStruct;//GetSecture返回
////////////////////////////////////OBU链路标识透传数据格式（2条透传数据）////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU地址
	uint8_t	MacControl;				//40：Mac控制域；下行、存在LPDU、专用链路
	uint8_t	LLCControl;				//F7：LLC控制域；类型3 包含反转位
	
	uint8_t	SegmentHead1;			//91：段字头；PDU=0010
	uint8_t	ActionRq1;				//05：Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ActionType1;			//03：TransferChannel
	uint8_t	ChannelRq1;				//18：容器类型18
	uint8_t	ChannelID1;				//02：Esam通道0x02
	uint8_t	APDUList1;				//01：APDU指令的数量，值取1
	uint8_t	L1Apdu1Len;				//05：指令1的长度，取值0x05
	uint8_t	L1Apdu1Data[5];		//00 b0 84 00 09
	
	uint8_t	SegmentHead2;			//91：段字头；PDU=0010
	uint8_t	ActionRq2;				//05：Action.req
	uint8_t	DsrcDid2;					//01:	DID
	uint8_t	ActionType2;			//03：TransferChannel
	uint8_t	ChannelRq2;				//18：容器类型18
	uint8_t	ChannelID2;				//01：Icc通道0x01
	uint8_t	APDUList2;				//02：APDU指令的数量，值取2
	uint8_t	L2Apdu1Len;				//05：指令2的长度，取值0x05
	uint8_t	L2Apdu1Data[5];		//00 b0 88 00 03
	uint8_t	L2Apdu2Len;				//05：指令2的长度，取值0x05
	uint8_t	L2Apdu2Data[5];		//00 84 00 00 08
}AID1LLObuTrans1RqStruct;	//Obu链路标识第一条透传命令
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU地址
	uint8_t	MacControl;				//E0：Mac控制域；下行、存在LPDU、专用链路
	uint8_t	LLCControl;				//77：LLC控制域；类型3 包含反转位
	uint8_t	LLCState;					//00
	
	uint8_t	SegmentHead1;			//91：段字头；PDU=0010
	uint8_t	ActionRs1;				//18：Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ChannelRs1;				//19：容器类型18
	uint8_t	ChannelID1;				//02：Esam通道0x02
	uint8_t	APDUList1;				//01：APDU指令的数量，值取1
	uint8_t	L1Apdu1Len;				//0B：数据1的长度，取值0x0B
	uint8_t	L1Apdu1Data[9];		//9data:EF04内容
	uint16_t	L1SW1;					//状态字：9000表示OK
	uint8_t	L1ResturnStatus;	//00:00表示执行成功
	
	uint8_t	SegmentHead2;			//91：段字头；PDU=0010
	uint8_t	ActionRs2;				//18：Action.req
	uint8_t	DsrcDid2;					//01:	DID
	uint8_t	ChannelRs2;				//19：容器类型18
	uint8_t	ChannelID2;				//01：Icc通道0x01
	uint8_t	APDUList2;				//02：APDU数据的数量，值取2
	uint8_t	L2Apdu1Len;				//05：数据2的长度，取值0x05
	uint8_t	L2Apdu1Data[3];		//3data:0008内容
	uint16_t	L2SW1;					//状态字：9000表示OK
	uint8_t	L2Apdu2Len;				//0a：数据3的长度，取值0x0a
	uint8_t	L2Apdu2Data[8];		//8data:8字节随机数
	uint16_t	L2SW2;					//状态字：9000表示OK
	uint8_t	L2ResturnStatus;	//00:00表示执行成功
}AID1LLObuTrans1RsStruct;	//Obu链路标识第一条透传响应

typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU地址
	uint8_t	MacControl;				//40：Mac控制域；下行、存在LPDU、专用链路
	uint8_t	LLCControl;				//77：LLC控制域；类型3 包含反转位
	
	uint8_t	SegmentHead1;			//91：段字头；PDU=0010
	uint8_t	ActionRq1;				//05：Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ActionType1;			//03：TransferChannel
	uint8_t	ChannelRq1;				//18：容器类型18
	uint8_t	ChannelID1;				//02：Esam通道0x02
	uint8_t	APDUList1;				//02：APDU指令的数量，值取1
	uint8_t	L1Apdu1Len;				//0E：指令1的长度，取值0x0E
	uint8_t	L1Apdu1Data[14];	//00 D6 84 00 09 + EF04前9字节内容
	uint8_t	L1Apdu2Len;				//0B：指令2的长度，取值0x0B
	uint8_t	L1Apdu2Data[11];	//00 D6 84 xx 09 + 6字节（路径信息2字节，时间戳4字节，UNIX时间）//xx为偏移量
	
	uint8_t	SegmentHead2;			//91：段字头；PDU=0010
	uint8_t	ActionRq2;				//05：Action.req
	uint8_t	DsrcDid2;					//01:	DID
	uint8_t	ActionType2;			//03：TransferChannel
	uint8_t	ChannelRq2;				//18：容器类型18
	uint8_t	ChannelID2;				//01：Icc通道0x01
	uint8_t	APDUList2;				//03：APDU指令的数量，值取3
	uint8_t	L2Apdu1Len;				//0D：指令1的长度，取值0x0D
	uint8_t	L2Apdu1Data[13];	//00 82 00 01 08 + 8字节认证数据（外部认证）
	uint8_t	L2Apdu2Len;				//08：指令2的长度，取值0x08
	uint8_t	L2Apdu2Data[8];		//00 D6 88 00 03 + 08文件前3字节
	uint8_t	L2Apdu3Len;				//07：指令3的长度，取值0x07
	uint8_t	L2Apdu3Data[7];		//00 D6 88 xx 02 + 2字节路径信息 //xx为偏移量
}AID1LLObuTrans2RqStruct;	//Obu链路标识第二条透传命令
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU地址
	uint8_t	MacControl;				//E0：Mac控制域；下行、存在LPDU、专用链路
	uint8_t	LLCControl;				//77：LLC控制域；类型3 包含反转位
	uint8_t	LLCState;					//00
	
	uint8_t	SegmentHead1;			//91：段字头；PDU=0010
	uint8_t	ActionRs1;				//18：Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ChannelRs1;				//19：容器类型18
	uint8_t	ChannelID1;				//02：Esam通道0x02
	uint8_t	APDUList1;				//02：APDU指令的数量，值取2
	uint8_t	L1Apdu1Len;				//02：数据1的长度，取值0x02
	uint16_t	L1SW1;					//状态字：9000表示OK
	uint8_t	L1Apdu2Len;				//02：数据1的长度，取值0x02
	uint16_t	L1SW2;					//状态字：9000表示OK
	uint8_t	L1ResturnStatus;	//00:00表示执行成功
	
	uint8_t	SegmentHead2;			//91：段字头；PDU=0010
	uint8_t	ActionRs2;				//18：Action.req
	uint8_t	DsrcDid2;					//01:	DID
	uint8_t	ChannelRs2;				//19：容器类型18
	uint8_t	ChannelID2;				//01：Icc通道0x01
	uint8_t	APDUList2;				//03：APDU数据的数量，值取3
	uint8_t	L2Apdu1Len;				//02：数据1的长度，取值0x02
	uint16_t	L2SW1;					//状态字：9000表示OK
	uint8_t	L2Apdu2Len;				//02：数据2的长度，取值0x02
	uint16_t	L2SW2;					//状态字：9000表示OK
	uint8_t	L2Apdu3Len;				//02：数据3的长度，取值0x02
	uint16_t	L2SW3;					//状态字：9000表示OK
	uint8_t	L2ResturnStatus;	//00:00表示执行成功
}AID1LLObuTrans2RsStruct;	//Obu链路标识第二条透传响应

////////////////////////////////////OBU链路标识SetMMI数据格式////////////////////////////
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
}AID1LLObuSetMMIRqStruct;//Obu链路标识SetMMI请求
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
}AID1LLObuSetMMIRsStruct;//Obu链路标识SetMMI返回

////////////////////////////////////CPC链路标识透传数据格式（1条透传数据）////////////////////////////
typedef struct
{
	uint8_t	MacAddr[4];				//01 02 03 04:OBU地址
	uint8_t	MacControl;				//40：Mac控制域；下行、存在LPDU、专用链路
	uint8_t	LLCControl;				//77：LLC控制域；类型3 包含反转位
	
	uint8_t	SegmentHead1;			//91：段字头；PDU=0010
	uint8_t	ActionRq1;				//05：Action.req
	uint8_t	DsrcDid1;					//01:	DID
	uint8_t	ActionType1;			//03：TransferChannel
	uint8_t	ChannelRq1;				//18：容器类型18
	uint8_t	ChannelID1;				//00：路径识别通道0x00
	uint8_t	APDUList1;				//01：APDU指令的数量，值取1
	uint8_t	L1Apdu1Len;				//13：指令1的长度，取值0x13
	uint8_t	L1Apdu1Data[16];	//天线使用SM4、外部认证密钥（外部认证根密钥经卡ID分散后得出）对随机数进行加密计算
	uint8_t	RouteInfo[2];			//路径信息
	uint8_t	SleepInterval;		//00:“建议休眠时长”用于指示CPC卡本次标识成功后，可以不再被唤醒的时长。0为不休眠，1~254为可休眠1~254min。255为无需再唤醒。
}AID1LLCpcTransRqStruct;	//Cpc链路标识透传命令
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
}AID1LLCpcTransRsStruct;	//Cpc链路标识透传响应

////////////////////////////////////OBU、CPC链路标识EvenReport数据格式////////////////////////////
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

