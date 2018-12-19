#include "myhead.h"

int code tab[]={358,319,284,269,239,213,189,179,160,142,135,119,106,95,90,80,71,67,60,53,47};

unsigned char code SONG_TONE1[]={8,8,12,12,13,13,12,11,11,10,10,9,9,8,12,12,11,11,10,10,9,12,12,11,11,10,10,9,8,8,12,12,13,13,12,11,11,10,10,9,9,8,0};
//unsigned char code SONG_LONG[]={12,12,12,12,12,12,24,12,12,12,12,12,12,24,12,12,12,12,12,12,24,12,12,12,12,12,12,24,12,12,12,12,12,12,24,12,12,12,12,12,12,24,0};
unsigned char code SONG_LONG1[]={10,10,10,10,10,10,20,10,10,10,10,10,10,20,10,10,10,10,10,10,20,10,10,10,10,10,10,20,10,10,10,10,10,10,20,10,10,10,10,10,10,20,0};

unsigned char code SONG_TONE2[]={5,5,6,5,8,7,5,5,6,5,9,8,5,5,12,10,8,7,6,11,11,10,8,9,1,0};
unsigned char code SONG_LONG2[]={6, 2, 8, 8, 8, 16, 6, 2, 8, 8, 8, 16, 6, 2, 8, 8, 8, 8, 8, 6, 2, 8, 8, 8, 16, 0};

unsigned char code SONG_TONE3[]={13,12,15,15,16,16,17,15,12,13,12,15,15,16,16,17,15,13,12,15,15,16,16,17,15,12,13,12,15,15,16,15,0};
unsigned char code SONG_LONG3[]={12, 4, 16, 16, 16, 16, 16, 16, 16, 8, 4, 16, 16, 16, 16, 16, 16, 12, 4, 16, 16, 16, 16, 16, 16, 16, 12, 4, 16, 16, 16, 32, 0};


void Init_Buzzer(void)
{ 
	P5M0&=0xdf;
	P5M1=0xdf;
	Beep=1;
}
	
void DelayMS(unsigned int x)
{
	unsigned char t;
	while(x--) 
		for(t=0;t<120;t++);
}

void PlayMusic1(void)
{
	unsigned int i=0,j,k;
	while(SONG_LONG1[i]!=0||SONG_TONE1[i]!=0)
	{ //播放各个音符，SONG_LONG 为拍子长度
		for(j=0;j<SONG_LONG1[i]*64;j++)
		{
			Beep=~Beep;
			//SONG_TONE 延时表决定了每个音符的频率
			for(k=0;k<tab[SONG_TONE1[i]-1];k++)
			{
				if(PlayFlag == 0)
				{
					k--;
				}
				if(MUSIC1==0){
					return;
				}
			}
		}
		DelayMS(10);
		i++;
	}
}

void PlayMusic2(void)
{
	unsigned int i=0,j,k;
	while(SONG_LONG2[i]!=0||SONG_TONE2[i]!=0)
	{ //播放各个音符，SONG_LONG 为拍子长度
		for(j=0;j<SONG_LONG2[i]*64;j++)
		{
			Beep=~Beep;
			//SONG_TONE 延时表决定了每个音符的频率
			for(k=0;k<tab[SONG_TONE2[i]-1];k++)
			{
				if(PlayFlag == 0)
				{
					k--;
				}
				if(MUSIC2==0){
					return;
				}
			}
		}
		DelayMS(10);
		i++;
	}
}

void PlayMusic3(void)
{
	unsigned int i=0,j,k;
	while(SONG_LONG3[i]!=0||SONG_TONE3[i]!=0)
	{ //播放各个音符，SONG_LONG 为拍子长度
		for(j=0;j<SONG_LONG3[i]*64;j++)
		{
			Beep=~Beep;
			//SONG_TONE 延时表决定了每个音符的频率
			for(k=0;k<tab[SONG_TONE3[i]-1];k++)
			{
				if(PlayFlag == 0)
				{
					k--;
				}
				if(MUSIC3==0){
					return;
				}
			}
		}
		DelayMS(10);
		i++;
	}
}