#include "config.h"
#include "motor.h"

Motor motor;				// �������ṹ��
PWMx_Duty pwmA_duty;		// ����PWMA�ṹ��

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ�����
//  @param      PWM	
//  @return     void					
//  @since      v1.0
//  Sample usage:	motor_init()
//-------------------------------------------------------------------------------------------------------------------
void motor_init(void)
{

	// ��ʼ�� PWMx
	PWMx_InitDefine PWMx_InitStructure;							// ����PWM��ʼ���ṹ��
	
	pwmA_duty.PWM3_Duty = 0;									// ��ʼ��PWM3 PWM4��ͨ����ռ�ձ�
	pwmA_duty.PWM4_Duty = 0;
	
	PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;		//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    = pwmA_duty.PWM3_Duty;		//PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect   = ENO3P;			//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM3, &PWMx_InitStructure);				//��ʼ��PWM3

	PWMx_InitStructure.PWM_Mode    =	CCMRn_PWM_MODE1;		//ģʽ,		CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty    = pwmA_duty.PWM4_Duty;		//PWMռ�ձ�ʱ��, 0~Period
	PWMx_InitStructure.PWM_EnoSelect   = ENO4P;			//���ͨ��ѡ��,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM4, &PWMx_InitStructure);				//��ʼ��PWM4

	PWMx_InitStructure.PWM_Period   = PWM_Peri;					//����ʱ��,   0~65535
	PWMx_InitStructure.PWM_DeadTime = 0;						//��������������, 0~255
	PWMx_InitStructure.PWM_MainOutEnable= ENABLE;				//�����ʹ��, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable   = ENABLE;				//ʹ�ܼ�����, ENABLE,DISABLE
	PWM_Configuration(PWMA, &PWMx_InitStructure);				//��ʼ��PWMͨ�üĴ���,  PWMA,PWMB
	
	motor.L_pwm_channel = PWM3;
	motor.R_pwm_channel = PWM4;
	
	PWM3_USE_P14P15();					//PWM3ѡ��P14P15
	PWM4_USE_P16P17();					//PWM4ѡ��P16P17
	
	GPIO_PU_Init();						//IO�ڳ�ʼ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ٶ�
//  @param      speed	����ѡ������Ϊ-3500~3500����Ϊ��ת������Ϊ��ת
//  @return     void				
//  @since      v1.0
//  Sample usage:	Set_Lmotor_Speed(5000)
//-------------------------------------------------------------------------------------------------------------------
void Set_Lmotor_Speed(long speed)
{
	u16 duty_cycle = 0;
	
    if (speed < -SAFESPEED)				// �����ٶ���-3500��3500֮��
        speed = -SAFESPEED;
	else if (speed > SAFESPEED)
        speed = SAFESPEED;
	
    if (labs(speed) < 200)				
		speed = 0;
	
	if(speed >= 0)
    {
		Lmotor_direction = 0;							// �����ת
		duty_cycle = (PWM_Peri / MAXSPEED) * speed; 		// ����ռ�ձ�
		pwmA_duty.PWM3_Duty = duty_cycle;
	}
	else 	//if(speed < 0)
	{
		Lmotor_direction = 1;							// �����ת
		duty_cycle = (PWM_Peri / MAXSPEED) * -speed; 		// ����ռ�ձ�
		pwmA_duty.PWM3_Duty = duty_cycle;
	}
	
	UpdatePwm(PWM3, &pwmA_duty) ;	   // ��������PWM������ŵ�ռ�ձ�

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����ҵ���ٶ�
//  @param      speed		����ѡ������Ϊ-3500~3500����Ϊ��ת������Ϊ��ת
//  @return     void				
//  @since      v1.0
//  Sample usage:	Set_Rmotor_Speed(5000)
//-------------------------------------------------------------------------------------------------------------------
void Set_Rmotor_Speed(long speed)
{
	u16 duty_cycle = 0;
	
    if (speed < -SAFESPEED)					// �����ٶ���-3500��3500֮��
        speed = -SAFESPEED;
	else if (speed > SAFESPEED)
        speed = SAFESPEED;

    if (labs(speed) < 200)				
		speed = 0;
	
	if(speed >= 0)
    {
		Rmotor_direction = 0;							// �����ת
		duty_cycle = (PWM_Peri / MAXSPEED) * speed; 		// ����ռ�ձ�
		pwmA_duty.PWM4_Duty = duty_cycle;
	}
	else 	//if(speed < 0)
	{

		Rmotor_direction = 1;							// �����ת
		duty_cycle = (PWM_Peri / MAXSPEED) * -speed; 		// ����ռ�ձ�
		pwmA_duty.PWM4_Duty = duty_cycle;
	}
	
	UpdatePwm(PWM4, &pwmA_duty) ;	   // �����ҵ��PWM������ŵ�ռ�ձ�
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ��IO��Ϊ׼˫���	�ڲ����ã��û��������
//  @param      void
//  @return     void					
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void GPIO_PU_Init(void)
{
	P1_MODE_IO_PU(GPIO_Pin_4|GPIO_Pin_6);	//P1.4��P1.6���PWM�Ĳ���
	P3_MODE_OUT_PP(GPIO_Pin_5);				//P3.5���Ƶ������
	P7_MODE_OUT_PP(GPIO_Pin_0);				//P7.0���Ƶ������
}
