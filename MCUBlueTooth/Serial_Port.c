#include "Serial_Port.H"
#include <intrins.h>

//变量定义
unsigned char xdata Recv_Buf[RECV_BUF_LEN]; //串行口1环形接收数据缓冲区
unsigned char xdata Trans_Buf[TRANS_BUF_LEN]; //串行口1环形发送数据缓冲区
unsigned char data Recv_In_Pointer, Recv_Out_Pointer; //串行口1环形接收缓冲区输入、输出指针。全局变量
unsigned char data Trans_In_Pointer, Trans_Out_Pointer; //串行口1环形发送缓冲区输入、输出指针。全局变量
unsigned char data Recv_Data, Trans_Data; //串行口1接收、发送缓冲器。全局变量
unsigned char bdata Uart_Flag; //串行口1工作标志
unsigned char data Recv_End; //串行口1接收完成标志。全局变量
unsigned char data Trans_End; //串行口1发送完成标志。全局变量


//函数定义

unsigned char Ascii_To_Hex(unsigned char Ascii_Char);
void UartInit(void);
unsigned char Recv_2Byte(void);
unsigned int Recv_4Byte(void);
void Send_2Byte(unsigned char Temp_Send_Data);
void Send_4Byte(unsigned int Temp_Send_Data16);
void Send_6Byte(unsigned long Temp_Send_Data32);
void Send_8Byte(unsigned long Temp_Send_Data32);

unsigned char Serial_In(void);
void Serial_Out(unsigned char Serial_Out_Data);
void String_Out(unsigned char *String_Pointer);
unsigned char Hex_To_Ascii(unsigned char Hex_Data);

unsigned char getchar(void);
unsigned char _getkey(void);
void putchar(unsigned char Out_Char);


void Uchar_To_Hex_String(unsigned char Hex_Data,char *String_Pointer);
void Uint_To_Hex_String(unsigned int Hex_Data,char *String_Pointer);
void Ulong_To_Hex_String(unsigned long Hex_Data,char *String_Pointer);
void Hex_Array_To_Hex_String(char *Hex_Array_Pointer,char *String_Pointer,unsigned char Array_Len);
void Uint_To_Dec_String(unsigned int Hex_Data,char *String_Pointer);


unsigned char getchar(void)
{
    unsigned char In_Char;
    In_Char=Serial_In();
    Serial_Out(In_Char);
    return In_Char;
}

unsigned char _getkey(void)
{
    unsigned char In_Char;
    In_Char=Serial_In();
    return In_Char;
}
void putchar(unsigned char Out_Char)
{
    Serial_Out(Out_Char);
}



//串口1初始化
void UartInit(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//清除定时器1模式位
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xDC;		//设定定时初值
	TH1 = 0xDC;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1

    EA = 1; //开总中断   

		
	TI=1;     //必须添加！！！！！！!

    Trans_End = 1;
    Recv_End = 0;
    Recv_In_Pointer = 0; //接收缓冲区输入、输出指针
    Recv_Out_Pointer = Recv_In_Pointer;
    Trans_In_Pointer = 0; //发送缓冲区输入、输出指针
    Trans_In_Pointer = Trans_Out_Pointer;
}

//*****************************************************************************
/******************************************************************************
函数名称:		serial_int 
功能描述:		串行口中断服务子程序
入口参数:		无
返回值:   	无
调用函数:		无
全局变量：	trans_end;recv_end;
recv_in_point;recv_out_point;
recv_buf[];
trans_in_point;trans_out_point;
trans_buf[]; 	
局部变量:		无 
******************************************************************************/

/*void Serial_Int(void) interrupt 4  //串行口1中断服务程序
{
    if (_testbit_(RI))
    {
        // 接收中断
        Recv_Buf[Recv_In_Pointer] = SBUF; //将接收数据存入环形接收缓冲区
        Recv_Data = Recv_Buf[Recv_In_Pointer];
        Recv_In_Pointer = (Recv_In_Pointer + 1) % RECV_BUF_LEN;
        Recv_End = 1; //置串行口接收完成标志
    }
    if (_testbit_(TI))
    {
        // 发送中断
        Trans_Out_Pointer = (Trans_Out_Pointer + 1) % TRANS_BUF_LEN;
        if (Trans_In_Pointer == Trans_Out_Pointer)
        {
		    //发送缓冲区中没有数据
            Trans_End = 1; //置串行口发送完成标志
        }
        else
        {
		    //发送缓冲区中有数据
            SBUF = Trans_Buf[Trans_Out_Pointer]; //发送缓冲区中的数据
            Trans_End = 0; //清串行口发送完成标志
        }
    }
}*/


