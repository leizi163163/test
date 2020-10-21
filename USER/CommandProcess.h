#ifndef	__CommandProcess_H
#define	__CommandProcess_H

#include "stm32f10x.h"

#define	PC_INIT_RQ						0xC0				//C0--��ʼ��ָ��
#define	PC_CONTINUE_RQ					0xC1				//C1--��������ָ��
#define	PC_STOP_RQ						0xC2				//C2--ֹͣ����ָ��
#define	PC_WriteEnter_RQ				0xC3				//C3--д���ָ��
#define	PC_PURCHASE_RQ					0xC6				//C6--���ѽ���ָ��
#define	PC_PASS_MONEY_RQ				0xC9				//C9--��ȡ��ʡͨ�н��ָ��
#define	PC_SWITCH_RQ					0x4C				//C4--RSU����ָ��
#define	PC_GET_TAC_RQ					0xC8				//C8--��ȡTACָ��
#define	PC_IP_PORT_RQ					0xC8				//C8--���û�ȡIPָ���ǰ��������IP
#define	PC_SET_RSU_ID_RQ				0xAB				//AB--��������ID�ɱ��̶�ָ��
#define	PC_PSAM_AUTH_INIT_RQ			0xCD				//CD--PSAM��Ȩ��ʼ��ָ��
#define	PC_PSAM_AUTH_RQ					0xCF				//CF--PSAM��Ȩָ��

#define	PC_SET_PARA_RQ					0xD8				//D8--����RSU��������ʱ�Ĳ���
#define	PC_READ_PARA_RQ					0xD9				//D9--��ȡRSU��������ʱ�Ĳ���


#define	PC_SEND_BST_RQ					0xD1				//D1--ָʾ���߷���BST
#define	PC_EVENT_REPORT_RQ				0xD2				//D2--ָʾ���߷���BST




#define	RSU_STATUS_RS					0xB0				//B0--�豸״̬��Ϣ֡
#define	RSU_OBU_INFO_RS					0xB2				//B2--���ص�Ԫ��Ϣ֡
#define	RSU_CAR_INFO_RS					0xB3				//B3--������Ϣ֡
#define	RSU_ICC_INFO_RS					0xB4				//B4--�û���Ϣ֡
#define	RSU_TRANS_INFO_RS				0xB5				//B5--������Ϣ֡
#define	PC_PASS_MONEY_RS				0xB9				//B9--��ʡͨ�н����Ϣ֡


#define	RSU_IP_PORT_RS					0xB8				//B8--����IP �˿�֡
#define	RSU_ROAD_INFO_RS				0xBA				//BA--��ȡ��·���ļ���Ϣ֡
#define	RSU_PSAM_AUTH_INIT_RS			0xBD				//BD--��Ȩ��ʼ����Ϣ֡
#define	RSU_PSAM_AUTH_RS				0xBF				//BF--��Ȩ��Ϣ֡


#define	PSAM_SLOT_0							0					//PSAM����0
#define	PSAM_SLOT_1							1					//PSAM����1
#define	PSAM_SLOT_2							2					//PSAM����2
#define	PSAM_SLOT_3							3					//PSAM����3
#define	PSAM_SLOT_4							4					//PSAM����4

#define	ANT_INFO_LEN						8					//RSU�豸��Ϣ����

typedef enum
{ 
	Data_OK   = 0x00,
	Data_ERROR,
	STX_Error,
	DataLen_Error,
	Data_CMD_Error,
	BCC_Error, 
}DataPackage_Status;

typedef enum
{ 
	DATA_FRAME_LEN = 6,
	RSU_STATUS_LEN = 15,
	RSU_OBU_INFO_LEN = 35,
	RSU_TRANS_INFO_LEN = 34,
	RSU_ICC_INFO_LEN = 0,
	RSU_MARK_LEN = 115,
	RSU_STATE_LEN = 0, 
}RSU_DATA_LEN;

typedef enum
{ 
	RE_FIND_OBU = 1,
	PC_RECEIVED = 7,
	EVENT_REPORT = 8,
	C2_INIT_FLAG = 9,
}STOP_TYPE;

