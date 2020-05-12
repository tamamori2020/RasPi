// List11-5_PhotoSen.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>				//入出力
#include <stdlib.h>				//一般ユーティリティ
#include <string.h>				//文字列操作
#include <wiringPi.h>			//wiringPi一般
#include <wiringPiI2C.h>		//I2C用wiringPi
#include  "MyPi.h"				//マイライブラリ

/* LCD関係　*/
#define LCD_ADR	    0x3e		//LCD スレーブアドレス
#define LCD_IR	    0x00	    //インストラクション
/* GPIOの番号　*/
#define SEN_RIGHT   4           //右側 反射型フォトセンサ
#define SEN_CENTER  5           //中央 反射型フォトセンサ
#define SEN_LEFT    6           //左側 反射型フォトセンサ
/* グローバル宣言 */
const int inGpio[5] = {SEN_RIGHT,SEN_CENTER,SEN_LEFT};
/* プロトタイプ宣言 */
int ReadSens(void);             //フォトセンサの位置情報の取得
void Sens2Lcd(int fd, int sensors);   //位置情報をLCDに表示

int main(void){
    int fd;
    int i;
    int sensors;                //現在のセンサの値
    int old=0b0111;             //過去のセンサの値
	wiringPiSetupGpio();
    for(i=0;i<3;i++){			//入力に設定
        pinMode(inGpio[i], INPUT);}
    for(i=0;i<3;i++){			//プルダウン抵抗をつける
        pullUpDnControl(inGpio[i],PUD_DOWN);}
	fd = wiringPiI2CSetup(LCD_ADR);		//LCDの有効化
	LcdSetup(fd);						//LCDの初期化

    while(1){           
        sensors = ReadSens();           //反射型フォトセンサの読み込み
        if (sensors!=old){              //センサの値が更新されたらLCDに表示する
            LcdClear(fd);               //画面クリア
            Sens2Lcd(fd, sensors);      //LCDにセンサの値を表示
            old=sensors;                //センサの値を更新
        }
    }
	return EXIT_SUCCESS;
}

/* int ReadSens(void)
  反射型フォトセンサの位置情報取得
  引数　なし
  戻り値　センサの位置情報
  2bit  1bit    0bit
  左(L)、中央(C)、右(R)　*/
int ReadSens(void){
    int i;
    int sensors=0;
    i = digitalRead(SEN_RIGHT);
    if(i==1) {sensors=1;}       //0bit目 右(R)センサ
    i = digitalRead(SEN_CENTER);
    if(i==1) {sensors+=2;}      //1bit目 中央(C)センサ
    i = digitalRead(SEN_LEFT);
    if(i==1) {sensors+=4;}      //2bit目 左(L)センサ
    return sensors;
}

/* void Sens2Lcd(int fd, int sensors)
　反射型フォトセンサの位置情報のLCDに表示
  引数　センサの位置情報
  戻り値　なし
  LCDの表示アドレス
  0123456789ABCDEF
  L      C       R
  アドレスカウンタの7bit目(ac7)は常に1 */
void Sens2Lcd(int fd, int sensors){
    int ac7=0b10000000;
    
    wiringPiI2CWriteReg8(fd, LCD_IR,ac7+0); //左センサ
    if(sensors & 0b0100){               
        LcdWriteChar(fd, '1');
    }
    else{
        LcdWriteChar(fd, '0');
    }
    
    wiringPiI2CWriteReg8(fd, LCD_IR,ac7+7);  //中央センサ  
    if(sensors & 0b0010){               
        LcdWriteChar(fd, '1');
    }
    else{
        LcdWriteChar(fd, '0');
    }
    
    wiringPiI2CWriteReg8(fd, LCD_IR,ac7+0xf);  //右(L)センサ                
    if(sensors & 0b0001){               
        LcdWriteChar(fd, '1');
    }
    else{
        LcdWriteChar(fd, '0');
    }                               
}
