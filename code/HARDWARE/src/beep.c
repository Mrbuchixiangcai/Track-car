#include "beep.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/10
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	



const u8 tone_cycle[14]={191,170,152,143,128,114,101,96,85,76,72,64,57,51}; //��λ��10΢��



//time:���ʱ�䣬��λ�Ǻ���
//tone��������1��14
void Set_Beep(u16 time,u8 tone)
{
	u16 period_back = tone_cycle[tone-1]; //
	u16 period = period_back;

	time = 100*time/period_back;
	while(time--)
	{
		BEEP=0;
		while(--period)	
			{delay_us(5);}
		period = period_back;
		
		BEEP=1;
		while(--period)	
			{delay_us(5);}
		period = period_back;
	}
	BEEP=1;
}


//��ʼ��PF8Ϊ�����
//BEEP IO��ʼ��
void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOFʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;  //PF8����
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //���ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //�ٶ�100M
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;   //����
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_13); //PF8����
}