typedef struct
{ 
	uint8_t RSCTL;
	uint8_t CMD_Type;			//C0--��ʼ��ָ��
	uint8_t UnixTime[4];		//��λ��ǰ
	uint8_t Datetime[7];
	uint8_t LaneMode;
	uint8_t BSTInterval;
	uint8_t TxPower;
	uint8_t PLLChannelID;
//	uint8_t TransMode;
//	uint8_t ICK04file[5];
//	uint8_t PsamAuth;
	uint8_t EF04_Opt;
	uint8_t EF04_Off[2];
	uint8_t EF04_Len[2];
	uint8_t PC_BCC;
}PC_InitDef;

typedef struct
{
	uint8_t CmdType;			//D1--ָʾ���߷���BST
	uint8_t ObuId[4];
	uint8_t ObuDivFactor[8];
}PC_SendBstDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t CmdType;			//C1--��������ָ��
	uint8_t ObuId[4];
//	uint8_t ObuDivFactor[8];
	uint8_t PC_BCC;
}PC_ContinueDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t CmdType;			//C2--ֹͣ����ָ��
	uint8_t ObuId[4];
	uint8_t StopType;
	uint8_t PC_BCC;
}PC_StopDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t CmdType;			//4C--RSU����ָ��
	uint8_t ControlType;
	uint8_t PC_BCC;
}PC_SwitchDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t CmdType;			//C6--���ѽ���ָ��
	uint8_t ObuId[4];
//	uint8_t ObuDivFactor[8];
	uint8_t TradeMode;
	uint8_t ConsumeMoney[4];
	uint8_t Station[40];
	uint8_t PurchaseTime[7];	
	uint8_t EF04_Off[2];
	uint8_t EF04_Len[2];
	uint8_t Ef04Info[98];
//	uint8_t PC_BCC;
}PC_PurchaseDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t CmdType;			//C9--��ȡ��ʡͨ�н��ָ��
	uint8_t ObuId[4];
	uint8_t EF04_Off[2];
	uint8_t EF04_Len[2];
	uint8_t PC_BCC;
}PC_PassMoneyDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t CmdType;			//C8--��ȡTACָ��
	uint8_t ObuId[4];
	uint8_t OptMode;
	uint8_t EF04_Off[2];
	uint8_t EF04_Len[2];
	uint8_t Ef04Info[98];
}PC_GetTACDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t CmdType;			//CD--PSAM��Ȩ��ʼ��ָ��
//	uint8_t PsamCnt;
	uint8_t PsamChannel;
	uint8_t PC_BCC;
}PC_PsamAuthInitDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t CmdType;			//CF--PSAM��Ȩָ��
//	uint8_t PsamCnt;
	uint8_t PsamChannelId;
	uint8_t PsamMacCode[8];
	uint8_t PC_BCC;
}PC_PsamAuthDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t CmdType;			//AB--����ID����ָ��
	uint8_t RsuID;
	uint8_t PC_BCC;
}PC_SetRsuIdDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t FrameType;			//B0--�豸״̬��Ϣ֡
	uint8_t RsuStatus;
//	uint8_t PsamCnt;			//PSAM������
	uint8_t PsamChannelId;
	uint8_t PsamVer;
	uint8_t PsamAuthStatus;
	uint8_t PsamTerminal[6];
	uint8_t RsuAlgId;
	uint8_t RsuManuId;
	uint8_t RsuId[3];
	uint8_t RsuVersion[2];
//	uint8_t EF04_OptStatus;
	uint8_t Reserved[5];
//	uint8_t RSU_BCC;
}RSU_StatusDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t FrameType;			//B2--���ص�Ԫ��Ϣ֡
	uint8_t ObuId[4];
	uint8_t ErrorCode;
	uint8_t Issuer[8];
	uint8_t ContractType;
	uint8_t ContractVersion;
	uint8_t SerialNumber[8];
	uint8_t IssueDate[4];
	uint8_t ExpireDate[4];	
	uint8_t CPUCardID[10];
	uint8_t EquitmentCV;
	uint8_t ObuStatus[2];
//	uint8_t nMark;
//	uint8_t Data[8];
//	uint8_t RSU_BCC;
//	uint8_t SL1102_RSSI;
}RSU_ObuInfoDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t FrameType;			//B3--������Ϣ֡
	uint8_t ObuId[4];
	uint8_t ErrorCode;
	uint8_t Vehicle_info[79];
