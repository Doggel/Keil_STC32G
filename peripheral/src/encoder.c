#include "encoder.h"
#include "intrins.h"
#include "STC32G_Timer.h"
#include "timer.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      2����������Ӧ�Ķ�ʱ����GPIO��ʼ��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	�����޸ĳ�ʼ����GPIO��
//-------------------------------------------------------------------------------------------------------------------
void Encoder_init()
{
	P0_MODE_IO_PU(GPIO_Pin_4);			//P04��ʼ��Ϊ˫���
	ctimer_count_init(CTIM3_P04);		//��ʱ��3��ʼ����Ϊ�ⲿ����

	P0_MODE_IO_PU(GPIO_Pin_6);			//P06��ʼ��Ϊ˫���
	ctimer_count_init(CTIM4_P06);		//��ʱ��4��ʼ����Ϊ�ⲿ����
	
	P2_MODE_IO_PU(GPIO_Pin_0|GPIO_Pin_2);			//P20,P22��ʼ��Ϊ˫��ڼ�¼����
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ��������ļ���ֵ
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	���ú���֮��ֱ��ʹ�ñ���Encoder1_cnt, speedL
//-------------------------------------------------------------------------------------------------------------------
int16 get_EncoderL()
{
	int16 EncoderL_cnt;

	if(Dir_Encode_L)
		EncoderL_cnt = (int16)ctimer_count_read(CTIM3_P04);
	else
		EncoderL_cnt = (int16)ctimer_count_read(CTIM3_P04) * -1;
	ctimer_count_clean(CTIM3_P04);					// �������
	return EncoderL_cnt;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ�ұ������ļ���ֵ
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:	���ú���֮��ֱ��ʹ�ñ���Encoder2_cnt, speedR
//-------------------------------------------------------------------------------------------------------------------
int16 get_EncoderR()
{
	int16 EncoderR_cnt;

	if(Dir_Encode_R == 0)
		EncoderR_cnt = (int16)ctimer_count_read(CTIM4_P06);
	else
		EncoderR_cnt = (int16)ctimer_count_read(CTIM4_P06) * -1;
	ctimer_count_clean(CTIM4_P06);			// �������
	return EncoderR_cnt;
}
