#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>


int CycleInputInt(char* stringToOutput, bool(* pChecker)(int));
char* StrDynInput();
char* CycleInputString(char* stringToOutput);


#endif //INPUT_H
