/*==========�������ͷ�ļ�============*/

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "config.h"

#define MAXSPEED 	(10000)
#define SAFESPEED 	(10000)

#define	PWM_Peri			65535	// PWM�����趨
#define Lmotor_direction	P35		// ���������������
#define Rmotor_direction	P70		// �ҵ�������������
#define Lmotor_PWM			P14		// ����PWM
#define Rmotor_PWM			P16		// �ҵ��PWM
// �������ṹ��
typedef struct {
    u8 L_pwm_channel; 		// PWM���ͨ��
	u8 R_pwm_channel; 		// PWM���ͨ��
} Motor;

// �ڲ���������	�û��������
void GPIO_PU_Init();


//����ԭ��
void motor_init(void);					//��ʼ�����
void Set_Lmotor_Speed(long speed);		//���������ٶ�
void Set_Rmotor_Speed(long speed);		//�����ҵ���ٶ�	


#endif
