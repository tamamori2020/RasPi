// macros.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// gccコマンドにライブラリlibMyPi.aを作成して追記します
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと
/* 
 * micros関数のテストプログラムです。micros関数の戻り値(32bit)はwiringPiSetup関数を
 * 実行してからのus単位の符号なしの経過時間です。約71分後にオーバーフローしてゼロに戻り、
 * カウントを再開します。本プログラムは実行するとターミナルとLCDにmicros関数で取得した
 * 経過時間(us)、１秒間の差分時間、経過時間(m:s)を表示します。差分時間はmicros関数が取得した
 * 現在と前回の時間の差です。約71分後に経過時間がオーバーフローしてゼロに戻るとき、差分時間は
 * 負になる場合があります。差分時間を負にしたくない場合は、abs関数で絶対値とします。
*/ 


#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <limits.h>			//整数型の大きさ
#include <unistd.h>			//POSIX関数等

#include <wiringPi.h>		//wiringPi
#include <wiringPiI2C.h>	//I2C用wiringPi
#include "MyPi.h"			//マイライブラリ

#define LED0 23				//LED0
#define LCD_ADR	0x3e		//LCD スレーブアドレス
#define BUZZER 18			//GPIO18をBUZZERと定義
#define SW0    4			//GPIO4をSW0と定義


int main(int argc, char *argv[]){
//	int i;
	int alt = 0;
//	int fd;
//	unsigned long long int cnt;
	unsigned int cnt,cntOld,diff,minutes,seconds,secRemainders;

	unsigned int minutes2,seconds2,secRemainders2;

//	char s1[17],s2[17],s3[17];
	
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
	
	pinMode(LED0,OUTPUT);
	digitalWrite(LED0,alt);
	//fd = wiringPiI2CSetup(LCD_ADR);			//LCDのI2Cセットアップ
	//i=LcdSetup(fd);							//LCDの初期化
	//if(i<0){
		//printf("LCD setup error.\n");
		//return EXIT_FAILURE;
	//}
	
	while(1){
		delay(1000);			//1秒間の時間待ち
//		sleep(1);
		cnt = micros();
		seconds2++;
//		sprintf(s1,"%llu",cnt);	//符号なし整数を10進数で出力する指定子

		diff = cnt - cntOld;
		if(cnt < cntOld){
			alt = !alt;
			digitalWrite(LED0,alt);
			pwmWrite(BUZZER,range/2);	//デューティ比50%
			delay(2000);
			pwmWrite(BUZZER,0);
			seconds2=0;
		}
		
		//if(cnt>cntOld){
			//diff = cnt - cntOld;	//差分時間の計算
		//}else{
////			diff = cnt+UINT_MAX-cntOld;
			//diff = cnt - cntOld;
			//alt = !alt;
			//digitalWrite(LED0,alt);
			//pwmWrite(BUZZER,range/2);	//デューティ比50%
			//delay(2000);
			//pwmWrite(BUZZER,0);
		//}
//		sprintf(s2,"%u",diff);
//		seconds = cnt/diff;
		seconds = cnt/1000000;
		secRemainders = seconds%60;
		minutes = seconds/60;
		//sprintf(s3,"  %02d:%02d",minutes,secRemainders);
		//LcdClear(fd);
		//LcdWriteString(fd,s1);
		//LcdNewline(fd);		
		//LcdWriteString(fd,s2);
		//LcdWriteString(fd,s3);
		//printf("%llu %u %02d:%02d",cnt,diff, minutes,secRemainders);
		printf("%u %u %02d:%02d",cnt,diff, minutes,secRemainders);
		
//		seconds2++;
		secRemainders2 = seconds2%60;  //秒の余り
		minutes2 = seconds2/60;
		printf("  %02d:%02d\n",minutes2,secRemainders2);
				
		if(secRemainders2 != secRemainders){
			pwmWrite(BUZZER,range/2);	//デューティ比50%
			delay(1000);
			pwmWrite(BUZZER,0);
			seconds2=seconds2+2;
		}
		cntOld = cnt;
	}
	return EXIT_SUCCESS;
}

