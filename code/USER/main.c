#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "tft_display.h"
#include "key.h"  
#include "malloc.h" 
#include "sdio_sdcard.h"
#include "malloc.h"
#include "w25qxx.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "piclib.h"
#include "touch.h"
#include "rtc.h"
#include "pwm.h"
#include "beep.h"
#include "adc.h"

#include "PESensor_Track.h"

#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

#include "string.h"	
#include "math.h"	
//ALIENTEK ̽����STM32F407������ ʵ��41
//ͼƬ��ʾ ʵ��-�⺯���汾 
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com
//������������ӿƼ����޹�˾    
//���ߣ�����ԭ�� @ALIENTEK 

//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
u16 pic_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO tfileinfo;	//��ʱ�ļ���Ϣ	
	u8 *fn;	 			 			   			     
    res=f_opendir(&tdir,(const TCHAR*)path); 	//��Ŀ¼
  	tfileinfo.lfsize=_MAX_LFN*2+1;				//���ļ�����󳤶�
	tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);//Ϊ���ļ������������ڴ�
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	        res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�	
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	} 
	return rval;
}


int main(void)
{        
	u16 t,toll=50;
	
	float pitch,roll,yaw; 	//ŷ����
	short aacx,aacy,aacz;	//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;//������ԭʼ���� 
	short temp;		        //�¶�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	BEEP_Init();
	KEY_Init();					//������ʼ�� 
	delay_init(168);  //��ʼ����ʱ����
	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED 
 	LCD_Init();					//LCD��ʼ��  
	TIM1_PWM_Init();
	TIM2_PWM_Init();
	TIM4_PWM_Init();
	TIM5_PWM_Init();
	PhotoelectricSensor_Init();
	
	MyRTC_Init();
	W25QXX_Init();				//��ʼ��W25Q128
	tp_dev.init();
	Adc_Init();
	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
	f_mount(fs[0],"0:",1); 	//����SD��
 	f_mount(fs[1],"1:",1); 	//����FLASH.
	
	while(MPU_Init())
	{
		GPIOE->ODR ^= GPIO_Pin_7;
		GPIOE->ODR ^= GPIO_Pin_8;
		GPIOB->ODR ^= GPIO_Pin_2;
		Set_Beep(100,1);
	}
	
	if(KEY0==0&&KEY1==0) TP_Adjust();  	//��ĻУ׼ 
	
//	POINT_COLOR=RED;      
//	while(font_init()) 		//����ֿ�
//	{
//		LCD_ShowString(30,50,200,16,16,"Font Error!");
//		delay_ms(200);
//		LCD_Fill(30,50,240,66,WHITE);//�����ʾ
//		delay_ms(200);
//	}
	
	Set_Beep(100,10);
	Set_Beep(100,11);
	Set_Beep(100,13);
	
	while(1)
	{
		if(PEN==0)
		{
			tp_dev.scan(0);
			LCD_DrawPoint(tp_dev.x[0],tp_dev.y[0]);
			LCD_DrawPoint(tp_dev.x[0]+1,tp_dev.y[0]+1);
		}
		
		if(mpu_mpl_get_data(&pitch,&roll,&yaw)==0)
        {
			GPIOE->ODR ^= GPIO_Pin_7;
            temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
//  			if(report)mpu9250_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//���ͼ��ٶ�+������ԭʼ����
//			if(report)usart1_report_imu((int)(roll*100),(int)(pitch*100),(int)(yaw*100),0,0);
			if((t%10)==0)
			{
				if(temp<0)
				{
					LCD_ShowChar(30+48,200,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,200,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,200,temp/100,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,200,temp%10,1,16);		//��ʾС������ 
				temp=pitch*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,220,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,220,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,220,temp%10,1,16);		//��ʾС������ 
				temp=roll*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,240,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,240,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,240,temp%10,1,16);		//��ʾС������ 
				temp=yaw*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,260,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,260,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,260,temp%10,1,16);		//��ʾС������  
				t=0;
				LED0=!LED0;//DS0��˸
			}
		}
		
//		Servor_Act_Angle(1,-5);Servor_Act_Angle(2,-15);Servor_Act_Angle(3,-25);Servor_Act_Angle(4,-35);
//		Servor_Turn_Angle(3,-20);Servor_Turn_Angle(4,-15);
//		delay_ms(800);
//		Servor_Act_Angle(1,35);Servor_Act_Angle(2,25);Servor_Act_Angle(3,15);Servor_Act_Angle(4,5);
//		Servor_Turn_Angle(3,20);Servor_Turn_Angle(4,15);
//		delay_ms(800);
		
		if(KEY0==0&&toll>1)toll--; 
		if(KEY1==0&&toll<100)toll++; 
		Motor_Speed(1,toll);
		
		LCD_ShowNum(30,20,Get_Adc_Average(8,20)*100/11,4,16);
		LCD_ShowNum(30,60,Get_Adc_Average(9,3)*5.7,4,16);
		LCD_ShowNum(30,100,(u32)Get_Temprate(),4,16);
		LCD_ShowNum(30,120,toll,4,16);
		
		LCD_ShowNum(30,140,PhotoelectricSensor_Check(),4,16);
		delay_ms(8);
	}
 	

}











