#include <stdio.h>			//入出力
#include <stdlib.h>			//一般ユーティリティ

int main(void){
	unsigned int a,b,ab,ba;
	a = 984095;
	b = 4294951235;
	ab = a - b;
	ba = b - a;
	printf("a-b=%u b-a=%u\n",ab,ba);
	return 0;
}
	
