// List11-4_PwmBuz.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <wiringPi.h>		//wiringPi一般
#include  "MyPi.h"			//マイライブラリ

#define BUZZER 18			//圧電サウンダ
#define SW0 27				//白色SW

int main (void){
	int divisor = 540;			//Pi 4Bの分周値　54MHz÷540 = 100kHz
//    int divisor = 192;      	//Pi 3/3B+の分周値　19.2MHz÷192 = 100kHz
    unsigned int range = 100; 	//PWMのレンジ値100分割　100kHz÷100=1kHz
	wiringPiSetupGpio();		//BCMのGPIO番号を使用
	pinMode(SW0,INPUT);			//SW0を入力に設定
	pullUpDnControl(SW0,PUD_DOWN);	//SW0にプルダウン抵抗をつける
	pinMode(BUZZER, PWM_OUTPUT);	//BUZZERをPWM出力に設定
	pwmSetClock(divisor);			//PWMクロックの設定　19.2MHz÷192 = 100kHz
	pwmSetMode(PWM_MODE_MS);		//PWMの信号パターンの設定
	pwmSetRange(range);			    //PWMレンジの設定
	
	while(1){
		if(digitalRead(SW0)==HIGH){	    //SW0が押された
			pwmWrite(BUZZER,range/2);	//デューティ比50%
		}
		else{						    //SW0が押されてない
			pwmWrite(BUZZER,0);	        //PWM信号出力停止
		}
	}
	return EXIT_SUCCESS;
}
