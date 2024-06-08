#ifndef __KALMAN_H__
#define __KALMAN_H__

typedef struct
{
    /*���ö�*/
    float LastP;//�ϴι���Э����
    float Now_P;//��ǰ����Э����
    float out;//�������˲������
    float Kg;//����������
	float Q;
	float R;
}Kalman_Typedef;

extern Kalman_Typedef Speed_Kalman_L,Speed_Kalman_R;

void Kalman_Init(Kalman_Typedef *klm, const float klm_Q, const float klm_R);
double KalmanFilter(Kalman_Typedef *klm, float input);

#endif

