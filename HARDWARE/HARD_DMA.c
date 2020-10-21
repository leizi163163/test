#include "stm32f10x.h"


uint16_t DMA_CurrentCndtrValue;


/*******************************************************************************
*
*����ԭ�ͣ�DMA_Config_ReceiveMode();
*��ڲ�������
*���ڲ�������
*�������ܣ�DMA��ʼ��������1����DMA��ʼ��
*
*******************************************************************************/
void DMA_Config_ReceiveMode(void)
{
    DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//����DMAʱ��

 	/*����DMAԴ���ڴ��ַ&�������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   

	/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DataBuff;
	
	/*���򣺴��ڴ浽����*/		
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	
	
	/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
    DMA_InitStructure.DMA_BufferSize = DATABUFF_SIZE;
	
	/*�����ַ����*/	    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	
	/*�ڴ��ַ����*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	
	/*�������ݵ�λ*/	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	/*�ڴ����ݵ�λ 8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	
	/*DMAģʽ��һ�δ��䣬ѭ��*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;	 
	
	/*���ȼ�����*/	
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
	
	/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/*����DMA1��4ͨ��*/		   
    DMA_Init(DMA1_Channel5, &DMA_InitStructure); 	   
	
	
	DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�

}


/*******************************************************************************
*
*����ԭ�ͣ�DMA_Config_SendMode();
*��ڲ�������
*���ڲ�������
*�������ܣ�DMA��ʼ��������1����DMA��ʼ��
*
*******************************************************************************/
void DMA_Config_SendMode(void)
{
    DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//����DMAʱ��

 	/*����DMAԴ���ڴ��ַ&�������ݼĴ�����ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;	   

	/*�ڴ��ַ(Ҫ����ı�����ָ��)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ReceiveBuff;
	
	/*���򣺴��ڴ浽����*/		
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
	
	/*�����СDMA_BufferSize=SENDBUFF_SIZE*/	
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
	
	/*�����ַ����*/	    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	
	/*�ڴ��ַ����*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	
	/*�������ݵ�λ*/	
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	/*�ڴ����ݵ�λ 8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 
	
	/*DMAģʽ��һ�δ��䣬ѭ��*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	 
	
	/*���ȼ�����*/	
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  
	
	/*��ֹ�ڴ浽�ڴ�Ĵ���	*/
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	/*����DMA1��4ͨ��*/		   
    DMA_Init(DMA1_Channel4, &DMA_InitStructure); 	   
	
//	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  //����DMA������ɺ�����ж�
	

}

