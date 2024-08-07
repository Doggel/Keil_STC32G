#include "config.h"
#include "uart.h"
#include "wireless.h"
#include "control.h"
#include "timer.h"
#include "screen.h"
#include "gyroscope.h"
#include "telemeter.h"
#include "iic.h"
#include "exti.h"
#define PRINT(a)	printf("a: %d\n",a);
#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,NULL,NULL);
#pragma userclass (near=default)
extern uint16 distance;
extern long keep_going;
extern uint8 enter_island_begin, enter_island_finish, leave_island_finish, leave_island_begin;
int start_get_distance = 0;
int main(void)
{	
	int distance_valid_count = 0;
	/******************************************************************/
	ceventInit();
	ceventPost(0);

//	Screen_Init();
	wireless_uart_init();
	
	Signal_Init();
	motor_init();
	Encoder_init();
	iic_init();
	Exti_config();
	EA = 1;

	icm20602_init();
	dl1b_init();
	timer_init();
	
	/******************************************************************/

	/******************************************************************/
	P6_MODE_OUT_PP(GPIO_Pin_1);	//����GET_ANGLE
	P3_MODE_OUT_PP(GPIO_Pin_4);	//������
	P34 = 0;
	P61 = 0;

	//��ʼ������
	/******************************************************************/
	//��ѭ��
	while(1)
	{	
		wireless_PID();
		
		if(start_get_distance % 10 == 0 && start_get_distance != 0){
			distance = dl1b_get_distance();			// ������
			start_get_distance = 0;
			if(distance > 1100 && distance < 1300){
				if(distance_valid_count++ > 25) P61 = 1;
			}
			else{
				distance_valid_count = 0;
				P61 = 0;
			}
		}
//		printf("distance:%d\n",distance);
//		printf("state:%ld,%ld,%ld,%ld,%ld,%ld,%ld,%ld\n",Left_Speed_State.actual,Right_Speed_State.actual,output_left,output_right,pid3_output_left,pid3_output_right,pid2_output,adc_state.integral);

//		printf("ADC:%d,%d,%d,%d,%d\n",distance,All_Signal_Data[0],All_Signal_Data[1],All_Signal_Data[2],All_Signal_Data[3]);
//		delay_ms(2);

//		printf("adc_state.actual:%ld\n",adc_state.actual);
//		Set_Motors(10000,10000);
//		Screen_ShowInt(0,0,BLACK,WHITE,Get_DMA_ADC_Result(0));
//		Screen_ShowInt(0,16,BLACK,WHITE,Get_DMA_ADC_Result(1));
//		Screen_ShowInt(0,32,BLACK,WHITE,Get_DMA_ADC_Result(2));
//		Screen_ShowInt(0,48,BLACK,WHITE,Get_DMA_ADC_Result(3));
//		Sample_All_Chanel();


//		Screen_ShowInt(0,64,BLACK,WHITE,Get_Regularized_Signal_Data(All_Signal_Data));
//		printf("%d\n",Get_Regularized_Signal_Data(All_Signal_Data));
//		printf("F: %d,%d,%d,%d,%d\n",Get_DMA_ADC_Result(0),Get_DMA_ADC_Result(1),Get_DMA_ADC_Result(2),Get_DMA_ADC_Result(3),Get_Regularized_Signal_Data(All_Signal_Data));
	}
}
