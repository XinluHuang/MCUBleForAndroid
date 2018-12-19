#include "light.c"

long NUMBER=0;
char SECOND=0;
char MINUTE=0;
char HOUR=0;
char SEGMENTS[8]={0};
char POSITION=0;
unsigned char c[]="120";
unsigned char i=sizeof(c)-1;//隐含'\0'
int sec=0;
int btn=0;
char pwm=0;//[0,10]
char SEG_FLAG=0;//2按钮 1开关 0快慢		
//set							0x04  0x02	0x01 判断 置1
//clear						0xfb	0xfd	0xfe	清零

//digital
void initSegment();
char setPoint(char i);
char number2seg(char num);

void setNumber(int num0);
void setPosition(char seg);
void showSegment();
void setSecond(char sec);
void setMinute(char min);
void setHour(char hour);
void setTime(char h,char m,char s);
void addTime(char h,char m,char s);
void initSegment();
//time
void T0Init_2ms();

void t0() interrupt 1{
	showSegment();
	
	sec++;
	if(sec>=500){
		addTime(0,0,1);
		sec=0;
	}
	
	if(isBit(SEG_FLAG,1)){
		if(isBit(SEG_FLAG,0)) M=1;//快速
		else M=~M;
	}else M=0;
	
	//按钮
	if(btn<200) btn++;
	if(isBit(SEG_FLAG,2)){
		if(KB2==0){
			addTime(1,0,0);
			btn=0;
		}else if(KB3==0){
			addTime(0,1,0);
			btn=0;
		}else if(KB4==0){
			addTime(0,0,1);
			btn=0;
		}else if(KB5==0){
			DHT_ON=~DHT_ON;
			btn=0;
		};
		SEG_FLAG=clearFlag(SEG_FLAG,2);
	}
	if((KB2==0||KB3==0||KB4==0||KB5==0) && btn>=200){
		SEG_FLAG=setFlag(SEG_FLAG,2);
	}
	
	//pwm
	pwm++;
	if(pwm>=10){
		pwm=0;
		breath();
	}
	
}

void T0Init_2ms(){
	//中断号1
	ET0=1;//允许中断
	EA=1;
	AUXR &= 0x7F;
	TMOD &= 0xF0;
	TL0 = 0xCD;
	TH0 = 0xF8;
	TF0 = 0;
	TR0 = 1;//开启定时
	PT0=1;
}

void showSegment(){
	POSITION++;
	POSITION%=8;
	setPosition(POSITION);
	P0=SEGMENTS[POSITION];
}
void setPosition(char seg){
	char t=0xff;
	switch(seg){
		case 0:t=0xfe;break;
		case 1:t=0xfd;break;
		case 2:t=0xfb;break;
		case 3:t=0xf7;break;
		case 4:t=0xef;break;
		case 5:t=0xdf;break;
		case 6:t=0xbf;break;
		case 7:t=0x7f;break;
		default:t=0xff;break;
	}
	P2=t;
}

void setNumber(long num){
	char i=0;
	NUMBER=num;
	POSITION=0;
	//左边为个位
	for(i=0;i<8;i++){
		if(num!=0){
			SEGMENTS[i]=number2seg(num%10);
			num/=10;
		}else{
			SEGMENTS[i]=0;
		}
	}
}

void setSecond(char sec){
	char i=0;
	SECOND=sec;
	POSITION=0;
	if(sec>=60){
		sec%=60;
		MINUTE++;
	}
	//左边为个位
	for(i=0;i<2;i++){
		if(sec!=0){
			SEGMENTS[i]=number2seg(sec%10);
			sec/=10;
		}else{
			SEGMENTS[i]=number2seg(0);
		}
	}
	SEGMENTS[i]=0x40;
}
void setMinute(char min){
	char i=0;
	MINUTE=min;
	POSITION=0;
	if(min>=60){
		min%=60;
		HOUR++;
	}
	//左边为个位
	for(i=3;i<5;i++){
		if(min!=0){
			SEGMENTS[i]=number2seg(min%10);
			min/=10;
		}else{
			SEGMENTS[i]=number2seg(0);
		}
	}
	SEGMENTS[i]=0x40;
}
void setHour(char hour){
	char i=0;
	HOUR=hour;
	POSITION=0;
	if(hour>=24){
		hour%=24;
	}
	//左边为个位
	for(i=6;i<8;i++){
		if(hour!=0){
			SEGMENTS[i]=number2seg(hour%10);
			hour/=10;
		}else{
			SEGMENTS[i]=number2seg(0);
		}
	}
}
void addTime(char h,char m,char s){
	SECOND+=s;
	MINUTE+=m;
	HOUR+=h;
	if(SECOND>=60){
		SECOND%=60;
		MINUTE++;
	}
	if(MINUTE>=60){
		MINUTE%=60;
		HOUR++;
	}
	if(HOUR>=24){
		HOUR%=24;
	}
	setSecond(SECOND);
	setMinute(MINUTE);
	setHour(HOUR);
}
void setTime(char h,char m,char s){
	SECOND=s;
	MINUTE=m;
	HOUR=h;
	setSecond(SECOND);
	setMinute(MINUTE);
	setHour(HOUR);
}
char number2seg(char num){
	char t=0;
	switch(num){
		case 0:t=0x3f;break;
		case 1:t=0x06;break;
		case 2:t=0x5b;break;
		case 3:t=0x4f;break;
		case 4:t=0x66;break;
		case 5:t=0x6d;break;
		case 6:t=0x7d;break;
		case 7:t=0x07;break;
		case 8:t=0x7f;break;
		case 9:t=0x6f;break;
		default:t=0x40;break;
	}
	return t;
}

char setPoint(char i){
	return i|0x80;
}
void initSegment(){
	P0M0=0xff;
	P0M1=0;
	P2M0=0xff;
	P2M1=0;
	EA=1;
	T0Init_2ms();
}