#include "pwm.h"
#include "led.h"


//PWM�����ʼ������
//�����̽�յƣ�������ƺ���

#define ACT_MAX_ANGLE 90
#define ACT_MIN_ANGLE -90

#define TURN_MAX_ANGLE 26
#define TURN_MIN_ANGLE -26

#define TURN_FORWARD_SERVO_COMPENSATE  (0)
#define TURN_BACKWARD_SERVO_COMPENSATE (-23)

#define MOTOR_MAX_SPEED 40
#define MOTOR_MIN_SPEED 0


//ǰ�յ�PWM�����ʼ��2KHZ
void TIM4_PWM_Init()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef  TIM_OCInitStructure;
	 
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13; //TIM4��ͨ��1��2
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	 GPIO_Init(GPIOD,&GPIO_InitStructure); 
	
	 GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	 GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	
	 TIM_TimeBaseStructure.TIM_Prescaler=83;  //��ʱ����Ƶ
	 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	 TIM_TimeBaseStructure.TIM_Period=499;   //�Զ���װ��ֵ
	 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	 TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	 
	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ1
 	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	 TIM_OCInitStructure.TIM_Pulse = 200;  //ռ�ձ�Ϊ0
	 TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4OC1��OC2
	 TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	 
	 TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCRx�ϵ�Ԥװ�ؼĴ���
	 TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	 
	 TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
	
	 TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}

//ǰ�յ����ȵ��ں���
void Light_Set(float per,u8 fx)//per:ռ�ձ� fx(����):ǰ�յ�λ��
{
   float width,CCR;
	 width =0.5f*per;
	 CCR=1000*width;
	 switch(fx)
	 {
			case LEFT :TIM_SetCompare1(TIM4,(u16)CCR);break;
			case RIGHT:TIM_SetCompare2(TIM4,(u16)CCR);break;
	 }
}

//�������嶯�������ʼ������ Ƶ��50HZ
void TIM5_PWM_Init()//PA���PWM
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //TIM5��ͨ��1��2��3��4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5); //GPIOF9����Ϊ��ʱ��14
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5); //GPIOF9����Ϊ��ʱ��14
	
	TIM_TimeBaseStructure.TIM_Prescaler=83;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=19999;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//��ʼ����ʱ��5
	
	//��ʼ��TIM5 Channel PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 1500;  //��ʼ��ռ�ձ�Ϊ0
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5OC1
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);
	TIM_OC4Init(TIM5, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM5��CCRx�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPEʹ�� 
	
	TIM_CtrlPWMOutputs(TIM5,ENABLE);//����PWM���Ϊʹ��
	
	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM5
}

//���嶯�����ƶ���Ƕ����ú���
void Servor_Act_Angle(u8 num,int8_t angle)//num:ͨ�����
{
	uint16_t CCR;//�Ƚ�ֵ��ߵ�ƽʱ��

	if(angle>ACT_MAX_ANGLE) angle=ACT_MAX_ANGLE;
	if(angle<ACT_MIN_ANGLE) angle=ACT_MIN_ANGLE;
	CCR=((float)angle*11.1)+1500;

	switch(num)
	{
		case 1:TIM_SetCompare1(TIM5,CCR);break;
		case 2:TIM_SetCompare2(TIM5,CCR);break;
		case 3:TIM_SetCompare3(TIM5,CCR);break;
		case 4:TIM_SetCompare4(TIM5,CCR);break;
	}
}

void TIM2_PWM_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11; //TIM2��ͨ��1��2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_Init(GPIOB,&GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_TIM2);

	TIM_TimeBaseStructure.TIM_Prescaler=83;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=19999;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 1500;  //ռ�ձ�Ϊ0
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2OC1��OC2
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCRx�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPEʹ�� 

	TIM_CtrlPWMOutputs(TIM2,ENABLE);//����PWM���Ϊʹ��

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
}

//ת��Ƕȿ��ƶ���Ƕ����ú���
void Servor_Turn_Angle(u8 fx,s8 angle)//fx:�������
{
	uint16_t CCR1,CCR2;//�Ƚ�ֵ��ߵ�ƽʱ��

	if(angle>TURN_MAX_ANGLE) angle=TURN_MAX_ANGLE;
	if(angle<TURN_MIN_ANGLE) angle=TURN_MIN_ANGLE;
	CCR1=((float)angle*11.1)+1500+TURN_FORWARD_SERVO_COMPENSATE;
	CCR2=((float)angle*11.1)+1500+TURN_BACKWARD_SERVO_COMPENSATE;
	switch(fx)
	{
		case TURN_SERVO_FORWARD :TIM_SetCompare3(TIM2,(u32)CCR1);break;
		case TURN_SERVO_BACKWARD:TIM_SetCompare4(TIM2,(u32)CCR2);break;
	}
}


//�������PWM�����ʼ��
void TIM1_PWM_Init()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	 TIM_OCInitTypeDef  TIM_OCInitStructure;
	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11; //TIM1��ͨ��1��2
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	 GPIO_Init(GPIOE,&GPIO_InitStructure); 
	 
	 GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);
	 GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
	
	 TIM_TimeBaseStructure.TIM_Prescaler=167;  //��ʱ����Ƶ
	 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	 TIM_TimeBaseStructure.TIM_Period=19999;   //�Զ���װ��ֵ
	 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	 TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	 
	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM������ȵ���ģʽ1
 	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	 TIM_OCInitStructure.TIM_Pulse = 0;  //ռ�ձ�Ϊ0
	 TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1OC1��OC2
	 TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	 
	 TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCRx�ϵ�Ԥװ�ؼĴ���
	 TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	 
	 TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� 
	 
	 TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
	 TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
}

//����ٶȿ��ƺ���
void Motor_Speed(u8 num,int8_t v)
{
	uint16_t CCR;//�Ƚ�ֵ
	
	if(v>MOTOR_MAX_SPEED)v=MOTOR_MAX_SPEED;
	if(v<MOTOR_MIN_SPEED)v=MOTOR_MIN_SPEED;
	
	CCR=(10*v)+1500;
	switch(num)
	{
		case 1 :TIM_SetCompare1(TIM1,CCR);break;
		case 2 :TIM_SetCompare2(TIM1,CCR);break;
	}
}
