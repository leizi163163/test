#ifndef __DSRC_H
#define __DSRC_H


#include "stm32f10x.h"

//extern uint32_t CurrentBST_UnixTime,CurrentOBU_TrampTime;

//Dsrc 链路层	Start



extern uint8_t EventReportFlag;

typedef enum
{
	BstType=0,
	GetSectureType,
	TransferCharnnelType,
	SetMMIType,
	EvenReportType
}DsrcType;

//Dsrc AppParameter
typedef enum
{
	DsrcSecussed=0,
	DsrcError,
	PsamErr,
	DsrcNull
}DsrcStatus;

typedef	enum
{
	DsrcRetransCount=70,		//BST和Transfer重发次数
	DsrcOverTime=10				//BST和Transfer接收超时
}DsrcTimeControl;

typedef	struct
{
	uint8_t	DsrcNFlag;
	uint8_t	Type;//BST or	Transfer
	uint8_t	TxDataLen;//发送数据长度
	uint8_t	RxDataLen;//接收数据长度
	uint8_t	RetransCount;//重复发送次数
	uint8_t	Count;//Dsrc接收到回复时的次数
	uint8_t	OverTime;//接收超时时间
	uint8_t	TxData[128];
	uint8_t	RxData[128];
}DsrcDataStruct;

#endif

