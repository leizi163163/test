#ifndef	__ANT_Config_H
#define	__ANT_Config_H

#include "stm32f10x.h"


#define	RedLedL			GPIOC->BSRR=GPIO_Pin_0
#define	RedLedH			GPIOC->BRR=GPIO_Pin_0

#define	GreLedL			GPIOC->BSRR=GPIO_Pin_6
#define	GreLedH			GPIOC->BRR=GPIO_Pin_6



//#define	Standard_ETC						1


//OBU厂商代码
#define	GZ_Artc								0x01				//广州埃特斯
#define	SZ_JINYI							0x02				//深圳金溢
#define	BJ_JULI								0x03				//北京聚利
#define	SH_DONGHAI							0x05				//上海东海
#define	BJ_WANJI							0x08				//北京万集
#define	SZ_CHENGGU							0x12				//深圳成谷


#define	X_CENTOR							32
#define	X_LEFT								15
#define	X_RIGHT								49

//天线模式配置
#define	ANT_MODE_STANDBY					0x00				//待机模式
#define	ANT_MODE_CARRIER					0x01				//载波模式
#define	ANT_MODE_BER						0x02				//BER模式
#define	ANT_MODE_ZERO						0x03				//全零码模式
#define	ANT_MODE_SEND_BST					0x04				//发送BST模式
#define	ANT_MODE_DSRC						0x05				//透传模式
#define	ANT_MODE_MARKER						0x06				//标识模式
#define	ANT_MODE_PN9							0x07				//发射PN9码

//标识模式下的4种标识模式
#define	MARKER_AID1GB						0x01				//AID = 1下的广播标识
#define	MARKER_AID1LL						0x02				//AID = 1下的链路标识
#define	MARKER_AID2GB						0x03				//AID = 2下的广播标识
#define	MARKER_AID2LL						0x04				//AID = 2下的链路标识


#define SYN_OUT_STATUS_IDLE()			GPIO_SetBits(GPIOG,GPIO_Pin_7)				//SYN_OUT	1闲
#define SYN_OUT_STATUS_BUSY()			GPIO_ResetBits(GPIOG,GPIO_Pin_7)			//SYN_OUT	0忙

#define SYN_IN_STATUS()					GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9)		//读G9管脚状态	0	禁止工作	1	开始工作

#define USER_Min(a,b)		((a > b) ? b : a)
#define USER_Max(a,b)		((a > b) ? a : b)

#define SUCC_MAX	5

typedef enum
{ 
	RunOK = 0x00,
	RunError,
}ANT_Run_Status;

typedef enum
{
	BstType=0,
	GetSectureType,
	TransferCharnnelType,
	SetMMIType,
	EvenReportType
}DsrcType;

typedef enum
{
	DsrcSecussed=0,
	DsrcError,
	PsamErr,
	DsrcNull
}DsrcStatus;

typedef enum
{ 
	RSU_IDLE = 0,
	RSU_B2 = 1,
	RSU_B3 = 2,
	RSU_B4 = 3,
	RSU_B5 = 4,
	RSU_B6 = 5,	
	RSU_B7 = 6,
	RSU_B2_C1 = 7,
	RSU_B3_C1 = 8,
	RSU_B4_C6 = 9,
	RSU_B5_C1 = 10,
	RSU_BA_C1 = 11,
	RSU_BA = 12,
	RSU_B5_C9 = 13,
	RSU_B9 = 14,
	RSU_B9_C1 = 15,
	
	RSU_B0_C1 = 20,
	RSU_BF_C1 = 21,
	
	RSU_B6_ERR = 80,
}RSU_MODE;

typedef enum
{ 
	ETC_ENTRY = 3,
	ETC_EXIT = 4,
	ETC_OPEN = 6,
	MARK_STATION = 7,
	PROVIN_STATION = 8,
}LAND_MODE;


typedef struct
{
	uint8_t Header[9];
	uint8_t Marker[100];
	uint8_t UnixTime[200];
}EF04_RoadInfoDef;


typedef struct
{
	uint8_t ObuId[4];
	uint8_t UnixTime[4];
}SuccessObuInfoDef;

//typedef struct
//{
//	uint8_t Header[3];
//	uint8_t Marker[100];
//}ICC0008_RoadInfoDef;


extern uint8_t AntSwitchState;
extern uint8_t AntSwitchCloseFlag;

extern uint8_t RSU_CurrentMode;
extern uint8_t B3_C1Flag;

extern uint8_t CurrentObuId[4];
//extern uint8_t SuccessObuId[5][4];
extern uint8_t SuccIndex;
extern uint8_t BstFlag;
extern uint8_t chanCmd;
extern uint8_t RsuIdFlag;

extern uint8_t HeartFlag;
//extern uint8_t ExAuthFlag;
extern uint8_t SL1102_RSSI;

extern uint8_t GetSecureFlag;

//extern uint8_t EnterB6_Count;

extern EF04_RoadInfoDef EF04_RoadInfoStructure;



extern uint8_t BK5824_Power;


extern SuccessObuInfoDef SuccessObuInfoStructure[SUCC_MAX];



void TX_CtrlOpen(void);
void TX_CtrlClose(void);


void RSU_SendHeartFrame(void);
void RSU_SendB2Frame(void);
void RSU_SendBAFrame(void);
void RSU_SendB4Frame(void);
void RSU_SendB6Frame(void);
void RSU_SendB5Frame(void);
	
uint8_t RSU_WorkMode(uint8_t current_mode);


void RSU_MarkModeFunction(void);

void RSU_SynchronizerPinInit(void);

void ClearSuccessObuId(void);

#endif


