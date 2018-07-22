#include "key.h"
#include "sys.h" 
#include "delay.h"
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
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(KEY1_Periph|KEY2_Periph, ENABLE);
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����	
	//=====KEY1
	GPIO_InitStructure.GPIO_Pin = KEY1_Pin;	
	GPIO_Init(KEY1_GPIOx, &GPIO_InitStructure);					//��ʼ��GPIO
	//=====KEY2
	GPIO_InitStructure.GPIO_Pin = KEY2_Pin;	
	GPIO_Init(KEY2_GPIOx, &GPIO_InitStructure);					//��ʼ��GPIO	
	
//	GPIO_SetBits();	
}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&((KEY1==1)||(KEY2==1)))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==1)return 1;
		else if(KEY2==1)return 2;
	}else if((KEY1==0)&&(KEY2==0))key_up=1; 	    
 	return 0;// �ް�������
}

