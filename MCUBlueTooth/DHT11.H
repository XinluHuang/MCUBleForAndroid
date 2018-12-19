#ifndef __DHT11_H_
#define __DHT11_H_


#include <STC15F2K60S2.H>		//STC15W单片机芯片硬件定义头文件
#include <stdio.h> 		  		//单片机C语言头文件
#include <stdlib.h>		  		//单片机C语言头文件
#include <ctype.h> 		  		//单片机C语言头文件
#include <string.h>		  		//单片机C语言头文件
#include <float.h>		  		//单片机C语言头文件
#include <absacc.h>		  		//单片机C语言头文件
#include <math.h>		  		//单片机C语言头文件
#include <intrins.h>	  		//单片机C语言头文件  

#define DHT11_OK	1
#define DHT11_ERR	0

extern void Delay_Ms(unsigned int n);
extern void Init_DHT11(void);
extern unsigned char Read_DHT11_Byte(unsigned char *Dtat_Pointer);
extern unsigned char Read_DHT11_Convert_Value(unsigned int *Data_Pointer);
extern void DHT11_Test(void);

#endif