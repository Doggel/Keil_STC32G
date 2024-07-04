#include "config.h"
#include "uart.h"
#include "timer.h"

#ifndef __BOARD_H__
#define __BOARD_H__



#define EXTERNAL_CRYSTA_ENABLE 	0			// ʹ���ⲿ����0Ϊ��ʹ�ã�1Ϊʹ�ã�����ʹ���ڲ�����
#define PRINTF_ENABLE			1			// printfʹ�ܣ�0Ϊʧ�ܣ�1Ϊʹ��
#define ENABLE_IAP 				1			// ʹ�����һ�����ع��ܣ�0Ϊʧ�ܣ�1Ϊʹ��

#define DEBUG_UART 			  	UART_1
#define DEBUG_UART_BAUD 	  	115200
#define DEBUG_UART_RX_PIN  		UART1_RX_P30
#define DEBUG_UART_TX_PIN  		UART1_TX_P31
#define DEBUG_UART_TIM			TIM_2

#define SET_P54_RESRT 	  (RSTCFG |= 1<<4)	//����P54Ϊ��λ����

extern int32 sys_clk;

void board_init(void);
void DisableGlobalIRQ(void);
void EnableGlobalIRQ(void);

void beeing();

#endif 