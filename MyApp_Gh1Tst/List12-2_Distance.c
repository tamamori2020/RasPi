// List12-2_Distance.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			    //入出力
#include <stdlib.h>			    //一般ユーティリティ
#include <string.h>			    //文字列操作
#include <wiringPi.h>		    //wiringPi一般
#include <wiringPiI2C.h>	    //I2C用wiringPi
#include <softPwm.h>            //ソフトウェア方式PWM(wiringPi)
#include  "MyPi.h"		        //マイライブラリ

/* LCD関係　*/
#define LCD_ADR		    0x3e	//LCD スレーブアドレス

/* 距離センサ関係　*/
#define GP2Y0E03_ADR    0x40    //GP2Y0E03 スレーブアドレス
#define GP2Y0E03_DIS_U	0x5e    //距離データレジスタ（上位8bit）
#define GP2Y0E03_DIS_L	0x5f    //距離データレジスタ（下位4bit）

/* プロトタイプ宣言 */
int ReadDistance(int fd2);

int main(void){
    int fd1;        //LCD用
    int fd2;        //GP2Y0E03用
    int i;
    char s1[17];	//17文字分のバッファの確保
	
    fd1 = wiringPiI2CSetup(LCD_ADR);		//LCDの有効化
    i=LcdSetup(fd1);						//LCDの初期化
    fd2 = wiringPiI2CSetup(GP2Y0E03_ADR);	//距離センサの有効化   
    while(1){
        i = ReadDistance(fd2);
        LcdClear(fd1);			        //画面クリア
        if(4<=i && i<=50){	
            sprintf(s1,"%d cm",i);		//測定値を文字列に変換
            LcdWriteString(fd1, s1);	//LCDに測定値を表示
            printf("Dis =%d cm\n",i);	//ターミナルにデバック表示
        }else{
            LcdWriteString(fd1,"out of range");
            printf("out of range\n");
        }
        delay(500);					//0.5秒待ち時間
    }
    return EXIT_SUCCESS;
}

/* 距離センサ GP2Y0E03
 * 引数　fd1
 * 戻り値　距離(整数) cm*/
int ReadDistance(int fd2){
    int disU8bit,disL4bit,dis12bit;	
    disU8bit = wiringPiI2CReadReg8(fd2, GP2Y0E03_DIS_U);    //上位8bit取得
    disL4bit = wiringPiI2CReadReg8(fd2, GP2Y0E03_DIS_L);    //下位4bit取得
    dis12bit = (disU8bit<<4)|disL4bit;  //12bit=上位8bit+下位4bit
    dis12bit = dis12bit>>6;             //6bit右シフト
    return dis12bit;
}

