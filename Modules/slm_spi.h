#ifndef _SLM_SPI_H
#define _SLM_SPI_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>  //printf
#include "stm32f10x.h"
#include "slm_type.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;


// ����ֻ������ GPIO ODR��IDR�������Ĵ�����λ����������ַ�������Ĵ�����û�ж���

//SRAM λ����:    0X2000 0000~0X200F 0000
//SRAM λ��������:0X2200 0000~0X23FF FFFF

//���� λ����:    0X4000 0000~0X400F FFFF
//���� λ��������:0X4200 0000~0X43FF FFFF

// �ѡ�λ����ַ+λ��š�ת���ɱ�����ַ�ĺ�
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x000FFFFF)<<5)+(bitnum<<2)) 

// ��һ����ַת����һ��ָ��
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))

// ��λ����������ַת����ָ��
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 


// GPIO ODR �� IDR �Ĵ�����ַӳ�� 
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) 
#define GPIOB_ODR_Addr    (GPIOB_BASE+20)   
#define GPIOC_ODR_Addr    (GPIOC_BASE+20)  
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20)      
#define GPIOG_ODR_Addr    (GPIOG_BASE+20)
#define GPIOH_ODR_Addr    (GPIOH_BASE+20)      
#define GPIOI_ODR_Addr    (GPIOI_BASE+20)

#define GPIOA_IDR_Addr    (GPIOA_BASE+16)  
#define GPIOB_IDR_Addr    (GPIOB_BASE+16)  
#define GPIOC_IDR_Addr    (GPIOC_BASE+16)   
#define GPIOD_IDR_Addr    (GPIOD_BASE+16)  
#define GPIOE_IDR_Addr    (GPIOE_BASE+16)    
#define GPIOF_IDR_Addr    (GPIOF_BASE+16)    
#define GPIOG_IDR_Addr    (GPIOG_BASE+16)  
#define GPIOH_IDR_Addr    (GPIOH_BASE+16)
#define GPIOI_IDR_Addr    (GPIOI_BASE+16)


// �������� GPIO��ĳһ��IO�ڣ�n(0,1,2...16),n��ʾ��������һ��IO��
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //���   
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //����   
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //���   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //����   
  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //���   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //����   
  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //���   
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //����   
  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //���   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����  
  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //���   
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����  
  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //���   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����  

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //���   
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����  

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //���   
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //����  



/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/
/**************************************************************************SL1102���� IO����***********************************************************************/
/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/
/**************************************************************************SL1102���� IO����***********************************************************************/
/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/
#define   SLTX_14K_CLK                 RCC_APB2Periph_GPIOB
#define   SLTX_14K_PIN                 GPIO_Pin_7                   /* PB.7 */  //14K����pin
#define   SLTX_14K_GPIO_PORT          	GPIOB                        /* GPIOB */
#define   SLTX_14K_HIGH                GPIO_SetBits(SLTX_14K_GPIO_PORT,SLTX_14K_PIN )
#define   SLTX_14K_LOW                 GPIO_ResetBits(SLTX_14K_GPIO_PORT,SLTX_14K_PIN )

#define   SLTX_CE_CLK                 RCC_APB2Periph_GPIOB
#define   SLTX_CE_PIN                 GPIO_Pin_5                   /* PB.5 */  //����ETCʹ��
#define   SLTX_CE_GPIO_PORT          	GPIOB                        /* GPIOB */
#define   SLTX_CE_HIGH                GPIO_SetBits(SLTX_CE_GPIO_PORT,SLTX_CE_PIN )
#define   SLTX_CE_LOW                 GPIO_ResetBits(SLTX_CE_GPIO_PORT,SLTX_CE_PIN )

#define   SLTX_PWR_HIGH               GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9 )
#define   SLTX_PWR_LOW                GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9  )

//PC12-->SCN	PC11-->CLK	PD2-->MOSI	���
//PC10-->MISO	����

