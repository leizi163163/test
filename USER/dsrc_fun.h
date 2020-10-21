#ifndef	__DSRC_FUN_H
#define	__DSRC_FUN_H

#include "stm32f10x.h"

#define		DSRC_B6_COUNT		50
#define		DSRC_B5_COUNT		50



typedef struct
{
	unsigned char MacAdrr[4];
	unsigned char Player[9];
	unsigned char EsamSysInfoContainer;
	unsigned char EsamSysInfoIssueFlag[8];
	unsigned char EsamSysInfoType;
	unsigned char EsamSysInfoVer;
	unsigned char EsamSysInfoSN[8];
	unsigned char EsamSysInfoStartTime[4];
	unsigned char EsamSysInfoEndTime[4];
	unsigned char EsamSysInfoObuStatus;
	unsigned char EsamSysInfoRet[10];
	unsigned char IccSysInfoContainer;
	unsigned char IccSysInfoIssueFlag[8];
	unsigned char IccSysInfoType;
	unsigned char IccSysInfoVer;
	unsigned char IccSysInfoNetNum[2];
	unsigned char IccSysInfoCardSN[8];
	unsigned char IccSysInfoStartTime[4];
	unsigned char IccSysInfoEndTime[4];
	unsigned char IccSysInfoCarNum[12];
	unsigned char IccSysInfoUserType;
	unsigned char IccSysInfoFCI[2];
	unsigned char ObuID[4];
	unsigned char EquipmentInfo;
	unsigned char ObuSatus;
	unsigned char EquipmentStatus;
}STRUCT_OBU_VST_Info;

//typedef struct
//{
//	unsigned char OBUID[4];
//	unsigned char ICC_Player[8];
//	unsigned char ICC_Apdu_Num;
//	unsigned char ICC_Money[4];
//	unsigned char ICC_MoneySw[2];
//	unsigned char Esam_Player[8];
//	unsigned char Esam_Apdu_num;
//	unsigned char Esam_Df01Info[60];
//}STRUCT_Transfer_IccMoney_EsamDf01_rs;

//日志中数据
//08848B9D E0F7009118011901 01 06 00897D389000 00 9118011902 01 11 6F0D8409A00000000386980701A5009000 00
typedef struct
{
	unsigned char OBUID[4];
	unsigned char ICC_Player[8];		//E0F7009118011901
	unsigned char ICC_Apdu_Num;			//01
	unsigned char ICC_MoneyLen;			//06
	unsigned char ICC_Money[4];			//00897D38
	unsigned char ICC_MoneySw[2];		//9000
	unsigned char ICC_ReturnStatus;		//00
	unsigned char Esam_Player[5];		//9118011902
	unsigned char Esam_Apdu_num;			//02	选择DF01和EF04
	unsigned char Esam_DF01_EF04Info[60];	//len1+data1+len2+data2
}STRUCT_Transfer_IccMoney_EsamDF01_EF04_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char SelectEF04_Len;
	unsigned char SelectReadEF04_Data[114];
}STRUCT_Transfer_SelectReadEsamEF04_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ReadEF04Header_Len;
	unsigned char ReadEF04Data[98];
//	unsigned char Obu_Ret;
}STRUCT_Transfer_ESAM_ReadEF04_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ReadEF04_Len;
	unsigned char ReadWriteEF04Data[98];
