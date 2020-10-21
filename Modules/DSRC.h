#ifndef __DSRC_H
#define __DSRC_H


#include "stm32f10x.h"

//extern uint32_t CurrentBST_UnixTime,CurrentOBU_TrampTime;

//Dsrc ��·��	Start



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
	DsrcRetransCount=70,		//BST��Transfer�ط�����
	DsrcOverTime=10				//BST��Transfer���ճ�ʱ
}DsrcTimeControl;

typedef	struct
{
	uint8_t	DsrcNFlag;
	uint8_t	Type;//BST or	Transfer
	uint8_t	TxDataLen;//�������ݳ���
	uint8_t	RxDataLen;//�������ݳ���
	uint8_t	RetransCount;//�ظ����ʹ���
	uint8_t	Count;//Dsrc���յ��ظ�ʱ�Ĵ���
	uint8_t	OverTime;//���ճ�ʱʱ��
	uint8_t	TxData[128];
	uint8_t	RxData[128];
}DsrcDataStruct;

#endif

