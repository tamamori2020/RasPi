// List6-1_PwmLed01.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ		
#include <wiringPi.h>		//wiringPi
#include <softPwm.h>		//ソフトウェア方式PWM(wiringPi)
#define LED0 23				//GPIO23をLED0と定義

int main (void){
    int pwmRange = 100;		//100Hzの周期は1ms、pwmRange*100us=10ms
    int initialValue = 0;   //softPwmCreate関数実行時には出力信号0V
	int value;              //softPwmWrite関数の引数
	wiringPiSetupGpio();		//BCMのGPIO番号を使用
	pinMode(LED0, OUTPUT);		//LED0を出力に設定
	softPwmCreate(LED0,initialValue,pwmRange); //ソフトPWMの設定
	while(1){
		for ( value=0; value<100;value++){	//100回繰り返す
			softPwmWrite(LED0,value);	    //デューティ比の設定
			delay(50);				        //待ち時間50ms
		}
	}
	return EXIT_SUCCESS;
}
