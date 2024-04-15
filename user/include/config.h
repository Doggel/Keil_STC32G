/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#ifndef		__CONFIG_H
#define		__CONFIG_H

//========================================================================
//                               主时钟定义
//========================================================================
//MAIN_FOSC可选值:35000000, 33177600, 30000000, 27000000. 24000000, 22118400
//#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		24000000L	//定义主时钟
//#define MAIN_Fosc		27000000L	//定义主时钟
//#define MAIN_Fosc		 30000000L	//定义主时钟
//#define MAIN_Fosc		35000000L	//定义主时钟
#define MAIN_Fosc		33177600L	//定义主时钟	// FOSC的值设置为33.1776Mhz,
													// 使用STC-ISP软件下载的时候，
													// 此频率需要跟STC-ISP软件中的 <输入用户程序运行时的IRC频率>选项的频率一致。

//========================================================================
//                                头文件
//========================================================================
/******************************库头文件*************************************/
#include 	"type_def.h"
#include 	"stc32g.h"
#include	"STC32G_GPIO.h"
#include	"STC32G_Delay.h"
#include	"STC32G_Exti.h"
#include	"STC32G_UART.h"						
#include	"STC32G_NVIC.h"
#include	"STC32G_Switch.h"
#include	"STC32G_Timer.h"
#include    "STC32G_ADC.h"
#include 	"STC32G_SPI.h"
#include 	"STC32G_DMA.h"
#include 	"STC32G_PWM.h"
#include  	"stc32_stc8_usb.h"
#include 	"STC32G_PWM.h"

/******************************系统头文件*************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

/******************************自定义头文件*************************************/
#include "cpost.h"
#include "cevent.h"
#include "interupt.h"

//========================================================================
//                             外部函数和变量声明
//========================================================================



/******************************基本参数*************************************/
#define	PI	3.1416
#define G	9.8


#endif
