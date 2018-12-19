#include "music.c"
//定义PWM周期(最大值为32767)
#define CYCLE   6000L
sbit PWM5 = P4 ^ 2;
sbit PWM3 = P4 ^ 5;
sbit PWM4 = P4 ^ 4;
char LIGHT_FLAG=0;
int PWM_COUNT=0;

void PWM_Init()
{
  P4M1 = 0x00;
  P4M0 |= 0x34;
  P_SW2 |= 0x80;
  PWM3CR = 0x08; //选择PWM3输出到P4.5,不中断
  PWM4CR = 0x08; //选择PWM4输出到P4.4,不中断
  PWM5CR = 0x08; //选择PWM5输出到P4.2,不中断
  PWMCFG = 0x00; //配置PWM的输出初始为低电平
  PWMCKS = 0x0f; //选择PWM的时钟为Fosc/(15+1)
  PWMC = CYCLE;  //设置PWM周期
  PWM3T1 = 0x0001;
  PWM3T2 = 0;
  PWM4T1 = 0x0001;
  PWM4T2 = 0;
  PWM5T1 = 0x0001;
  PWM5T2 = 0;
  PWMCR |= 0x80; //使能PWM模块
  P_SW2 &= ~0x80;

}
void pwm3(unsigned int Duty)//PWM3
{
  if(Duty == 0)
  {
    //占空比为0时关闭PWM3
    PWMCR &= ~0x02;
    PWM3 = 0; //固定输出0
  }
  else
  {
    if(Duty == 100)
    {
      //占空比为100时关闭PWM3
      PWMCR &= ~0x02;
      PWM3 = 1; //固定输出1
    }
    else
    {
      P_SW2 |= 0x80;
      PWM3T1 = 0x0001;
      PWM3T2 = CYCLE * Duty / 100;
      P_SW2 &= ~0x80;
      PWMCR |= 0x02;
    }
  }
}
void pwm4(unsigned int  Duty)//PWM4
{
  if(Duty == 0)
  {
    PWMCR &= ~0x04;
    PWM4 = 0;
  }
  else
  {
    if (Duty == 100)
    {
      PWMCR &= ~0x04;
      PWM4 = 1;
    }
    else
    {
      P_SW2 |= 0x80;
      PWM4T1 = 0x0001;
      PWM4T2 = CYCLE * Duty / 100;
      P_SW2 &= ~0x80;
      PWMCR |= 0x04;
    }
  }
}

void pwm5( unsigned int Duty) //PWM5
{
  if(Duty == 0)
  {
    PWMCR &= ~0x08;
    PWM5 = 0;
  }
  else
  {
    if  (Duty == 100)
    {
      PWMCR &= ~0x08;
      PWM5 = 1;
    }
    else
    {
      P_SW2 |= 0x80;
      PWM5T1 = 0x0001;
      PWM5T2 = CYCLE * Duty / 100;
      P_SW2 &= ~0x80;
      PWMCR |= 0x08;
    }
  }
}
void lightOff(){
	LIGHT_FLAG=0;
	P4&=0xcb;
	pwm3(0);
	pwm4(0);
	pwm5(0);
}
void breath(){
	//[0,199]
	if(LIGHT_FLAG){
		if(PWM_COUNT<100){
			pwm3(PWM_COUNT) ;
			pwm4(100 - PWM_COUNT) ;
			pwm5(PWM_COUNT) ;
		}else{
			pwm3(200 - PWM_COUNT);
			pwm4(PWM_COUNT-100) ;
			pwm5(200 - PWM_COUNT) ;
		}
		PWM_COUNT++;
		PWM_COUNT%=200;
	}
}