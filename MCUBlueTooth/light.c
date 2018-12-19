#include "music.c"
//����PWM����(���ֵΪ32767)
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
  PWM3CR = 0x08; //ѡ��PWM3�����P4.5,���ж�
  PWM4CR = 0x08; //ѡ��PWM4�����P4.4,���ж�
  PWM5CR = 0x08; //ѡ��PWM5�����P4.2,���ж�
  PWMCFG = 0x00; //����PWM�������ʼΪ�͵�ƽ
  PWMCKS = 0x0f; //ѡ��PWM��ʱ��ΪFosc/(15+1)
  PWMC = CYCLE;  //����PWM����
  PWM3T1 = 0x0001;
  PWM3T2 = 0;
  PWM4T1 = 0x0001;
  PWM4T2 = 0;
  PWM5T1 = 0x0001;
  PWM5T2 = 0;
  PWMCR |= 0x80; //ʹ��PWMģ��
  P_SW2 &= ~0x80;

}
void pwm3(unsigned int Duty)//PWM3
{
  if(Duty == 0)
  {
    //ռ�ձ�Ϊ0ʱ�ر�PWM3
    PWMCR &= ~0x02;
    PWM3 = 0; //�̶����0
  }
  else
  {
    if(Duty == 100)
    {
      //ռ�ձ�Ϊ100ʱ�ر�PWM3
      PWMCR &= ~0x02;
      PWM3 = 1; //�̶����1
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