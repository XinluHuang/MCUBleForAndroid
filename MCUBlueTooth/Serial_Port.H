#ifndef __Serial_Port_H
#define __Serial_Port_H	 

#include <STC15F2K60S2.H>
//#include <stdio.h> 		  //��Ƭ��C����ͷ�ļ�
#include <stdlib.h>		  //��Ƭ��C����ͷ�ļ�
#include <ctype.h> 		  //��Ƭ��C����ͷ�ļ�
#include <string.h>		  //��Ƭ��C����ͷ�ļ�
#include <float.h>		  //��Ƭ��C����ͷ�ļ�
#include <absacc.h>		  //��Ƭ��C����ͷ�ļ�
#include <math.h>		  //��Ƭ��C����ͷ�ļ�
#include <intrins.h>	  //��Ƭ��C����ͷ�ļ�


#define CR  	0x0d	        //�س�
#define LF  	0x0a	        //����
#define Ctrl_Z 	0x1a	        //Ctrl+Z ��
#define ESC 	0x1b	        //�˳���Escape����
#define TAB 	0x09	        //�Ʊ�����TAB����
//#define NULL 	0x00

//***************************�û����ô��пڲ�����ʼ****************************
#define RECV_BUF_LEN	64//���ý��ջ��������� 
#define TRANS_BUF_LEN	64	//���÷��ͻ��������� 
#define MAIN_FOSC		11059200UL	//������ʱ��
//#define MAIN_FOSC		12000000UL	//������ʱ��




//��������
extern unsigned char xdata Recv_Buf[RECV_BUF_LEN]; //���п�1���ν������ݻ�����
extern unsigned char xdata Trans_Buf[TRANS_BUF_LEN]; //���п�1���η������ݻ�����
extern unsigned char data Recv_In_Pointer, Recv_Out_Pointer; //���п�1���ν��ջ��������롢���ָ�롣ȫ�ֱ���
extern unsigned char data Trans_In_Pointer, Trans_Out_Pointer; //���п�1���η��ͻ��������롢���ָ�롣ȫ�ֱ���
extern unsigned char data Recv_Data, Trans_Data; //���п�1���ա����ͻ�������ȫ�ֱ���
extern unsigned char bdata Uart_Flag; //���п�1������־
extern unsigned char data Recv_End; //���п�1������ɱ�־��ȫ�ֱ���
extern unsigned char data Trans_End; //���п�1������ɱ�־��ȫ�ֱ���

 
//��������

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


