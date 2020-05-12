// List11-2_Lcd.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>				//入出力
#include <stdlib.h>				//一般ユーティリティ
#include <string.h>				//文字列操作
#include <wiringPi.h>			//wiringPi一般
#include <wiringPiI2C.h>		//I2C用wiringPi
#include  "MyPi.h"				//マイライブラリ

#define LCD_ADR	0x3e			//LCD スレーブアドレス

int main(void){
	int fd; 
	int i=0;                    //カウンタ
	char s1[17];                //17文字(Null含む)のバッファ
	
	fd=wiringPiI2CSetup(LCD_ADR);   //LCDの有効化
	LcdSetup(fd);                   //LCD初期化
	
	while(1){
		LcdClear(fd);               //画面クリア
		sprintf(s1,"i=%d",i);       //文字列へ変換
		LcdWriteString(fd,s1);      //LCDに表示
		i++;						//カウントアップ
		delay(100);				//1秒間のディレイ
	}
	return EXIT_SUCCESS;
}
