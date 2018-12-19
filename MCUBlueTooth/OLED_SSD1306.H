#ifndef __OLED_SSD1306_H_
#define __OLED_SSD1306_H_
#include <STC15F2K60S2.H>
#include <ctype.h>
#include <stdio.h>
#include <intrins.h>
#include <math.h>
#include "Serial_Port.H"


//7、OLED模块硬件连接：
#define  OLED_CS   P60
#define  OLED_RST 	P62 
#define  OLED_DC 	P61 
#define  OLED_SCLK 	P64 //D0
#define  OLED_SDIN 	P63 //D1

//#define OLED_Module0 		0
//#define OLED_Module1 		1
//#define OLED_Module2		2
//#define OLED_Module3		3
//#define OLED_Module_All		4


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define MAX_X				128
#define MAX_Y				64


//延时函数
extern void Delay1ms(void);
extern void Delay_Ms(unsigned int Time_Ms_Val);

//OLED控制用函数
extern void Init_OLED_SSD1306(void);
extern void Clear_OLED_Display(void);
extern void Full_Image_Ram(unsigned char Full_Data);

extern void SSD1306_Spi_Write(unsigned char Write_Data,unsigned char Cmd_Dat_Flag);
extern void OLED_Set_Position(unsigned char x, unsigned char y);
extern unsigned char Read_Font_Bit_Form_Image_Ram(unsigned char x,unsigned char y);
extern void Refresh_Image_Ram_To_OLED_Display(void);
extern void Refresh_Part_Image_Ram_To_OLED_Display(unsigned char x,unsigned char y,unsigned char M,unsigned char N);
extern void Reverse_Image_Ram_Data(unsigned char x,unsigned char y,unsigned char M,unsigned char N);

extern void Write_Point_To_Image_Ram(unsigned char x, unsigned char y, unsigned char Font_Bit );
extern void Write_HZ_ASC_String_TO_OLED_SSD1306(unsigned char Font_Num,unsigned char x,unsigned char y,unsigned char *HZ_ASC_String_Pointer);
extern void Write_Bmp_MxN_To_Image_Ram(unsigned char x,unsigned char y,unsigned char M,unsigned char N,unsigned char *Bmp_Pointer);

extern unsigned char  Image_Ram[8][128];


#endif
