#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 
/************ 	 �˿ڶ��� ������ֲ�޸�    ***************************/
/*  KEY1����  */
#define	 KEY1_Periph		 RCC_AHB1Periph_GPIOE
#define	 KEY1_Pin 	  	     GPIO_Pin_5
#define	 KEY1_GPIOx	  	     GPIOE
#define  KEY1				 PEin(5)
/*  KEY1����  */
#define	 KEY2_Periph		 RCC_AHB1Periph_GPIOE
#define	 KEY2_Pin 	  	     GPIO_Pin_6
#define	 KEY2_GPIOx	  	     GPIOE
#define  KEY2				 PEin(6)

#define KEY_LEFT	1
#define KEY_RIGHT	2

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
