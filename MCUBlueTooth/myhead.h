#include <STC15F2K60S2.H>
sbit M=P6^6;

sbit KB2=P3^2;
sbit KB3=P3^3;
sbit KB4=P3^4;
sbit KB5=P3^5;
sbit GREEN=P4^2;
sbit RED=P4^4;
sbit BLUE=P4^5;
sbit Beep=P5^5;
bit PlayFlag = 0;
bit DHT_ON=0;

bit MUSIC1=0;
bit MUSIC2=0;
bit MUSIC3=0;

char setFlag(char flag,char index){
	return flag|=1<<index;
}

char clearFlag(char flag,char index){
	return flag&=~(1<<index);
}

char isBit(char flag,char index){
	return flag&(1<<index);
}

void initMotor(){
	P6M0|=0xc0;
	P6M1&=0x3f;
	P66=0;
	P67=0;
}
void initRGB(){
	P4M0|=0x34;
	P4M1&=0xcb;
	P4&=0xcb;
}

