#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "tft_display.h"
#include "key.h"   
#include "stmflash.h" 
#include "iap.h"   
#include "menu.h"
#include "beep.h"

pFunction Jump_To_Application;


//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		Jump_To_Application=(iapfun)*(vu32*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		for(int i = 0; i < 8; i++)
		{			
			NVIC->ICER[i] = 0xFFFFFFFF;	/* �ر��ж�*/
			NVIC->ICPR[i] = 0xFFFFFFFF;	/* ����жϱ�־λ */
		}
		Jump_To_Application();									//��ת��APP.
	}
}	

//RTCд�������SRAM
//BKPx:�����Ĵ������,��Χ:0~19
//data:Ҫд�������,32λ����
void RTC_Write_BKR(u32 BKRx,u32 data)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4;   
	(*(u32*)temp)=data; 
}
//RTC��ȡ������SRAM
//BKPx:�����Ĵ������,��Χ:0~19
//����ֵ:��ȡ��������
u32 RTC_Read_BKR(u32 BKRx)
{
	u32 temp=0; 
	temp=RTC_BASE+0x50+BKRx*4;  
	return (*(u32*)temp);		//���ض�ȡ����ֵ
}

void Serial_PutString(uint8_t *s);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200

	LED_Init();					//��ʼ��LED
	LCD_Init();					//LCD��ʼ��
	KEY_Init();					//������ʼ�� 
	BEEP_Init();
	LCD_Clear(BACK_COLOR);
	POINT_COLOR=BLUE;    //��������Ϊ��ɫ 
	LCD_ShowString(30,30,200,16,16, "Bootload with Ymodem");	
	
	LCD_ShowString(30,46,240,16,16,__DATE__);
	LCD_ShowString(30,62,240,16,16,__TIME__);
	
	POINT_COLOR=RED;//��������Ϊ��ɫ	 
	
	Set_Beep(200,6);
	
	RCC->APB1ENR|=1<<28;			//ʹ�ܵ�Դ�ӿ�ʱ��
	PWR->CR|=1<<8;					//���������ʹ��(RTC+SRAM)
	
	if(PEin(5)==0 || (RTC_Read_BKR(18) == 0x43215678 && RTC_Read_BKR(19) == 0x12348765))  //��������
	{
		if(Main_Menu()==1)
		{
			
		}
		RTC_Write_BKR(18,0);
		RTC_Write_BKR(19,0);
	}
	LCD_ShowString(30,150,200,16,16,"enter app");
	iap_load_app(APPLICATION_ADDRESS);
	LCD_ShowString(30,150,200,16,16,"can not enter app!!!");
	while(1)
	{
		LCD_ShowString(30,150,200,16,16,"                    ");Set_Beep(200,2);
		LCD_ShowString(30,150,200,16,16,"can not enter app!!!");Set_Beep(200,2);
		RTC_Write_BKR(18,0x43215678);
		RTC_Write_BKR(19,0x12348765);
		Serial_PutString("can not enter app!!!\r");
		Sys_Soft_Reset();
	}  
}

