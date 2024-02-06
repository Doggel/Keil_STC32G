#include "screen.h"

#pragma userclass (near=CEVENT)	
	CEVENT_EXPORT(0,Screen_Init,NULL);
#pragma userclass (near=default)
	
/// @brief 默认使用硬件SPI进行数据传输，速度较快
/// @param  无
void HardWare_SPI_Config(void)
{
	SPI_InitTypeDef		SPI_InitStructure;
	SPI_InitStructure.SPI_Enable    = ENABLE;				//SPI启动
	SPI_InitStructure.SPI_SSIG      = DISABLE;				//片选位，DISABLE表示用SS脚确定是不是主机
	SPI_InitStructure.SPI_FirstBit  = SPI_MSB;				//移位方向   SPI_MSB, SPI_LSB
	SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;		//主从选择   SPI_Mode_Master, SPI_Mode_Slave
	SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;			//时钟相位   SPI_CPOL_High,   SPI_CPOL_Low
	SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;		//数据边沿   SPI_CPHA_1Edge,第一个时钟边沿采样，第二个边沿移位
	SPI_InitStructure.SPI_Speed     = SPI_Speed_4;			//SPI速度    SPI_Speed_4, SPI_Speed_8, SPI_Speed_16, SPI_Speed_2
	SPI_Init(&SPI_InitStructure);
	NVIC_SPI_Init(DISABLE,Priority_3);		//中断配置，是否使用中断
	
	SPI_SW(SPI_P22_P23_P24_P25);	//SPI_P54_P13_P14_P15,SPI_P22_P23_P24_P25,SPI_P54_P40_P41_P43,SPI_P35_P34_P33_P32
    SPI_SS_2 = 1;                   //SS置高电平，取消从机的使能
}

//向SPI总线传输一个8位数据
void  SPI_Simulate_WriteData(u8 Data)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
        if(Data&0x80)	
            SDA = 1; //输出数据
        else 
            SDA = 0;
        SCL = 0;       
        SCL = 1;
        Data<<=1; 
	}
}

void GPIO_SPI_Init(uchar GPIO_Pin)
{
    P2_MODE_IO_PU(GPIO_Pin);		//P2 设置为准双向口
}

//向液晶屏写一个8位指令
void  Screen_WriteCommand(uchar c)
{
	CS=0;
	DC=0;
	SPI_WriteByte(c);
	CS=1;    
}
//向液晶屏写一个8位数据
void  Screen_WriteData(uchar d)
{
	CS=0;
	DC=1;
	SPI_WriteByte(d);
	CS=1;
}
//向液晶屏写一个16位数据
void Screen_WriteData_16Bit(int Data)
{
	CS=0;
	DC=1;
	SPI_WriteByte(Data>>8);
	SPI_WriteByte(Data);
	CS=1;
	
}

//屏幕重置
void Reset()
{
    RST = 0;
    delay_ms(100);
    RST = 1;
    delay_ms(100);
}

