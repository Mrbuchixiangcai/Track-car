#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									   
//********************************************************************************
//20140507 	V1.1
//1,�޸�Adc_Init����,֧���ڲ��¶ȴ��������ݲɼ�.
//2,����Get_Temprate����,���ڻ�ȡ�ڲ��¶ȴ������ɼ������¶�ֵ
//20140714  V1.2
//1,����Adc3_Init����,���ڳ�ʼ��ADC3
//2,����Get_Adc3����,���ڶ�ȡADC3��ĳ��ͨ��
////////////////////////////////////////////////////////////////////////////////// 


#define ADC_CH5  		5 		 	//ͨ��5	   	      	    
#define ADC_CH_TEMP  	16 		 	//ͨ��16,�ڲ��¶ȴ�����ר��ͨ��	   	    

void Adc_Init(void); 				//ADC��ʼ��
u16  Get_Adc(u8 ch); 				//���ĳ��ͨ��ֵ 
float Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
short Get_Temprate(void);			//��ȡ�ڲ��¶ȴ�����ֵ 
void Adc3_Init(void); 				//ADC3��ʼ��
u16  Get_Adc3(u8 ch); 				//���ADC3ĳ��ͨ��ֵ  
#endif 














