// List6-4_PwmBuz01.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ		
#include <wiringPi.h>		//wiringPi

#define BUZZER 18			//GPIO18をBUZZERと定義
#define SW0    4			//GPIO4をSW0と定義

int main (void){
    int divisor = 540;      	//Pi4Bの場合、内部クロック(54MHz)の分周して最小変化幅を10usにする設定値
//	int divisor = 192;			//Pi3B/3B+の場合、内部クロック(19.2MHz)の分周して最小変化幅を10usにする設定値
    unsigned int range = 100; 	//PWMのレンジ値100分割　100kHz÷100=1kHz
	wiringPiSetupGpio();		//BCMのGPIO番号を使用
	pinMode(SW0,INPUT);			//SW0を入力に設定
	pullUpDnControl(SW0,PUD_DOWN);	//SW0にプルダウン抵抗をつける
	pinMode(BUZZER, PWM_OUTPUT);	//BUZZERをPWM信号の出力端子に設定
	pwmSetClock(divisor);			//内部クロックの分周の設定
	pwmSetRange(range);			    //階調の設定	
	pwmSetMode(PWM_MODE_MS);		//信号波形モードの設定
	
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
