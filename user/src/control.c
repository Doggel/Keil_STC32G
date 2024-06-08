#include "control.h"



PID_Calibration PID_accy 		= {50,0,50};	// �ڻ�accy��PID��������PD
PID_Calibration PID_adc 		= {50,50,100};	// �л�adc��PID����
PID_Calibration PID_out_left 	= {80,0,0};	// �⻷���ٶȵ�PID����
PID_Calibration PID_out_right 	= {80,0,0};	// �⻷���ٶȵ�PID����

PID_State accy_state		= {0};				        //accy״̬����
PID_State adc_state 		= {0};				        //adc״̬����
PID_State Left_Speed_State 	= {0};				//�����ٶ�״̬����
PID_State Right_Speed_State = {0};				//�ҵ���ٶ�״̬����

/// @brief ͨ��PID�㷨��������ٶȴﵽĿ���ٶ�
/// @param Left_Speed ��������������Ŀ���ٶ�
/// @param Right_Speed ���������ҵ����Ŀ���ٶ�
// �ڻ� error_in = y_acc - 0
void Speed_Ctrl_in(unsigned int accy_target)
{
	extern float fil_acc_y;
	//��ȡ��ʵaccy
	accy_state.actual = fil_acc_y;
	//��ȡĿ��accy
	accy_state.target = accy_target;
	
	pid_increase(&PID_accy,&accy_state);
//	Update_Motors(&accy_state,&accy_state,&Turn_State);
}

// �л�
void Speed_Ctrl_mid(unsigned int adc_target)
{
	//��ȡ��ʵadc
	adc_state.actual = (int16) Get_Regularized_Signal_Data(All_Signal_Data);
	//adc_state.actual = 0;
	//��ȡĿ��adc
	adc_state.target = adc_target;

	pid_increase(&PID_adc,&adc_state);
//	Update_Motors(&adc_state,&adc_state,&Turn_State);
}

// �⻷
void Speed_Ctrl_out(unsigned int Left_Speed,unsigned int Right_Speed)
{
	//��ȡ��ʵ�ٶ�
	Left_Speed_State.actual = (get_EncoderL()/MAXENCODER)*MAXSPEED;
	Right_Speed_State.actual = (get_EncoderR()/MAXENCODER)*MAXSPEED;
	
	//��ȡĿ���ٶ�
	Left_Speed_State.target = Left_Speed;
	Right_Speed_State.target = Right_Speed;
	
	pid_increase(&PID_out_left,&Left_Speed_State);
	pid_increase(&PID_out_right,&Right_Speed_State);
//	Update_Motors(&Left_Speed_State,&Right_Speed_State,&Turn_State);
}



/// @brief ͨ��PID�㷨��������ٶȴﵽĿ���ٶȺ�Ŀ��Ƕ�
/// @param Gyro ��������С����Ŀ��Ƕ�
void Turn_Ctrl(unsigned int Gyro)
{
	
}

/// @brief ��׼���������������Ӿ�������������磺���ţ��޷�����һ���Ȳ���
/// @param left_state 
/// @param right_state 
void Regularization(PID_State * left_state,PID_State * right_state)
{
	
}

/// @brief ���µ��PWMת��
/// @param left_state 
/// @param right_state 
/// @param gyro_state 
void Update_Motors(PID_State * left_state,PID_State * right_state,PID_State * gyro_state)
{
	Set_Rmotor_Speed(right_state->output + gyro_state->output);
	Set_Lmotor_Speed(left_state->output + gyro_state->output);
}

void Set_Motors(unsigned int left, unsigned int right)
{
	if(left > SAFESPEED || right > SAFESPEED)
	{
		left = SAFESPEED;
		right = SAFESPEED;
	}
	Set_Lmotor_Speed(right);
	Set_Rmotor_Speed(left);
}

void Stop_Car(void)		// С��ֹͣ
{
	Set_Rmotor_Speed(0);
	Set_Lmotor_Speed(0);
}


