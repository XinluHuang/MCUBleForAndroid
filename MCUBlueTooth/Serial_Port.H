#ifndef __Serial_Port_H
#define __Serial_Port_H	 

#include <STC15F2K60S2.H>
//#include <stdio.h> 		  //单片机C语言头文件
#include <stdlib.h>		  //单片机C语言头文件
#include <ctype.h> 		  //单片机C语言头文件
#include <string.h>		  //单片机C语言头文件
#include <float.h>		  //单片机C语言头文件
#include <absacc.h>		  //单片机C语言头文件
#include <math.h>		  //单片机C语言头文件
#include <intrins.h>	  //单片机C语言头文件


#define CR  	0x0d	        //回车
#define LF  	0x0a	        //换行
#define Ctrl_Z 	0x1a	        //Ctrl+Z 键
#define ESC 	0x1b	        //退出（Escape键）
#define TAB 	0x09	        //制表符（TAB键）
//#define NULL 	0x00

//***************************用户设置串行口参数开始****************************
#define RECV_BUF_LEN	64//设置接收缓冲器长度 
#define TRANS_BUF_LEN	64	//设置发送缓冲器长度 
#define MAIN_FOSC		11059200UL	//定义主时钟
//#define MAIN_FOSC		12000000UL	//定义主时钟




//变量定义
extern unsigned char xdata Recv_Buf[RECV_BUF_LEN]; //串行口1环形接收数据缓冲区
extern unsigned char xdata Trans_Buf[TRANS_BUF_LEN]; //串行口1环形发送数据缓冲区
extern unsigned char data Recv_In_Pointer, Recv_Out_Pointer; //串行口1环形接收缓冲区输入、输出指针。全局变量
extern unsigned char data Trans_In_Pointer, Trans_Out_Pointer; //串行口1环形发送缓冲区输入、输出指针。全局变量
extern unsigned char data Recv_Data, Trans_Data; //串行口1接收、发送缓冲器。全局变量
extern unsigned char bdata Uart_Flag; //串行口1工作标志
extern unsigned char data Recv_End; //串行口1接收完成标志。全局变量
extern unsigned char data Trans_End; //串行口1发送完成标志。全局变量

 
//函数定义

extern unsigned char Ascii_To_Hex(unsigned char Ascii_Char);
extern void UartInit(void);
extern unsigned char Recv_2Byte(void);
extern unsigned int Recv_4Byte(void);
extern void Send_2Byte(unsigned char Temp_Send_Data);
extern void Send_4Byte(unsigned int Temp_Send_Data16);
extern void Send_6Byte(unsigned long Temp_Send_Data32);
extern void Send_8Byte(unsigned long Temp_Send_Data32);
extern unsigned char Serial_In(void);
extern void Serial_Out(unsigned char Serial_Out_Data);
extern void String_Out(unsigned char *String_Pointer);
extern unsigned char Hex_To_Ascii(unsigned char Hex_Data);


extern void Uchar_To_Hex_String(unsigned char Hex_Data,char *String_Pointer);
extern void Uint_To_Hex_String(unsigned int Hex_Data,char *String_Pointer);
extern void Ulong_To_Hex_String(unsigned long Hex_Data,char *String_Pointer);
extern void Hex_Array_To_Hex_String(char *Hex_Array_Pointer,char *String_Pointer,unsigned char Array_Len);
extern void Uint_To_Dec_String(unsigned int Hex_Data,char *String_Pointer);
#endif



