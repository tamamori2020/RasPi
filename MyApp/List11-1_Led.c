// List11-1_Led.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>				//入出力
#include <stdlib.h>				//一般ユーティリティ
#include <wiringPi.h>			//wiringPi
#include  "MyPi.h"				//マイライブラリ

#define LED	20					//GPIO番号の定義				

int main (void){
	wiringPiSetupGpio();		//BCMのGPIO番号を使用
	pinMode(LED, OUTPUT);		//LEDを出力に設定
	for(;;){
		digitalWrite(LED,HIGH);	//LED0から順番にHighを出力
		delay(500);				//0.5秒待つ
		digitalWrite(LED,LOW);	//LED0から順番にLowを出力
		delay(500);				//0.5秒待つ
	}
	return EXIT_SUCCESS;
}
