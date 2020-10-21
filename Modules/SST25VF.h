#ifndef	__SST25VF_H
#define	__SST25VF_H

#include "stm32f2xx.h"

//SST25VF016B---16Mbit---2MB
//0x000000--0x1FFFFF
#define  SST25VF_CHIP_SIZE				  0x200000			//Ð¾Æ¬´æ´¢¿Õ¼ä´óÐ¡

#define SST25VF_COMMAND_WREN		      0x06				//Ð´Ê¹ÄÜ
#define SST25VF_COMMAND_WRDI		      0x04				//Ð´½ûÖ¹
#define SST25VF_COMMAND_RDSR		      0x05				//¶Á×´Ì¬¼Ä´æÆ÷
#define SST25VF_COMMAND_EWSR		      0x50				//Ê¹ÄÜ Ð´×´Ì¬¼Ä´æÆ÷
#define SST25VF_COMMAND_WRSR		      0x01				//Ð´×´Ì¬¼Ä´æÆ÷
#define SST25VF_COMMAND_READ		      0x03				//¶Á--25MHz
#define SST25VF_COMMAND_FAST_READ		  0x0B				//¶Á--50MHz
#define SST25VF_COMMAND_BYTE_PROGRAM	  0x02				//×Ö½Ú±à³Ì
#define SST25VF_COMMAND_AAI_PROGRAM		  0xAD				//AAI±à³Ì---Auto Address Increment Programming
#define SST25VF_COMMAND_RDID		      0x90				//¶ÁÐ¾Æ¬ID
#define SST25VF_COMMAND_JEDECID		      0x9F				//¶ÁÐ¾Æ¬JEDEC-ID
#define SST25VF_COMMAND_EBSY		      0x70				//Ê¹ÄÜ--SO¹Ü½Å×÷ÎªÐ¾Æ¬ÄÚ²¿ÔÚAAIÄ£Ê½ÏÂ×´Ì¬Êä³ö
#define SST25VF_COMMAND_DBSY		      0x80				//½ûÖ¹--SO¹Ü½Å×÷ÎªÐ¾Æ¬ÄÚ²¿ÔÚAAIÄ£Ê½ÏÂ×´Ì¬Êä³ö
#define SST25VF_COMMAND_4K_ERASE	      0x20				//4KÉÈÇø²Á³ý
#define SST25VF_COMMAND_32K_ERASE	      0x52				//32K¿é²Á³ý
#define SST25VF_COMMAND_64K_ERASE	      0xD8				//64K¿é²Á³ý
#define SST25VF_COMMAND_CHIP_ERASE	      0x60				//È«Æ¬²Á³ý



//CS
#define SST25VF_CS_High()				(GPIOE->BSRRL = GPIO_Pin_1)
#define SST25VF_CS_Low()				(GPIOE->BSRRH = GPIO_Pin_1)
//SCK
#define SST25VF_SCK_High()				(GPIOE->BSRRL = GPIO_Pin_4)
#define SST25VF_SCK_Low()				(GPIOE->BSRRH = GPIO_Pin_4)
//SI--SST25VFÊäÈë¶Ë¿Ú
#define SST25VF_SI_High()				(GPIOE->BSRRL = GPIO_Pin_3)
#define SST25VF_SI_Low()				(GPIOE->BSRRH = GPIO_Pin_3)
//SO--SST25VFÊä³ö¶Ë¿Ú
#define SST25VF_SO_Status()				(GPIOE->IDR & GPIO_Pin_2)



void  SST25VF_Init(void);
void SST25VF_ReadData(uint32_t addr,uint8_t *buf,uint16_t buf_size);
void SST25VF_HighSpeed_Read(uint32_t addr,uint8_t *buf,uint16_t buf_size);
void SST25VF_Byte_Program(uint32_t addr,uint8_t byte);
void SST25VF_AAI_Program(uint32_t addr,uint8_t *buf,uint16_t buf_size);
void SST25VF_Sector_Erase_4K(uint32_t addr);
void SST25VF_Block_Erase_32K(uint32_t addr);
void SST25VF_Block_Erase_64K(uint32_t addr);
void SST25VF_Chip_Erase(void);
uint32_t SST25VF_Jedec_ID_Read(void) ;


#endif


