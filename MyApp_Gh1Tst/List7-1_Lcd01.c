// List7-1_Lcd01.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <wiringPi.h>		//wiringPi
#include <wiringPiI2C.h>	//I2C用wiringPi

#define LCD_ADR	0x3e		//LCD スレーブアドレス
#define LCD_IR	0x00		//インストラクション
#define LCD_DR 0x40			//データ

//LCD用関数のプロトタイプ宣言
int LcdSetup(int fd);
int LcdWriteChar(int fd, char c);
int LcdNewline(int fd);
int LcdClear(int fd);
int LcdWriteString(int fd, char *s);


/* グローバル変数  */
int g_position =0;			//文字の位置
int g_line = 0;				//行の位置
int g_charsPerLine = 16;	//最大表示文字数
int g_dispLines=2;			//最大表示行数

int main(int argc, char *argv[]){
	int i;
	int fd;
	fd = wiringPiI2CSetup(LCD_ADR);			//LCDのI2Cセットアップ
	i=LcdSetup(fd);							//LCDの初期化
	if(i<0){
		printf("LCD setup error.\n");
		return EXIT_FAILURE;
	}
	
	if(argc ==1){
		LcdWriteString(fd, "* Hello World! *");	//文字列の表示
		LcdWriteString(fd, "0123456789ABCDEF");
	}else{			//main関数に文字列の引数がある場合の処理
		i=argc;
		for (i=1;i<argc;i++){
			LcdWriteString(fd, argv[i]);
			LcdWriteChar(fd, ' ');		//空白で複数の文字列を区切る
		}
	}
	return EXIT_SUCCESS;
}

//関数名  int LcdSetup(int fd)
//引数    fd  wiringPiI2CSetup関数で取得したファイルハンドル
//戻り値  エラーのときは-1
//概要    LCDの初期化
int LcdSetup(int fd){
	int i,k;
	char lcdCmd1[6]={0x38,0x39,0x14,0x73,0x56,0x6c}; //データシートより
	char lcdCmd2[3]={0x38,0x01,0x0c};				 //データシートより
	
	for (i=0;i<6;i++){
		k = wiringPiI2CWriteReg8(fd,LCD_IR,lcdCmd1[i]);
		delay(20);		//20ms時間待ち
	}
	delay(200);			//200ms時間待ち
	
	for (i=0;i<3;i++){
		k = wiringPiI2CWriteReg8(fd,LCD_IR,lcdCmd2[i]);
		delay(20);		//20ms時間待ち	
	}
	return k;
}

//関数名  int LcdWriteChar(int fd, char c)
//引数    fd  wiringPiI2CSetup関数で取得したファイルハンドル
//引数    c   1文字
//戻り値  エラーのときは-1
//概要    1文字の表示
int LcdWriteChar(int fd, char c){
	int i;
    //行の最大文字数を超えたかの判定
	if( g_position==(g_charsPerLine*(g_line+1))){		
		LcdNewline(fd);		//改行処理
	}
	i = wiringPiI2CWriteReg8(fd,LCD_DR,c);
//	printf("%c",c);fflush;	//debug用
	g_position +=1;
    delay(1);               //1ms時間待ち
	return i;
}

//関数名  int LcdNewline(int fd)
//引数    fd  wiringPiI2CSetup関数で取得したファイルハンドル
//戻り値  エラーのときは-1
//概要    LCD改行処理
int LcdNewline(int fd){
	int i;
	if (g_line == (g_dispLines-1)){
		LcdClear(fd);
	}else{
		g_line +=1;
		i = wiringPiI2CWriteReg8(fd,LCD_IR,0xc0);
        delay(10);      //10ms時間待ち
	}
	return i;
}

//関数名  int LcdClear(int fd)
//引数    fd  wiringPiI2CSetup関数で取得したファイルハンドル
//戻り値  エラーのときは-1
//概要    LCDのディスプレイクリア
int LcdClear(int fd){
	int i;
	i = wiringPiI2CWriteReg8(fd,LCD_IR,0x01);
	g_position =0;
	g_line=0;
	delay(10);			//10ms時間待ち
	return i;
}

//関数名  int LcdWriteString(int fd, char *s)
//引数    fd  wiringPiI2CSetup関数で取得したファイルハンドル
//引数    *s   文字列
//戻り値  エラーのときは-1
//概要    LCDに文字列を表示
int LcdWriteString(int fd, char *s){
	int i,k;
	for(i=0;i<strlen(s);i++){
		k = LcdWriteChar(fd, s[i]);
//		putchar(s[i]);	//debug用
		delay(1);       //1ms時間待ち
	}
	return k;
}


