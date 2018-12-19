#include "Serial_Port.H"
#include <intrins.h>

//��������
unsigned char xdata Recv_Buf[RECV_BUF_LEN]; //���п�1���ν������ݻ�����
unsigned char xdata Trans_Buf[TRANS_BUF_LEN]; //���п�1���η������ݻ�����
unsigned char data Recv_In_Pointer, Recv_Out_Pointer; //���п�1���ν��ջ��������롢���ָ�롣ȫ�ֱ���
unsigned char data Trans_In_Pointer, Trans_Out_Pointer; //���п�1���η��ͻ��������롢���ָ�롣ȫ�ֱ���
unsigned char data Recv_Data, Trans_Data; //���п�1���ա����ͻ�������ȫ�ֱ���
unsigned char bdata Uart_Flag; //���п�1������־
unsigned char data Recv_End; //���п�1������ɱ�־��ȫ�ֱ���
unsigned char data Trans_End; //���п�1������ɱ�־��ȫ�ֱ���


//��������

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



//����1��ʼ��
void UartInit(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�����ʱ��1ģʽλ
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xDC;		//�趨��ʱ��ֵ
	TH1 = 0xDC;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1

    EA = 1; //�����ж�   

		
	TI=1;     //������ӣ�����������!

    Trans_End = 1;
    Recv_End = 0;
    Recv_In_Pointer = 0; //���ջ��������롢���ָ��
    Recv_Out_Pointer = Recv_In_Pointer;
    Trans_In_Pointer = 0; //���ͻ��������롢���ָ��
    Trans_In_Pointer = Trans_Out_Pointer;
}

//*****************************************************************************
/******************************************************************************
��������:		serial_int 
��������:		���п��жϷ����ӳ���
��ڲ���:		��
����ֵ:   	��
���ú���:		��
ȫ�ֱ�����	trans_end;recv_end;
recv_in_point;recv_out_point;
recv_buf[];
trans_in_point;trans_out_point;
trans_buf[]; 	
�ֲ�����:		�� 
******************************************************************************/

/*void Serial_Int(void) interrupt 4  //���п�1�жϷ������
{
    if (_testbit_(RI))
    {
        // �����ж�
        Recv_Buf[Recv_In_Pointer] = SBUF; //���������ݴ��뻷�ν��ջ�����
        Recv_Data = Recv_Buf[Recv_In_Pointer];
        Recv_In_Pointer = (Recv_In_Pointer + 1) % RECV_BUF_LEN;
        Recv_End = 1; //�ô��пڽ�����ɱ�־
    }
    if (_testbit_(TI))
    {
        // �����ж�
        Trans_Out_Pointer = (Trans_Out_Pointer + 1) % TRANS_BUF_LEN;
        if (Trans_In_Pointer == Trans_Out_Pointer)
        {
		    //���ͻ�������û������
            Trans_End = 1; //�ô��пڷ�����ɱ�־
        }
        else
        {
		    //���ͻ�������������
            SBUF = Trans_Buf[Trans_Out_Pointer]; //���ͻ������е�����
            Trans_End = 0; //�崮�пڷ�����ɱ�־
        }
    }
}*/


//�Ӵ��п�����һ���ֽ�
unsigned char Serial_In(void)
{
    unsigned char Temp_Data;
    
    while (Recv_Out_Pointer == Recv_In_Pointer){}//û�����ݵȴ���
    
    Temp_Data = Recv_Buf[Recv_Out_Pointer]; //�ӽ��ջ�������ȡ����
    Recv_Out_Pointer = (Recv_Out_Pointer + 1) % RECV_BUF_LEN; //
    if (Recv_Out_Pointer == Recv_In_Pointer)
    {
        Recv_End = 0;
    }

    return Temp_Data;    //���ؽ�������

}

/******************************************************************************
��������:        Serial_Out 
��������:        �Ӵ��п����һ���ֽ�,���������������������д�뻺����;û����ֱ�ӷ���
��ڲ���:        �����͵�����
����ֵ:       ��
���ú���:        ��
ȫ�ֱ�����    trans_end;trans_in_pointer;trans_out_pointer;trans_buf[];    
 ******************************************************************************/
void Serial_Out(unsigned char Serial_Out_Data)
{

    while (((Trans_In_Pointer + 1) % TRANS_BUF_LEN) == Trans_Out_Pointer)
    {
        //���ͻ�������ʱ�ȴ�
    }
    if (Trans_End == 1)
    {
        SBUF = Serial_Out_Data;
        Trans_Buf[Trans_In_Pointer] = Serial_Out_Data; //�������ݽ��뷢�ͻ�����(���жϳ���������)
        Trans_In_Pointer = (Trans_In_Pointer + 1) % TRANS_BUF_LEN; //�������ݼ�1
        Trans_End = 0; //�巢�ͽ�����־
    }
    else
    {
        Trans_Buf[Trans_In_Pointer] = Serial_Out_Data; //�������ݽ��뷢�ͻ�����(���жϳ���������)
        Trans_In_Pointer = (Trans_In_Pointer + 1) % TRANS_BUF_LEN; //�������ݼ�1
    }



}
//�Ӵ��п����һ���ַ���
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
��������:   ��Ascii�ַ�ת����ʮ��������
��ڲ���:   Ascii�ַ�        
����ֵ:     ʮ�������� 
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
��������:   ������0x00��0x0fת����Ascii�룬0x0a��0x0fת����a��f(Сд��ĸ)
��ڲ���:   ��ת����ʮ��������
����ֵ:     ���ش�ת����ʮ����������Ascii��
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
        Ascii_Code = 'a'+ (Hex_Data -10); //Сд��ĸ(��д'A')
    }
    return Ascii_Code;
}

/******************************************************************************
��������:   ��һ���ֽ�ת��������Ascii��,��λ��ǰ����λ�ں�Ӵ��п����
��ڲ���:   �����͵��ֽ�
����ֵ:     ��
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
��������:   �������ֽ�ת�����ĸ�Ascii��,��λ��ǰ����λ�ں�Ӵ��п����
��ڲ���:   �����͵��ֽ�
����ֵ:     ��
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
��������:   �������ֽ�ת��������Ascii��,��λ��ǰ����λ�ں�Ӵ��п����
��ڲ���:   �����͵��ֽ�
����ֵ:     ��
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
��������:   ��4���ֽ�ת����8��Ascii��,��λ��ǰ����λ�ں�Ӵ��п����
��ڲ���:   �����͵��ֽ�
����ֵ:     ��
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
��������:   �Ӵ��пڽ�����λAscii�ַ�(��λ��ǰ����λ�ں�),��ת����ʮ������������
��ڲ���:   ��        
����ֵ:     ʮ�������� 
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
��������:   �Ӵ��пڽ���4λAscii�ַ�(��λ��ǰ����λ�ں�),��ת����ʮ������������
��ڲ���:   ��        
����ֵ:     ʮ�������� 
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




