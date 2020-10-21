#ifndef __PsamApp_H
#define __PsamApp_H

#include "stm32f10x.h"

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

//Operate Psam Cos declaration
#define CosRspOK	0x0090
#define	CosRspErr	0x9669
typedef struct
{
	uint8_t	num;
	uint8_t	CmdLen;
	uint8_t	Cmd[262];
	uint8_t	DataLen;
	uint8_t	Data[257];
	union
	{
		uint16_t	result;
		uint8_t	SW[2];
	}CosSw;
}ApduStruct;

typedef	enum
{
	CosStateOK=0,
	CosStateErr,
	Select3F00Len=7,
	SelectDF01Len=7,
	Read0017Len=5,
	Read0018Len=5,
	ReadGBTerminalIDLen=5,
	GetRandom8Len=5,
	InitForDescryptLen=21,
	SwLen=2,
	CmdHeadLen=5,
	CmdRevSWLen=2
}ApduLenEnum;


typedef	struct
{
	uint8_t	PsamNo[10];
	uint8_t	PsamVersion;
	uint8_t	PsamType;
	uint8_t	PsamUserData[2];
}Psam0015Struct;


extern	uint8_t	Select3F00[Select3F00Len];
extern	uint8_t	Select3F01[Select3F00Len];
extern	uint8_t	SelectDF01[SelectDF01Len];
extern	uint8_t	Read0017[Read0017Len];
extern	uint8_t	Read0018[Read0018Len];
extern	uint8_t	ReadPsam0015[5];
extern	uint8_t	ReadGBTerminalID[ReadGBTerminalIDLen];
extern	uint8_t	GetRandom8[GetRandom8Len];
extern	ApduStruct	Psam2;


extern	Psam0015Struct		Psam0015Structure[2];
extern	uint8_t PSAM_SerialNum[2][6];
extern	uint8_t PSAM_AreaCode[8];
extern	uint8_t PSAM_RandData8[8];
extern	uint8_t PSAM_ExternalAuth[13];

uint8_t	PsamSelectDirc(uint8_t	*Dirc,uint8_t	Len);

void	PsamParameterInit(void);
uint8_t	PSAMReset(ApduStruct*	Psam	);
uint8_t	PSAMTransferAPDU(ApduStruct*	Psam	);


uint8_t USER_ReadPSAM(uint8_t psam_num);
uint8_t USER_ReadPsamAreaCode(uint8_t psam_num);
uint8_t USER_ReadPsamRand8(uint8_t psam_num);



#endif


