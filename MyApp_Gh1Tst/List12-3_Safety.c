// List12-3_Safety.c
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
#define LCD_ADR	0x3e			//LCD I2Cアドレス
#define LCD_IR	0x00	        //制御用レジスタ
/* 距離センサ関係　*/
#define GP2Y0E03_ADR    0x40    //GP2Y0E03 スレーブアドレス
#define GP2Y0E03_DIS_U	0x5e    //距離データレジスタ（上位8bit）
#define GP2Y0E03_DIS_L	0x5f    //距離データレジスタ（下位4bit）
#define GP2Y0E03_RES	40		//応答時間(ms)
#define DANGER          15      //物体検出距離(cm)

/* Servo関係　*/
#define SERVO           13	    //GPIO13をSERVOと定義

/* GPIOの番号　*/
#define SW_RED          17      //赤色SW
#define SW_WHITE        27      //白色SW
#define SEN_RIGHT       4       //右側 反射型フォトセンサ
#define SEN_CENTER      5       //中央 反射型フォトセンサ
#define SEN_LEFT        6       //左側 反射型フォトセンサ
#define MOT_RIGHT2      22      //右モータ BIN2
#define MOT_RIGHT1      23      //右モータ BIN1
#define MOT_LEFT2       24      //左モータ AIN2
#define MOT_LEFT1       25      //左モータ AIN1

/* グローバル宣言 */
//入力用 SW、反射型フォトセンサ
const int inGpio[5] = {SW_RED,SW_WHITE,SEN_RIGHT,SEN_CENTER,SEN_LEFT}; 
//出力用 モータ駆動IC信号線
const int outGpio[4] = {MOT_RIGHT1,MOT_RIGHT2,MOT_LEFT1,MOT_LEFT2};  
/* プロトタイプ宣言 */
int ReadSens(void);             //フォトセンサのデータの取得
void Sens2Lcd(int fd1, int positionNow);   //フォトセンサの値をLCDに表示
void RightForward(void);	    //右モータ前転
void RightBack(void);		    //右モータ後転
void LeftForward(void);		    //左モータ前転
void LeftBack(void);		    //左モータ後転
void Forward(void);		        //前進
void Back(void);		        //バック
void Stop(void);		        //モータ停止、ブレーキモード
void LcdClrMsg(int fd1, char *s);	//LCDメッセージマクロ
int ReadDistance(int fd2);

int main(void){
    int fd1;                    //LCD用
    int fd2;                    //GP2Y0E03用
    int i;
    char s1[17];	            //17文字分のバッファの確保
    int positionNow = 0;        //現在の位置センサの値
    int distanceNow = 0;        //現在の距離の値(cm)
    int distanceOld = 100;      //過去の距離の値(cm)
    int tON= 0;        		    //パルスオン時間
    int pwmRange = 200;		    //T = 100us*200 = 20ms(50Hz)
	unsigned int timeNow1;      //距離センサ用
	unsigned int timeOld1 =0;
	unsigned int timeNow2;      //RCサーボモータ用
	unsigned int timeOld2 =0;
    
	wiringPiSetupGpio();
    for(i=0;i<5;i++){			//入力に設定
        pinMode(inGpio[i], INPUT);}
    for(i=0;i<5;i++){			//プルダウン抵抗をつける
        pullUpDnControl(inGpio[i],PUD_DOWN);}
    for(i=0;i<4;i++){            //出力に設定
        pinMode(outGpio[i],OUTPUT);}
	fd1 = wiringPiI2CSetup(LCD_ADR);		//LCDの有効化
	i=LcdSetup(fd1);						//LCDの初期化
    fd2 = wiringPiI2CSetup(GP2Y0E03_ADR);	//距離センサの有効化    
    pinMode(SERVO, OUTPUT);
    softPwmCreate(SERVO, tON, pwmRange);    //PWM信号の初期化
    
    softPwmWrite(SERVO,5);                  //RCサーボモータ右側に向ける 
    delay(1000);
    softPwmWrite(SERVO,17);                 //RCサーボモータ左側に向ける
    delay(1000);
    softPwmWrite(SERVO,11);                 //RCサーボモータ正面に向ける
    delay(1000);
    softPwmWrite(SERVO,0);

    LcdClrMsg(fd1,"Push White SW");         //LCDに表示
    do{
        i = digitalRead(SW_WHITE);
    }while(i != HIGH);

    while(1){       
        do{
			timeNow1 = millis();
			if(timeNow1 > timeOld1 + GP2Y0E03_RES){
            	distanceNow = ReadDistance(fd2);
				timeOld1 = timeNow1;
			}
            if(distanceNow != distanceOld){
                LcdClear(fd1);
                sprintf(s1,"%d cm",distanceNow);	//測定値を文字列に変換
                LcdWriteString(fd1, s1);	        //LCDに測定値を表示
                
                if(distanceNow < DANGER){
                    Stop();
                    softPwmWrite(SERVO,0);
                    i = abs(distanceNow - distanceOld);
                    if(1<i && i<10){
                        delay(500);
                        system("raspistill -t 100 -a 12 -a 1024 -o `date '+%Y%m%d-%H%M%S'`.jpg");
                    }
                }
            }
            distanceOld = distanceNow;
        }while(distanceNow < DANGER);
            
        positionNow = ReadSens();

        timeNow2 = millis();
        if(timeNow2 > timeOld2 + 400){
            switch (positionNow){
            case 0b000:                     //ノーマルポジション
            case 0b101:                     //白線上にいる
                softPwmWrite(SERVO,11);     //RCサーボモータ正面に向ける
                break;
            case 0b100:                     //白線の右側にいる
            case 0b110:
                softPwmWrite(SERVO,15);     //RCサーボモータ左側に向ける
                break;
            case 0b001:                     //白線の左側にいる
            case 0b011:
                softPwmWrite(SERVO,7);      //RCサーボモータ右側に向ける
                break;
            default:
                softPwmWrite(SERVO,0);      //RCサーボモータ動作停止
                break;
            }   
            timeOld2 = timeNow2;
        }
     
        switch (positionNow){
        case 0b000:         //ノーマルポジション
        case 0b101:         //白線上にいる
            Forward();
            break;
        case 0b100:         //白線の右側にいる
        case 0b110:
            RightForward();
            break;
        case 0b001:         //白線の左側にいる
        case 0b011:
            LeftForward();
            break;
        default:
            Stop();
            softPwmWrite(SERVO,0);
            break;
        }
    }
	return EXIT_SUCCESS;
}

