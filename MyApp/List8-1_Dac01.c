// List8-1_Dac01.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <wiringPi.h>		//wiringPi
#include <wiringPiSPI.h>	//SPI用wiringPi

//SPIインタフェース
#define SPI_SS1  1			//DACスレーブセレクト
#define SPI_SPEED   500000  //SPIのSCKクロック信号の周波数

//D/Aコンバータ　MCP4922
#define MCP4922_DACA	0		//VOUTAの出力
#define MCP4922_DACB	1		//VOUTBの出力
#define MCP4922_GAx1	0x20	//ゲイン1倍
#define MCP4922_SHDN	0x10	//VOUTピン出力の有効

int Mcp4922Write(int ssNo, unsigned char dacCh, unsigned short dacCode); 

int main (void){
	unsigned int dacCode;
	wiringPiSetupGpio();
	wiringPiSPISetup(SPI_SS1, SPI_SPEED);	//SPIのセットアップ
    while(1){
		printf("16進数で0からfffまでの値を入力してください >>>");
		scanf("%x",&dacCode);
		if(0<=dacCode && dacCode <=0xfff){
			printf("VoutA,VoutB = %5.3f V\n\n", ((3.3/4096) * dacCode));			
			Mcp4922Write(SPI_SS1,MCP4922_DACA,dacCode);
			Mcp4922Write(SPI_SS1,MCP4922_DACB,dacCode);
		}else{
			printf("値が範囲外です\n\n");
			Mcp4922Write(SPI_SS1,MCP4922_DACA,0);
			Mcp4922Write(SPI_SS1,MCP4922_DACB,0);
		}
	}
    return EXIT_SUCCESS;
}

//関数名  int Mcp4922Write(int ssNo,unsigned char dacCh, unsigned short dacCode)
//引数    ssNo スレーブ選択番号 0=SS0, 1=SS1
//引数    dacCh 0=DOUTA, 1=DOUTB
//引数    dacCode 12bit DACコード 0000-0fff
//戻り値  wiringPiSPIDataRWの戻り値
//概要　　0Vから約3.3Vの範囲で、12bit分解能でアナログ電圧を出力
int Mcp4922Write(int ssNo,unsigned char dacCh, unsigned short dacCode)
{
	int i;
	unsigned char buff[2];
	unsigned short dacData;
	buff[0]=0;
	if(dacCh == MCP4922_DACB){							//DACB
		buff[0] = buff[0]|0x80;	
	}

	buff[0]=buff[0]|MCP4922_GAx1|MCP4922_SHDN;
	dacData=((dacCode>>8)&0x0f);				
	buff[0]=buff[0]|dacData;					//MSB 4bit DAデータ上位4ビット
	dacData= dacCode&0x00ff;
	buff[1]=dacData;							//LSB 8bit DAデータ下位8ビット
	
	i=wiringPiSPIDataRW(ssNo, buff, sizeof(buff));  //SPIバスへ出力
	return i;
}