//	unsigned char Obu_Ret;
}STRUCT_Transfer_ESAM_ReadWriteEF04_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ReadEF04Header_Len;
	unsigned char ReadEF04Header_Beacon15[20];
	unsigned char ReadEF04Headersw[2];
	unsigned char ReadEF04Ender_Len;
	unsigned char ReadEF04Ender_Beacon15[40];
	unsigned char ReadEF04Endersw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ESAM_ReadEF04_EX_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ICC0008_Len;
	unsigned char ICC0008[35];						//最大支持37个标识点,与EF04路径信息使用短文件标识符匹配		37 * 2 + 3
	unsigned char ICC0008sw[2];
	unsigned char Rand8_Len;
	unsigned char Rand8[8];
	unsigned char Rand8sw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_Read0008_Rand8_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ExAuth_Len;
	unsigned char ExAuthsw[2];
	unsigned char Write0008_Header_Len;
	unsigned char Write0008_Headersw[2];
	unsigned char Write0008_Len;
	unsigned char Write0008sw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_ExAuth_Write0008_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ExAuth_Len;
	unsigned char ExAuthsw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_ExAuth_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char Write0008_Header_Len;
	unsigned char Write0008_Headersw[2];
	unsigned char Write0008_Len;
	unsigned char Write0008sw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_Write0008_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ICC_MoneyLen;			//06
	unsigned char ICC_Money[4];			//00897D38
	unsigned char ICC_MoneySw[2];		//9000
	unsigned char ICC_0019_Len;
	unsigned char ICC_0019[43];
	unsigned char ICC_0019sw[2];
	unsigned char ICC_0015_Len;
	unsigned char ICC_0015[7];
	unsigned char ICC_0015sw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_Money_Read0019_0015_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ICC_MoneyLen;			//06
	unsigned char ICC_Money[4];			//00897D38
	unsigned char ICC_MoneySw[2];		//9000
	unsigned char ICC_0019_Len;
	unsigned char ICC_0019[43];
	unsigned char ICC_0019sw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_Money_Read0019_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ICC_0019_Len;
	unsigned char ICC_0019[43];
	unsigned char ICC_0019sw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_Read0019_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char ICC_000C_Len;
	unsigned char ICC_000C[64];
	unsigned char ICC_000Csw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_Read000C_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char GetTAC_Len;
	unsigned char GetTAC_Mac[4];
	unsigned char GetTAC_Tac[4];
	unsigned char GetTACsw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_GetTAC_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char CappPurchase_Len;
	unsigned char CappPurchase_Tac[4];
	unsigned char CappPurchase_Mac2[4];
	unsigned char CappPurchasesw[2];
	unsigned char GetMoney_Len;
	unsigned char GetMoney_Money[4];
	unsigned char GetMoneysw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_CappPurchase_GetMoney_rs;

typedef struct
{
	unsigned char OBUID[4];
	unsigned char Player[8];
	unsigned char Apdu_Num;
	unsigned char CappInit_Len;
	unsigned char CappInit_Money[4];
	unsigned char CappInit_PurchaseSN[2];
	unsigned char CappInit_PurchaseLimit[3];
	unsigned char CappInit_KeyVer;
	unsigned char CappInit_KeyDesFlag;
	unsigned char CappInit_Rand4[4];
	unsigned char CappInitsw[2];
	unsigned char UpdataCapp_Len;
	unsigned char UpdataCappsw[2];
	unsigned char Obu_Ret;
}STRUCT_Transfer_ICC_CappPurchaseInit_rs;

typedef struct
{
	unsigned char CappMac1_EquimPurseSN[4];
	unsigned char CappMac1_Mac1[4];
	unsigned char CappMac1_sw[2];
}STRUCT_PSAM_ICC_CappPurseMac1_rs;



typedef struct
{
	unsigned char CarInfo[88];
	unsigned char sw[2];
}STRUCT_PSAM_CarDedes_rs;

typedef struct
{
	unsigned char CarInfo[32];
	unsigned char sw[2];
}STRUCT_PSAM_CarDedes32_rs;



