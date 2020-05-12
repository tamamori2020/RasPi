// macros.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// gccコマンドにライブラリlibMyPi.aを作成して追記します
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと
/* 
 * micros関数のテストプログラムです。micros関数の戻り値(32bit)はwiringPiSetup関数を
 * 実行してからのus単位の符号なしの経過時間です。約71分後にオーバーフローしてゼロに戻り、
 * カウントを再開します。本プログラムは実行するとターミナルとLCDにmicros関数で取得した
 * 経過時間(us)、１秒間の差分時間、経過時間(m:s)を表示します。差分時間はmicros関数が取得した
 * 現在と前回の時間の差です。経過時間(m:s)には誤差を含みます。
 * プログラムの実行結果から分かること（Pi4B使用）
 * 1.delay(1000)のとき、遅延時間が10001XXusである。約1XXusがwhile文の処理時間となる。
 *   ±XXus常に変動する。
 * 2.オーバーフロー（cnt<cntOld）のとき、変数はunsigned intで宣言しているため
 * 　diff = cnt - cntOldの結果diffは負にならない。cntは桁借りする。
*/ 

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <limits.h>			//整数型の大きさ
#include <unistd.h>			//POSIX関数等

#include <wiringPi.h>		//wiringPi
#include <wiringPiI2C.h>	//I2C用wiringPi
#include "MyPi.h"			//マイライブラリ

/* LEDのGPIOを配列で定義 */
const int ledGpio[4] ={23,22,25,24};	//グローバル変数

#define LCD_ADR	0x3e		//LCD スレーブアドレス
#define BUZZER 18			//GPIO18をBUZZERと定義
#define SW0    4			//GPIO4をSW0と定義

int main(int argc, char *argv[]){
	int alt = 0;
	int i,no=1;
	unsigned int acnt=1;
	unsigned long long int ave=0;
	unsigned int cnt,cntOld,diff,minutes,seconds,secRemainders;
	unsigned int minutes2,seconds2,secRemainders2;

	int divisor = 540;      	//Pi4Bの場合、内部クロック(54MHz)の分周して最小変化幅を10usにする設定値
//	int divisor = 192;			//Pi3B/3B+の場合、内部クロック(19.2MHz)の分周して最小変化幅を10usにする設定値
    unsigned int range = 100; 	//PWMのレンジ値100分割　100kHz÷100=1kHz
	cntOld=0;
	seconds2=0;
	
	wiringPiSetupGpio();		//BCMのGPIO番号を使用
	pinMode(BUZZER, PWM_OUTPUT);//BUZZERをPWM信号の出力端子に設定
	pwmSetClock(divisor);		//内部クロックの分周の設定
	pwmSetRange(range);			//階調の設定	
	pwmSetMode(PWM_MODE_MS);	//信号波形モードの設定
	
    for(i=0;i<4;i++)	{		//LED0からLED3を出力に設定
        pinMode(ledGpio[i], OUTPUT);
    }
	digitalWrite(ledGpio[0],alt);
	digitalWrite(ledGpio[3],LOW);
	while(1){
		delay(1000);			//1秒間の時間待ち
		pwmWrite(BUZZER,0);		//ブザー止める		
		cnt = micros();
		diff = cnt - cntOld;
		ave = (diff + ave*(acnt-1))/acnt;  //平均値
		if(cnt > cntOld){		//オーバーフローしたときに余りを秒に含めない
			seconds = cnt / ave;
		}else{
			seconds=0;
		}
//		seconds = cnt/1000150;		//1秒
//		seconds = cnt/diff;
		secRemainders = seconds%60;  //秒の余り
		minutes = seconds/60;
		printf("%d %u %u %u %llu %02d:%02d",no,cnt,diff,acnt,ave, minutes,secRemainders);
		
		if(cnt > cntOld){
			seconds2++;
		}else{
			seconds2=0;
		}		
		secRemainders2 = seconds2%60;
		minutes2 = seconds2/60;
		printf("  %02d:%02d\n",minutes2,secRemainders2);
		
		//経過時間とループ文のカウント数（秒）と齟齬が発生した場合のアラーム	
		if(secRemainders2 != secRemainders){
			pwmWrite(BUZZER,range/2);	//ブザーを鳴らす
			digitalWrite(ledGpio[3],HIGH);
		}
		
		//経過時間がオーバーフローしたときのアラーム。
		if(cnt < cntOld){
			no++;
			alt = !alt;
			digitalWrite(ledGpio[0],alt);
			pwmWrite(BUZZER,range/2);	//ブザーを鳴らす
//			cntOld = 0;
			acnt = 1;
			cntOld = cnt;
		}else{
			cntOld = cnt;			//更新
			acnt++;
		}
		
//		acnt++;
	}
	return EXIT_SUCCESS;
}
