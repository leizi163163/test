#include "stm32f2xx.h"

//CS----PE1
//SCLK--PE4
//SI----PE3		--SST25VF016B输入
//SO----PE2		--SST25VF016B输出
void  SST25VF_Init(void)
{  	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//CS 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	SST25VF_CS_High();

	//SCK 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	SST25VF_SCK_Low();
	
	//SI 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	//SO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);		
}

uint8_t SST25VF_Send_Byte(uint8_t buf)
{	
	SST25VF_SCK_Low();
	if(buf & 0x80)
	{
		SST25VF_SI_High();
	}	
	else
	{
		SST25VF_SI_Low();
	}
	buf <<= 1;
	SST25VF_SCK_High();
	buf |= (SST25VF_SO_Status() >> 2);
	
	SST25VF_SCK_Low();
	if(buf & 0x80)
	{
		SST25VF_SI_High();
	}	
	else
	{
		SST25VF_SI_Low();
	}
	buf <<= 1;
	SST25VF_SCK_High();	
	buf |= (SST25VF_SO_Status() >> 2);
	
	SST25VF_SCK_Low();
	if(buf & 0x80)
	{
		SST25VF_SI_High();
	}	
	else
	{
		SST25VF_SI_Low();
	}
	buf <<= 1;
	SST25VF_SCK_High();	
	buf |= (SST25VF_SO_Status() >> 2);
	
	SST25VF_SCK_Low();
	if(buf & 0x80)
	{
		SST25VF_SI_High();
	}	
	else
	{
		SST25VF_SI_Low();
	}
	buf <<= 1;
	SST25VF_SCK_High();	
	buf |= (SST25VF_SO_Status() >> 2);
	
	SST25VF_SCK_Low();
	if(buf & 0x80)
	{
		SST25VF_SI_High();
	}	
	else
	{
		SST25VF_SI_Low();
	}
	buf <<= 1;
	SST25VF_SCK_High();	
	buf |= (SST25VF_SO_Status() >> 2);
	
	SST25VF_SCK_Low();
	if(buf & 0x80)
	{
		SST25VF_SI_High();
	}	
	else
	{
		SST25VF_SI_Low();
	}
	buf <<= 1;
	SST25VF_SCK_High();	
	buf |= (SST25VF_SO_Status() >> 2);
	
	SST25VF_SCK_Low();
	if(buf & 0x80)
	{
		SST25VF_SI_High();
	}	
	else
	{
		SST25VF_SI_Low();
	}
	buf <<= 1;
	SST25VF_SCK_High();	
	buf |= (SST25VF_SO_Status() >> 2);
	
	SST25VF_SCK_Low();
	if(buf & 0x80)
	{
		SST25VF_SI_High();
	}	
	else
	{
		SST25VF_SI_Low();
	}
	buf <<= 1;
	SST25VF_SCK_High();	
	buf |= (SST25VF_SO_Status() >> 2);

	SST25VF_SCK_Low();
	
	return buf;
}

void SST25VF_EWSR(unsigned char chip)
{
	SST25VF_CS_Low();								/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_EWSR);		/* send WREN command */
	SST25VF_CS_High();								/* disable device */
}

void SST25VF_WREN(void)
{
	SST25VF_CS_Low();								/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_WREN);		/* send WREN command */
	SST25VF_CS_High();								/* disable device */
}

void SST25VF_WRDI(void)
{
	SST25VF_CS_Low();								/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_WRDI);		/* send WREN command */
	SST25VF_CS_High();								/* disable device */
}

void SST25VF_EBSY(void)
{
	SST25VF_CS_Low();								/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_EBSY);		/* send WREN command */
	SST25VF_CS_High();								/* disable device */
}

void SST25VF_DBSY(void)
{
	SST25VF_CS_Low();								/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_DBSY);		/* send WREN command */
	SST25VF_CS_High();								/* disable device */
}

uint8_t SST25VF_ReadStatusRegister(void)
{
	uint8_t byte = 0;
	SST25VF_CS_Low();								/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_RDSR);		/* send RDSR command */
	byte = SST25VF_Send_Byte(0xFF); 				/* receive byte */
	SST25VF_CS_High();								/* disable device */
	return byte;
}

