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
	
	
	Init_Chinese_Font_Lib();  //汉字库初始化
  Init_OLED_SSD1306();			//OLED显示屏初始化
  Full_Image_Ram(0);				//OLED显示屏清除显示内容
	
	Init_DHT11();//初始化温湿度传感器
  Write_HZ_ASC_String_TO_OLED_SSD1306(HZ_16x16,0,0,"当前温度:");		//用SST25VF032b中的字库
	Write_HZ_ASC_String_TO_OLED_SSD1306(HZ_16x16,0,32,"当前湿度:");
	Write_HZ_ASC_String_TO_OLED_SSD1306(HZ_16x16,88,2,"C");		//用SST25VF032b中的字库
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
		Status=Read_DHT11_Convert_Value(Convert_Result);  //读DHT11温湿度值
		if (Status==DHT11_OK)
		{	  
			Humidity_Value=Convert_Result[0]; 
			Temperature_Value=Convert_Result[2]; 
			sendIntWith(Humidity_Value,'\0');//湿度
			Delay_Ms(20);//保证传输完成
			sendIntWith(Temperature_Value,'\1');//温度
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