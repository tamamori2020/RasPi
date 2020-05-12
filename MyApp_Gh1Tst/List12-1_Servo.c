// List12-1_Servo.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <wiringPi.h>		//wiringPi一般
#include <wiringPiI2C.h>	//I2C用wiringPi
#include <softPwm.h>        //ソフトウェア方式PWM(wiringPi)
#include  "MyPi.h"		    //マイライブラリ

/* LCD関係　*/
#define LCD_ADR	    0x3e	//LCD スレーブアドレス

/* Servo関係　*/
#define SERVO       13		//GPIO13をSERVOと定義

/* プロトタイプ宣言 */
void LcdClrMsg(int fd, char *s);	//ディスプレイクリア＋文字列表示

int main(void){
    int fd; 
    float theta;                	
    char s1[17];                //17文字(Null含む)のバッファ
    int tON= 0;        		    //パルスオン時間
    int pwmRange = 200;		    //T = 100us*200 = 20ms(50Hz)
    wiringPiSetupGpio();
    pinMode(SERVO, OUTPUT);
    softPwmCreate(SERVO, tON, pwmRange);	
    fd = wiringPiI2CSetup(LCD_ADR);	//LCDの有効化
    LcdSetup(fd);			        //LCDの初期化
    
    while(1){
        printf("パルスオン時間(0.1ms*) として0から30までの整数を入力 >>> ");
        scanf("%d",&tON);
        if (0<=tON && tON<=30){
            softPwmWrite(SERVO,tON);		//PWM信号発生
            theta = tON*0.1;
            printf("tON = %2.1f ms\n\n",theta);
            sprintf(s1,"tON = %2.1f ms",theta);
            LcdClrMsg(fd,s1);
        }else{
            softPwmWrite(SERVO,0);			//PWM停止
            printf("値が範囲外です\n\n"); 
            LcdClrMsg(fd,"error");         
        }
    }
    return EXIT_SUCCESS;
}

/* void LcdClrMsg(char *s)
  LCDクリアして文字列を表示
  引数　文字列
  戻り値　なし　　　　 */
void LcdClrMsg(int fd, char *s){
    LcdClear(fd);
    LcdWriteString(fd, s);		//LCDに表示
}

