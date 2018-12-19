#include "DHT11.H"		//ͷ�ļ�

sbit DHT11_DATA=P4^6;

//void Delay_Ms(unsigned int n);		//@11.0592MHz 
void Init_DHT11(void);
unsigned char Read_DHT11_Byte(unsigned char *Dtat_Pointer);
unsigned char Read_DHT11_Convert_Value(unsigned int *Data_Pointer);
void DHT11_Test(void);

//void Delay_Ms(unsigned int n)		//@11.0592MHz
//{
//	unsigned char i, j;
//	unsigned int k;
//  for(k=0;k<n;k++)
//	{
//		_nop_();
//		_nop_();
//		_nop_();
//		i = 11;
//		j = 190;
//		do
//		{
//			while (--j);
//		} while (--i);
//	}
//}

void Init_DHT11(void)
{
	P4M0&=0xbf;
	P4M1&=0xbf;
	DHT11_DATA=1;
}


unsigned char Read_DHT11_Convert_Value(unsigned int *Data_Pointer)
{
    unsigned char i,j;
    unsigned char Temp_Data;
    unsigned char Count_80us_L_Value,Count_80us_H_Value,Count_H_Value;
    unsigned char Compare_50us_Value,Temp_Count_Value;


    Count_80us_L_Value=0;
    Count_80us_H_Value=0;
    Delay_Ms(800);

    DHT11_DATA=0;	//������������18MS
    Delay_Ms(20);

    
    
    DHT11_DATA=1;  //��������20-40��S	
    Temp_Count_Value=0;
    while(DHT11_DATA==1)//�ȴ�DHT11�ĵ͵�ƽ��Ӧ�ź�80��S
    {
        Temp_Count_Value++;
        if (Temp_Count_Value==0)
        return DHT11_ERR;
    }
    
    //����DHT11�͵�ƽ��Ӧ�ź�80��S�ļ���ֵ
    while(DHT11_DATA==0)
    {
        Count_80us_L_Value++;
        if (Count_80us_L_Value==0)
        return DHT11_ERR;
    }

    //����DHT11�ߵ�ƽ��Ӧ�ź�80��S�ļ���ֵ
    while(DHT11_DATA==1)
    {
        Count_80us_H_Value++;
        if (Count_80us_H_Value==0)
        return DHT11_ERR;
    }
    //Bit0��ʼ50��S��϶

    
    Temp_Data=Count_80us_L_Value+Count_80us_H_Value;//160us����ֵ
    Compare_50us_Value=(Temp_Data>>2)+(Temp_Data>>4);//40us����ֵ+10us����ֵ=50us����ֵ
    
    for(i=0;i<5;i++)
    {
        Temp_Data=0;
        for (j=0;j<8;j++)
        {
            Temp_Data<<=1;
            Count_H_Value=0;
            
            Temp_Count_Value=0;
            while(DHT11_DATA==0)//�ȴ�50��Sλ��϶�͵�ƽ����
            {
                Temp_Count_Value++;
                if (Temp_Count_Value==0)
                return DHT11_ERR;
            }
            
            while(DHT11_DATA==1)//����DHT11����λ�͵�ƽ����ֵ
            {
                Count_H_Value++;
                if (Count_H_Value==0)
                return DHT11_ERR;
            }
            if (Count_H_Value>Compare_50us_Value)
            {
                Temp_Data++;
            }
        }
        *(Data_Pointer+i)=Temp_Data;
    }    
    
    Temp_Data=0;
    for(i=0;i<4;i++)
    {
        Temp_Data+=*(Data_Pointer+i);
    }
    if (Temp_Data==*(Data_Pointer+4))
    {
        return DHT11_OK;
    }
    else
    {
       return DHT11_ERR;    
    }
}





