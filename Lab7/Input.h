#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>

enum Sizes
{
    // Перечисление наибольших размеров массивов.
    INPUT_SIZE = 100,
};


int CycleInputInt(char* stringToOutput, bool(* pChecker)(int));
char* StrDynInput();
char* CycleInputString(char* stringToOutput);


#endif //INPUT_H