//液晶屏初始化 for ST7735R
void Lcd_Init()
{	
    Reset();//Reset before LCD Init.
	BLK = 1;	
	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Screen_WriteCommand(0x11);//Sleep exit 
	delay_ms (120);
		
	//ST7735R Frame Rate
	Screen_WriteCommand(0xB1); 
	Screen_WriteData(0x01); 
	Screen_WriteData(0x2C); 
	Screen_WriteData(0x2D); 

	Screen_WriteCommand(0xB2); 
	Screen_WriteData(0x01); 
	Screen_WriteData(0x2C); 
	Screen_WriteData(0x2D); 

	Screen_WriteCommand(0xB3); 
	Screen_WriteData(0x01); 
	Screen_WriteData(0x2C); 
	Screen_WriteData(0x2D); 
	Screen_WriteData(0x01); 
	Screen_WriteData(0x2C); 
	Screen_WriteData(0x2D); 
	
	Screen_WriteCommand(0xB4); //Column inversion 
	Screen_WriteData(0x07); 
	
	//ST7735R Power Sequence
	Screen_WriteCommand(0xC0); 
	Screen_WriteData(0xA2); 
	Screen_WriteData(0x02); 
	Screen_WriteData(0x84); 
	Screen_WriteCommand(0xC1); 
	Screen_WriteData(0xC5); 

	Screen_WriteCommand(0xC2); 
	Screen_WriteData(0x0A); 
	Screen_WriteData(0x00); 

	Screen_WriteCommand(0xC3); 
	Screen_WriteData(0x8A); 
	Screen_WriteData(0x2A); 
	Screen_WriteCommand(0xC4); 
	Screen_WriteData(0x8A); 
	Screen_WriteData(0xEE); 
	
	Screen_WriteCommand(0xC5); //VCOM 
	Screen_WriteData(0x0E); 
	
	Screen_WriteCommand(0x36); //MX, MY, RGB mode 

	Screen_WriteData(0xC8); //竖屏C8 横屏08 A8
	
	//ST7735R Gamma Sequence
	Screen_WriteCommand(0xe0); 
	Screen_WriteData(0x0f); 
	Screen_WriteData(0x1a); 
	Screen_WriteData(0x0f); 
	Screen_WriteData(0x18); 
	Screen_WriteData(0x2f); 
	Screen_WriteData(0x28); 
	Screen_WriteData(0x20); 
	Screen_WriteData(0x22); 
	Screen_WriteData(0x1f); 
	Screen_WriteData(0x1b); 
	Screen_WriteData(0x23); 
	Screen_WriteData(0x37); 
	Screen_WriteData(0x00); 	
	Screen_WriteData(0x07); 
	Screen_WriteData(0x02); 
	Screen_WriteData(0x10); 

	Screen_WriteCommand(0xe1); 
	Screen_WriteData(0x0f); 
	Screen_WriteData(0x1b); 
	Screen_WriteData(0x0f); 
	Screen_WriteData(0x17); 
	Screen_WriteData(0x33); 
	Screen_WriteData(0x2c); 
	Screen_WriteData(0x29); 
	Screen_WriteData(0x2e); 
	Screen_WriteData(0x30); 
	Screen_WriteData(0x30); 
	Screen_WriteData(0x39); 
	Screen_WriteData(0x3f); 
	Screen_WriteData(0x00); 
	Screen_WriteData(0x07); 
	Screen_WriteData(0x03); 
	Screen_WriteData(0x10);  
	
	Screen_WriteCommand(0x2a);
	Screen_WriteData(0x00);
	Screen_WriteData(0x00+2);
	Screen_WriteData(0x00);
	Screen_WriteData(0x80+2);

	Screen_WriteCommand(0x2b);
	Screen_WriteData(0x00);
	Screen_WriteData(0x00+3);
	Screen_WriteData(0x00);
	Screen_WriteData(0x80+3);
	
	Screen_WriteCommand(0xF0); //Enable test command  
	Screen_WriteData(0x01); 
	Screen_WriteCommand(0xF6); //Disable ram power save mode 
	Screen_WriteData(0x00); 
	
	Screen_WriteCommand(0x3A); //65k mode 
	Screen_WriteData(0x05); 
	
	
	Screen_WriteCommand(0x29);//Display on

}

/// @brief 设置一块显示的区域
/// @param x_start 
/// @param y_start 
/// @param x_end 
/// @param y_end 
void Screen_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{	
    Screen_WriteCommand(0x2a);
    Screen_WriteData(0x00);
    Screen_WriteData(x_start+2);
    Screen_WriteData(0x00);
    Screen_WriteData(x_end+2);

    Screen_WriteCommand(0x2b);
    Screen_WriteData(0x00);
    Screen_WriteData(y_start+3);
    Screen_WriteData(0x00);
    Screen_WriteData(y_end+3);	

    Screen_WriteCommand(0x2c);
}

/// @brief 设置lcd显示起始点
/// @param x 
/// @param y 
void Screen_SetLocation(u16 x,u16 y)
{
  	Screen_SetRegion(x,y,x,y);
}
/**************************用户调用的接口函数*************************/


/// @brief 屏幕初始化
void Screen_Init(void)
{
    HardWare_SPI_Config();
    GPIO_SPI_Init(GPIO_Pin_LOW|GPIO_Pin_5|GPIO_Pin_6);
    Lcd_Init();
}

