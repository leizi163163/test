
#include "stm32f2xx.h"

//CS----PE0
//SCLK--PE4
//SI----PE3		--FM25256 ‰»Î
//SO----PE2		--FM25256 ‰≥ˆ
void  FM25256_Init(void)
{  	
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	//CS 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	FM25256_CS_High();

	//SCK 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	FM25256_SCK_Low();
	
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

uint8_t FM25256_Send_Byte(uint8_t buf)
{	
	FM25256_SCK_Low();
	if(buf & 0x80)
	{
		FM25256_SI_High();
	}	
	else
	{
		FM25256_SI_Low();
	}
	buf <<= 1;
	FM25256_SCK_High();
	buf |= (FM25256_SO_Status() >> 2);
	
	FM25256_SCK_Low();
	if(buf & 0x80)
	{
		FM25256_SI_High();
	}	
	else
	{
		FM25256_SI_Low();
	}
	buf <<= 1;
	FM25256_SCK_High();	
	buf |= (FM25256_SO_Status() >> 2);
	
	FM25256_SCK_Low();
	if(buf & 0x80)
	{
		FM25256_SI_High();
	}	
	else
	{
		FM25256_SI_Low();
	}
	buf <<= 1;
	FM25256_SCK_High();	
	buf |= (FM25256_SO_Status() >> 2);
	
	FM25256_SCK_Low();
	if(buf & 0x80)
	{
		FM25256_SI_High();
	}	
	else
	{
		FM25256_SI_Low();
	}
	buf <<= 1;
	FM25256_SCK_High();	
	buf |= (FM25256_SO_Status() >> 2);
	
	FM25256_SCK_Low();
	if(buf & 0x80)
	{
		FM25256_SI_High();
	}	
	else
	{
		FM25256_SI_Low();
	}
	buf <<= 1;
	FM25256_SCK_High();	
	buf |= (FM25256_SO_Status() >> 2);
	
	FM25256_SCK_Low();
	if(buf & 0x80)
	{
		FM25256_SI_High();
	}	
	else
	{
		FM25256_SI_Low();
	}
	buf <<= 1;
	FM25256_SCK_High();	
	buf |= (FM25256_SO_Status() >> 2);
	
	FM25256_SCK_Low();
	if(buf & 0x80)
	{
		FM25256_SI_High();
	}	
	else
	{
		FM25256_SI_Low();
	}
	buf <<= 1;
	FM25256_SCK_High();	
	buf |= (FM25256_SO_Status() >> 2);
	
	FM25256_SCK_Low();
	if(buf & 0x80)
	{
		FM25256_SI_High();
	}	
	else
	{
		FM25256_SI_Low();
	}
	buf <<= 1;
	FM25256_SCK_High();	
	buf |= (FM25256_SO_Status() >> 2);

	FM25256_SCK_Low();
	
	return buf;
}

void FM25256_WREN(void)
{
	FM25256_CS_Low();								/* enable device */
	FM25256_Send_Byte(FM25256_COMMAND_WREN);		/* send WREN command */
	FM25256_CS_High();								/* disable device */
}

void FM25256_WRDI(void)
{
	FM25256_CS_Low();								/* enable device */
	FM25256_Send_Byte(FM25256_COMMAND_WRDI);		/* send WREN command */
	FM25256_CS_High();								/* disable device */
}

uint8_t FM25256_ReadStatusRegister(void)
{
	uint8_t byte = 0;
	FM25256_CS_Low();								/* enable device */
	FM25256_Send_Byte(FM25256_COMMAND_RDSR);		/* send RDSR command */
	byte = FM25256_Send_Byte(0xFF); 				/* receive byte */
	FM25256_CS_High();								/* disable device */
	return byte;
}

void FM25256_WriteStatusRegister(uint8_t value)
{
	FM25256_WREN();
	FM25256_CS_Low();								/* enable device */	
	FM25256_Send_Byte(FM25256_COMMAND_WRSR);		/* send RDSR command */
	FM25256_Send_Byte(value); 						/* receive byte */
	FM25256_CS_High();								/* disable device */
}

uint8_t FM25256_WREN_Check(void)
{
	uint8_t byte;
	byte = FM25256_ReadStatusRegister();	/* read the status register */
	if (byte & 0x02)				/* verify that WEL bit is set */
	{
		return 1;
	}
	return 0;
}

void FM25256_ReadData(uint16_t addr,uint8_t *buf,uint16_t buf_size)
{
	uint16_t i = 0;
	FM25256_CS_Low();								/* enable device */
	FM25256_Send_Byte(FM25256_COMMAND_READ); 		/* read command */
	FM25256_Send_Byte(addr >> 8);
	FM25256_Send_Byte(addr & 0xFF);
	
	for (i = 0; i < buf_size; i++)					
	{
		buf[i] = FM25256_Send_Byte(0xFF);			
	}
	FM25256_CS_High();								/* disable device */
}

void FM25256_Program(uint16_t addr,uint8_t *buf,uint16_t buf_size)
{
	uint16_t i = 0;
		
	FM25256_WREN();	 	
	FM25256_CS_Low();								/* enable device */
	FM25256_Send_Byte(FM25256_COMMAND_WRITE);		/* send write command */
	FM25256_Send_Byte(addr >> 8);
	FM25256_Send_Byte(addr & 0xFF);

	for (i = 0; i < buf_size; i++)					/* read until no_bytes is reached */
	{
		FM25256_Send_Byte(buf[i]);
	}
	FM25256_CS_High();								/* disable device */		
}

uint8_t FM25256_WriteData(uint16_t addr,uint8_t *buf,uint16_t buf_size)
{
	uint8_t buffer[256];

	FM25256_Program(addr,buf,buf_size);
	FM25256_ReadData(addr,buffer,buf_size);

	if (memcmp(buf,buffer,buf_size) == 0)
	{
		return 0;
	}

	return 1;
}

void FM25256_Clear(void)
{
	uint16_t i = 0;
		
	FM25256_WREN();	 	
	FM25256_CS_Low();								/* enable device */
	FM25256_Send_Byte(FM25256_COMMAND_WRITE);		/* send write command */
	FM25256_Send_Byte(0x00);
	FM25256_Send_Byte(0x00);

	for (i = 0; i < FM25256_CHIP_SIZE; i++)					/* read until no_bytes is reached */
	{
		FM25256_Send_Byte(0x00);
	}
	FM25256_CS_High();								/* disable device */		
}

void FM25256_Fill(void)
{
	uint16_t i = 0;
		
	FM25256_WREN();	 	
	FM25256_CS_Low();								/* enable device */
	FM25256_Send_Byte(FM25256_COMMAND_WRITE);		/* send write command */
	FM25256_Send_Byte(0x00);
	FM25256_Send_Byte(0x00);

	for (i = 0; i < FM25256_CHIP_SIZE; i++)					/* read until no_bytes is reached */
	{
		FM25256_Send_Byte(i);
	}
	FM25256_CS_High();								/* disable device */		
}