#define SLTX_SCK_PIN                  GPIO_Pin_11                 /* PC.11 */
#define SLTX_SCK_GPIO_PORT            GPIOC                       /* GPIOC */
#define SLTX_SCK_GPIO_CLK             RCC_APB2Periph_GPIOC
#define SLTX_SCK_SOURCE               GPIO_PinSource11

#define SLTX_SCK_HIGH                 SLTX_SCK_GPIO_PORT->BSRR=SLTX_SCK_PIN//GPIO_SetBits(SLTX_SCK_GPIO_PORT,SLTX_SCK_PIN )
#define SLTX_SCK_LOW                  SLTX_SCK_GPIO_PORT->BRR=SLTX_SCK_PIN //GPIO_ResetBits(SLTX_SCK_GPIO_PORT,SLTX_SCK_PIN )
//#define SPI_SCK_AF                   GPIO_AF_0

#define SLTX_MISO_PIN                 GPIO_Pin_10                 /* PB.10 */
#define SLTX_MISO_GPIO_PORT           GPIOC                       /* GPIOC */
#define SLTX_MISO_GPIO_CLK            RCC_APB2Periph_GPIOC
#define SLTX_MISO_SOURCE              GPIO_PinSource10
//#define SPI_MISO_AF                  GPIO_AF_0


#define SLTX_MOSI_PIN                 GPIO_Pin_2                  /* PD.2 */
#define SLTX_MOSI_GPIO_PORT           GPIOD                        /* GPIOD */
#define SLTX_MOSI_GPIO_CLK            RCC_APB2Periph_GPIOD
#define SLTX_MOSI_SOURCE              GPIO_PinSource2

#define SLTX_MOSI_HIGH                 SLTX_MOSI_GPIO_PORT->BSRR=SLTX_MOSI_PIN//GPIO_SetBits(SLTX_MOSI_GPIO_PORT,SLTX_MOSI_PIN )
#define SLTX_MOSI_LOW                  SLTX_MOSI_GPIO_PORT->BRR=SLTX_MOSI_PIN//GPIO_ResetBits(SLTX_MOSI_GPIO_PORT,SLTX_MOSI_PIN )
//#define SPI_MOSI_AF                  GPIO_AF_0

#define SLTX_CSN_PIN                   GPIO_Pin_12                  /* PC.12 */  //������ͨIO��
#define SLTX_CSN_GPIO_PORT             GPIOC                        /* GPIOC */


#define SLTX_CSN_HIGH                  SLTX_CSN_GPIO_PORT->BSRR=SLTX_CSN_PIN//GPIO_SetBits(SLTX_CSN_GPIO_PORT,SLTX_CSN_PIN )
#define SLTX_CSN_LOW                   SLTX_CSN_GPIO_PORT->BRR=SLTX_CSN_PIN//GPIO_ResetBits(SLTX_CSN_GPIO_PORT,SLTX_CSN_PIN )

/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/
/**************************************************************************SL1102���� IO����***********************************************************************/
/******************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************/
#define   SLRX_CE_CLK                 RCC_APB2Periph_GPIOA
#define   SLRX_CE_PIN                 GPIO_Pin_11                   /* PA.11 */  //����ETCʹ��
#define   SLRX_CE_GPIO_PORT          	GPIOA                        /* GPIOB */
#define   SLRX_CE_HIGH                GPIO_SetBits(SLRX_CE_GPIO_PORT,SLRX_CE_PIN )
#define   SLRX_CE_LOW                 GPIO_ResetBits(SLRX_CE_GPIO_PORT,SLRX_CE_PIN )

#define   SLRX_PWR_CLK                RCC_APB2Periph_GPIOB
#define   SLRX_PWR_PIN                GPIO_Pin_6                   /* PB.5 */  //����ETCʹ��
#define   SLRX_PWR_GPIO_PORT         	GPIOB                        /* GPIOB */
#define   SLRX_PWR_HIGH               GPIO_SetBits(SLRX_PWR_GPIO_PORT,SLRX_PWR_PIN )
#define   SLRX_PWR_LOW                GPIO_ResetBits(SLRX_PWR_GPIO_PORT,SLRX_PWR_PIN )

