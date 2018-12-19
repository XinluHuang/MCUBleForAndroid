#include "OLED_SSD1306.H"
#include "Chinese_Font_Lib.H"

		
void Delay1ms(void);
void Delay_Ms(unsigned int Time_Ms_Val);

void Init_OLED_SSD1306(void);
void Clear_OLED_Display(void);
void Full_Image_Ram(unsigned char Full_Data);

void SSD1306_Spi_Write(unsigned char Write_Data,unsigned char Cmd_Dat_Flag);
void OLED_Set_Position(unsigned char x, unsigned char y);
unsigned char Read_Font_Bit_Form_Image_Ram(unsigned char x,unsigned char y);
void Refresh_Image_Ram_To_OLED_Display(void);
void Refresh_Part_Image_Ram_To_OLED_Display(unsigned char x,unsigned char y,unsigned char M,unsigned char N);

void Reverse_Image_Ram_Data(unsigned char x,unsigned char y,unsigned char M,unsigned char N);
void Write_Bmp_MxN_To_Image_Ram(unsigned char x,unsigned char y,unsigned char M,unsigned char N,unsigned char *Bmp_Pointer);
void Write_Point_To_Image_Ram(unsigned char x, unsigned char y, unsigned char Font_Bit );

void Write_HZ_ASC_String_TO_OLED_SSD1306(unsigned char Font_Num,unsigned char x,unsigned char y,unsigned char *HZ_ASC_String_Pointer);


unsigned char  Image_Ram[8][128];//ӳ��RAM

/*
 	OLEDģ��0ӳ��RAM		
	    Image_Ram[8][128]	
              |  |   |
ҳ(Page)���-+   |   +-��(Column)���  
                 |
                  

Column = 0   1   2 ............ 127
                    
		 D0  D0	 D0.............D0	
		 D1  D1  D1.............D1
		 D2  D2  D2.............D2
Page=0	 D3  D3  D3.............D3   Image_Ram[*][0][0], Image_Ram[*][0][1], ..... , Image_Ram[*][0][127]  
		 D4  D4  D4.............D4
		 D5  D5  D5.............D5
		 D6  D6  D6.............D6
		 D7  D7  D7.............D7

		 D0  D0	 D0.............D0	
		 D1  D1  D1.............D1
		 D2  D2  D2.............D2
Page=1	 D3  D3  D3.............D3   Image_Ram[*][1][0], Image_Ram[*][1][1], ..... , Image_Ram[*][1][127] 
		 D4  D4  D4.............D4
		 D5  D5  D5.............D5
		 D6  D6  D6.............D6
		 D7  D7  D7.............D7
          .   .   .     .        .
          .   .   .     .        .
          .   .   .     .        .
		 D0  D0	 D0.............D0	
		 D1  D1  D1.............D1
		 D2  D2  D2.............D2
Page=7	 D3  D3  D3.............D3   Image_Ram[*][7][0], Image_Ram[*][7][1], ..... , Image_Ram[*][7][127]  
		 D4  D4  D4.............D4
		 D5  D5  D5.............D5
		 D6  D6  D6.............D6
		 D7  D7  D7.............D7
          |   |   |              |
          |   |   |              |
Byte_Bits +---+---+--------------+
(�ֽڵ�λ)                  


����X��Y��ҳPage����Column���ֽ�λByte_Bits֮��Ĺ�ϵ�� 

Column = X  ; Page = Y/8 ;  Byte_Bits = Y%8 ;  

X = Column ; Y = Page*8 + Byte_Bits ;

*/

void Delay_Ms(unsigned int Time_Ms_Val)
{
    while (Time_Ms_Val!=0)
    {
        Delay1ms();   
        Time_Ms_Val--;
    }
}	


void Delay1ms(void)		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}




//��ָ��OLEDģ�飨SSD1306��д��һ���ֽڡ�
//Write_Data:Ҫд�������/����
//Cmd_Dat_Flag:����/�����־ 0,��ʾ����;1,��ʾ����;
void SSD1306_Spi_Write(unsigned char Write_Data,unsigned char Cmd_Dat_Flag)
{
    unsigned char i;

    if(Cmd_Dat_Flag)
    {
        OLED_DC=1;
    }
    else
    {
        OLED_DC=0;
    }
    OLED_CS = 0;
    for ( i = 0; i < 8; i++ )
    {
        OLED_SCLK = 0;
        if (Write_Data&0x80)
        {
            OLED_SDIN =1;
        }
        else
        {
            OLED_SDIN =0;
        }
        Write_Data<<=1;
        OLED_SCLK = 1;
    }
    OLED_CS = 1; 
    OLED_DC=1;
}

