#include "cevent.h"

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "config.h"

#define MAXENCODER 		(1650)	//10ms��1024�߱���������ȡ������
#define Encode_L		P04
#define Encode_R		P06
#define Dir_Encode_L	P20
#define Dir_Encode_R	P22

//����ԭ��
void Encoder_init();
uint16 get_EncoderL();
uint16 get_EncoderR();

// �ڲ���������	�û��������
void EncoderL();
void EncoderR();


#endif