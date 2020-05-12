// List5-5_Int01.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ		
#include <wiringPi.h>		//wiringPi

#define LED0    23          //GPIO23をLED0と定義
#define LED1    22          //GPIO22をLED1と定義
#define SW0     4           //GPIO4をSW0と定義
#define SW1     5           //GPIO5をSW1と定義
/* プロトタイプ宣言 */
void int_sw0(void);         	//SW0の割込み関数
void int_sw1(void);         	//SW1の割込み関数

int main (void){
    wiringPiSetupGpio();	//BCMのGPIO番号を使用
    pinMode(LED0, OUTPUT);  //LED0を出力に設定
    pinMode(LED1, OUTPUT);  //LED1を出力に設定
    pinMode(SW0, INPUT);    //SW0を入力に設定
    pullUpDnControl(SW0,PUD_DOWN);  //SW0にプルダウン抵抗をつける
    pinMode(SW1, INPUT);    //SW1を入力に設定
    pullUpDnControl(SW1,PUD_DOWN);  //SW1にプルダウン抵抗をつける
       
    //SW0の立上りエッジで割込みを発生
    wiringPiISR(SW0, INT_EDGE_RISING, (void*)int_sw0);
    //SW1の立上りエッジで割込みを発生
    wiringPiISR(SW1, INT_EDGE_RISING, (void*)int_sw1);
    for(;;){
        digitalWrite(LED0,HIGH);    //LED0を0.5秒間隔で点灯
        delay(500);
        digitalWrite(LED0,LOW);
        delay(500);
    }
    return EXIT_SUCCESS;
}

void int_sw0(void)               //SW0の割込み関数
{
    digitalWrite(LED1,HIGH);    //LED1を点灯
}
void int_sw1(void)               //SW1の割込み関数
{
    digitalWrite(LED1,LOW);    //LED1を消灯
}

