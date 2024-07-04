#include "control.h"



PID_Calibration PID_accy 		= {0,0,0};	// �ڻ�accy��PID��������PD
PID_Calibration PID_adc 		= {0,0,0};	// �л�adc��PID���� 20
PID_Calibration PID_out_left 	= {20,0,25};	// �⻷���ٶȵ�PID����
PID_Calibration PID_out_right 	= {20,0,25};	// �⻷���ٶȵ�PID����
//P: 32max
PID_State accy_state		= {0};				        //accy״̬����
PID_State adc_state 		= {0};				        //adc״̬����
PID_State Left_Speed_State 	= {0};				//�����ٶ�״̬����
PID_State Right_Speed_State = {0};				//�ҵ���ٶ�״̬����

int TargetSpeed = 10000;

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
void Speed_Ctrl_mid(long adc_target)
{
	//��ȡ��ʵadc
	adc_state.actual = (long) Get_Regularized_Signal_Data(All_Signal_Data);
	//adc_state.actual = 0;
	//��ȡĿ��adc
	adc_state.target = adc_target;

	pid_location(&PID_adc,&adc_state);

}

// �⻷
void Speed_Ctrl_out(long Left_Speed,long Right_Speed)
{
	//��ȡ��ʵ�ٶ�*MAXSPEED/MAXENCODER
	Left_Speed_State.actual = get_EncoderL();
	Right_Speed_State.actual = get_EncoderR();
	
	//��ȡĿ���ٶ�
	Left_Speed_State.target = Left_Speed * MAXENCODER_L / MAXSPEED;
	Right_Speed_State.target = Right_Speed * MAXENCODER_R / MAXSPEED;
	
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

void Set_Motors(long left, long right)
{
	Set_Lmotor_Speed(left * MAXSPEED / MAXENCODER_L);
	Set_Rmotor_Speed(right * MAXSPEED / MAXENCODER_R);
}

void Stop_Car(void)		// С��ֹͣ
{
	TargetSpeed = 0;
}


