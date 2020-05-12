// List5-2_Sw02.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ		
#include <wiringPi.h>		//wiringPi

#define SW0	    4		//GPIO4をSW0と定義
int ledGpio[4] ={23,22,25,24};  //LEDのGPIOを配列で定義

int main (void){
    int i;
    wiringPiSetupGpio();	//BCMのGPIO番号を使用
    for(i=0;i<4;i++)	{	//LED0からLED3を出力に設定
        pinMode(ledGpio[i], OUTPUT);
    }
    pinMode(SW0, INPUT);		    //SW0を入力に設定
    pullUpDnControl(SW0,PUD_DOWN);	//プルダウン抵抗を有効にする
      
    while(1){
        if(digitalRead(SW0)==HIGH){	//SW0の状態を取得
            digitalWrite(ledGpio[0], HIGH);//SW0がHIGHならLED0を点灯
        }
        else {
            digitalWrite(ledGpio[0], LOW); //SW0がLOWならLED0を消灯
        }
    }
   return EXIT_SUCCESS;	
}
