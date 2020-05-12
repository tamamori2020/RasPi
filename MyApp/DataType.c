// DataType.c
// gcc -Wall -o "%e" "%f" -lwiringPi -lpthread -g -O0
// Geanyのオプションの"%e"は実行ファイル名, "%f"はソースファイル名のこと

#include <stdio.h>              //入出力
#include <stdlib.h>             //一般ユーティリティ
#include <limits.h>             //整数型の大きさ
#include <float.h>              //浮動小数点型の特性

int main (void)
{
    printf("%s %d %dbyte\n", "(unsigned) char型の最大値", UCHAR_MAX, sizeof(unsigned char));
    printf("%s %d %dbyte\n", "signed char型の最大値", SCHAR_MAX, sizeof(signed char));
    printf("%s %d %dbyte\n\n", "signed char型の最小値", SCHAR_MIN, sizeof(signed char));

    printf("%s %d %dbyte\n", "unsigned short int型の最大値", USHRT_MAX, sizeof(unsigned short int));
    printf("%s %d %dbyte\n", "(signed) short int型の最大値", SHRT_MAX, sizeof(signed short int));
    printf("%s %d %dbyte\n\n", "(signed) short int型の最小値", SHRT_MIN, sizeof(signed short int));

    printf("%s %u %dbyte\n", "unsigned int型の最大値", UINT_MAX, sizeof(unsigned int));    
    printf("%s %d %dbyte\n", "(signed) int型の最大値", INT_MAX, sizeof(signed int));
    printf("%s %d %dbyte\n\n", "(signed) int型の最小値", INT_MIN, sizeof(signed int));

    printf("%s %lu %dbyte\n", "unsigned long int型の最大値", ULONG_MAX, sizeof(unsigned long int));    
    printf("%s %ld %dbyte\n", "(signed) long int型の最大値", LONG_MAX, sizeof(signed long int));
    printf("%s %ld %dbyte\n\n", "(signed) long int型の最小値", LONG_MIN, sizeof(signed long int));

    printf("%s %llu %dbyte\n", "unsigned long long int型の最大値", ULLONG_MAX, sizeof(unsigned long long int));    
    printf("%s %lld %dbyte\n", "(signed) long long int型の最大値", LLONG_MAX, sizeof(signed long long int));
    printf("%s %lld %dbyte\n\n", "(signed) long long int型の最小値", LLONG_MIN, sizeof(signed long long int));

    printf("%s %g %dbyte\n", "float型の最大値", FLT_MAX, sizeof(float));
    printf("%s %g %dbyte\n\n", "float型の最小な正数", FLT_MIN, sizeof(float));

    printf("%s %g %dbyte\n", "double型の最大値", DBL_MAX, sizeof(double));
    printf("%s %g %dbyte\n\n", "double型の最小な正数", DBL_MIN, sizeof(double));

    printf("%s %Lg %dbyte\n", "long double型の最大値", LDBL_MAX, sizeof(long double));
    printf("%s %Lg %dbyte\n", "long double型の最小な正数", LDBL_MIN, sizeof(long double));

    return EXIT_SUCCESS;
}
