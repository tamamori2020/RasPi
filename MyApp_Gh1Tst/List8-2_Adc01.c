// List8-2_Adc01.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0 -lm libMyPi.a
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ
#include <string.h>			//文字列操作
#include <wiringPi.h>		//wiringPi
#include <wiringPiSPI.h>	//SPI用wiringPi


//SPIインタフェース
#define SPI_SS0  0			//スレーブセレクト
#define SPI_SPEED   500000  // 500kHz

//A/Dコンバータ MCP3208
#define MCP3208_CH0 0		// CH0入力
#define MCP3208_CH1 1		// CH1入力
#define MCP3208_CH2 2		// CH2入力
#define MCP3208_CH3 3		// CH3入力
#define MCP3208_CH4 4		// CH4入力
#define MCP3208_CH5 5		// CH5入力
#define MCP3208_CH6 6		// CH6入力
#define MCP3208_CH7 7		// CH7入力

unsigned short  Mcp3208RW(int ssNo, unsigned char adcChannel);

int main(void)
{
	unsigned short adcCode   = 0;		//ADCコード
	wiringPiSetupGpio();				//BCMのGPIO番号を使用
	wiringPiSPISetup(SPI_SS0, SPI_SPEED);	//SPIのセットアップ
	while(1){			//AD変換処理
		adcCode = Mcp3208RW(SPI_SS0,MCP3208_CH7);
		printf("CH7 = %3XH", adcCode);
		printf("\t電圧 = %5.3f V\n", ((3.3/4096) * adcCode));
		delay(500);
	}
    return EXIT_SUCCESS;
}

//関数名  unsigned short Mcp3208RW(int ssNo, unsigned char adcChannel)
//引数    ssNo スレーブ選択番号 0=SS0, 1=SS1
//引数    adcChannel    MPC3208のチャンネル番号　0から7
//戻り値  12bitAD変換データ 0000-0fff
//概要　　0Vから約3.3Vの範囲のアナログ電圧を12bit分解能でディジタル値に変換 
unsigned short Mcp3208RW(int ssNo, unsigned char adcChannel)
{
	unsigned short adcCode = 0;
	unsigned char buff[3];
	buff[0]=buff[1]=buff[2]=0;			
	buff[0] =0b00000110|(adcChannel>>2);  	// 0,0,0,0,0,ST,SGL,D2
	buff[1]=adcChannel<<6;					// D1,D0,0,0,0,0,0,0
	wiringPiSPIDataRW(ssNo, buff, sizeof(buff));
	//デバック表示用
	//printf("buff0=%3XH buff1=%x buff2=%3XH \n",buff[0],buff[1],buff[2]);  
	adcCode =(((buff[1]&0x0f) << 8) | buff[2]); // 0000+MSB----------LSB
	return adcCode;
}

