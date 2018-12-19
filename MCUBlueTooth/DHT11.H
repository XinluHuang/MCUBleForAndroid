#ifndef __DHT11_H_
#define __DHT11_H_


#include <STC15F2K60S2.H>		//STC15W��Ƭ��оƬӲ������ͷ�ļ�
#include <stdio.h> 		  		//��Ƭ��C����ͷ�ļ�
#include <stdlib.h>		  		//��Ƭ��C����ͷ�ļ�
#include <ctype.h> 		  		//��Ƭ��C����ͷ�ļ�
#include <string.h>		  		//��Ƭ��C����ͷ�ļ�
#include <float.h>		  		//��Ƭ��C����ͷ�ļ�
#include <absacc.h>		  		//��Ƭ��C����ͷ�ļ�
#include <math.h>		  		//��Ƭ��C����ͷ�ļ�
#include <intrins.h>	  		//��Ƭ��C����ͷ�ļ�  

#define DHT11_OK	1
#define DHT11_ERR	0

extern void Delay_Ms(unsigned int n);
extern void Init_DHT11(void);
extern unsigned char Read_DHT11_Byte(unsigned char *Dtat_Pointer);
extern unsigned char Read_DHT11_Convert_Value(unsigned int *Data_Pointer);
extern void DHT11_Test(void);

#endif