//	uint8_t RSU_BCC;
}RSU_CarInfoDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t FrameType;			//B4--CPU�û�����Ϣ֡
	uint8_t ObuId[4];
	uint8_t ErrorCode;
	uint8_t ReadIccStatus;
	uint8_t CardRestMoney[4];
	uint8_t IssuerInfo[43];
	uint8_t LastStation[43];
	uint8_t ReadEf04Status;
	uint8_t EF04_Off[2];
	uint8_t EF04_Len[2];
	uint8_t EF04_Info[98];
//	uint8_t RSU_BCC;
}RSU_IccInfoDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t FrameType;			//B5--������Ϣ֡
	uint8_t ObuId[4];
	uint8_t ErrorCode;
	uint8_t TransTime[7];
	uint8_t PsamTransSerial[4];	
	uint8_t IccPaySerial[2];
	uint8_t TransType;
	uint8_t CardBalance[4];
	uint8_t TAC[4];
	uint8_t WrFileTime[4];		//UNIX
	uint8_t KeyType;
	uint8_t EF04InfoWriteStatus;
//	uint8_t RSU_BCC;
}RSU_TransInfoDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t FrameType;			//B9--������Ϣ֡
	uint8_t ObuId[4];
	uint8_t ErrorCode;
	uint8_t EF04_Info[98];
//	uint8_t RSU_BCC;
}RSU_PassMoneyDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t FrameType;			//BD--PSAM��Ȩ��ʼ����Ϣ֡
	uint8_t ErrorCode;
//	uint8_t PsamCnt;
//	uint8_t PsamChannelId;
//	uint8_t PsamAuthStatus;
	uint8_t PsamNo[10];
	uint8_t PsamVersion;
	uint8_t PsamAreaCode[4];
	uint8_t PsamRandCode[8];
}RSU_PsamAuthInitDef;

typedef struct
{
	uint8_t RSCTL;
	uint8_t FrameType;			//BF--PSAM��Ȩ��Ϣ֡
	uint8_t ErrorCode;
//	uint8_t PsamCnt;
//	uint8_t PsamChannelId;
	uint8_t PsamAuthSW[2];
}RSU_PsamAuthDef;

typedef struct
{
	uint8_t ObuId[4];
	uint8_t ObuStatus[2];
}OBU_FalseListDef;

typedef union
{
	uint32_t UnixTime;
	uint8_t UnixTimeTab[4];
}UnixTimeDef;

extern uint8_t PSAM_ResetFlag;
extern uint8_t RSU_DataFrameCount;

extern uint8_t TouFlag;


extern PC_InitDef 				PC_InitStructure;
extern PC_ContinueDef 			PC_ContinueStructure;
extern PC_StopDef				PC_StopStructure;
extern PC_SwitchDef				PC_SwitchStructure;
extern PC_PurchaseDef			PC_PurchaseStructure;
extern PC_PassMoneyDef			PC_PassMoneyStructure;
extern PC_GetTACDef				PC_GetTACStructure;
extern PC_SetRsuIdDef			PC_SetRsuIdStructure;


extern RSU_StatusDef 			RSU_StatusStructure;
extern RSU_ObuInfoDef 			RSU_ObuInfoStructure;
extern RSU_CarInfoDef			RSU_CarInfoStructure;
extern RSU_IccInfoDef 			RSU_IccInfoStructure;
extern RSU_TransInfoDef 		RSU_TransInfoStructure;
extern RSU_PassMoneyDef			RSU_PassMoneyStructure;

extern OBU_FalseListDef			OBU_FalseListStructure[10];
extern uint8_t OBU_FalseListAddr;

extern uint8_t SendBuf[256];
extern uint8_t SendBufCount;

extern UnixTimeDef UnixTimeUnion;

void RSU_USART_SendFunction(uint8_t *buf,uint8_t buf_len);

void PC_SetParaFunction(void);
void PC_ReadParaFunction(void);
uint8_t PC_DataParse(void);
void PC_DataProcess(void);

void RSU_Frame98Fuction(void);


#endif


