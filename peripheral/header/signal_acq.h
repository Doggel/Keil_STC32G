#ifndef __SIGNAL_ACQ_H__
#define __SIGNAL_ACQ_H__

#include "config.h"
#define CHANNEL_NUM         4           //һ����Ҫ�ɼ���ͨ����Ŀ
#define CONVERT_TIMES       4           //DMA_ADC�ɼ��Ĵ���
#define MAX_MESU         3000           //С���ܲɼ�������ѹ
#define MIN_MESU         300          //С���ܲɼ�����С��ѹ
#define MAX_BIAS          100           //��׼�����ʾ��ѹ��С�Ĳ������ֵ
#define MIN_BIAS          (-100)        //��׼�����ʾ��ѹ��С����Сֵ

#define STANDERD            (1500)    //�м��вɼ��Ĵ���ת���ѹ
                                  
#define REF_VOLTAGE         (3.3)         //�ο���ѹ
#define CAPTURE_LIMIT       (4095.0)

extern uint16 All_Signal_Data[CHANNEL_NUM];

int32 Get_Regularized_Signal_Data(const uint16 * Data_Array);
void Sample_All_Chanel();
uint16 Get_DMA_ADC_Result(uint8 channel);
void Signal_Init(void);
float Get_Voltage();
#endif