//OLEDģ��������ʼ����
void OLED_Set_Position(unsigned char x, unsigned char y)
{
    SSD1306_Spi_Write(0xb0+y,OLED_CMD);
    SSD1306_Spi_Write(((x&0xf0)>>4)|0x10,OLED_CMD);
    SSD1306_Spi_Write((x&0x0f)|0x01,OLED_CMD);
}


//���OLEDģ��ӳ��RAM
void Full_Image_Ram(unsigned char Full_Data)
{
    unsigned char Page, Colunm;
    for ( Page = 0; Page < 8; Page++ )
    {
        for ( Colunm = 0; Colunm < 128; Colunm++ )
        {
           Image_Ram[Page][Colunm]=Full_Data;
        }
    }
}

//ȡ��OLEDģ��ӳ��RAM�е����ݣ������Ƿ���
void Reverse_Image_Ram_Data(unsigned char x,unsigned char y,unsigned char M,unsigned char N)
{
    unsigned char i,j,Font_Bit;
	for (i=0;i<M;i++)
	{
		for (j=0;j<N;j++)
		{
			Font_Bit=Read_Font_Bit_Form_Image_Ram(x+j,y+i);
			Font_Bit=Font_Bit^0x01;//����ȡ�� 
			Write_Point_To_Image_Ram(x+j,y+i,Font_Bit);
		}
	}
}

