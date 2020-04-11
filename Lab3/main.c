#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

enum SymbolCodes
{
    // Перечесление кодов символов для функции ввода строки
    BACKSPACE_KEY = 8,
    START_CHAR_RANGE = 32,
    END_CHAR_RANGE = 126
};

enum OperationsCodes
{
    // Перечисление кодов операций для организации главного меню.
    KEYBOARD_INPUT = 1,
    SHOW = 2,
    TASK = 3,
    CONSOLE_OUTPUT = 4,
    QUIT = 5
};

enum Sizes
{
    // Перечисление наибольших размеров массивов.
    INPUT_SIZE = 100,
    TOKEN_NAME_SIZE = 20
};

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

char* CycleInputString(char* stringToOutput, bool(* pChecker)(char*))
{
    // Функция для ввода строки с проверкой ввода.
    //
    // char* stringToOutput - строка, которую нужно выводить
    // ... в запросе ввода;
    // bool(* pChecker)(char*) - указатель на функцию, проверяющую
    // ... дополнительные условия.
    printf("%s\n", stringToOutput);
    char* stringToReturn;
    while (true)
    {
        stringToReturn = StrDynInput();
        if (pChecker(stringToReturn))
        {
            return stringToReturn;
        }
        printf("Wrong format!\n");
        free(stringToReturn);
    }
}

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

bool OperationInputChecker(int operationCode)
{
    // Функция для вызова в функциях ввода с проверкой.
    // ... Возвращает true, если введенное значение может быть
    // значением кода операции в меню.
    //
    // int operationCode - число, которое нужно проверить.

    return operationCode >= KEYBOARD_INPUT && operationCode <= QUIT;
}

bool MathCharsInputChecker(char* stringToCheck)
{
    // Функция для вызова в функциях ввода с проверкой.
    // ... Возвращает true, если введенное значение может быть
    // строкой только с числами или с определенными символами.
    //
    // char* stringToCheck - число, которое нужно проверить.
    for (int i = 0; i < strlen(stringToCheck); i++)
    {
        char k = stringToCheck[i];
        if (!(k >= '(' && k <= '9' && k != ','))
        {
            return false;
        }
    }
    return true;
}

typedef struct
{
    // Структура для хранения токенов.
    char tokenName[TOKEN_NAME_SIZE];    // Название токена
    char content;   // Значение токена
} SomeToken;

typedef struct
{
    // Структура для хранения строки и её длины
    char* content;    // Строка
    bool isFilled;    // true, если выделена память
    int length;    // Длина строки
} DynStr;

typedef struct
{
    // Структура для хранения динамического массива токенов
    SomeToken* content;    // Массив
    bool isFilled;    // true, если выделана память
} DynArrTokens;

void FreeDynArrStr(DynStr* arrToClean)
{
    // Функция для очистки памяти, выделенной под arrToClean
    if (arrToClean->isFilled)
    {
        free(arrToClean->content);
        arrToClean->isFilled = false;
    }
    arrToClean->length = 0;
}

void FreeDynArrTokens(DynArrTokens* arrToClean)
{
    // Функция для очистки памяти, выделенной под arrToClean
    if (arrToClean->isFilled)
    {
        free(arrToClean->content);
        arrToClean->isFilled = false;
    }
}


int main()
{
    DynStr objStr;
    objStr.isFilled = false;
    objStr.length = 0;

    DynArrTokens tokens;
    tokens.isFilled = false;

    int operationCode;
    while (true)
    {
        printf("\n1. Enter the string with keyboard.\n"
               "2. Show the object string.\n"
               "3. Perform the task.\n"
               "4. Print result to the console.\n"
               "5. Quit.\n\n");
        operationCode = CycleInputInt(
                "Choose the command and enter its number",
                OperationInputChecker);

        // Ввод строки к анализу с клавиатуры
        if (operationCode == KEYBOARD_INPUT)
        {
            FreeDynArrStr(&objStr);
            FreeDynArrTokens(&tokens);
            objStr.content = CycleInputString("Enter string to analyze",
                                              &MathCharsInputChecker);
            objStr.isFilled = true;
            objStr.length = strlen(objStr.content);
        }

        // Вывод строки к анализу в консоль
        if (operationCode == SHOW)
        {
            if (objStr.isFilled)
            {
                printf("%s", objStr.content);
            }
            else
            {
                printf("No object string!\n");
            }
        }

        // Выполнение анализа
        if (operationCode == TASK)
        {
            FreeDynArrTokens(&tokens);
            if (!objStr.isFilled)
            {
                printf("No object string!\n");
            }
            else
            {
                // Выделение памяти под массив токенов
                tokens.content = (SomeToken*) malloc(
                        objStr.length * sizeof(SomeToken*));
                tokens.isFilled = true;
                for (int i = 0; i < objStr.length; i++)
                {
                    // Непосредственно анализ
                    if (objStr.content[i] >= '0' && objStr.content[i] <= '9')
                    {
                        tokens.content[i].content = objStr.content[i];
                        strcpy(tokens.content[i].tokenName, "DIGIT");
                    }
                    if (objStr.content[i] == '/')
                    {
                        tokens.content[i].content = objStr.content[i];
                        strcpy(tokens.content[i].tokenName, "DIVISION");
                    }
                    if (objStr.content[i] == '.')
                    {
                        tokens.content[i].content = objStr.content[i];
                        strcpy(tokens.content[i].tokenName, "DOT");
                    }
                    if (objStr.content[i] == '+')
                    {
                        tokens.content[i].content = objStr.content[i];
                        strcpy(tokens.content[i].tokenName, "PLUS");
                    }
                    if (objStr.content[i] == '-')
                    {
                        tokens.content[i].content = objStr.content[i];
                        strcpy(tokens.content[i].tokenName, "MINUS");
                    }
                    if (objStr.content[i] == '*')
                    {
                        tokens.content[i].content = objStr.content[i];
                        strcpy(tokens.content[i].tokenName, "MULTIPL");
                    }
                    if (objStr.content[i] == ')')
                    {
                        tokens.content[i].content = objStr.content[i];
                        strcpy(tokens.content[i].tokenName, "CLOSE_PARANTH");
                    }
                    if (objStr.content[i] == '(')
                    {
                        tokens.content[i].content = objStr.content[i];
                        strcpy(tokens.content[i].tokenName, "OPEN_PARANTH");
                    }
                }
            }


        }

        // Вывод результатов анализа в консоль
        if (operationCode == CONSOLE_OUTPUT)
        {
            if (!tokens.isFilled)
            {
                printf("No results for current string!\n");
            }
            else
            {
                for (int i = 0; i < objStr.length; i++)
                {
                    printf("%s, %c\n", tokens.content[i].tokenName,
                           tokens.content[i].content);
                }
            }
        }

        // Выход из меню
        if (operationCode == QUIT)
        {
            break;
        }
    }
    // Очистка памяти и завершение программы
    FreeDynArrStr(&objStr);
    FreeDynArrTokens(&tokens);
    return 0;
}