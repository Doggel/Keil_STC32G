/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����            */
/*---------------------------------------------------------------------*/

#include	"STC32G_Timer.h"
#include 	"gyroscope.h"
#include	"control.h"

u8 cnt = 0;
unsigned int pid1_output = 0;
unsigned int pid2_output = 0;
unsigned int pid3_output_left = 0;
unsigned int pid3_output_right = 0;
unsigned int output_left = 0;
unsigned int output_right = 0;
//========================================================================
// ����: Timer0_ISR_Handler
// ����: Timer0�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2020-09-23
//========================================================================
// 
void Timer0_ISR_Handler (void) interrupt TMR0_VECTOR		//���ж�ʱ�Ѿ������־ 
{
	output_left = pid1_output + pid2_output + pid3_output_left;
	output_right = pid1_output + pid2_output + pid3_output_right;
	Set_Motors(output_left,output_right);		// ���µ��PWM = ��׼�ٶ� + pid_in(error) + pid_mid(error) +pid_out(error) 
	cnt++;
	
//	if(cnt % 1 == 0)
//	{
//	//�ڻ�pid ����gyro���ݣ�PID, I����С��D��������΢��һ�㡣����ʹ��PD
//	//Get_gyro_gyro();		// ��ȡ���ٶ�
//	//��׼���PWM�����ã���֤��ֱ�ߣ�С����������������
//	//PID���룺y_acc(�˲����ֵ)
//	//��Ҫ����������error_in = y_acc - 0
//	//pid(error_in)
//	//��������л�������(��һ��������)
//	//
//	//Get_gyro_accdata();			// ��ȡ���ٶ�
//	
//		accy_filter();			// ��ȡ�˲����accy
//		Speed_Ctrl_in(0);		// PID��targetΪ0
//		pid1_output = accy_state.output;
//		
//	}


	if(cnt % 1 == 0)	// �л�pid adc����ÿһС�ε�ƫ�ƣ�PID
	{
		// adc��ȡ
		// error_mid = pid(error_in) - adc��ȡֵ   ��ע����ܼ�����
		// pid(error_mid)
		// PID��������⻷������(��һ��������)

		// ���µ��PWM = pid(error_mid)
		Sample_All_Chanel();
		Speed_Ctrl_mid(0);
		pid2_output = adc_state.output;
		
	}
	
	
	if(cnt % 1 == 0)	// �⻷pid �ٶȻ�����PI
	{
		// PID���룺�л��������������
		// error = �л������ - ʵ���ٶ�
		// ���µ��PWM = pid(error)
		// �������º���Ӧ��ѹ��
		Speed_Ctrl_out(200,200);
		
		pid3_output_left = Left_Speed_State.output;
		pid3_output_right = Right_Speed_State.output;
	}
	

//	if(output_left > 1500 | output_right > 1500)	// ����,��ֹPWM����
//		{
//			Stop_Car();
//		}
//		else 
//		{
//			
//		}
}

//========================================================================
// ����: Timer1_ISR_Handler
// ����: Timer1�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2020-09-23
//========================================================================
void Timer1_ISR_Handler (void) interrupt TMR1_VECTOR		//���ж�ʱ�Ѿ������־
{
	// TODO: �ڴ˴�����û�����
	P60 = ~P60;
}

//========================================================================
// ����: Timer2_ISR_Handler
// ����: Timer2�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2020-09-23
//========================================================================
void Timer2_ISR_Handler (void) interrupt TMR2_VECTOR		//���ж�ʱ�Ѿ������־
{
	AUXINTIF &= ~0x01;	// ���жϱ�־
	// TODO: �ڴ˴�����û�����
	
}

//========================================================================
// ����: Timer3_ISR_Handler
// ����: Timer3�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2020-09-23
//========================================================================
void Timer3_ISR_Handler (void) interrupt TMR3_VECTOR		//���ж�ʱ�Ѿ������־
{
	AUXINTIF &= ~0x02;	// ���жϱ�־
	// TODO: �ڴ˴�����û�����

}

//========================================================================
// ����: Timer4_ISR_Handler
// ����: Timer4�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2020-09-23
//========================================================================
void Timer4_ISR_Handler (void) interrupt TMR4_VECTOR		//���ж�ʱ�Ѿ������־
{
	AUXINTIF &= ~0x04;	// ���жϱ�־
	// TODO: �ڴ˴�����û�����
	
}
