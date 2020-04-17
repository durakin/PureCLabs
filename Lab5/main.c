#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>


enum Sizes
{
    // Перечисление наибольших размеров массивов.
    INPUT_SIZE = 100,
};


enum SymbolCodes
{
    // Перечесление кодов символов для функции ввода строки
    START_CHAR_RANGE = 32,
    END_CHAR_RANGE = 126
};


enum OperationsCodes
{
    // Перечисление кодов операций для организации главного меню.
    LOAD_FROM_FILE = 1,
    SAVE_TO_FILE = 2,
    ADD_ROOM = 3,
    DELETE_ROOM = 4,
    PRINT = 5,
    PRINT_SORTED = 6,
    PRINT_SORTED_BY_PRICE = 1,
    PRINT_SORTED_BY_CAPACITY = 2,
    PRINT_SORTED_BY_TYPE = 3,
    FILTER_SETTINGS = 7,
    SET_FILTERS = 1,
    RESET_FILTERS = 2,
    QUIT = 8
};


enum RoomTypes
{
    // Перечисление типов комнат по комфортабельности
    JUNISUITE = 1,
    STUDIO = 2,
    LUX = 3,
    APARTMENT = 4,
    SUITE = 5
};


enum PriceFactors
{
    // Перечисление множителей для вычисления стоимости комнат
    // (эти числа и формулы выбраны произвольно)
    BASE_PRICE = 625,
    BASE_CAPACITY_PRICE = 500
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

