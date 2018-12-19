#include "Serial_Port.H"
#include "Chinese_Font_Lib.H"
#include "OLED_SSD1306.H"
#include "DHT11.H"
#include "bluetooth.c"


int main()
{		
	unsigned char Status;
	unsigned int Convert_Result[5],Temperature_Value=0,Humidity_Value=0;
	unsigned char HV_Array[8];
	unsigned char TV_Array[8];
	
	
	Init_Chinese_Font_Lib();  //���ֿ��ʼ��
  Init_OLED_SSD1306();			//OLED��ʾ����ʼ��
  Full_Image_Ram(0);				//OLED��ʾ�������ʾ����
	
	Init_DHT11();//��ʼ����ʪ�ȴ�����
  Write_HZ_ASC_String_TO_OLED_SSD1306(HZ_16x16,0,0,"��ǰ�¶�:");		//��SST25VF032b�е��ֿ�
	Write_HZ_ASC_String_TO_OLED_SSD1306(HZ_16x16,0,32,"��ǰʪ��:");
	Write_HZ_ASC_String_TO_OLED_SSD1306(HZ_16x16,88,2,"C");		//��SST25VF032b�е��ֿ�
	Write_HZ_ASC_String_TO_OLED_SSD1306(HZ_16x16,88,32,"%");
	Refresh_Image_Ram_To_OLED_Display();
	
	
	blueToothInit();
	initRGB();
	initSegment();
	initMotor();
	initSegment();
	PWM_Init();
	Init_Buzzer();
	
	while(1)
	{

		
	if(DHT_ON){
		Status=Read_DHT11_Convert_Value(Convert_Result);  //��DHT11��ʪ��ֵ
		if (Status==DHT11_OK)
		{	  
			Humidity_Value=Convert_Result[0]; 
			Temperature_Value=Convert_Result[2]; 
			sendIntWith(Humidity_Value,'\0');//ʪ��
			Delay_Ms(20);//��֤�������
			sendIntWith(Temperature_Value,'\1');//�¶�
		}	
		
		Uint_To_Dec_String(Humidity_Value,HV_Array);	
		Uint_To_Dec_String(Temperature_Value,TV_Array);
		Write_HZ_ASC_String_TO_OLED_SSD1306(HZ_16x16,72,2,TV_Array);		
		Write_HZ_ASC_String_TO_OLED_SSD1306(HZ_16x16,72,34,HV_Array);
	}
	
	if(PlayFlag){
		if(MUSIC1){
			PlayMusic1();
		}else if(MUSIC2){
			PlayMusic2();
		}else if(MUSIC3){
			PlayMusic3();
		}
	}

	}
}