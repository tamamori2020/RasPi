// List11-6_Motor.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <wiringPi.h>		//wiringPi一般
#include <wiringPiI2C.h>	//I2C用wiringPi
#include  "MyPi.h"			//マイライブラリ

#define LCD_ADR		0x3e	//LCD スレーブアドレス
#define SW_WHITE    27      //白色SW

#define MOT_RIGHT2  22      //右モータ BIN2
#define MOT_RIGHT1  23      //右モータ BIN1
#define MOT_LEFT2   24      //左モータ AIN2
#define MOT_LEFT1   25      //左モータ AIN1
#define RUN_TIME    2000    //動作時間２秒

/* グローバル宣言 */
//出力用 モータ駆動IC信号線
const int outGpio[4] = {MOT_RIGHT1,MOT_RIGHT2,MOT_LEFT1,MOT_LEFT2};  
/* プロトタイプ宣言 */
void RightForward(void);	//右モータ正転
void RightReverse(void);	//右モータ逆転
void LeftForward(void);		//左モータ正転
void LeftReverse(void);		//左モータ逆転
void Forward(void);		    //左右のモータの正転
void Reverse(void);		    //左右のモータの逆転
void Stop(void);		    //左右のモータのストップ
void LcdClrMsg(int fd, char *s);	//ディスプレイクリア＋文字列表示

int main(void){
	int i;
    int fd;             
	wiringPiSetupGpio();
    pinMode(SW_WHITE, INPUT);		//白色SW 入力設定
    pullUpDnControl(SW_WHITE,PUD_DOWN);
    for(i=0;i<4;i++){            	//出力に設定
        pinMode(outGpio[i], OUTPUT);}
	fd = wiringPiI2CSetup(LCD_ADR);	//LCDの有効化
	LcdSetup(fd);			        //LCDの初期化

    LcdClrMsg(fd,"Push white SW.");  
    while(1){
        if(digitalRead(SW_WHITE)==HIGH){

        RightForward();
        LcdClrMsg(fd,"Right wheel moves forward.");
        delay(RUN_TIME);

        RightReverse();
        LcdClrMsg(fd,"Right wheel moves reverse.");
        delay(RUN_TIME);

        LeftForward();
        LcdClrMsg(fd,"Left wheel moves forward.");
        delay(RUN_TIME);
        
        LeftReverse();
        LcdClrMsg(fd,"Left wheel moves reverse.");
        delay(RUN_TIME);
        
        Forward();
        LcdClrMsg(fd,"All wheels move  forward.");
        delay(RUN_TIME);
        
        Reverse();
        LcdClrMsg(fd,"All wheels move  reverse.");
        delay(RUN_TIME);
        
        Stop();
        LcdClrMsg(fd,"All wheels stop."); 
        delay(RUN_TIME);
        
        LcdClrMsg(fd,"Push white SW.");  
        }
    }
 
	return EXIT_SUCCESS;
}

/* モータ関係の関数
  右モータ正転　RightForward()
  右モータ逆転　RightReverse()
  左モータ正転　LeftForward()
  左モータ逆転　LeftReverse()
  左右のモータの正転　Forward()
  左右のモータの逆転　Reverse()
  左右のモータのストップ　Stop()
  引数および戻り値　なし */
void RightForward(void){
    digitalWrite(MOT_RIGHT2,LOW);
    digitalWrite(MOT_RIGHT1,HIGH);
    digitalWrite(MOT_LEFT2,LOW);
    digitalWrite(MOT_LEFT1,LOW);
}
void RightReverse(void){
    digitalWrite(MOT_RIGHT2,HIGH);
    digitalWrite(MOT_RIGHT1,LOW);
    digitalWrite(MOT_LEFT2,LOW);
    digitalWrite(MOT_LEFT1,LOW);
}
void LeftForward(void){
    digitalWrite(MOT_RIGHT2,LOW);
    digitalWrite(MOT_RIGHT1,LOW);
    digitalWrite(MOT_LEFT2,LOW);
    digitalWrite(MOT_LEFT1,HIGH);
}
void LeftReverse(void){
    digitalWrite(MOT_RIGHT2,LOW);
    digitalWrite(MOT_RIGHT1,LOW);
    digitalWrite(MOT_LEFT2,HIGH);
    digitalWrite(MOT_LEFT1,LOW);
}
void Forward(void){
    digitalWrite(MOT_RIGHT2,LOW);
    digitalWrite(MOT_RIGHT1,HIGH);
    digitalWrite(MOT_LEFT2,LOW);
    digitalWrite(MOT_LEFT1,HIGH);
}
void Reverse(void){
    digitalWrite(MOT_RIGHT2,HIGH);
    digitalWrite(MOT_RIGHT1,LOW);
    digitalWrite(MOT_LEFT2,HIGH);
    digitalWrite(MOT_LEFT1,LOW);
}
void Stop(void){
    digitalWrite(MOT_RIGHT2,HIGH);
    digitalWrite(MOT_RIGHT1,HIGH);
    digitalWrite(MOT_LEFT2,HIGH);
    digitalWrite(MOT_LEFT1,HIGH);
}
/* void LcdClrMsg(char *s)
  LCDクリアして文字列を表示
  引数　文字列
  戻り値　なし　　　　 */
void LcdClrMsg(int fd, char *s){
    LcdClear(fd);
    LcdWriteString(fd, s);		//LCDに表示
}
