// List9-1_Cam01.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>		//入出力
#include <stdlib.h>		//一般ユーティリティ
#include <wiringPi.h>		//wiringPi

#define SW0	4		//GPIO4をSW0と定義

int main (void){
    wiringPiSetupGpio();	//BCMのGPIO番号を使用
    pinMode(SW0, INPUT);	//SW0を入力に設定
    pullUpDnControl(SW0,PUD_DOWN);	//SW0の入力にプルダウン抵抗をつける
      
    while(1){
        if(digitalRead(SW0)==HIGH){	//SW0が押されたならLED0を点灯
	    system("raspistill -t 100 -a 12 -a 1024 -o `date '+%Y%m%d-%H%M%S'`.jpg");
	}
    }
   return EXIT_SUCCESS;	
}
