#include "ad_da.h"
		

#define DAC_DHR8R1    (u32)&(DAC->DHR8R1) 

void Dac1_Init(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4)	;
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5)	;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
					
	DAC_StructInit(&DAC_InitType);
	DAC_InitType.DAC_Trigger=DAC_Trigger_T2_TRGO;
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Enable ;
  DAC_Init(DAC_Channel_1,&DAC_InitType);
// 	DAC_Init(DAC_Channel_2,&DAC_InitType);

	DAC_Cmd(DAC_Channel_1, ENABLE);
// 	DAC_Cmd(DAC_Channel_2, ENABLE);
  
//   DAC_SetChannel1Data(DAC_Align_12b_R, 0);
// 	DAC_SetChannel2Data(DAC_Align_12b_R, 0);

 DAC_DMACmd(DAC_Channel_1, ENABLE); 
}



// //设置通道1输出电压
// //vol:0~3300,代表0~3.3V
// void Dac1_Set_Vol(u16 vol)
// {
// 	DAC->DHR12R1 = vol;
// }

extern u16 OutputData[CutIndex][DA_BUF_LEN][DAFq];

void Wave_DMA_Config(void)//DMA2
{                  
    DMA_InitTypeDef            	DMA_InitStructure;
	  NVIC_InitTypeDef 						NVIC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
     
    DMA_StructInit( &DMA_InitStructure);      
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = DA_BUF_LEN*DAFq;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)OutputData;

    DMA_Init(DMA2_Channel3, &DMA_InitStructure);
    DMA_Cmd(DMA2_Channel3, ENABLE);  
	
    
		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;      
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           
		NVIC_Init(&NVIC_InitStructure);
		DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE); 
}




