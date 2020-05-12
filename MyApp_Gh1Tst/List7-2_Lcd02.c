// List7-2_Lcd02.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// gccコマンドにライブラリlibMyPi.aを作成して追記します
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <wiringPi.h>		//wiringPi
#include <wiringPiI2C.h>	//I2C用wiringPi
#include "MyPi.h"			//マイライブラリ

#define LCD_ADR	0x3e		//LCD スレーブアドレス

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

