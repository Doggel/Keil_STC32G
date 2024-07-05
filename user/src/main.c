#include "config.h"
#include "uart.h"
#include "wireless.h"
#include "control.h"
#include "timer.h"
#include "screen.h"
#include "gyroscope.h"
#include "iic.h"

#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,NULL,NULL);
#pragma userclass (near=default)

#define LED P34
int main(void)
{	
	
	uint8 send_buf[] = {0x11, 0x22, 0x33};
	uint8 read_buf[10];

	uint32 dat_len = 0;
	uint8 test_str[] = "seekfree.taobao.com\n";
	/******************************************************************/
	ceventInit();
	ceventPost(0);
//	Screen_Init();
	wireless_uart_init();
	
	Signal_Init();
	motor_init();
	Encoder_init();
	iic_init();
	beeing();
	EA = 1;
	
	icm20602_init();
	timer_init();
	printf("wireless init done\n");
	/******************************************************************/

	/******************************************************************/


	//��ʼ������
	
	/******************************************************************/

	//��ѭ��
	while(1)
	{	
		//printf("angle:%f\n",Angle_Z);
		beeing();
		//wireless_uart_send_buff("hello", 5);
		delay_ms(100);

        // ��תLED
//        LED = !LED;
//        // ��ȡfifo�е�����
//        dat_len = wireless_uart_read_buff(read_buf, 10);
//        // �����ȡ������
//        if(dat_len != 0)
//        {
//            // ����ȡ����fifo���ͳ�ȥ
//            
////        }

	}

}
