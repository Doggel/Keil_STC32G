#include "cevent.h"

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "config.h"

#define MAXENCODER_R 		(1895)	//10ms��1024�߱���������ȡ������
#define MAXENCODER_L 		(1870)

#define Encode_L		P04
#define Encode_R		P06
#define Dir_Encode_L	P20
#define Dir_Encode_R	P22

//����ԭ��
void Encoder_init();
int16 get_EncoderL();
int16 get_EncoderR();

// �ڲ���������	�û��������
void EncoderL();
void EncoderR();


#endif