//��ָ��OLEDģ����Ļ��ʾ!������OLEDģ��ӳ��RAM��
void Clear_OLED_Display()
{
    unsigned char i,n;
    for(i=0; i<8; i++)
    {
        SSD1306_Spi_Write (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
        SSD1306_Spi_Write (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
        SSD1306_Spi_Write (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ
        for(n=0; n<128; n++)
        {
            SSD1306_Spi_Write(0,OLED_DATA);
        }
    } //������ʾ
}


/*-----IO������-------|-----------GPIOx_CRH/L----------|---GPIOx_ODR----+
|      ����˵��       |       | CNFi[1:0] | MODEi[1:0] |                |
|   x=A B C D E F G   |       |   |    |  |   |    |   |       Di       |
|   i=0 1 2 ... 15    |       | Di+3 Di+2 | Di+1 Di+0  |                |
|---------------------+-------+-----------+------------+----------------|
|ͨ�����      ����   | 0x3*  |   0    0  | 00:����    |  0/1(ʵ�����) |
|ͨ�����      ��©   | 0x7*  |   0    1  | 01:���10M |  0/1(ʵ�����) | 
|�������      ����   | 0xb*  |   1    0  | 10:���2M  |     ��ʹ��     |
|�������      ��©   | 0xf*  |   1    1  | 11:���50M*|     ��ʹ��     |
|  ����      ģ������ | 0x0   |   0    0  |   0    0   |     ��ʹ��     |
|  ����      �������� | 0x4   |   0    1  |   0    0   |     ��ʹ��     |
|  ����      �������� | 0x8   |   1    0  |   0    0   |  0(��������)   |
|  ����      �������� | 0x8   |   1    0  |   0    0   |  1(��������)   |
+------IO������-------|-----------GPIOx_CRH/L----------|---GPIOx_ODR---*/
void Init_OLED_SSD1306(void)
{
    P6M0&=0xe0;
    P6M1&=0xe0;
    
    OLED_CS = 1; 
    OLED_RST=1;
    Delay_Ms(100);
    OLED_RST=0;
    Delay_Ms(100);
    OLED_RST=1;
    
    OLED_CS = 0;
    SSD1306_Spi_Write(0xAE,OLED_CMD);//--turn off oled panel
    SSD1306_Spi_Write(0x00,OLED_CMD);//---set low column address
    SSD1306_Spi_Write(0x10,OLED_CMD);//---set high column address
    SSD1306_Spi_Write(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    SSD1306_Spi_Write(0x81,OLED_CMD);//--set contrast control register
    SSD1306_Spi_Write(0xCF,OLED_CMD);// Set SEG Output Current Brightness
    SSD1306_Spi_Write(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    SSD1306_Spi_Write(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    SSD1306_Spi_Write(0xA6,OLED_CMD);//--set normal display
    SSD1306_Spi_Write(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    SSD1306_Spi_Write(0x3f,OLED_CMD);//--1/64 duty
    SSD1306_Spi_Write(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    SSD1306_Spi_Write(0x00,OLED_CMD);//-not offset
    SSD1306_Spi_Write(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    SSD1306_Spi_Write(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    SSD1306_Spi_Write(0xD9,OLED_CMD);//--set pre-charge period
    SSD1306_Spi_Write(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    SSD1306_Spi_Write(0xDA,OLED_CMD);//--set com pins hardware configuration
    SSD1306_Spi_Write(0x12,OLED_CMD);
    SSD1306_Spi_Write(0xDB,OLED_CMD);//--set vcomh
    SSD1306_Spi_Write(0x40,OLED_CMD);//Set VCOM Deselect Level
    SSD1306_Spi_Write(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    SSD1306_Spi_Write(0x02,OLED_CMD);
    SSD1306_Spi_Write(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    SSD1306_Spi_Write(0x14,OLED_CMD);//--set(0x10) disable
    SSD1306_Spi_Write(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    SSD1306_Spi_Write(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7)
    SSD1306_Spi_Write(0xAF,OLED_CMD);//display ON

    Clear_OLED_Display();
    OLED_Set_Position(0,0);
    OLED_CS = 1; 

}

//��ӳ��RAM��������OLEDģ����ʾ
void Refresh_Image_Ram_To_OLED_Display(void)
{
    unsigned char Page, Colunm;
    for ( Page = 0; Page < 8; Page++ )
    {
        OLED_DC = 0;
        OLED_CS = 0;
        SSD1306_Spi_Write(0xb0 + Page ,OLED_CMD); 
        SSD1306_Spi_Write(0x00, OLED_CMD); //������ʾλ�á��е�4λ��ַ
        SSD1306_Spi_Write(0x10, OLED_CMD); //������ʾλ�á��и�4λ��ַ
        OLED_DC = 1;
        for ( Colunm = 0; Colunm < 128; Colunm++ )
        {
            SSD1306_Spi_Write(Image_Ram[Page][Colunm],OLED_DATA);
        }
        OLED_CS = 1; 
    }
}
//��ӳ��RAM��������OLEDģ����ʾ
void Refresh_Part_Image_Ram_To_OLED_Display(unsigned char x,unsigned char y,unsigned char M,unsigned char N)
{
    unsigned char Page=y/8,Page_Max=(y+M+7)/8; 
		unsigned char	Colunm=x,Colunm_Max=x+N;
		unsigned char xl=x&0x0f,xh=(x>>4)|0x10;
    for ( Page = y/8; Page < Page_Max; Page++ )
    {
        OLED_DC = 0;
        OLED_CS = 0;
        SSD1306_Spi_Write(0xb0 + Page ,OLED_CMD); 
        SSD1306_Spi_Write(xl, OLED_CMD); //������ʾλ�á��е�4λ��ַ
        SSD1306_Spi_Write(xh, OLED_CMD); //������ʾλ�á��и�4λ��ַ
        OLED_DC = 1;
        for ( Colunm = x; Colunm < Colunm_Max; Colunm++ )
        {
            SSD1306_Spi_Write(Image_Ram[Page][Colunm],OLED_DATA);
        }
        OLED_CS = 1; 
    }
}

//��ӳ��RAM��дһ���㣨Font_Bit=0��1���ɣ�������Ϊ x,y
void Write_Point_To_Image_Ram(unsigned char x, unsigned char y, unsigned char Font_Bit )
{
    unsigned char Font_Byte_Data, Font_Byte_Bits;
    unsigned char Page, Column;

    Column = x % 128;
    Page =( y / 8)%8;
    Font_Byte_Bits = y % 8;

    Font_Byte_Data =Image_Ram[Page][Column];

    if ( Font_Bit == 1 )
    {
        Font_Byte_Data = Font_Byte_Data | ( 0x01 << Font_Byte_Bits );
    }
    else
    {
        Font_Byte_Data = Font_Byte_Data & ( ( 0x01 << Font_Byte_Bits ) ^ 0xff );
    }

    Image_Ram[Page][Column] = Font_Byte_Data;//������ʾ������OLED_Module_Image_Ram
}

//��ӳ��RAM�ж�һ���㣬����Ϊ x,y
unsigned char Read_Font_Bit_Form_Image_Ram(unsigned char x,unsigned char y)
{
    unsigned char Font_Bit,Font_Byte_Data, Font_Byte_Bits;
    unsigned char Page, Column;
	
    Column = x % 128;
    Page =( y / 8)%8;
    Font_Byte_Bits = y % 8;

    Font_Byte_Data =Image_Ram[Page][Column];

	Font_Bit=(Font_Byte_Data>>Font_Byte_Bits)&0x01;

	return Font_Bit;
}



//  ��ָ��Bmp_Pointerָ��ĵ�����������=M,��=N��ʽд��ָ��ӳ��RAM��0��1��2��3����
/*          M=16,N=20 Bmp�ֽ���=M*((N+7)/8)=48

      N=1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 

  (x,y) |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | |---��Ч----|
      \
	   \         BYTE0                   BYTE1                   BYTE3
M=1	->  D7 D6 D5 D4 D3 D2 D1 D0 D7 D6 D5 D4 D3 D2 D1 D0 D7 D6 D5 D4 D3 D2 D1 D0 
	             BYTE4                   BYTE5                   BYTE6
M=2	->  D7 D6 D5 D4 D3 D2 D1 D0 D7 D6 D5 D4 D3 D2 D1 D0 D7 D6 D5 D4 D3 D2 D1 D0 
	               .                       .                       . 
	               .                       .                       .
	               .                       .                       .
	             BYTE42                  BYTE43                  BYTE44
M=15->  D7 D6 D5 D4 D3 D2 D1 D0 D7 D6 D5 D4 D3 D2 D1 D0 D7 D6 D5 D4 D3 D2 D1 D0 
	             BYTE45                  BYTE46                  BYTE47
M=16->  D7 D6 D5 D4 D3 D2 D1 D0 D7 D6 D5 D4 D3 D2 D1 D0 D7 D6 D5 D4 D3 D2 D1 D0 
*/
void Write_Bmp_MxN_To_Image_Ram(unsigned char x,unsigned char y,unsigned char M,unsigned char N,unsigned char *Bmp_Pointer)
{
    unsigned char Bmp_Data;
    unsigned i,j,k;
    unsigned char Col_Bytes;
    unsigned char Mask[8]= {128,64,32,16,8,4,2,1};


    Col_Bytes=(N+7)/8;

    for ( i = 0; i < M; i++ )
    {
        for ( j = 0; j < Col_Bytes; j++ )
        {
            Bmp_Data=*Bmp_Pointer;
            Bmp_Pointer++;
            for (k=0; k<8; k++)
            {
                if ((j*8+k)>=N)
                {
                    break;
                }
                if ((Bmp_Data&Mask[k])!=0)
                {
                    Write_Point_To_Image_Ram(x+j*8+k,y+i,1);
                }
                else
                {
                    Write_Point_To_Image_Ram(x+j*8+k,y+i,0);
                }
            }
        }
    }

}










//�ַ��������
//unsigned char *Signed_Long_to_Dec_String(signed long Long_Data,unsigned char *Out_Dec_String,unsigned char Num_Bits)
//{
//    unsigned char i,*Char_Pointer;
//    unsigned char String_Len;
//    unsigned long Temp_Long_Data;
//    unsigned char Temp_String[20];//0xffffffff = 4,294,967,295
//  

//	//��Num_Bits���ո�
//    Char_Pointer=Temp_String;
//	for ( i=0;i<Num_Bits;i++ )
//    {
//        *(Out_Dec_String+i)=' ';
//		*Char_Pointer=' ';
//        Char_Pointer++;
//    }
//    *Char_Pointer=0x00;//�ַ�������
//	*(Out_Dec_String+i)=0x00;//�ַ�������
//	
//    //�Ӹ�λ��ʼ��λת��
//    Temp_Long_Data=abs(Long_Data);//ȡ���ݵľ���ֵ
//     for ( i=0;i<10;i++ )
//    {
// 		Temp_String[i]=Temp_Long_Data%10+'0';
// 		Temp_Long_Data=(Temp_Long_Data/10);
// 		
// 		if (Temp_Long_Data==0)
// 			break;
//    }
//	String_Len=i;
//	
//   	//Temp_String[]���㣬�ӡ�����-��
//    if (Long_Data>=0)
//	{
//		*Out_Dec_String=' ';
//	}
//	else
//	{
//		*Out_Dec_String='-';
//	}

//	//�����ַ���Temp_String[i]	
//    for ( i=0;i<String_Len+1;i++ )
//    {
// 		*(Out_Dec_String+i+1)=Temp_String[String_Len-i];
//    }
//    
//    return Out_Dec_String;
//}

void OLED_SSD1306_Test(void)
{
//    unsigned char Recv_Cmd; 
//    P6M0&=0xe0;
//    P6M1&=0xe0;


//    printf("\nOLED_SSD1306���� ��ESC���˳�!\n");   
//    Init_OLED_SSD1306();
//    Full_Image_Ram(0,0);
//    Delay_Ms(100);
//    Clear_OLED_Display(0);
//    Delay_Ms(100);

//    Write_Bmp_MxN_To_Image_Ram(0,16,0,16,16,HZ_Bmp0);
//    Write_Bmp_MxN_To_Image_Ram(0,32,0,16,16,HZ_Bmp1);
//    Write_Bmp_MxN_To_Image_Ram(0,80,0,16,16,HZ_Bmp2);
//    Write_Bmp_MxN_To_Image_Ram(0,96,0,16,16,HZ_Bmp3);
//    
//    Write_HZ_ASC_String_TO_OLED(0,0,16,14,"AD1=");
//    Write_HZ_ASC_String_TO_OLED(0,28,18,12,"0xFFF");
//    Write_HZ_ASC_String_TO_OLED(0,64,16,14,"AD2=");
//    Write_HZ_ASC_String_TO_OLED(0,64+28,18,12,"0xFFF");

//    Write_HZ_ASC_String_TO_OLED(0,0,16+16,14,"AD3=");
//    Write_HZ_ASC_String_TO_OLED(0,28,18+16,12,"0xFFF");
//    Write_HZ_ASC_String_TO_OLED(0,64,16+16,14,"AD4=");
//    Write_HZ_ASC_String_TO_OLED(0,64+28,18+16,12,"0xFFF");

//Write_HZ_ASC_String_TO_OLED_SSD1306(HZ_16x16,48,0,"����");//��SST25VF032b�е��ֿ�

////   Write_HZ_ASC_String_TO_OLED(0,0,0,16,"1234");
////    delay_ms(100);
////    Write_HZ_ASC_String_TO_OLED(0,0,16,14,"5678");
////    Write_HZ_ASC_String_TO_OLED(0,0,32,12,"2345bcd");
////    Write_HZ_ASC_String_TO_OLED(0,0,48,8,"7890efgh");
//   // Clear_OLED_Display(0);    
//    //String_Out("ѡ������źŵƺ󣬽���ָ����λ����\n");
//    

//    Write_Bmp_MxN_To_Image_Ram(0,0,48,16,32,JDQ_Bmp00);
//    Write_Bmp_MxN_To_Image_Ram(0,32,48,16,32,JDQ_Bmp01);
//    Write_Bmp_MxN_To_Image_Ram(0,64,48,16,32,JDQ_Bmp10);
//    Write_Bmp_MxN_To_Image_Ram(0,96,48,16,32,JDQ_Bmp11);

//    Refresh_Image_Ram_To_OLED_Display(0,0);

//    
//    while (1)
//    {
//        Get_Adc_Value_To_OLED_UART(0);
//        Delay_Ms(100);
//        Get_Adc_Value_To_OLED_UART(1);
//        Delay_Ms(100);
//        Get_Adc_Value_To_OLED_UART(2);
//        Delay_Ms(100);
//        Get_Adc_Value_To_OLED_UART(3);
//        Delay_Ms(100);
//        Get_Adc_Value_To_OLED_UART(4);
//        Delay_Ms(100);

//         
//        if(Recv_End==1)
//        {
//            Recv_Cmd=Serial_In();
//            if (Recv_Cmd==0x1b)
//            {
//                printf("�˳�OLED_SSD1306���� !\n");
//                Clear_OLED_Display(0);               
//                return ;
//            }
//        }
//    }

}



void Write_HZ_ASC_String_TO_OLED_SSD1306(unsigned char Font_Num,unsigned char x,unsigned char y,unsigned char *HZ_ASC_String_Pointer)
{
    unsigned char M,N;
		unsigned int Font_Width,HZ_ASC_Code;	
		unsigned char HZ_Font_Num,A_Font_Num;
		HZ_Font_Num=A_Font_Num=Font_Num;
		 switch(Font_Num)
		 {
		  case	HZ_12x12: A_Font_Num=ASCII_6x12;break;
			case	HZ_16x16: A_Font_Num=ASCII_8x16;break;
			case 	HZ_24x24: A_Font_Num=ASCII_12x24;break;
			case 	HZ_32x32: A_Font_Num=ASCII_16x32;break;
			case 	HZ_EXT_CHAR_6x12: A_Font_Num=ASCII_6x12;break;
			case 	ASCII_6x12: 
			case 	ASC_Arial_12: 
			case 	ASC_Time_12: HZ_Font_Num=HZ_12x12;break;
			case 	HZ_EXT_CHAR_8x16: 
			case 	ASCII_8x16:HZ_Font_Num=HZ_16x16;break;
			case 	ASCII_5x7:
			case  ASCII_7x8:HZ_Font_Num=HZ_12x12;break;
			case  ASC_Arial_16:
			case 	ASC_Time_16:HZ_Font_Num=HZ_32x32;break;
			case 	HZ_EXT_CHAR_12x24:A_Font_Num=ASCII_12x24;break;
			case  ASCII_12x24:
			case  ASC_Arial_24:
			case 	ASC_Time_24:HZ_Font_Num=HZ_24x24;break;
			case 	HZ_EXT_CHAR_16x32:A_Font_Num=ASCII_16x32;break;
			case 	ASCII_16x32:
			case 	ASC_Arial_32:
			case  ASC_Time_32:HZ_Font_Num=HZ_32x32;break;
			case  HZ_SPECIAL_CHAR_8x16:A_Font_Num=ASCII_8x16;break;
			default:break;
		}
   		
		x=x%MAX_X;y=y%MAX_Y;
		while (*HZ_ASC_String_Pointer!='\0')
		{
        HZ_ASC_Code=*HZ_ASC_String_Pointer;
        if (HZ_ASC_Code>=0x80)
        {
            HZ_ASC_Code<<=8;
            HZ_ASC_String_Pointer++;
            HZ_ASC_Code+=(*HZ_ASC_String_Pointer);
						Font_Num=HZ_Font_Num;
        }
				else
					Font_Num=A_Font_Num;
				Font_Width=Read_Font_Lib(Font_Num,HZ_ASC_Code,Font_Buffer);
        M=(Font_Width>>8)&0xff;
        N=Font_Width&0xff;
        Font_Width=N;
				if ((x+Font_Width)>=MAX_X)//���ݷ��ص���Ŀ����ж��Ƿ�Ӧ����
				{
					x=0;y=(y+M)%MAX_Y;			
				}		
    	
				Write_Bmp_MxN_To_Image_Ram(x,y,M,N,Font_Buffer);	//д������ָ��ӳ��RAM		

				Refresh_Part_Image_Ram_To_OLED_Display(x,y,M,N);	//�����²���ӳ��RAMˢ����OLEDģ����ʾ
				HZ_ASC_String_Pointer++;
				
				x=x+Font_Width;//���ݵ�ǰ�ַ��Ŀ���ȷ����һ���ַ�����ʼx����
				
				if (x>=MAX_X)//������һ���ַ�����ʼx�����ж��Ƿ�Ӧ����
				{
					x=0;y=(y+M)%MAX_Y;			
				}		
	}	
}






