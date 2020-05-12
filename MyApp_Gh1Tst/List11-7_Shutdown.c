// List11-7_Shutdown.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <wiringPi.h>		//wiringPi一般
#include <wiringPiI2C.h>	//I2C用wiringPi
#include  "MyPi.h"			//マイライブラリ

#define LCD_ADR	0x3e		//LCD スレーブアドレス 
#define SW_RED    17        //赤色SW

int main(void){
    int fd;		          
    int swStat;                 //赤色SWの値
    int swCnt=0;                //赤色SWのカウンタ
    wiringPiSetupGpio();
    pinMode(SW_RED, INPUT);	    //赤色SW 入力設定
    pullUpDnControl(SW_RED,PUD_DOWN);
    fd = wiringPiI2CSetup(LCD_ADR);	//LCDの有効化
    LcdSetup(fd);			        //LCDの初期化
    
    while(swCnt<3){        //3秒秒待つ
        swStat=digitalRead(SW_RED);
        if(swStat==HIGH){
            swCnt++;        //SWが押されたとき
        }else{
            swCnt=0;        //SWが押されてないとき
        }
        delay(1000);        //1秒
    }
    
    LcdClear(fd);
    LcdWriteString(fd,"Shutdown start!");
    system("shutdown -h now");
    return EXIT_SUCCESS;
}




