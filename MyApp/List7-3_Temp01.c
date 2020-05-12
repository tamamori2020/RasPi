// List7-3_Temp01.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <wiringPi.h>		//wiringPi
#include <wiringPiI2C.h>	//I2C用wiringPi

#define ADT7410_ADR	0x48	//温度センサのスレーブアドレス
#define ADT7410_REG	0		//温度レジスタの番号

int main(void){
	int fd;								//ファイルディスクリプタ
	int wdat;							//温度センサバイナリ値
	double temp;						//温度 実数値℃
	const float tempDelta = 0.0625F;	//温度の刻値　0.0625℃
		
	fd = wiringPiI2CSetup(ADT7410_ADR);	//温度センサのI2Cセットアップ
	while(1){
		wdat = wiringPiI2CReadReg16(fd, ADT7410_REG);	//センサよりデータを取得
		wdat = (wdat&0xff00)>>8 | (wdat&0xff)<<8;		//上位と下位のバイトを交換する
		wdat = wdat >>3;								//13bitフォーマット形式
		
		if ((wdat&0x1000)==0){			//温度の正負を判断
			temp = wdat *tempDelta;		//プラス温度の計算
		}else{
			temp = ((~wdat&0x1fff)+1)*(-tempDelta);	//マイナス温度の計算
		}
		printf("%4.1f C\n",temp);
		delay(500);	
	}
}