/* int ReadSens(void)
  反射型フォトセンサの検出
  引数　なし
  戻り値　3bit形式 
  2bit  1bit    0bit
  左(L)、中央(C)、右(R)　*/
int ReadSens(void){
    int i;
    int positionNow=0;
    i = digitalRead(SEN_RIGHT);
    if(i==1) {positionNow=1;}       //0bit目 右(R)センサ
    i = digitalRead(SEN_CENTER);
    if(i==1) {positionNow+=2;}      //1bit目 中央(C)センサ
    i = digitalRead(SEN_LEFT);
    if(i==1) {positionNow+=4;}      //2bit目 左(L)センサ
    return positionNow;
}

/* void Sens2Lcd(int fd, int positionNow)
　反射型フォトセンサのLCD表示
  引数　センサの値
  戻り値　なし
  LCDの表示アドレス
  0123456789ABCDEF
  L      C       R
  アドレスカウンタの7bit目(ac7)は常に1 */
void Sens2Lcd(int fd, int positionNow){
    int ac7=0b10000000;
    
    wiringPiI2CWriteReg8(fd, LCD_IR,ac7+0); //左センサ
    if(positionNow & 0b0100){               
        LcdWriteChar(fd, '1');
    }
    else{
        LcdWriteChar(fd, '0');
    }
    
    wiringPiI2CWriteReg8(fd, LCD_IR,ac7+7);  //中央センサ  
    if(positionNow & 0b0010){               
        LcdWriteChar(fd, '1');
    }
    else{
        LcdWriteChar(fd, '0');
    }
    
    wiringPiI2CWriteReg8(fd, LCD_IR,ac7+0xf);  //右(L)センサ                
    if(positionNow & 0b0001){               
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
  左右のモータ停止　Stop()
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
void LcdClrMsg(int fd1, char *s){
    LcdClear(fd1);
    LcdWriteString(fd1, s);		//LCDに表示
}

/* 距離センサ GP2Y0E03
 * 引数　fd1
 * 戻り値　距離(整数) cm*/
int ReadDistance(int fd2){
    int disU8bit,disL4bit,dis12bit;	
    disU8bit = wiringPiI2CReadReg8(fd2, GP2Y0E03_DIS_U);    //上位8bit取得
    disL4bit = wiringPiI2CReadReg8(fd2, GP2Y0E03_DIS_L);     //下位4bit取得
    dis12bit = (disU8bit<<4)|disL4bit;  //12bit=上位8bit+下位4bit
    dis12bit = dis12bit>>6;             //測定式
    return dis12bit;
}
