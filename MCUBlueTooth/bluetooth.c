#include "segment.c"

unsigned char R_BUFFER[16]={0};
unsigned char T_BUFFER[16]={0};
unsigned char len=0;
unsigned char t_len=0;
unsigned char t_point=0;
char BT_FLAG=0;//4			3音乐 2电机 1灯光 0时间		
//set						0x10		0x08	0x04	0x02	0x01 判断 置1
//clear					0xef		0xf7	0xfb	0xfd	0xfe	清零
long temp=0;


void receiveChar(unsigned char c);
void sendChar(unsigned char c[],unsigned char length);
void blueToothUart();
void blueToothInit();
long pow10(char index);

void music(char index);
void motor(char index);
void light(char index);
void time(long t);
void dht(char index);

void dht(char index){
	DHT_ON=~DHT_ON;
}

void time(long t){
	char s;
	char m;
	char h;
	s=t%100;
	t/=100;
	m=t%100;
	t/=100;
	h=t%100;
	setTime(h,m,s);
}
void light(char index){
	if(LIGHT_FLAG) lightOff();
	else LIGHT_FLAG=1;
}

void music(char index){
	switch(index){
		case 0:
			PlayFlag=0;
			MUSIC1=0;
			MUSIC2=0;
			MUSIC3=0;
		break;
		case 1:
			PlayFlag=1;
			MUSIC1=1;
			MUSIC2=0;
			MUSIC3=0;
		break;
		case 2:
			PlayFlag=1;
			MUSIC1=0;
			MUSIC2=1;
			MUSIC3=0;
		break;
		case 3:
			PlayFlag=1;
			MUSIC1=0;
			MUSIC2=0;
			MUSIC3=1;
		break;
		case 4:
			PlayFlag=~PlayFlag;
		break;
		default:break;
	}
}
void motor(char index){
	switch(index){
		case 2:
			SEG_FLAG=setFlag(SEG_FLAG,0);
			SEG_FLAG=setFlag(SEG_FLAG,1);
		break;
		case 1:
			SEG_FLAG=setFlag(SEG_FLAG,1);
			SEG_FLAG=clearFlag(SEG_FLAG,0);
		break;
		case 0:
		default:
			SEG_FLAG=clearFlag(SEG_FLAG,1);
		break;
	}
}

void sendCharWith(unsigned char c[],unsigned char length,char replace){
	t_len=length+2;
	T_BUFFER[0]=replace;
	for(t_point=1;t_point<=length;t_point++){
		T_BUFFER[t_point]=c[t_point-1];
	}
	T_BUFFER[t_point]=replace;
	SBUF=T_BUFFER[0];
	t_point=1;
}

void sendIntWith(char c,char replace){
	t_len=3;
	T_BUFFER[0]=replace;
	T_BUFFER[1]=c;
	T_BUFFER[2]=replace;
	SBUF=T_BUFFER[0];//已经发送第一个
	t_point=1;
}


void receiveChar(unsigned char c){
	char i=0;
	switch(c){
		case '\0'://时间调试模式
				if(isBit(BT_FLAG,0)){
					BT_FLAG=clearFlag(BT_FLAG,0);
					for(i=0,temp=0;i<len;i++){
						temp+=R_BUFFER[i]*pow10(len-1-i);
					}
					time(temp);
				}else{
					BT_FLAG=setFlag(BT_FLAG,0);
					len=0;
				}
		break;
		case '\1'://light
				if(isBit(BT_FLAG,1)){
					BT_FLAG=clearFlag(BT_FLAG,1);
					for(i=0,temp=0;i<len;i++){
						temp+=R_BUFFER[i]*pow10(len-1-i);
					}
					light(temp);
				}else{
					BT_FLAG=setFlag(BT_FLAG,1);
					len=0;
				}
		break;
		case '\2'://电机
			if(isBit(BT_FLAG,2)){
					BT_FLAG=clearFlag(BT_FLAG,2);
					for(i=0,temp=0;i<len;i++){
						temp+=R_BUFFER[i]*pow10(len-1-i);
					}
					motor(temp);
				}else{
					BT_FLAG=setFlag(BT_FLAG,2);
					len=0;
				}
		break;
		case '\3'://music
				if(isBit(BT_FLAG,3)){
					BT_FLAG=clearFlag(BT_FLAG,3);
					for(i=0,temp=0;i<len;i++){
						temp+=R_BUFFER[i]*pow10(len-1-i);
					}
					music(temp);
				}else{
					BT_FLAG=setFlag(BT_FLAG,3);
					len=0;
				}
		break;
				case '\4'://温湿度检测
				if(isBit(BT_FLAG,4)){
					BT_FLAG=clearFlag(BT_FLAG,4);
					for(i=0,temp=0;i<len;i++){
						temp+=R_BUFFER[i]*pow10(len-1-i);
					}
					dht(temp);
				}else{
					BT_FLAG=setFlag(BT_FLAG,4);
					len=0;
				}
		break;
		default:
			//如果是某个模式
			if(BT_FLAG){R_BUFFER[len]=c-'0';len++;}
		break;
	}
}

void sendChar(unsigned char c[],unsigned char length){
	t_len=length;
	for(t_point=0;t_point<length;t_point++){
		T_BUFFER[t_point]=c[t_point];
	}
	SBUF=c[0];
	t_point=1;
}

long pow10(char index){
	char i=0;
	long t=1;
	for(i=0;i<index;i++){
		t*=10;
	}
	return t;
}

void blueToothUart(){//9600bps  @11.0592MHz  串口初始化函数定义
	//8位数据 定时器1为8位自动重装方式 定时器1时钟1T
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE0;		//设定定时初值
	TH1 = 0xFE;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	AUXR1|=0x80;AUXR1&=0xbf;// RxD1.6 TxD1.7
	//AUXR1|=0x40;AUXR1&=0x7f; // RxD3.6 TxD3.7
	EA=1;
	ES=1;
	PS=1;
}

void blueTooth() interrupt 4{
	if(RI){
			RI=0;
			receiveChar(SBUF);
		}
	if(TI){
		TI=0;
		if(t_point<t_len){
			SBUF=T_BUFFER[t_point];
			t_point++;
		}
	}
}

void blueToothInit(){
	P1M0=0;
	P1M1=0;
	TI=0;
	RI=0;
	blueToothUart();
}