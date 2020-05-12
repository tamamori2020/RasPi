// List11-3_Sw.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>		//入出力
#include <stdlib.h>		//一般ユーティリティ
#include <string.h>		//文字列操作
#include <wiringPi.h>		//wiringPi一般
#include <wiringPiI2C.h>	//I2C用wiringPi
#include  "MyPi.h"		//マイライブラリ

#define LCD_ADR	0x3e		//LCD スレーブアドレス
#define SW_RED	17    		//赤色SW
#define SW_WHITE 27		//白色SW

int main(void){
    int fd;
    int i;			//現在のSWの値
    int old=3;     		//過去のSWの値
    char s1[17] ;		//17文字(Null含む)のバッファ
    
    wiringPiSetupGpio();		//BCMのGPIO番号を使用
    pinMode(SW_RED, INPUT);		//赤色SW 入力設定
    pullUpDnControl(SW_RED,PUD_DOWN);
    pinMode(SW_WHITE, INPUT);		//白色SW 入力設定
    pullUpDnControl(SW_WHITE,PUD_DOWN);
    fd = wiringPiI2CSetup(LCD_ADR);	//LCDの有効化
    LcdSetup(fd);			//LCDの初期化

    while(1){          
	i = digitalRead(SW_RED)*2+digitalRead(SW_WHITE);
/* i=0b00のとき、Red SW is OFF,White SW is OFF
 * i=0b01のとき、Red SW is OFF,White SW is ON
 * i=0b10のとき、Red SW is ON, White SW is OFF
 * i=0b11のとき、Red SW is ON, White SW is ON */
	if(i!=old){
	    LcdClear(fd);               //ディスプレイクリア
	    switch(i){
		case 0b00:
		    sprintf(s1,"Red SW is OFF");   //文字列に変換
		    LcdWriteString(fd, s1);	   //LCDに表示
		    LcdNewline(fd);		   //改行		    
		    sprintf(s1,"White SW is OFF"); //文字列に変換
		    LcdWriteString(fd, s1);	   //LCDに表示
		    old = i;
		    break;
		case 0b01:
		    sprintf(s1,"Red SW is OFF");	//文字列に変換
		    LcdWriteString(fd, s1);	//LCDに表示		    
		    LcdNewline(fd);
		    sprintf(s1,"White SW is ON"); //文字列に変換
		    LcdWriteString(fd, s1);	   //LCDに表示
		    old = i;
		    break;		
		case 0b10:
		    sprintf(s1,"Red SW is ON");   //文字列に変換
		    LcdWriteString(fd, s1);	   //LCDに表示		    
		    LcdNewline(fd);
		    sprintf(s1,"White SW is OFF");  //文字列に変換
		    LcdWriteString(fd, s1);	   //LCDに表示
		    old = i;
		    break;		
		case 0b11:
		    sprintf(s1,"Red SW is ON");	//文字列に変換
		    LcdWriteString(fd, s1);	//LCDに表示		    
		    LcdNewline(fd);
		    sprintf(s1,"White SW is ON"); //文字列に変換
		    LcdWriteString(fd, s1);	  //LCDに表示
		    old = i;
		    break;
	    }
	}	
    }
    return EXIT_SUCCESS;
}