//从串行口输入一个字节
unsigned char Serial_In(void)
{
    unsigned char Temp_Data;
    
    while (Recv_Out_Pointer == Recv_In_Pointer){}//没有数据等待。
    
    Temp_Data = Recv_Buf[Recv_Out_Pointer]; //从接收缓冲区中取数据
    Recv_Out_Pointer = (Recv_Out_Pointer + 1) % RECV_BUF_LEN; //
    if (Recv_Out_Pointer == Recv_In_Pointer)
    {
        Recv_End = 0;
    }

    return Temp_Data;    //返回接收数据

}

/******************************************************************************
函数名称:        Serial_Out 
功能描述:        从串行口输出一个字节,如输出缓冲器中有数据则写入缓冲器;没有则直接发送
入口参数:        待发送的数据
返回值:       无
调用函数:        无
全局变量：    trans_end;trans_in_pointer;trans_out_pointer;trans_buf[];    
 ******************************************************************************/
void Serial_Out(unsigned char Serial_Out_Data)
{

    while (((Trans_In_Pointer + 1) % TRANS_BUF_LEN) == Trans_Out_Pointer)
    {
        //发送缓冲区满时等待
    }
    if (Trans_End == 1)
    {
        SBUF = Serial_Out_Data;
        Trans_Buf[Trans_In_Pointer] = Serial_Out_Data; //发送数据进入发送缓冲区(在中断程序发送数据)
        Trans_In_Pointer = (Trans_In_Pointer + 1) % TRANS_BUF_LEN; //发送数据加1
        Trans_End = 0; //清发送结束标志
    }
    else
    {
        Trans_Buf[Trans_In_Pointer] = Serial_Out_Data; //发送数据进入发送缓冲区(在中断程序发送数据)
        Trans_In_Pointer = (Trans_In_Pointer + 1) % TRANS_BUF_LEN; //发送数据加1
    }



}
//从串行口输出一个字符串
void String_Out(unsigned char *String_Pointer)
{
    while (1)
    {
        if (*String_Pointer != 0x00)
        {
            Serial_Out(*String_Pointer);
            String_Pointer++;
        }
        else
        {
            break;
        }
    }
}

/******************************************************************************
功能描述:   将Ascii字符转换成十六进制数
入口参数:   Ascii字符        
返回值:     十六进制数 
 ******************************************************************************/
unsigned char Ascii_To_Hex(unsigned char Ascii_Char)
{
    unsigned char Temp_data = 0xff;
    if (Ascii_Char >= '0' && Ascii_Char <= '9')
    {
        Temp_data = Ascii_Char - '0';
    }
    if (Ascii_Char >= 'a' && Ascii_Char <= 'f')
    {
        Temp_data = Ascii_Char - 'a' + 10;
    }
    if (Ascii_Char >= 'A' && Ascii_Char <= 'F')
    {
        Temp_data = Ascii_Char - 'A' + 10;
    }

    return Temp_data;
}

/******************************************************************************
功能描述:   将数字0x00至0x0f转换成Ascii码，0x0a至0x0f转换成a至f(小写字母)
入口参数:   待转换的十六进制数
返回值:     返回待转换的十六进制数的Ascii码
 ******************************************************************************/
unsigned char Hex_To_Ascii(unsigned char Hex_Data) //ok
{
    unsigned char Ascii_Code;
    if (Hex_Data <= 9)
    {
        Ascii_Code = '0'+ Hex_Data ;
    }
    else
    {
        Ascii_Code = 'a'+ (Hex_Data -10); //小写字母(大写'A')
    }
    return Ascii_Code;
}

/******************************************************************************
功能描述:   将一个字节转换成两个Ascii码,高位在前、低位在后从串行口输出
入口参数:   待发送的字节
返回值:     无
 ******************************************************************************/
void Send_2Byte(unsigned char Temp_Send_Data) //ok
{
    unsigned char Temp_Data;

    Temp_Data = Temp_Send_Data >> 4;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = Temp_Send_Data &0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);
}


//*****************************************************************************
/******************************************************************************
功能描述:   将两个字节转换成四个Ascii码,高位在前、低位在后从串行口输出
入口参数:   待发送的字节
返回值:     无
 ******************************************************************************/
void Send_4Byte(unsigned int Temp_Send_Data16) //ok
{
    unsigned char Temp_Data;

    Temp_Data = (Temp_Send_Data16>>12)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data16>>8)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data16>>4)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);
    
    Temp_Data = (Temp_Send_Data16>>0)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);
}

/******************************************************************************
功能描述:   将三个字节转换成六个Ascii码,高位在前、低位在后从串行口输出
入口参数:   待发送的字节
返回值:     无
 ******************************************************************************/
void Send_6Byte(unsigned long Temp_Send_Data32) //ok
{
    unsigned char Temp_Data;

    Temp_Data = (Temp_Send_Data32>>20)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data32>>16)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data32>>12)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data32>>8)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data32>>4)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);
    
    Temp_Data = (Temp_Send_Data32>>0)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);
}