void SST25VF_WriteStatusRegister(uint8_t value)
{
	SST25VF_WREN();
	SST25VF_CS_Low();								/* enable device */	
	SST25VF_Send_Byte(SST25VF_COMMAND_WRSR);		/* send RDSR command */
	SST25VF_Send_Byte(value); 						/* receive byte */
	SST25VF_CS_High();								/* disable device */
}

uint8_t SST25VF_WREN_Check(void)
{
	uint8_t byte;
	byte = SST25VF_ReadStatusRegister();			/* read the status register */
	if (byte & 0x02)								/* verify that WEL bit is set */
	{
		return 1;
	}
	return 0;
}

uint8_t SST25VF_WREN_AAI_Check(void)
{
	uint8_t byte;

	byte = SST25VF_ReadStatusRegister();			/* read the status register */
	if ((byte & 0x42) == 0x42)						/* verify that AAI and WEL bit is set */
	{
		return 1;
	}
	return 0;
}

void SST25VF_Busy(void)
{
	uint8_t status = 0;
	do
	{
		status = SST25VF_ReadStatusRegister();	
	}
	while ( status & 0x01);							/* waste time until not busy */
}

void SST25VF_Wait_Busy_AAI(void)
{
	uint8_t status = 0;
	do
	{
		status = SST25VF_ReadStatusRegister();	
	}
	while ( (status & 0x43) == 0x43);							/* waste time until not busy */
}


void SST25VF_ReadData(uint32_t addr,uint8_t *buf,uint16_t buf_size)
{
	uint16_t i = 0;
	SST25VF_CS_Low();								/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_READ); 		/* read command */
	SST25VF_Send_Byte(addr >> 16);
	SST25VF_Send_Byte(addr >> 8);
	SST25VF_Send_Byte(addr & 0xFF);
	
	for (i = 0; i < buf_size; i++)					
	{
		buf[i] = SST25VF_Send_Byte(0xFF);			
	}
	SST25VF_CS_High();								/* disable device */
}

void SST25VF_HighSpeed_Read(uint32_t addr,uint8_t *buf,uint16_t buf_size)
{
	uint16_t i = 0;
	SST25VF_CS_Low();								/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_FAST_READ); 	/* read command */
	SST25VF_Send_Byte(addr >> 16);
	SST25VF_Send_Byte(addr >> 8);
	SST25VF_Send_Byte(addr & 0xFF);
	SST25VF_Send_Byte(0xFF);						/*dummy byte*/
	for (i = 0; i < buf_size; i++)					/* read until no_bytes is reached */
	{
		buf[i] = SST25VF_Send_Byte(0xFF);			/* receive byte and store at address 80H - FFH */
	}
	SST25VF_CS_High();								/* disable device */
}

void SST25VF_Byte_Program(uint32_t addr,uint8_t byte)
{

	SST25VF_WREN();											//打开写功能
	SST25VF_WriteStatusRegister(0x00);						//写保护禁止
	SST25VF_WREN();											//写状态寄存器后，需重新打开写功能

	SST25VF_CS_Low();										/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_BYTE_PROGRAM); 		/* send Byte Program command */
	SST25VF_Send_Byte(addr >> 16);
	SST25VF_Send_Byte(addr >> 8);
	SST25VF_Send_Byte(addr & 0xFF);
	SST25VF_Send_Byte(byte);								/* send byte to be programmed */
	SST25VF_CS_High();										/* disable device */

	SST25VF_Busy();
}