/// @brief 将指定颜色填充整个屏幕
/// @param color 颜色参数
void Fill_With_Color(int color)
{
 	uchar i,j;
	Screen_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
 	for (i=0;i<X_MAX_PIXEL;i++)
    	for (j=0;j<Y_MAX_PIXEL;j++)
        	Screen_WriteData_16Bit(color);
}
	
/// @brief 画一个点
/// @param x 横坐标
/// @param y 纵坐标
/// @param Data 
void Screen_DrawPoint(u16 x,u16 y,u16 Data)
{
	Screen_SetRegion(x,y,x+1,y+1);
	Screen_WriteData_16Bit(Data);

} 


/// @brief 以给定中心画给定半径的圆
/// @param X 
/// @param Y 
/// @param R 
/// @param font_color 
void Screen_Circle(u16 X,u16 Y,u16 R,u16 font_color) 
{
    //Bresenham算法 
    unsigned short  a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        Screen_DrawPoint(X+a,Y+b,font_color);     //        7 
        Screen_DrawPoint(X-a,Y+b,font_color);     //        6 
        Screen_DrawPoint(X+a,Y-b,font_color);     //        2 
        Screen_DrawPoint(X-a,Y-b,font_color);     //        3 
        Screen_DrawPoint(X+b,Y+a,font_color);     //        8 
        Screen_DrawPoint(X-b,Y+a,font_color);     //        5 
        Screen_DrawPoint(X+b,Y-a,font_color);     //        1 
        Screen_DrawPoint(X-b,Y-a,font_color);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        Screen_DrawPoint(X+a,Y+b,font_color); 
        Screen_DrawPoint(X+a,Y+b,font_color); 
        Screen_DrawPoint(X+a,Y-b,font_color); 
        Screen_DrawPoint(X-a,Y-b,font_color); 
        Screen_DrawPoint(X+b,Y+a,font_color); 
        Screen_DrawPoint(X-b,Y+a,font_color); 
        Screen_DrawPoint(X+b,Y-a,font_color); 
        Screen_DrawPoint(X-b,Y-a,font_color); 
    } 
	
} 


/// @brief 在显示区域中确定两个点，画一条线
/// @param x0 
/// @param y0 
/// @param x1 
/// @param y1 
/// @param Color 
void Screen_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)   
{

    int dx,             // difference in x's
        dy,             // difference in y's
        dx2,            // dx,dy * 2
        dy2, 
        x_inc,          // amount in pixel space to move during drawing
        y_inc,          // amount in pixel space to move during drawing
        error,          // the discriminant i.e. error i.e. decision variable
        index;          // used for looping	


	Screen_SetLocation(x0,y0);
	dx = x1-x0;//计算x距离
	dy = y1-y0;//计算y距离

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
	{//且线的点数等于x距离，以x轴递增画点
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
		{
			//画点
			Screen_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //是否需要增加y坐标值
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//增加y坐标值
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x坐标值每次画点后都递增1
		} // end for
	} // end if |slope| <= 1
	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
	{//以y轴为递增画点
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			Screen_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}

/// @brief 在屏幕上显示字符串
/// @param x 横坐标
/// @param y 纵坐标
/// @param font_color 字体色
/// @param background_color 背景色
/// @param string 需要显示的字符串，注意要采用GBK16编码
/// @note 注意每个字母的宽度为8个像素高度为16个像素
void Screen_Show_String(u16 x, u16 y, u16 font_color, u16 background_color, u8 *string_list)
{
	unsigned char i,j;
	unsigned short k,x0;

	x0=x;
	while(*string_list) 
	{	
		if((*string_list) < 128)                          //小于128为英文
		{
			k=*string_list;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
	
			    for(i=0;i<16;i++)
				    for(j=0;j<8;j++) 
					{
				    	if(TFT_ASCII[k*16+i]&(0x80>>j))	Screen_DrawPoint(x+j,y+i,font_color);
						else 
						{
							if (font_color!=background_color) Screen_DrawPoint(x+j,y+i,background_color);
						}
					}
				x+=8;
			}
			string_list++;
		}
		
	}
}

