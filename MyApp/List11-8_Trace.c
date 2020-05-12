// List11-8_Trace.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <wiringPi.h>		//wiringPi一般
#include <wiringPiI2C.h>	//I2C用wiringPi
#include  "MyPi.h"			//マイライブラリ

/* LCD関係　*/
#define LCD_ADR	0x3e		//LCD I2Cアドレス
#define LCD_IR	0x00	    //制御用レジスタ
/* GPIOの番号　*/
#define SW_RED      17      //赤色SW
#define SW_WHITE    27      //白色SW
#define SEN_RIGHT   4       //右側 反射型フォトセンサ
#define SEN_CENTER  5       //中央 反射型フォトセンサ
#define SEN_LEFT    6       //左側 反射型フォトセンサ
#define MOT_RIGHT2  22      //右モータ BIN2
#define MOT_RIGHT1  23      //右モータ BIN1
#define MOT_LEFT2   24      //左モータ AIN2
#define MOT_LEFT1   25      //左モータ AIN1
/* グローバル宣言 */
//入力用 SW、反射型フォトセンサ
const int inGpio[5] = {SW_RED,SW_WHITE,SEN_RIGHT,SEN_CENTER,SEN_LEFT}; 
//出力用 モータ駆動IC信号線
const int outGpio[4] = {MOT_RIGHT1,MOT_RIGHT2,MOT_LEFT1,MOT_LEFT2};  
/* プロトタイプ宣言 */
int ReadSens(void);             //フォトセンサの位置情報の取得
void Sens2Lcd(int fd, int sensors);   //位置情報をLCDに表示
void RightForward(void);	//右モータ正転
void RightReverse(void);	//右モータ逆転
void LeftForward(void);		//左モータ正転
void LeftReverse(void);		//左モータ逆転
void Forward(void);		    //左右のモータの正転
void Reverse(void);		    //左右のモータの逆転
void Stop(void);		    //左右のモータのストップ
void LcdClrMsg(int fd, char *s);	//ディスプレイクリア＋文字列表示

int main(void){
    int fd;
    int i;
    int sensors;                //現在のセンサの値
    int old=0b0111;             //過去のセンサの値
	wiringPiSetupGpio();
    for(i=0;i<5;i++){			//入力に設定
        pinMode(inGpio[i], INPUT);}
    for(i=0;i<5;i++){			//プルダウン抵抗をつける
        pullUpDnControl(inGpio[i],PUD_DOWN);}
    for(i=0;i<4;i++){            //出力に設定
        pinMode(outGpio[i],OUTPUT);}
	fd = wiringPiI2CSetup(LCD_ADR);		//LCDの有効化
	i=LcdSetup(fd);						//LCDの初期化

    LcdClrMsg(fd,"Push White SW"); 
    while(1){
        if(digitalRead(SW_WHITE)==HIGH){
            while(1){
                sensors = ReadSens();
                if (sensors != old){  
                    LcdClear(fd);               
                    Sens2Lcd(fd, sensors);
                    old = sensors;            
                    switch (sensors){
                    case 0b000:
                    case 0b101:
                        Forward();
                        break;
                    case 0b100:
                    case 0b110:
                        RightForward();
                        break;
                    case 0b001:
                    case 0b011:
                        LeftForward();
                        break;
                    default:
                        Stop();
                        break;
                    }
                }
           }
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
