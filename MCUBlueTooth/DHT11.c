#include "DHT11.H"		//头文件

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

    DHT11_DATA=0;	//主机至少拉低18MS
    Delay_Ms(20);

    
    
    DHT11_DATA=1;  //主机拉高20-40μS	
    Temp_Count_Value=0;
    while(DHT11_DATA==1)//等待DHT11的低电平响应信号80μS
    {
        Temp_Count_Value++;
        if (Temp_Count_Value==0)
        return DHT11_ERR;
    }
    
    //测量DHT11低电平响应信号80μS的计数值
    while(DHT11_DATA==0)
    {
        Count_80us_L_Value++;
        if (Count_80us_L_Value==0)
        return DHT11_ERR;
    }

    //测量DHT11高电平响应信号80μS的计数值
    while(DHT11_DATA==1)
    {
        Count_80us_H_Value++;
        if (Count_80us_H_Value==0)
        return DHT11_ERR;
    }
    //Bit0开始50μS间隙

    
    Temp_Data=Count_80us_L_Value+Count_80us_H_Value;//160us计数值
    Compare_50us_Value=(Temp_Data>>2)+(Temp_Data>>4);//40us计数值+10us计数值=50us计数值
    
    for(i=0;i<5;i++)
    {
        Temp_Data=0;
        for (j=0;j<8;j++)
        {
            Temp_Data<<=1;
            Count_H_Value=0;
            
            Temp_Count_Value=0;
            while(DHT11_DATA==0)//等待50μS位间隙低电平结束
            {
                Temp_Count_Value++;
                if (Temp_Count_Value==0)
                return DHT11_ERR;
            }
            
            while(DHT11_DATA==1)//测量DHT11数据位低电平计数值
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