/// @brief 显示单个字符
/// @param x 
/// @param y 
/// @param font_color 
/// @param background_color 
/// @param dat 
void Screen_ShowChar(uint16 x,uint16 y,u16 font_color, u16 background_color,const char dat)
{
	uint8 tmp_str[2];
	
	sprintf(tmp_str, "%c", dat); // 将整数转换为字符串
	Screen_Show_String(x,y,font_color,background_color,tmp_str);
}

/// @brief 显示有16位符号整数，范围是(-32768~32767)
/// @param x 
/// @param y 
/// @param font_color 
/// @param background_color 
/// @param num 
void Screen_ShowInt(uint16 x,uint16 y,u16 font_color, u16 background_color,const int16 num)
{
	uint8 tmp_str[6];
	
	sprintf(tmp_str, "%d", num); // 将整数转换为字符串
	Screen_Show_String(x,y,font_color,background_color,tmp_str);
}

/// @brief 显示float型浮点数
/// @param x 
/// @param y 
/// @param font_color 
/// @param background_color 
/// @param flot 
void Screen_ShowFloat(uint16 x,uint16 y,u16 font_color, u16 background_color,const float flot)
{
	uint8 tmp_str[8];
	
	sprintf(tmp_str, "%f", flot); // 将整数转换为字符串
	Screen_Show_String(x,y,font_color,background_color,tmp_str);
}


