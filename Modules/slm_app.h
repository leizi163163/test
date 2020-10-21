#ifndef  __SLM_APP_H
#define  __SLM_APP_H

#include    "slm_type.h"

typedef unsigned int  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
extern void SLTXCheckRx(void);
extern void SLRXCheckRx(void);
extern void SLTXauto_carrier_en(u8 auto_en);
extern void SLRXauto_carrier_en(u8 auto_en);

extern u8   bst[28];
extern u8 	SLDataBuf[128];
extern u8 	SLDataBufLen;
extern u8		SLStatus;

extern u8 BstFunc(	u8 *bst_buf,u8 bst_len,u8 *bst_revbuf,u8 *bst_revlen,u8 bst_counter,u8 bst_overtime);

#endif