//PC12-->SCN	PC11-->CLK(sck)	PD2-->MOSI	���
//PC10-->MISO	����

#define SLRX_SCK_PIN                  GPIO_Pin_8                 /* PC.8 */
#define SLRX_SCK_GPIO_PORT            GPIOC                       /* GPIOC */
#define SLRX_SCK_GPIO_CLK             RCC_APB2Periph_GPIOC
#define SLRX_SCK_SOURCE               GPIO_PinSource8

#define SLRX_SCK_HIGH                 SLRX_SCK_GPIO_PORT->BSRR=SLRX_SCK_PIN//GPIO_SetBits(SLRX_SCK_GPIO_PORT,SLRX_SCK_PIN )
#define SLRX_SCK_LOW                  SLRX_SCK_GPIO_PORT->BRR=SLRX_SCK_PIN//GPIO_ResetBits(SLRX_SCK_GPIO_PORT,SLRX_SCK_PIN )
//#define SPI_SCK_AF                   GPIO_AF_0

#define SLRX_MISO_PIN                 GPIO_Pin_7                 /* PC.7 */
#define SLRX_MISO_GPIO_PORT           GPIOC                       /* GPIOC */
#define SLRX_MISO_GPIO_CLK            RCC_APB2Periph_GPIOC
#define SLRX_MISO_SOURCE              GPIO_PinSource7
//#define SPI_MISO_AF                  GPIO_AF_0


#define SLRX_MOSI_PIN                 GPIO_Pin_8                  /* PA.8 */
#define SLRX_MOSI_GPIO_PORT           GPIOA                        /* GPIOD */
#define SLRX_MOSI_GPIO_CLK            RCC_APB2Periph_GPIOA
#define SLRX_MOSI_SOURCE              GPIO_PinSource8

#define SLRX_MOSI_HIGH                 SLRX_MOSI_GPIO_PORT->BSRR=SLRX_MOSI_PIN//GPIO_SetBits(SLRX_MOSI_GPIO_PORT,SLRX_MOSI_PIN )
#define SLRX_MOSI_LOW                  SLRX_MOSI_GPIO_PORT->BRR=SLRX_MOSI_PIN//GPIO_ResetBits(SLRX_MOSI_GPIO_PORT,SLRX_MOSI_PIN )
//#define SPI_MOSI_AF                  GPIO_AF_0

#define SLRX_CSN_PIN                   GPIO_Pin_9                  /* PC.9 */  //������ͨIO��
#define SLRX_CSN_GPIO_PORT             GPIOC                        /* GPIOC*/


#define SLRX_CSN_HIGH                  SLRX_CSN_GPIO_PORT->BSRR=SLRX_CSN_PIN//GPIO_SetBits(SLRX_CSN_GPIO_PORT,SLRX_CSN_PIN )
#define SLRX_CSN_LOW                   SLRX_CSN_GPIO_PORT->BRR=SLRX_CSN_PIN//GPIO_ResetBits(SLRX_CSN_GPIO_PORT,SLRX_CSN_PIN )

void TIM4_Init(void);
void SL1102RX_GPIO_Init(void);
void  SL1102RX_SPI_Init(void);
extern void SL1102RX_Init(void);
extern u8 SL1102RX_ReadSR(u8 sr);
extern void SL1102RX_WriteSR(u8 sr, u8 value);
extern void SL1102RX_SPI_Read(u8 reg, u8 *pBuf, INT32U _UINT8_T);
extern void SL1102RX_SPI_Write(u8 reg, u8 *pBuf, INT32U _UINT8_T);

extern void	SLRX_BER_TEST(void);
extern void SLRXbbp_rst(void);


#ifdef __cplusplus
}
#endif
#endif