uint8 xdata TFT_ASCII[] = 
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //" "
0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x10,0x10,0x00,0x00, //"!"
0x00,0x00,0x6C,0x6C,0x24,0x24,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00, //"""
0x00,0x24,0x24,0x24,0x24,0xFE,0x48,0x48,0x48,0x48,0xFC,0x90,0x90,0x90,0x90,0x00, //"#"
0x00,0x10,0x3C,0x54,0x92,0x90,0x50,0x38,0x14,0x12,0x12,0x92,0x54,0x78,0x10,0x00, //"$"
0x00,0x00,0x22,0x5C,0x94,0xA8,0x48,0x10,0x10,0x24,0x2A,0x52,0x54,0x88,0x00,0x00, //"%"
0x00,0x00,0x30,0x48,0x48,0x50,0x20,0x6E,0x54,0x94,0x8C,0x88,0x8A,0x74,0x00,0x00, //"&"
0x00,0x00,0x30,0x30,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //"'"
0x00,0x04,0x08,0x10,0x10,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0x08,0x04,0x00, //"("
0x00,0x80,0x40,0x20,0x20,0x10,0x10,0x10,0x10,0x10,0x10,0x20,0x20,0x40,0x80,0x00, //")"
0x00,0x00,0x00,0x00,0x10,0x54,0x38,0x10,0x38,0x54,0x10,0x00,0x00,0x00,0x00,0x00, //"*"
0x00,0x00,0x00,0x10,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x10,0x00,0x00,0x00,0x00, //"+"
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x20,0x00, //","
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //"-"
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x00, //"."
0x00,0x00,0x04,0x04,0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x40,0x80,0x80,0x00,0x00, //"/"
0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00, //"0"
0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00, //"1"
0x00,0x00,0x38,0x44,0x82,0x82,0x04,0x08,0x10,0x20,0x40,0x82,0x84,0xFC,0x00,0x00, //"2"
0x00,0x00,0x38,0x44,0x82,0x02,0x04,0x38,0x04,0x02,0x02,0x82,0x44,0x38,0x00,0x00, //"3"
0x00,0x00,0x04,0x0C,0x14,0x14,0x24,0x24,0x44,0x44,0xFE,0x04,0x04,0x0E,0x00,0x00, //"4"
0x00,0x00,0xFC,0x80,0x80,0x80,0xB8,0xC4,0x82,0x02,0x02,0x82,0x84,0x78,0x00,0x00, //"5"
0x00,0x00,0x3C,0x42,0x82,0x80,0xB8,0xC4,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00, //"6"
0x00,0x00,0x7E,0x42,0x82,0x04,0x04,0x08,0x08,0x08,0x10,0x10,0x10,0x10,0x00,0x00, //"7"
0x00,0x00,0x38,0x44,0x82,0x82,0x44,0x38,0x44,0x82,0x82,0x82,0x44,0x38,0x00,0x00, //"8"
0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x46,0x3A,0x02,0x82,0x44,0x38,0x00,0x00, //"9"
0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x00,0x00,0x00, //":"
0x00,0x00,0x00,0x00,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x20,0x00,0x00, //";"
0x00,0x00,0x00,0x00,0x06,0x18,0x60,0x80,0x60,0x18,0x06,0x00,0x00,0x00,0x00,0x00, //"<"
0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00, //"="
0x00,0x00,0x00,0x00,0xC0,0x30,0x0C,0x02,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00, //">"
0x00,0x38,0x44,0x82,0x82,0x02,0x04,0x08,0x10,0x10,0x10,0x00,0x10,0x10,0x00,0x00, //"?"
0x00,0x00,0x38,0x44,0x82,0x9A,0xAA,0xAA,0xAA,0xAA,0xAA,0x96,0x80,0x42,0x3C,0x00, //"@"
0x00,0x00,0x10,0x10,0x10,0x28,0x28,0x28,0x44,0x44,0x7C,0x44,0x44,0xEE,0x00,0x00, //"A"
0x00,0x00,0xFC,0x42,0x42,0x42,0x42,0x7C,0x42,0x42,0x42,0x42,0x42,0xFC,0x00,0x00, //"B"
0x00,0x00,0x3C,0x44,0x82,0x80,0x80,0x80,0x80,0x80,0x82,0x82,0x44,0x38,0x00,0x00, //"C"
0x00,0x00,0xF8,0x44,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0xF8,0x00,0x00, //"D"
0x00,0x00,0xFC,0x44,0x42,0x40,0x44,0x7C,0x44,0x40,0x40,0x42,0x44,0xFC,0x00,0x00, //"E"
0x00,0x00,0xFC,0x44,0x42,0x40,0x44,0x7C,0x44,0x40,0x40,0x40,0x40,0xF0,0x00,0x00, //"F"
0x00,0x00,0x34,0x4C,0x82,0x80,0x80,0x80,0x8E,0x84,0x84,0x84,0x4C,0x34,0x00,0x00, //"G"
0x00,0x00,0xEE,0x44,0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44,0x44,0xEE,0x00,0x00, //"H"
0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00, //"I"
0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0x88,0x70,0x00,0x00, //"J"
0x00,0x00,0xEE,0x44,0x48,0x48,0x50,0x60,0x50,0x48,0x48,0x44,0x44,0xEE,0x00,0x00, //"K"
0x00,0x00,0xE0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x42,0x44,0xFC,0x00,0x00, //"L"
0x00,0x00,0xC6,0x44,0x6C,0x6C,0x6C,0x54,0x54,0x54,0x44,0x44,0x44,0xEE,0x00,0x00, //"M"
0x00,0x00,0xCE,0x44,0x64,0x64,0x64,0x54,0x54,0x4C,0x4C,0x4C,0x44,0xE4,0x00,0x00, //"N"
0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00, //"O"
0x00,0x00,0xF8,0x44,0x42,0x42,0x42,0x44,0x78,0x40,0x40,0x40,0x40,0xE0,0x00,0x00, //"P"
0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xBA,0x44,0x3C,0x02,0x00, //"Q"
0x00,0x00,0xF0,0x48,0x44,0x44,0x44,0x48,0x70,0x48,0x44,0x44,0x44,0xE6,0x00,0x00, //"R"
0x00,0x00,0x3C,0x44,0x82,0x80,0x40,0x30,0x0C,0x02,0x02,0x82,0x44,0x78,0x00,0x00, //"S"
0x00,0x00,0x7C,0x54,0x92,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00, //"T"
0x00,0x00,0xEE,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x38,0x00,0x00, //"U"
0x00,0x00,0xEE,0x44,0x44,0x44,0x44,0x28,0x28,0x28,0x28,0x10,0x10,0x10,0x00,0x00, //"V"
0x00,0x00,0xEE,0x44,0x54,0x54,0x54,0x54,0x54,0x54,0x28,0x28,0x28,0x28,0x00,0x00, //"W"
0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x10,0x10,0x28,0x28,0x44,0x44,0xEE,0x00,0x00, //"X"
0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00, //"Y"
0x00,0x00,0x7E,0x44,0x84,0x08,0x08,0x10,0x20,0x20,0x40,0x82,0x84,0xFC,0x00,0x00, //"Z"
0x00,0x1C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1C,0x00, //"["
0x00,0x00,0xEE,0x44,0x54,0x54,0xFE,0x54,0x54,0x54,0x28,0x28,0x28,0x28,0x00,0x00, //"\"
0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x70,0x00, //"]"
0x00,0x30,0x48,0x84,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //"^"
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00, //"_"
0x00,0x40,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //"`"
0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x84,0x04,0x7C,0x84,0x84,0x8C,0x76,0x00,0x00, //"a"
0x00,0x00,0xC0,0x40,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x42,0x64,0x58,0x00,0x00, //"b"
0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x80,0x80,0x80,0x80,0x44,0x38,0x00,0x00, //"c"
0x00,0x00,0x0C,0x04,0x04,0x04,0x34,0x4C,0x84,0x84,0x84,0x84,0x4C,0x36,0x00,0x00, //"d"
0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x84,0x84,0xFC,0x80,0x80,0x84,0x78,0x00,0x00, //"e"
0x00,0x00,0x18,0x24,0x20,0x20,0xF8,0x20,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00, //"f"
0x00,0x00,0x00,0x00,0x00,0x00,0x3A,0x44,0x44,0x78,0x80,0x7C,0x82,0x82,0x7C,0x00, //"g"
0x00,0x00,0xC0,0x40,0x40,0x40,0x58,0x64,0x44,0x44,0x44,0x44,0x44,0xEE,0x00,0x00, //"h"
0x00,0x00,0x10,0x10,0x00,0x00,0x30,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00, //"i"
0x00,0x00,0x10,0x10,0x00,0x00,0x30,0x10,0x10,0x10,0x10,0x10,0x10,0x90,0x60,0x00, //"j"
0x00,0x00,0xC0,0x40,0x40,0x40,0x5C,0x48,0x50,0x60,0x50,0x48,0x44,0xEE,0x00,0x00, //"k"
0x00,0x00,0x30,0x10,0x10,0x10,0x10,0x11,0x10,0x10,0x10,0x10,0x10,0x39,0x00,0x00, //"l"
0x00,0x00,0x00,0x00,0x00,0x00,0xAC,0xD2,0x92,0x92,0x92,0x92,0x92,0xD6,0x00,0x00, //"m"
0x00,0x00,0x00,0x00,0x00,0x00,0x58,0xE4,0x44,0x44,0x44,0x44,0x44,0xEE,0x00,0x00, //"n"
0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00, //"o"
0x00,0x00,0x00,0x00,0x00,0x00,0xD8,0x64,0x42,0x42,0x42,0x64,0x58,0x40,0xE0,0x00, //"p"
0x00,0x00,0x00,0x00,0x00,0x00,0x36,0x4C,0x84,0x84,0x84,0x4C,0x34,0x04,0x0E,0x00, //"q"
0x00,0x00,0x00,0x00,0x00,0x00,0x6C,0x30,0x20,0x20,0x20,0x20,0x20,0x70,0x00,0x00, //"r"
0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x88,0x84,0x60,0x18,0x84,0x44,0x78,0x00,0x00, //"s"
0x00,0x00,0x00,0x20,0x20,0x20,0xF8,0x20,0x20,0x20,0x20,0x20,0x24,0x18,0x00,0x00, //"t"
0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0x42,0x42,0x42,0x42,0x42,0x46,0x3A,0x00,0x00, //"u"
0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x28,0x10,0x10,0x00,0x00, //"v"
0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x44,0x44,0x54,0x54,0x28,0x28,0x28,0x00,0x00, //"w"
0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x44,0x28,0x10,0x10,0x28,0x44,0xEE,0x00,0x00, //"x"
0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x10,0x10,0xA0,0xC0,0x00, //"y"
0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x44,0x88,0x10,0x20,0x42,0x84,0xFC,0x00,0x00, //"z"
0x00,0x0C,0x10,0x10,0x10,0x10,0x10,0x60,0x10,0x10,0x10,0x10,0x10,0x10,0x0C,0x00, //"{"
0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00, //"|"
0x00,0xC0,0x20,0x20,0x20,0x20,0x20,0x18,0x20,0x20,0x20,0x20,0x20,0x20,0xC0,0x00, //"}"
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x72,0x8C,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //"~"
};