//*****************************************************************************

/******************************************************************************
功能描述:   将4个字节转换成8个Ascii码,高位在前、低位在后从串行口输出
入口参数:   待发送的字节
返回值:     无
 ******************************************************************************/
void Send_8Byte(unsigned long Temp_Send_Data32) //ok
{
    unsigned char Temp_Data;

    Temp_Data = (Temp_Send_Data32>>28)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data32>>24)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data32>>20)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data32>>16)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data32>>12)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data32>>8)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);

    Temp_Data = (Temp_Send_Data32>>4)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);
    
    Temp_Data = (Temp_Send_Data32>>0)&0x0f;
    Temp_Data = Hex_To_Ascii(Temp_Data);
    Serial_Out(Temp_Data);
}

//*****************************************************************************




/******************************************************************************
功能描述:   从串行口接收两位Ascii字符(高位在前、低位在后),并转换成十六进制数返回
入口参数:   无        
返回值:     十六进制数 
 ******************************************************************************/
unsigned char Recv_2Byte(void)
{
    unsigned char Temp_Recv_Data, Temp_Data_H, Temp_Data_L;

    Temp_Data_H = Serial_In();
    Temp_Recv_Data = (Ascii_To_Hex(Temp_Data_H)<<4);
    Temp_Data_L = Serial_In();
    Temp_Recv_Data = Temp_Recv_Data|Ascii_To_Hex(Temp_Data_L);

    return Temp_Recv_Data;
}


/******************************************************************************
功能描述:   从串行口接收4位Ascii字符(高位在前、低位在后),并转换成十六进制数返回
入口参数:   无        
返回值:     十六进制数 
 ******************************************************************************/
unsigned int Recv_4Byte(void)
{
    unsigned char Temp_Data;
    unsigned int Temp_Recv_Data=0;

    Temp_Data = Serial_In();
    Temp_Recv_Data =(Temp_Recv_Data|Ascii_To_Hex(Temp_Data))<<4;
    
    Temp_Data = Serial_In();
    Temp_Recv_Data =(Temp_Recv_Data|Ascii_To_Hex(Temp_Data))<<4;

    Temp_Data = Serial_In();
    Temp_Recv_Data =(Temp_Recv_Data|Ascii_To_Hex(Temp_Data))<<4;

    Temp_Data = Serial_In();
    Temp_Recv_Data =(Temp_Recv_Data|Ascii_To_Hex(Temp_Data))<<0;
    
    return Temp_Recv_Data;
    
}


void Uchar_To_Hex_String(unsigned char Hex_Data,char *String_Pointer)
{
	unsigned char i,Temp_Data;

	for (i=0;i<2;i++)
	{
		Temp_Data=(Hex_Data>>((1-i)*4))&0x0f;
		*(String_Pointer+i)	=Hex_To_Ascii(Temp_Data);
	}
	*(String_Pointer+i)=0x00;	
}

void Uint_To_Hex_String(unsigned int Hex_Data,char *String_Pointer)
{
	unsigned char i,Temp_Data;

	for (i=0;i<4;i++)
	{
		Temp_Data=(Hex_Data>>((3-i)*4))&0x0f;
		*(String_Pointer+i)	=Hex_To_Ascii(Temp_Data);
	}
	*(String_Pointer+i)=0x00;	
}

void Ulong_To_Hex_String(unsigned long Hex_Data,char *String_Pointer)
{
	unsigned char i,Temp_Data;

	for (i=0;i<8;i++)
	{
		Temp_Data=(Hex_Data>>((7-i)*4))&0x0f;
		*(String_Pointer+i)	=Hex_To_Ascii(Temp_Data);
	}
	*(String_Pointer+i)=0x00;	
}

void Hex_Array_To_Hex_String(char *Hex_Array_Pointer,char *String_Pointer,unsigned char Array_Len)
{
	unsigned char i,Temp_Data;

	for (i=0;i<Array_Len;i++)
	{
		Temp_Data=*(Hex_Array_Pointer+i);
		*(String_Pointer+i*2)	=Hex_To_Ascii(Temp_Data>>4);
		*(String_Pointer+i*2+1)	=Hex_To_Ascii(Temp_Data&0x0f);
	}
	*(String_Pointer+Array_Len*2)=0x00;	
    
}

void Uint_To_Dec_String(unsigned int Hex_Data,char *String_Pointer)
{
	unsigned int i,Temp_Data,Base;
	Base=10;
	for (i=0;i<2;i++)
	{
		Temp_Data=Hex_Data/Base;
		*(String_Pointer+i)	=Hex_To_Ascii(Temp_Data);
		Hex_Data=(Hex_Data%Base);
		Base=Base/10;
	}
	*(String_Pointer+i)=0x00;	
}




