#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

#include "Input.h"
#include "Constants.h"

enum SymbolCodes
{
    // Перечесление кодов символов для функции ввода строки
    START_CHAR_RANGE = 32,
    END_CHAR_RANGE = 126,
    BACKSPACE_KEY = 8
};


int CycleInputInt(char* stringToOutput, bool(* pChecker)(int))
{
    // Функция для ввода целого числа с проверкой ввода.
    //
    // char* stringToOutput - строка, которую нужно выводить
    // ... в запросе ввода;
    // bool(* pChecker)(int) - указатель на функцию, проверяющую
    // ... дополнительные условия.

    int number;   // Необходимое число
    int position;   // Позиция числа в введенной строке
    char input[INPUT_SIZE];   // Строка для ввода

    // Считывает и проверяет ввод по нескольким условиям, до тех пор,
    // пока не будет введено корректно.
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
    // Функция для ввода строки без указания длины
    // источник - Лекция 3. Строки. Массивы строк. Операции над строками.pdf
    char* userStr = (char*) malloc(1 * sizeof(char));
    userStr[0] = '\0';
    char curChar = 0;
    int curSize = 1;
    while (curChar != '\n')
    {
        curChar = getchar();
        int deltaVal = 0; // Определяет, на сколько изменится длина массива
        int lengthDif = 0;
        // Если мы ситраем символы, а не пишем их,
        if (curChar == BACKSPACE_KEY)
        {
            deltaVal = -1; // то будем уменьшать длину массива
            lengthDif = 1; // и копировать строку до предпоследнего символа
        }
            // Иначе проверяем, входит ли введённый символ в диапазон печатных
        else
        {
            if (curChar >= START_CHAR_RANGE && curChar <=
                                               END_CHAR_RANGE)
            {
                deltaVal = 1; // Если да, то будем увеличиватьдлину на 1
                lengthDif = 2; // Не заполняем последние 2 символа -
                // оставлем мето для введённого символа и 0
            }
            else
            {
                continue;
            } // Если это не печатный символ, то пропускаем его
        }
        // Если стирать больше нечего, но пользователь всё равно жмёт Backspace,
        int newSize = curSize + deltaVal;
        if (newSize == 0)
        {
            continue;
        } // то мы переходим на следующую итерацию - ждём '\n'
        char* tmpStr = (char*) malloc(newSize * sizeof(char));
        if (tmpStr) // Проверяем, выделилась ли память
        {
            // Идём до предпоследнего символа, т.к. надо в конец записать 0
            for (int i = 0; i < newSize - lengthDif; ++i)
            {
                tmpStr[i] = userStr[i];
            }

            if (curChar != BACKSPACE_KEY)
            {// Если введён печатный символ,
                tmpStr[newSize - 2] = curChar; // Добавляем его в строку
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
    // Функция для ввода строки с проверкой ввода.
    //
    // char* stringToOutput - строка, которую нужно выводить
    // ... в запросе ввода;
    // bool(* pChecker)(char*) - указатель на функцию, проверяющую
    // ... дополнительные условия.
    printf("%s\n", stringToOutput);
    return StrDynInput();

}

