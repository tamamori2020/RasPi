// List4-1_Led01.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ		
#include <wiringPi.h>		//wiringPi
#define LED0 23				

int main (void){
	wiringPiSetupGpio();			
	pinMode(LED0, OUTPUT);		
	for(;;){						
		digitalWrite(LED0,HIGH);	
		delay(500);				
		digitalWrite(LED0, LOW);	
		delay(500);
	}
	return EXIT_SUCCESS;
}
