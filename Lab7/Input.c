#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include "Input.h"
#include "Constants.h"

enum SymbolCodes
{
    START_CHAR_RANGE = 32,
    END_CHAR_RANGE = 126,
    BACKSPACE_KEY = 8
};


int CycleInputInt(char* stringToOutput, bool(* pChecker)(int))
{
    int number;
    int position;
    char input[INPUT_SIZE];

    while (true)
    {
        printf("%s\n", stringToOutput);
        fflush(stdout);
        char* fgetsRet = fgets(input, INPUT_SIZE, stdin);
        if (fgetsRet == NULL)
        {
            printf("Wrong format!\n");
            continue;
        }
        int inputLength = strlen(input) - 1;
        input[inputLength] = '\0';
        int sscanfRet = sscanf(input, "%d%n", &number, &position);
        if (position != inputLength)
        {
            printf("Wrong format!\n");
            continue;
        }
        if (pChecker && !pChecker(number))
        {
            printf("Wrong format!\n");
            continue;
        }
        if (sscanfRet == 1) break;
        printf("Wrong format!\n");
    }
    return number;
}


char* StrDynInput()
{
    char* userStr = (char*) malloc(1 * sizeof(char));
    userStr[0] = '\0';
    char curChar = 0;
    int curSize = 1;
    while (curChar != '\n')
    {
        curChar = getchar();
        int deltaVal = 0;
        int lengthDif = 0;
        if (curChar == BACKSPACE_KEY)
        {
            deltaVal = -1;
            lengthDif = 1;
        }
        else
        {
            if (curChar >= START_CHAR_RANGE && curChar <=
                                               END_CHAR_RANGE)
            {
                deltaVal = 1;
                lengthDif = 2;
            }
            else
            {
                continue;
            }
        }
        int newSize = curSize + deltaVal;
        if (newSize == 0)
        {
            continue;
        }
        char* tmpStr = (char*) malloc(newSize * sizeof(char));
        if (tmpStr)
        {
            for (int i = 0; i < newSize - lengthDif; ++i)
            {
                tmpStr[i] = userStr[i];
            }

            if (curChar != BACKSPACE_KEY)
            {
                tmpStr[newSize - 2] = curChar;
            }
            tmpStr[newSize - 1] = '\0';
            free(userStr);
            userStr = tmpStr;
            curSize = newSize;
        }
        else
        {
            printf("Couldn't allocate memory!");
            break;
        }
    }

    return userStr;
}


char* CycleInputString(char* stringToOutput)
{
    printf("%s\n", stringToOutput);
    return StrDynInput();

}

