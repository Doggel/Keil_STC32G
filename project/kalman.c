#include "kalman.h"

Kalman_Typedef Speed_Kalman_L,Speed_Kalman_R;
float KalmanFilter(Kalman_Typedef *klm, float input)
{
    //Ԥ��Э����̣�kʱ��ϵͳ��������Э���� = �� 1 - ���������棩  * k-1ʱ�̵�ϵͳ����Э���� + ��������Э����
    klm->Now_P = (1-klm->Kg) * klm->LastP + klm->Q;
    //���������淽�̣����������� = kʱ��ϵͳ��������Э���� / ��kʱ��ϵͳ��������Э���� + �۲�����Э���
    klm->Kg = klm->Now_P / (klm->Now_P + klm->R);
    //��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
    klm->out = klm->out + klm->Kg * (input -klm->out);//��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
    //����Э�����: ���ε�ϵͳЭ����� klm->LastP Ϊ��һ������׼����
    klm->LastP = klm->Now_P;
	
	return (klm->out);
}

void Kalman_Init(Kalman_Typedef *klm, const float klm_Q, const float klm_R)//�¶�klm_Q=0.01 klm_R=0.25
{
	klm->LastP=0.02;		//�ϴι���Э����
	klm->Now_P=0;			//��ǰ����Э����
	klm->out=0;				//�������˲������
	klm->Kg=0;				//����������
	klm->Q=klm_Q;			//Q:��������Э���� Q�������˲��������ƽ���̶ȣ�QԽСԽƽ��;
	klm->R=klm_R;			//R:�۲�����Э���� R���������˲����������ʵ�����ߵ�����̶ȣ�RԽСԽ�ӽ�(����Խ��)
}

