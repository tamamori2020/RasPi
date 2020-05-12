// List5-3_Alt01.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ		
#include <wiringPi.h>		//wiringPi

#define LED0	23		//GPIO23をLED0と定義
#define SW0	    4	    //GPIO4をSW0と定義

int main (void){
    int i=0;
    int alt0=LOW;		//SW0のトグル状態を記憶する変数、LOWに初期化
    int s0Now=LOW;      //SW0が押された現在の値
    int s0Old=LOW;      //SW0が押される前の値
    wiringPiSetupGpio();	//BCMのGPIO番号を使用
    pinMode(LED0, OUTPUT);	//LED0を出力に設定
    pinMode(SW0, INPUT);	//SW0を入力に設定
    pullUpDnControl(SW0,PUD_DOWN);	//SW0にプルダウン抵抗をつける

    while(1){		
        s0Now =digitalRead(SW0);			
        if((s0Now==HIGH)&&(s0Old==LOW)){
            alt0 =  !alt0;		
            digitalWrite(LED0, alt0); 
            s0Old=HIGH;                
            printf("%d\n",++i);	
        }			
        else if((s0Now==HIGH)&&(s0Old==HIGH)){
            s0Old=HIGH;			
        }else{               //SW0が押されていない、または離された瞬間
            s0Old=LOW;	
        }		
    }			
    return EXIT_SUCCESS;
}			
