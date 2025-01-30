#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/plays.h"
#include "test_plays.h"


void testGetInputExpectedStr()
{
    //"1, 2\n" →1, 2
    int x = 0, y = 0;
    char input[] = "1, 2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 1); 
    assert(x == 1 && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
}

void testGetInputWithoutComma()
{
    //"1 2\n" → 1, 2
    int x = 0, y = 0;
    char input[] = "1 2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 1); 
    assert(x == 1 && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
}

void testGetInputWithSpace()
{
    //"1 , 2 \n" → 1, 2
    int x = 0, y = 0;
    char input[] = "1 , 2 \n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 1); 
    assert(x == 1 && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
}

void testGetInputWithoutSpace()
{
    //"1,2\n" → 1, 2
    int x = 0, y = 0;
    char input[] = "1,2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 1); 
    assert(x == 1 && y == 2);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;
}

void testGetInputFailedWithAlphabet()
{
    //"a b\n"
    int x = 0, y = 0;
    char input[] = "a b\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0); 
    assert(x == 0 && y == 0);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;

}

void testGetInputFailedTooManyNumerics()
{
    //"10 10 10\n"
    int x = 0, y = 0;
    printf("%d,%d\n", x, y);
    char input[] = "10 10 10\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0);

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup; 
}

void testGetInputFailedFloat()
{
    //"3.5 2\n"
    int x = 0, y = 0;
    char input[] = "3.5 2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;

}

void testGetInputFailedOutOfRange()
{
    //"-1 2\n"
    int x = 0, y = 0;
    char input[] = "-1 2\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;

}

void testGetInputFailedEmpty()
{
    //" \n"
    int x = 0, y = 0;
    char input[] = "\n";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;

}

void testGetInputFailedEOF()
{
    //"EOF"
    //"3.5 2\n"
    int x = 0, y = 0;
    char input[] = "EOF";

    // 標準入出力のバックアップ
    FILE* stdin_backup = stdin; 
    FILE* stdout_backup = stdout; 
    freopen("/dev/null", "w", stdout);
    
    stdin = fmemopen(input, sizeof(input), "r");

    assert(getInput(&x, &y) == 0); 

    // 標準入力を元に戻す
    stdin = stdin_backup;
    stdout = stdout_backup;

}

void runPlaysTests() {
    printf("Start runPlaysTests...\n");
    testGetInputExpectedStr();
    testGetInputWithoutComma();
    testGetInputWithSpace();
    testGetInputWithoutSpace();
    testGetInputFailedWithAlphabet();
    testGetInputFailedTooManyNumerics();
    testGetInputFailedFloat();
    testGetInputFailedOutOfRange();
    testGetInputFailedEmpty();
    testGetInputFailedEOF();
    printf("Finished runPlaysTests.\n");
}

