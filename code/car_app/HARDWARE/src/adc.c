#include "adc.h"
#include "delay.h"		 


#define ADC_BUFFER_SIZE  30

static uint16_t AdcBuffer[ADC_BUFFER_SIZE]={0};


//��ʼ��ADC
//����ADC1_CH8 	ADC1_CH9
void  Adc_Init(void)
{   
	//�ȳ�ʼ��IO��
 	RCC->APB2ENR|=1<<8;    	//ʹ��ADC1ʱ�� �ߵ�ƽʹ��
	RCC->AHB1ENR|=1<<1;    	//ʹ��PORTBʱ��	��GPIOB
	GPIO_Set(GPIOB,PIN0,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PB0,ģ������,��������
	GPIO_Set(GPIOB,PIN1,GPIO_MODE_AIN,0,0,GPIO_PUPD_NONE);	//PB1,ģ������,��������
	
	RCC->AHB1ENR|=1<<22;//DMA2ʱ��ʹ�� 
	
	DMA2_Stream0->CR = 0;
	delay_us(10);
	DMA2_Stream0->CR |= 0<<25;
	DMA2_Stream0->CR |= 0<<23;//�洢��ͻ����������
	DMA2_Stream0->CR |= 0<<21;//����ͻ����������
	DMA2_Stream0->CR |= 0<<19;
	DMA2_Stream0->CR |= 0<<18;
	DMA2_Stream0->CR |= 1<<16;//�����ȼ�
	DMA2_Stream0->CR |= 0<<15;
	DMA2_Stream0->CR |= 1<<13;
	DMA2_Stream0->CR |= 1<<11;
	DMA2_Stream0->CR |= 1<<10;
	DMA2_Stream0->CR |= 0<<9;
	DMA2_Stream0->CR |= 1<<8;
	DMA2_Stream0->CR |= 0<<6;
	DMA2_Stream0->CR |= 0<<5;
	
	DMA2_Stream0->M0AR = (u32)AdcBuffer;
	DMA2_Stream0->PAR  = (u32)&ADC1->DR;
	DMA2_Stream0->NDTR = ADC_BUFFER_SIZE;
	
	DMA2_Stream0->CR |= 1<<0; //����DMA
	
	RCC->APB2RSTR|=1<<8;   	//ADCs��λ��0-31�е�8λ��1��λ��0����
	RCC->APB2RSTR&=~(1<<8);	//ADCs�˳���λ״̬����ȻADC_DR�ȼĴ���һֱ���ڸ�λ״̬	 
	ADC->CCR=3<<16;			//ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 8��Ƶ
	ADC->CCR|=1<<23;		//ʹ���ڲ��¶ȴ�����
 	
	ADC1->CR1=0;   			//CR1��������
	ADC1->CR2=0;   			//CR2��������
	
	ADC1->CR1|=0<<24;      	//12λģʽ
	ADC1->CR1|=1<<8;    	//��ɨ��ģʽ
	
	ADC1->CR2|=1<<8;	   	//����DMA
	ADC1->CR2|=1<<9;
	ADC1->CR2|=1<<1;    	//����ת��ģʽ
	
	ADC1->SQR1&=~(0X0F<<20); //��������дֵ
	ADC1->SQR1|=1<<20;     	//2��ת���ڹ��������� Ҳ����ֻת����������1��2			   
	//���ò���ʱ��
	ADC1->SMPR2&=~(7<<24);//ͨ��8����ʱ�����
	ADC1->SMPR2|=7<<24; 	
	ADC1->SMPR2&=~(7<<27);//ͨ��9����ʱ�����
	ADC1->SMPR2|=7<<27; 
	ADC1->SMPR1&=~(7<<18);//ͨ��16����ʱ�����
	ADC1->SMPR1|=7<<18; 
	
	ADC1->SQR3&=0XFFFF6000;//��������1 ͨ��ch
	ADC1->SQR3|=((8<<5)|(9<<0)|(ADC_CH_TEMP<<10));
	
 	ADC1->CR2|=1<<0;	   	//����ADת����
	ADC1->CR2|=1<<30;       //��������ת��ͨ��
}



//��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ��
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
float Get_Adc_Average(u8 ch,u8 times)
{
	static float last_val1,last_val2;
	
	float temp_val;
	
	if(ch==8)
	{
		temp_val = (float)(AdcBuffer[1])*2.5/40.96;
		last_val1 = last_val1*0.92+temp_val*0.08;
		return last_val1;
	}
	else if(ch==9)
	{
		temp_val = (float)(AdcBuffer[0])*2.5/40.96;
		last_val2 = last_val2*0.8+temp_val*0.2;
		return last_val2;
	}
	return 0;
}

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
short Get_Temprate(void)
{
	u32 adcx;
	short result;
 	double temperate;
	adcx=AdcBuffer[2];	//��ȡͨ��16,20��ȡƽ��
	temperate=(float)adcx*(2.5/4096);		//��ѹֵ
	temperate=(temperate-0.76)/0.0025+25; 	//ת��Ϊ�¶�ֵ 
	result=temperate*=100;					//����100��.
	return result;
}






