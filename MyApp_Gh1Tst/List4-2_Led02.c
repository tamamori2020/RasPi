// List4-2_Led02.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ		
#include <wiringPi.h>		//wiringPi

#define LED0	23				//LED0からLED3までのGPIO番号の定義
#define LED1	22
#define LED2	25
#define LED3	24

int main (void){
    int i;
	wiringPiSetupGpio();		//BCMのGPIO番号を使用
	pinMode(LED0, OUTPUT);		//LED0から順番に出力に設定
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);	
	pinMode(LED3, OUTPUT);
	
	for(i=0;i<10;i++){
		digitalWrite(LED0,1);	//LED0から順番にHighを出力
		digitalWrite(LED1,1);
		digitalWrite(LED2,1);
		digitalWrite(LED3,1);
		delay(500);				//0.5秒待つ
		digitalWrite(LED0,0);	//LED0から順番にLowを出力
		digitalWrite(LED1,0);
		digitalWrite(LED2,0);
		digitalWrite(LED3,0);
		delay(500);				//0.5秒待つ
	}
	return EXIT_SUCCESS;		//正常終了
}
