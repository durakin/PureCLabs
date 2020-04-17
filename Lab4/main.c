#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

enum OperationsCodes
{
    // Перечисление кодов операций для организации главного меню.
    CALCULATE = 1,
    QUIT = 2
};

enum Sizes
{
    // Перечисление наибольших размеров массивов.
    INPUT_SIZE = 100
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

bool MainMenuInputChecker(int operationCode)
{
    // Функция для вызова в функциях ввода с проверкой.
    // ... Возвращает true, если введенное значение может быть
    // значением кода операции в меню.
    //
    // int operationCode - число, которое нужно проверить.

    return operationCode >= CALCULATE && operationCode <= QUIT;
}

bool NSWPrimeArgChecker(int numberToCheck)
{
    // Функция для вызова в функциях ввода с проверкой.
    // ... Возвращает true, если введенное значение может быть
    // значением N для задачи.
    //
    // int numberToCheck - число, которое нужно проверить.
    return numberToCheck > 2;
}

int RecNSWPrime(int n)
{
    // Функция вычисляет n-ое NSW-число через рекуррентное соотношение
    //
    // int n - порядок нужного члена последовательности NSW-чисел.
    if (n < 2)
    {
        return 1;
    }
    return 2 * RecNSWPrime(n - 1) + RecNSWPrime(n - 2);
}

float NSWPrime(int n)
{
    // Функция вычисляет n-ое NSW-число по явной формуле
    //
    // int n - порядок нужного члена последовательности NSW-чисел.
    return (powf(1 + sqrt(2), n) + powf(1 - sqrt(2), n)) / 2;
}

int main()
{
    int operationCode;
    while (true)
    {
        printf("\n1. Calculate with new N.\n"
               "2. Quit.\n");
        operationCode = CycleInputInt(
                "Choose the command and enter its number",
                MainMenuInputChecker);

        // Получение числа N и проведение всех вычислений
        if (operationCode == CALCULATE)
        {
            int arg;
            float res;
            int recRes;
            arg = CycleInputInt("Enter N", NSWPrimeArgChecker);
            res = NSWPrime(arg);
            recRes = RecNSWPrime(arg);
            printf("NSW prime by explicit formula = %f\n", res);
            printf("NSW prime by recurrence relation = %d\n", recRes);
            printf("Calculation error = %f\n", fabsf(res - recRes));
        }

        // Выход из меню
        if (operationCode == QUIT)
        {
            break;
        }
    }
    return 0;
}