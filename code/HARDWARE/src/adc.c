#include "adc.h"
#include "delay.h"		 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//ADC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									   
//********************************************************************************
//20140507 	V1.1
//1,�޸�Adc_Init����,֧���ڲ��¶ȴ��������ݲɼ�.
//2,����Get_Temprate����,���ڻ�ȡ�ڲ��¶ȴ������ɼ������¶�ֵ
//20140714  V1.2
//1,����Adc3_Init����,���ڳ�ʼ��ADC3
//2,����Get_Adc3����,���ڶ�ȡADC3��ĳ��ͨ��
////////////////////////////////////////////////////////////////////////////////// 


//��ʼ��ADC
//����ADC1_CH8 	ADC1_CH9
void  Adc_Init(void)
{    
	//�ȳ�ʼ��IO��
 	RCC->APB2ENR|=1<<8;    	//ʹ��ADC1ʱ�� �ߵ�ƽʹ��
	RCC->AHB1ENR|=1<<1;    	//ʹ��PORTBʱ��	��GPIOB
	GPIO_Set(GPIOB,PIN0,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PB0,ģ������,��������
	GPIO_Set(GPIOB,PIN1,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PB1,ģ������,��������
	
	RCC->APB2RSTR|=1<<8;   	//ADCs��λ��0-31�е�8λ��1��λ��0����
	RCC->APB2RSTR&=~(1<<8);	//ADCs�˳���λ״̬����ȻADC_DR�ȼĴ���һֱ���ڸ�λ״̬	 
	ADC->CCR=3<<16;			//ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 8��Ƶ
	ADC->CCR|=1<<23;		//ʹ���ڲ��¶ȴ�����
 	
	ADC1->CR1=0;   			//CR1��������
	ADC1->CR2=0;   			//CR2��������
	ADC1->CR1|=0<<24;      	//12λģʽ
	ADC1->CR1|=0<<8;    	//��ɨ��ģʽ	
	
	ADC1->CR2&=~(1<<1);    	//����ת��ģʽ������ת��
 	ADC1->CR2&=~(1<<11);   	//�Ҷ���
	ADC1->CR2|=0<<28;    	//�����������ֹ�ⲿ����
	
	ADC1->SQR1&=~(0XF<<20); //��������дֵ
	ADC1->SQR1|=1<<20;     	//2��ת���ڹ��������� Ҳ����ֻת����������1��2			   
	//����ͨ��8��9�Ĳ���ʱ��
	ADC1->SMPR2&=~(7<<(3*8));//ͨ��8����ʱ�����
	ADC1->SMPR2&=~(7<<(3*9));//ͨ��9����ʱ�����
 	ADC1->SMPR2|=7<<(3*8); 	//ͨ��8  480������,��߲���ʱ�������߾�ȷ��	
	ADC1->SMPR2|=7<<(3*9); 	//ͨ��9  480������,��߲���ʱ�������߾�ȷ��
 	ADC1->CR2|=1<<0;	   	//����ADת����	  
}


//���ADCֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_Adc(u8 ch)   
{
	//����ת������	  		 
	ADC1->SQR3&=0XFFFFFFE0;//��������1 ͨ��ch
	ADC1->SQR3|=ch;		  			    
	ADC1->CR2|=1<<30;       //��������ת��ͨ�� 
	while(!(ADC1->SR&1<<1));//�ȴ�ת������	 	   
	return ADC1->DR;		//����adcֵ	
}
//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
float Get_Adc_Average(u8 ch,u8 times)
{
	static float last_val1,last_val2;
	
	float temp_val=0;
	
	temp_val = (float)(Get_Adc(ch))*2.5/40.96;
	if(ch==8)
	{
		last_val1 = last_val1*0.92+temp_val*0.08;
		return last_val1;
	}
	else if(ch==9)
	{
		last_val2 = last_val2*0.8+temp_val*0.2;
		return last_val2;
	}
}  
//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
short Get_Temprate(void)
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=Get_Adc_Average(ADC_CH_TEMP,20);	//��ȡͨ��16,20��ȡƽ��
	temperate=(float)adcx*(3.3/4096);		//��ѹֵ
	temperate=(temperate-0.76)/0.0025+25; 	//ת��Ϊ�¶�ֵ 
	result=temperate*=100;					//����100��.
	return result;
}






