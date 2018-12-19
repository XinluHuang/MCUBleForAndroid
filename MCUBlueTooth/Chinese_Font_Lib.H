#ifndef __Chinese_Font_Lib_H
#define __Chinese_Font_Lib_H

#include <STC15F2K60S2.H>
#include <ctype.h>
#include <stdio.h>
#include <intrins.h>
#include "Serial_Port.H"





sbit  Font_Lib_Chip_CS=P4^7;    //  2018实验板SST25VF016b片选
sbit  Font_Lib_Chip_SCK=P4^3;   //  2018实验板SST25VF016b时钟
sbit  Font_Lib_Chip_SI=P4^0;    //  2018实验板SST25VF016b串行输入
sbit  Font_Lib_Chip_SO=P4^1;    //  2018实验板SST25VF016b串行输出

#define  Font_Lib_Chip_HOLD 	//VCC 
#define   Font_Lib_Chip_WP 	    //	VCC 


extern void Font_Lib_Chip_Spi_Write(unsigned char Write_Data);
extern unsigned char Font_Lib_Chip_Spi_Read(void);
extern unsigned long Read_Font_Lib(unsigned char Font_Num,unsigned int FontCode,unsigned char *Data_Pointer);
extern void Init_Chinese_Font_Lib(void);


extern unsigned int Read_Font_Lib_From_C_Array(unsigned char Font_Num,unsigned char ASCII_Code,unsigned char *Data_Pointer);
extern unsigned char Font_Buffer[32*4+2];


//点阵字库首地址
//  点阵字库名_M x N_START_ADDR     			起始地址	//编码体系	字符数	

#define  	HZ_12x12_START_ADDR					0x000000	//GB2312	6763+846
#define 	HZ_16x16_START_ADDR					0x02C9D0	//GB2312	6763+846
#define  	HZ_24x24_START_ADDR					0x068190	//GB2312	6763+846
#define  	HZ_32x32_START_ADDR					0x0EDF00	//GB2312	6763+846

#define  	HZ_EXT_CHAR_6x12_START_ADDR			0x1DBE0C	//GB2312	126
#define  	ASCII_6x12_START_ADDR				0x1DBE00	//ASCII		96
#define  	ASC_Arial_12_START_ADDR				0x1DC400	//ASCII		96
#define  	ASC_Time_12_START_ADDR				0x1DCDC0	//ASCII		96

#define  	HZ_EXT_CHAR_8x16_START_ADDR			0x1DD790	//GB2312	126
#define  	ASCII_8x16_START_ADDR				0x1DD780	//ASCII		96
#define  	ASCII_5x7_START_ADDR				0x1DDF80	//ASCII		96
#define  	ASCII_7x8_START_ADDR				0x1DE280	//ASCII		96
#define  	ASC_Arial_16_START_ADDR				0x1DE580	//ASCII		96
#define  	ASC_Time_16_START_ADDR				0x1DF240	//ASCII		96

#define  	HZ_EXT_CHAR_12x24_START_ADDR		0x1DFF30	//GB2312	126
#define  	ASCII_12x24_START_ADDR				0x1DFF00	//ASCII		96
#define  	ASC_Arial_24_START_ADDR				0x1E22D0	//ASCII		96
#define  	ASC_Time_24_START_ADDR				0x1E3E90	//ASCII		96

#define  	HZ_EXT_CHAR_16x32_START_ADDR		0x1E5A90	//GB2312	126
#define  	ASCII_16x32_START_ADDR				0x1E5A50	//ASCII		96
#define  	ASC_Arial_32_START_ADDR				0x1E99D0	//ASCII		96
#define  	ASC_Time_32_START_ADDR				0x1ECA90	//ASCII		96

#define  	HZ_SPECIAL_CHAR_8x16_START_ADDR		0x1F2880	//GB2312	64



//字形编号

#define  	HZ_12x12				1 
#define 	HZ_16x16				2 
#define  	HZ_24x24				3 
#define  	HZ_32x32				4 
#define  	HZ_EXT_CHAR_6x12		5 
#define  	ASCII_6x12			    6 
#define  	ASC_Arial_12			7 
#define  	ASC_Time_12			    8 
#define  	HZ_EXT_CHAR_8x16		9 
#define  	ASCII_8x16			    10
#define  	ASCII_5x7			    11
#define  	ASCII_7x8			    12
#define  	ASC_Arial_16			13
#define  	ASC_Time_16			    14
#define  	HZ_EXT_CHAR_12x24	    15
#define  	ASCII_12x24			    16
#define  	ASC_Arial_24			17
#define  	ASC_Time_24			    18
#define  	HZ_EXT_CHAR_16x32	    19
#define  	ASCII_16x32			    20
#define  	ASC_Arial_32			21
#define  	ASC_Time_32			    22
#define  	HZ_SPECIAL_CHAR_8x16	24




#define  	ASC_7x5				30 
#define  	ASC_8x6				31
#define  	ASC_12x6			32 
#define  	ASC_14x7			33 
#define  	ASC_16x8			34 






extern unsigned char code Ascii_Font_7x5[96][7];
extern unsigned char code Ascii_Font_8x6[96][8];
extern unsigned char code Ascii_Font_12x6[96][12];
extern unsigned char code Ascii_Font_14x7[96][14];
extern unsigned char code Ascii_Font_16x8[96][16];

extern unsigned char code HZ_Bmp0[];
extern unsigned char code HZ_Bmp1[];
extern unsigned char code HZ_Bmp2[];
extern unsigned char code HZ_Bmp3[];
extern unsigned char code HZ_Bmp4[];
extern unsigned char code HZ_Bmp5[];
extern unsigned char code HZ_Bmp6[];
extern unsigned char code HZ_Bmp7[];
extern unsigned char code HZ_Bmp8[];
extern unsigned char code HZ_Bmp9[];
extern unsigned char code HZ_Bmp10[];
extern unsigned char code HZ_Bmp11[];
extern unsigned char code HZ_Bmp12[];
extern unsigned char code HZ_Bmp13[];
extern unsigned char code HZ_Bmp14[];
extern unsigned char code HZ_Bmp15[];
extern unsigned char code HZ_Bmp16[];
extern unsigned char code HZ_Bmp17[];
extern unsigned char code HZ_Bmp18[];
extern unsigned char code HZ_Bmp19[];
extern unsigned char code HZ_Bmp20[];


extern unsigned char code JDQ_Bmp00[];
extern unsigned char code JDQ_Bmp01[];
extern unsigned char code JDQ_Bmp10[];
extern unsigned char code JDQ_Bmp11[];





#endif