            tmpStr[newSize - 2] = curChar; // Добавляем символ в строку

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


bool OperationInputChecker(int operationCode)
{
    // Функция для вызова в функциях ввода с проверкой.
    // ... Возвращает true, если введенное значение может быть
    // значением кода операции в меню.
    //
    // int operationCode - число, которое нужно проверить.

    return operationCode >= LOAD_FROM_FILE && operationCode <= QUIT;
}


bool PositiveIntInputChecker(int intToCheck)
{
    return intToCheck > 0;
}


bool RoomTypeInputChecker(int roomType)
{
    return roomType >= JUNISUITE && roomType <= SUITE;
}


typedef struct
{
    int number;
    int capacity;
    int roomType;
    int price;
} Room;


typedef struct
{
    int basePrice;
    int baseCapacityPrice;
    int size;
    Room* rooms;
    bool* isFilled;
} Hotel;


void CalculateRoomPrice(Room* objectRoom, Hotel* objectHotel)
{
    objectRoom->price = objectHotel->basePrice * objectRoom->roomType +
                        objectHotel->baseCapacityPrice * objectRoom->capacity;
}


void FreeHotel(Hotel* hotelToFree)
{
    if (hotelToFree->isFilled)
    {
        free(hotelToFree->rooms);
    }
}


void LoadHotel(FILE* saveFile, Hotel* newHotel)
{
    FreeHotel(newHotel);
    //Hotel newHotel;
    fscanf(saveFile, "%d", &newHotel->basePrice);
    fscanf(saveFile, "%d", &newHotel->baseCapacityPrice);
    fscanf(saveFile, "%d", &newHotel->size);
    newHotel->rooms = (Room*) malloc(newHotel->size * sizeof(Room));
    for (int i = 0; i < newHotel->size; i++)
    {
        Room newRoom;
        fscanf(saveFile, "%d", &newRoom.number);
        fscanf(saveFile, "%d", &newRoom.capacity);
        fscanf(saveFile, "%d", &newRoom.roomType);
        CalculateRoomPrice(&newRoom, newHotel);
        newHotel->rooms[i] = newRoom;
    }
    newHotel->isFilled = true;
}


void SaveHotel(FILE* saveFile, Hotel* hotelToSave)
{
    fprintf(saveFile, "%d\n", hotelToSave->basePrice);
    fprintf(saveFile, "%d\n", hotelToSave->baseCapacityPrice);
    fprintf(saveFile, "%d\n", hotelToSave->size);
    for (int i = 0; i < hotelToSave->size; i++)
    {
        fprintf(saveFile, "%d\n", hotelToSave->rooms[i].number);
        fprintf(saveFile, "%d\n", hotelToSave->rooms[i].capacity);
        fprintf(saveFile, "%d\n", hotelToSave->rooms[i].roomType);
    }
}


void AddRoom(Hotel* objectHotel, int number, int capacity, int roomType)
{
    if (!objectHotel->isFilled)
    {
        objectHotel->size = 1;
        objectHotel->rooms = (Room*) malloc(sizeof(Room));
        objectHotel->isFilled = true;
    }
    else
    {
        objectHotel->size++;
        objectHotel->rooms = (Room*) realloc(objectHotel->rooms,
                                             objectHotel->size *
                                             sizeof(Room));

    }
    objectHotel->rooms[objectHotel->size - 1].number = number;
    objectHotel->rooms[objectHotel->size - 1].capacity = capacity;
    objectHotel->rooms[objectHotel->size - 1].roomType = roomType;
    CalculateRoomPrice(&objectHotel->rooms[objectHotel->size - 1],
                       objectHotel);
}


void PrintHotel(Hotel* hotelToPrint)
{
    printf("Rooms amount: %d\n\n", hotelToPrint->size);
    for (int i = 0; i < hotelToPrint->size; i++)
    {
        int roomType;
        roomType = hotelToPrint->rooms[i].roomType;
        char roomTypeName[20];
        if (roomType == SUITE)
        {
            strcpy(roomTypeName, "Suite");
        }
        if (roomType == APARTMENT)
        {
            strcpy(roomTypeName, "Apartment");
        }
        if (roomType == LUX)
        {
            strcpy(roomTypeName, "Lux");
        }
        if (roomType == STUDIO)
        {
            strcpy(roomTypeName, "Studio");
        }
        if (roomType == JUNISUITE)
        {
            strcpy(roomTypeName, "Junior studio");
        }
        printf("Room #%d:\nCapacity: %d\nType: %s\nPrice: %d\n\n",
               hotelToPrint->rooms[i].number, hotelToPrint->rooms[i].capacity,
               roomTypeName, hotelToPrint->rooms[i].price);

    }
}


int main()
{
    Hotel object;
    object.basePrice = BASE_PRICE;
    object.baseCapacityPrice = BASE_CAPACITY_PRICE;
    object.isFilled = false;

    bool isFiltered;
    isFiltered = false;

    int operationCode;
    while (true)
    {
        if (!object.isFilled)
        {
            printf("\nNO DATABASE\nLoad or start creating new rooms.");
        }
        printf("\n1. Load data from savefile.\n"
               "2. Save current data to savefile.\n"
               "3. Add new room.\n"
               "4. Delete existing room.\n"
               "5. Print all data.\n"
               "6. Print data sorted.\n"
               "7. Change filter settings.\n"
               "8. Quit program without saving.\n\n");
        operationCode = CycleInputInt(
                "Choose the command and enter its number",
                OperationInputChecker);

        if (operationCode == LOAD_FROM_FILE)
        {
            FILE* savefile;
            savefile = fopen("savefile.txt", "r");
            if (savefile == NULL)
            {
                printf("Couldn't open savefile!\n");
            }
            else
            {
                LoadHotel(savefile, &object);
                printf("Loaded!\n");
            }
            fclose(savefile);
        }

        if (operationCode == SAVE_TO_FILE)
        {
            FILE* savefile;
            savefile = fopen("savefile.txt", "w");
            if (savefile == NULL)
            {
                printf("Couldn't write new data!\n");
            }
            else
            {
                SaveHotel(savefile, &object);
            }
            fclose(savefile);
        }

        if (operationCode == ADD_ROOM)
        {
            int number = CycleInputInt("Enter room's number",
                                       PositiveIntInputChecker);
            int capacity = CycleInputInt("Enter room's capacity",
                                         PositiveIntInputChecker);
            int roomType = CycleInputInt("Choose room's type:\n"
                                         "1. Junior suite.\n"
                                         "2. Studio.\n"
                                         "3. Lux.\n"
                                         "4. Apartment.\n"
                                         "5. Suite.\n",
                                         RoomTypeInputChecker);
            AddRoom(&object, number, capacity, roomType);
            printf("Room added! Price is %d\n", object.rooms->price);
        }

        if (operationCode == PRINT)
        {
            PrintHotel(&object);
        }


        if (operationCode == QUIT)
        {
            break;
        }
    }
    return 0;
}


/*
        // Перечисление кодов операций для организации главного меню.
        LOAD_FROM_FILE = 1,
        SAVE_TO_FILE = 2,
        ADD_ROOM = 3,
        DELETE_ROOM = 4,
        PRINT = 5,
        PRINT_SORTED = 6,
        PRINT_SORTED_BY_PRICE = 1,
        PRINT_SORTED_BY_CAPACITY = 2,
        PRINT_SORTED_BY_TYPE = 3,
        FILTER_SETTINGS = 7,
        SET_FILTERS = 1,
        RESET_FILTERS = 2,
        QUIT = 8
    };
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
                        objStr.length * sizeof(SomeToken * ));
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
 */