extern unsigned char OBU_BST_Info[];
extern STRUCT_OBU_VST_Info  OBU_VST_Info;
extern STRUCT_Transfer_IccMoney_EsamDF01_EF04_rs	Transfer_IccMoney_EsamDF01_EF04_rs;
extern STRUCT_Transfer_ESAM_ReadEF04_rs	Transfer_ESAM_ReadEF04_rs;
extern STRUCT_Transfer_ESAM_ReadWriteEF04_rs	Transfer_ESAM_ReadWriteEF04_rs;
extern STRUCT_Transfer_SelectReadEsamEF04_rs Transfer_SelectReadEsamEF04_rs;
extern STRUCT_Transfer_ESAM_ReadEF04_EX_rs	Transfer_ESAM_ReadEF04_EX_rs;
extern STRUCT_Transfer_ICC_Read0008_Rand8_rs	Transfer_ICC_Read0008_Rand8_rs;
//extern STRUCT_Transfer_ICC_ExAuth_Write0008_rs	Transfer_ICC_ExAuth_Write0008_rs;
extern STRUCT_Transfer_ICC_Money_Read0019_0015_rs Transfer_ICC_Money_Read0019_0015_rs;
extern STRUCT_Transfer_ICC_Money_Read0019_rs Transfer_ICC_Money_Read0019_rs;
//extern STRUCT_Transfer_ICC_Read0019_rs Transfer_ICC_Read0019_rs;
//extern STRUCT_Transfer_ICC_Read000C_rs Transfer_ICC_Read000C_rs;

extern STRUCT_Transfer_ICC_CappPurchase_GetMoney_rs	Transfer_ICC_CappPurchase_GetMoney_rs;
extern STRUCT_Transfer_ICC_CappPurchaseInit_rs	Transfer_ICC_CappPurchaseInit_rs;
extern STRUCT_PSAM_ICC_CappPurseMac1_rs	PSAM_ICC_CappPurseMac1_rs;
extern STRUCT_Transfer_ICC_GetTAC_rs	Transfer_ICC_GetTAC_rs;


extern STRUCT_PSAM_CarDedes_rs  PSAM_CarDedes_rs;
extern STRUCT_PSAM_CarDedes32_rs  PSAM_CarDedes32_rs;

extern unsigned char Dsrc_NFlag;
extern unsigned char DsrcRxLen;
extern unsigned char Dsrc_NFlag;
extern unsigned char DsrcOverTime_0;




void Dsrc_Procol_Test_func(void);
uint8_t Dsrc_Trans_Func(unsigned char DsrcBstFlag,unsigned char *txBuff,unsigned char txLen,unsigned char *rxBuff,unsigned char rxLen,unsigned char ReCount,unsigned char *Count,unsigned char OverTime);

uint8_t SelectReadEsamEF04_func(void);
uint8_t ESAM_ReadEF04_func(void);
uint8_t ESAM_ReadWriteEF04_func(void);
uint8_t ESAM_WriteEF04_func(uint8_t write_p1,uint8_t write_p2,uint8_t *data_buf,uint8_t write_len);
uint8_t ESAM_SelectWriteEF04_func(uint8_t write_p1,uint8_t write_p2,uint8_t *data_buf,uint8_t write_len);
uint8_t ESAM_WriteEF04_406_func(void);
uint8_t ESAM_ClearEF04_func(void);
uint8_t ICC_CappPurchase_func(void);
uint8_t ICC_CappPurchaseInit_func(void);
//uint8_t ICC_ExAuth_Write0008_func(void);
uint8_t ICC_ExAuth_func(void);
uint8_t ICC_Write0008_func(void);
uint8_t ICC_ExAuth_Clear0008_func(void);
uint8_t ICC_Money_Read0019_0015_func(void);
uint8_t ICC_Money_Read0019_func(void);
//uint8_t ICC_Read0019_func(void);
//uint8_t ICC_Read000C_func(void);
uint8_t ICC_Read0008_Rand8_func(void);
uint8_t SelectEsamDF01_EF04_func(void);
uint8_t SelectEsamEF04_func(void);
void DSRC_Waiting_func(void);
uint8_t ICCMoney_EsamDF01_EF04_func(void);
uint8_t OBU_Bst_func(void);
uint8_t OBU_GetSecure_func(void);
uint8_t PSAM_CarDedes_func(void);
uint8_t ICC_GetTac_func(void);
uint8_t PSAM_ICC_CappPurseMac1_func(void);
uint8_t PSAM_CheckMac2_func(void);
//uint8_t SC_ICC_WriteBin_func(void);
uint8_t SetMMI_func(void);
uint8_t EventReport_func(void);
uint8_t SuccessEventReport_func(uint8_t *obuid);


#endif

