
#ifndef _ad_da_h_
#define _ad_da_h_

#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"


extern volatile u16 fsin;

extern volatile u16 fcos;


extern void Adc_Init(void);
extern void Dac1_Init(void);
extern void Dac1_Set_Vol(u16 vol);
extern  void Get_Adc(void);

#define DA_BUF_LEN (2+200+100)
#define DAFq 8

#define Img_H 200
#define Img_V 150

#define CutLen 		10
#define CutIndex 	(Img_V/CutLen)

void Wave_DMA_Config(void);

#endif