//buf_size为偶数才可用AAI编程模式
void SST25VF_AAI_Program(uint32_t addr,uint8_t *buf,uint16_t buf_size)
{
	uint16_t i = 0;
	uint16_t len = 0;

	SST25VF_WREN();											//打开写功能
	SST25VF_WriteStatusRegister(0x00);						//写保护禁止
	SST25VF_WREN();											//写状态寄存器后，需重新打开写功能

	SST25VF_CS_Low();										/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_AAI_PROGRAM);			/* send AAI command */
	SST25VF_Send_Byte(addr >> 16);
	SST25VF_Send_Byte(addr >> 8);
	SST25VF_Send_Byte(addr & 0xFF);

	SST25VF_Send_Byte(buf[0]);
	SST25VF_Send_Byte(buf[1]);

	SST25VF_CS_High();										/* disable device */
	SST25VF_Wait_Busy_AAI();

	if (buf_size > 2)
	{
		len = (buf_size-1) / 2;
	}
	
	for (i = 0; i < len; i++)								/* read until no_bytes is reached */
	{
		SST25VF_CS_Low();									/* enable device */

		SST25VF_Send_Byte(SST25VF_COMMAND_AAI_PROGRAM);		/* send AAI command */
		SST25VF_Send_Byte(buf[i * 2 + 2]);
		SST25VF_Send_Byte(buf[i * 2 + 3]);

		SST25VF_CS_High();									/* disable device */		
		SST25VF_Wait_Busy_AAI();
	}
	SST25VF_WRDI();											//退出AAI模式
}

void SST25VF_Sector_Erase_4K(uint32_t addr)
{
	SST25VF_WREN();											//打开写功能
	SST25VF_WriteStatusRegister(0x00);						//写保护禁止
	SST25VF_WREN();											//写状态寄存器后，需重新打开写功能

	SST25VF_CS_Low();										/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_4K_ERASE);			/* send Sector Erase command */
	SST25VF_Send_Byte(addr >> 16);
	SST25VF_Send_Byte(addr >> 8);
	SST25VF_Send_Byte(addr & 0xFF);
	SST25VF_CS_High();										/* disable device */

	SST25VF_Busy();
}

void SST25VF_Block_Erase_32K(uint32_t addr)
{
	SST25VF_WREN();											//打开写功能
	SST25VF_WriteStatusRegister(0x00);						//写保护禁止
	SST25VF_WREN();											//写状态寄存器后，需重新打开写功能

	SST25VF_CS_Low();										/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_32K_ERASE);			/* send Sector Erase command */
	SST25VF_Send_Byte(addr >> 16);
	SST25VF_Send_Byte(addr >> 8);
	SST25VF_Send_Byte(addr & 0xFF);
	SST25VF_CS_High();										/* disable device */

	SST25VF_Busy();
}

void SST25VF_Block_Erase_64K(uint32_t addr)
{
	SST25VF_WREN();											//打开写功能
	SST25VF_WriteStatusRegister(0x00);						//写保护禁止
	SST25VF_WREN();											//写状态寄存器后，需重新打开写功能

	SST25VF_CS_Low();										/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_64K_ERASE);			/* send Sector Erase command */
	SST25VF_Send_Byte(addr >> 16);
	SST25VF_Send_Byte(addr >> 8);
	SST25VF_Send_Byte(addr & 0xFF);
	SST25VF_CS_High();										/* disable device */

	SST25VF_Busy();
}

void SST25VF_Chip_Erase(void)
{
	SST25VF_WREN();											//打开写功能
	SST25VF_WriteStatusRegister(0x00);						//写保护禁止
	SST25VF_WREN();											//写状态寄存器后，需重新打开写功能

	SST25VF_CS_Low();										/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_CHIP_ERASE);			/* send Chip Erase command (60h or C7h) */
	SST25VF_CS_High();										/* disable device */

	SST25VF_Busy();
}

//Jedec_ID = 0xBF2541
uint32_t SST25VF_Jedec_ID_Read(void) 
{
	uint8_t temp[3];

	SST25VF_CS_Low();									/* enable device */
	SST25VF_Send_Byte(SST25VF_COMMAND_JEDECID);		 	/* send JEDEC ID command (9Fh) */
	temp[0] = SST25VF_Send_Byte(0xFF);
	temp[1] = SST25VF_Send_Byte(0xFF);
	temp[2] = SST25VF_Send_Byte(0xFF);
	SST25VF_CS_High();								/* disable device */	

	return (temp[0] << 16 | temp[1] << 8 | temp[2]);
}






