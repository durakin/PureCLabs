#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>


enum Sizes
{
    // Перечисление наибольших размеров массивов.
    INPUT_SIZE = 100,
};


enum SymbolCodes
{
    // Перечесление кодов символов для функции ввода строки
    START_CHAR_RANGE = 32,
    END_CHAR_RANGE = 126,
    BACKSPACE_KEY = 8
};


enum OperationsCodes
{
    // Перечисление кодов операций для организации всех меню.
    // Команды для главного меню.
    LOAD_FROM_FILE = 1,
    SAVE_TO_FILE = 2,
    ADD = 3,
        ADD_ORDER = 1,
        ADD_PRODUCT = 2,
        ADD_RELATION = 3,
        ADD_BACK = 4,
    PRINT = 4,
        PRINT_ORDERS = 1,
        PRINT_PRODUCTS = 2,
        PRINT_BY_ORDER = 3,
        PRINT_BY_PRODUCT = 4,
        PRINT_BACK = 5,
    DELETE = 5,
        DELETE_ORDER = 1,
        DELETE_PRODUCT = 2,
        DELETE_BACK = 3,
    SORT_ORDERS = 6,
        SORT_ORDERS_ORD = 1,
        SORT_ORDERS_SHIP = 2,
        SORT_ORDERS_NAME = 3,
        SORT_ORDERS_BACK = 4,
    SORT_PRODUCTS = 7,
        SORT_PRODUCTS_PRICE = 1,
        SORT_PRODUCTS_WEIGHT = 2,
        SORT_PRODUCTS_NAME = 3,
        SORT_PRODUCTS_BACK = 4,
    QUIT = 8,
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


bool MainMenuInputChecker(int operationCode)
{
    // Функция для вызова в функциях ввода с проверкой.
    // Возвращает true, если введенное значение может быть
    // значением кода операции в главном меню.
    //
    // int operationCode - число, которое нужно проверить.

    return operationCode >= LOAD_FROM_FILE && operationCode <= QUIT;
}


bool PositiveIntInputChecker(int intToCheck)
{
    // Функция для вызова в функциях ввода с проверкой.
    // Возвращает true, если введенное значение является положительным
    // числом. В настоящей программе такие значения должны принимать
    // значения вместимости и цены номера.
    //
    // int intToCheck - число, которое нужно проверить.
    return intToCheck > 0;
}


bool AddMenuInputChecker(int operationCode)
{
    // Функция для вызова в функциях ввода с проверкой.
    // Возвращает true, если введенное значение может быть
    // значением кода операции в меню сортировки.
    //
    // int operationCode - число, которое нужно проверить.
    return operationCode >= ADD_ORDER &&
           operationCode <= ADD_BACK;
}


bool PrintMenuInputChecker(int operationCode)
{
    // Функция для вызова в функциях ввода с проверкой.
    // Возвращает true, если введенное значение может быть
    // значением кода операции в меню настройки фильтров.
    //
    // int operationCode - число, которое нужно проверить.
    return operationCode >= PRINT_ORDERS && operationCode <= PRINT_BACK;
}


bool DeleteMenuInputChecker(int operationCode)
{
    return operationCode >= DELETE_ORDER && operationCode <= DELETE_BACK;
}


bool SortOrdersMenuInputChecker(int operationCode)
{
    return operationCode >= SORT_ORDERS_ORD &&
           operationCode <= SORT_ORDERS_BACK;
}


bool SortProductsMenuInputChecker(int operationCode)
{
    return operationCode >= SORT_PRODUCTS_PRICE &&
           operationCode <= SORT_PRODUCTS_BACK;
}


typedef struct ListLink
{
    // Структура для хранения двусвязного списка комнат.
    struct ListLink* previous;    // Указатель на предыдущий элемент
    struct ListLink* next;    // Указатель на следующий элемент
    void* content;    // Указатель на информацию о комнате
} ListLink;


ListLink* ListLinkHead(ListLink* objectLink)
{
    // Функция для поиска первого элемента списка. Возвращает указатель
    // на первый элемент.
    //
    // ListLink* objectLink - элемент списка, первый элемент которого
    // ... необходимо найти
    ListLink* position;
    position = objectLink;
    if (position == NULL)
    {
        return NULL;
    }
    while (position->previous != NULL)
    {
        position = position->previous;
    }
    return position;
}


ListLink* ListLinkTail(ListLink* objectLink)
{
    // Функция для поиска последнего элемента списка. Возвращает указатель
    // на последний элемент.
    //
    // ListLink* objectLink - элемент списка, последний элемент которого
    // ... необходимо найти
    ListLink* position;
    position = objectLink;
    if (position == NULL)
    {
        return NULL;
    }
    while (position->next != NULL)
    {
        position = position->next;
    }
    return position;
}


ListLink* ListLinkAdd(ListLink* objectList, void* content)
{
    ListLink* linkToAdd;
    linkToAdd = (ListLink*) malloc(sizeof(ListLink));
    linkToAdd->next = NULL;
    linkToAdd->previous = ListLinkTail(objectList);
    linkToAdd->content = content;

    if (linkToAdd->previous != NULL)
    {
        linkToAdd->previous->next = linkToAdd;
    }
    return linkToAdd;
}


ListLink* ListLinkDelete(ListLink* linkToDelete)
{
    ListLink* linkToReturn;
    if (linkToDelete->next != NULL)
    {
        linkToDelete->next->previous = linkToDelete->previous;
    }
    linkToReturn = ListLinkHead(linkToDelete);
    if (linkToReturn != linkToDelete)
    {
        linkToDelete->previous->next = linkToDelete->next;
    }
    else
    {
        linkToReturn = linkToDelete->next;
    }
    free(linkToDelete);
    return linkToReturn;
}


void ListLinkSwap(ListLink* firstLink, ListLink* secondLink)
{
    // Функция для замены местами двух элементов списка.
    //
    // ListLink* firstLink - элемент списка, который нужно поменять с другим
    // ListLink* secondLink - элемент списка, который нужно поменять с другим
    void* roomDataBox;
    void* contentBox;
    contentBox = firstLink->content;
    firstLink->content = secondLink->content;
    secondLink->content = contentBox;
}


int ListLinkSize(ListLink* objectLink)
{
    // Функция для нахождения количества элементов списка.
    // Возвращает количество элементов списка.
    //
    // ListLink* objectLink - элемент списка, размер которого
    // ... необходимо найти
    ListLink* position;
    if (objectLink == NULL)
    {
        return 0;
    }
    position = ListLinkHead(objectLink);
    int size;
    size = 1;
    while (position->next != NULL)
    {
        size++;
        position = position->next;
    }
    return size;
}


void ListLinkBubbleSort(ListLink* objectLink, int(* comparator)(void*, void*))
{
    // Функция, сортирующая методом пузырьков двусвязный список.
    //
    // ListLink* objectLink - элемент списка, который нужно отсортировать
    // int(* comparator)(Room*, Room*) - функция-компоратор по аналогии
    // ... с функцией qsort()
    ListLink* position;
    position = NULL;
    ListLink* index;
    index = NULL;
    for (position = ListLinkHead(objectLink);
         position->next != NULL; position = position->next)
    {
        for (index = position->next;
             index != NULL; index = index->next)
        {
            if (comparator(index->content, position->content))
            {
                ListLinkSwap(position, index);
            }
        }
    }
}


typedef struct
{
    char owner[INPUT_SIZE];
    int ordDay;
    int ordMonth;
    int ordYear;
    int shipDay;
    int shipMonth;
    int shipYear;
} Order;


typedef struct
{
    char name[INPUT_SIZE];
    int price;
    int weight;
} Product;


typedef struct
{
    Product* product;
    Order* order;
} Relation;


void PrintOrder(Order* objectOrder)
{
    // Функция для форматированного вывода на экран информации о конкретной
    // комнате.
    //
    // Room* objectRoom - указатель на комнату, информацию о которой
    // ... нужно вывести;

    printf("ORDER by: %s\nDate of order: %d.%d.%d\nDate of shipment:"
           "%d.%d.%d\n", objectOrder->owner, objectOrder->ordMonth,
           objectOrder->ordDay, objectOrder->ordYear, objectOrder->shipMonth,
           objectOrder->shipDay, objectOrder->shipYear);


    /*

     */
}


void PrintProduct(Product* objectProduct)
{
    printf("PRODUCT: %s\nPrice: %d\nWeight: %d\n", objectProduct->name,
           objectProduct->price, objectProduct->weight);
}


void PrintAllOrders(ListLink* objectList)
{
    ListLink* i = ListLinkHead(objectList);
    while (i != NULL)
    {
        PrintOrder(i->content);
        printf("\n");
        i = i->next;
    }
}


void PrintAllProducts(ListLink* objectList)
{
    ListLink* i;
    i = ListLinkHead(objectList);
    while (i != NULL)
    {
        PrintProduct(i->content);
        printf("\n");
        i = i->next;
    }
}


void PrintProductsByOrder(Order* objectOrder, ListLink* relations)
{
    PrintOrder(objectOrder);
    printf("----Contents:----\n");
    ListLink* i = ListLinkHead(relations);
    while (i != NULL)
    {
        Relation* relation;
        relation = i->content;
        if (relation->order == objectOrder)
        {
            PrintProduct(relation->product);
        }
        i = i->next;
    }
}


void PrintOrdersByProduct(Product* objectProduct, ListLink* relations)
{
    PrintProduct(objectProduct);
    printf("----Contents:----\n");
    ListLink* i = ListLinkHead(relations);
    while (i != NULL)
    {
        Relation* relation;
        relation = i->content;
        if (relation->product == objectProduct)
        {
            PrintOrder(relation->order);
        }
        i = i->next;
    }
}


ListLink* FindOrder(ListLink* objectList, char* owner)
{
    ListLink* i;
    i = ListLinkHead(objectList);
    while (i != NULL)
    {
        Order* order;
        order = i->content;
        if (strcmp(order->owner, owner) == 0)
        {
            return i;
        }
        i = i->next;
    }
    return NULL;
}


ListLink* FindProduct(ListLink* objectList, char* name)
{
    ListLink* i;
    i = ListLinkHead(objectList);
    while (i != NULL)
    {
        Product* product;
        product = i->content;
        if (strcmp(product->name, name) == 0)
        {
            return i;
        }
        i = i->next;
    }
    return NULL;
}


ListLink* FindRelation(ListLink* objectList, Order* order, Product* product)
{
    ListLink* i;
    i = ListLinkHead(objectList);
    while (i != NULL)
    {
        Relation* relation;
        relation = i->content;
        if (relation->order == order && relation->product == product)
        {
            return i;
        }
        i = i->next;
    }
    return NULL;
}


ListLink* AddOrder(ListLink* objectList, char* owner, int ordDay,
                   int ordMonth, int ordYear, int shipDay, int shipMonth,
                   int shipYear)
{
    if (FindOrder(objectList, owner) != NULL)
    {
        return NULL;
    }

    Order* order;
    order = (Order*) malloc(sizeof(Order));
    strcpy(order->owner, owner);
    order->ordDay = ordDay;
    order->ordMonth = ordMonth;
    order->ordYear = ordYear;
    order->shipDay = shipDay;
    order->shipMonth = shipMonth;
    order->shipYear = shipYear;

    return ListLinkAdd(objectList, order);
}


ListLink* AddProduct(ListLink* objectList, char* name, int price, int weight)
{
    if (FindProduct(objectList, name) != NULL)
    {
        return NULL;
    }

    Product* product;
    product = (Product*) malloc(sizeof(Product));
    strcpy(product->name, name);
    product->weight = weight;
    product->price = price;

    return ListLinkAdd(objectList, product);
}


ListLink* AddRelation(ListLink* objectList, ListLink* order,
                      ListLink* product)
{

    if (FindRelation(objectList, order->content, product->content) != NULL)
    {
        return NULL;
    }

    Relation* relation;
    relation = (Relation*) malloc(sizeof(Relation));
    relation->product = product->content;
    relation->order = order->content;

    return ListLinkAdd(objectList, relation);
}


ListLink* DeleteRelation(ListLink* relationToDelete)
{
    if(relationToDelete == NULL)
    {
        return NULL;
    }
    free(relationToDelete->content);
    return ListLinkDelete(relationToDelete);
}


ListLink* DeleteOrder(ListLink* orderLinkToDelete, ListLink* relations)
{
    if (orderLinkToDelete == NULL)
    {
        return NULL;
    }
    ListLink* i = ListLinkHead(relations);
    while (i != NULL)
    {
        ListLink* relationLinkToDelete;
        Relation* relation;
        relation = i->content;
        relationLinkToDelete = i;
        i = i->next;
        if (relation->order == orderLinkToDelete->content)
        {
            DeleteRelation(relationLinkToDelete);
        }
    }
    return ListLinkDelete(orderLinkToDelete);
}


ListLink* DeleteProduct(ListLink* productLinkToDelete, ListLink* relations)
{
    if (productLinkToDelete == NULL)
    {
        return NULL;
    }
    ListLink* i = ListLinkHead(relations);
    while (i != NULL)
    {
        ListLink* relationLinkToDelete;
        Relation* relation;
        relation = i->content;
        relationLinkToDelete = i;
        i = i->next;
        if (relation->product == productLinkToDelete->content)
        {
            DeleteRelation(relationLinkToDelete);
        }
    }
    return ListLinkDelete(productLinkToDelete);
}





/*

void LoadHotel(FILE* saveFile, Hotel* objectHotel)
{
    // Функция для загрузки набора номеров из документа.
    // Работа функции в случае "повреждения" данных из-за
    // редактирования нештатным образом не определена.
    // Гарантируется, что файл, созданный функцией SaveHotel()
    // будет обработан корректно.
    //
    // FILE* saveFile - документ с данными;
    // Hotel* objectHotel - указатель на набор комнат, который будет заполнен
    // ... данными из документа;
    int inputRoomNumber;
    fscanf(saveFile, "%d", &objectHotel->basePrice);
    fscanf(saveFile, "%d", &objectHotel->baseCapacityPrice);
    fscanf(saveFile, "%d", &inputRoomNumber);
    for (int i = 0; i < inputRoomNumber; i++)
    {
        int number;
        int capacity;
        int roomType;
        fscanf(saveFile, "%d", &number);
        fscanf(saveFile, "%d", &capacity);
        fscanf(saveFile, "%d", &roomType);
        AddRoom(objectHotel, number, capacity, roomType);
    }
}


void SaveHotel(FILE* saveFile, Hotel* objectHotel)
{
    // Функция для создания файла сохранения набора номеров.
    //
    // FILE* saveFile - документ с данными (был создан или перезаписан);
    // Hotel* objectHotel - указатель на набор комнат, который будет сохранен
    // ... в документе;
    fprintf(saveFile, "%d\n", objectHotel->basePrice);
    fprintf(saveFile, "%d\n", objectHotel->baseCapacityPrice);
    fprintf(saveFile, "%d\n", objectHotel->size);
    ListLink* position;
    position = objectHotel->rooms;
    while (position != NULL)
    {
        fprintf(saveFile, "%d\n", position->roomData->number);
        fprintf(saveFile, "%d\n", position->roomData->capacity);
        fprintf(saveFile, "%d\n", position->roomData->roomType);
        position = position->next;
    }
}


void ListLinkFree(ListLink* objectLink)
{
    // Функция для очистки памяти, занятой под информацию о комнатах.
    //
    // ListLink* objectLink - элемент списка, памяьб для хранения
    // ... которого необходимо очистить;
    if (objectLink != NULL)
    {
        free(objectLink->roomData);
        if (objectLink->next != NULL)
        {
            ListLinkFree(objectLink->next);
        }
        if (objectLink->previous != NULL)
        {
            ListLinkFree(objectLink->previous);
        }
        free(objectLink);
    }
}


int PriceComparator(Room* room1, Room* room2)
{
    // Компаратор для сортировки комнат по
    // цене.
    return room1->price > room2->price;
}


int CapacityComparator(Room* room1, Room* room2)
{
    // Компаратор для сортировки комнат по
    // вместимости.
    return room1->capacity > room2->capacity;
}


int TypeComparator(Room* room1, Room* room2)
{
    // Компаратор для сортировки комнат по
    // комфортабельности.
    return room1->roomType > room2->roomType;
}


int NumberComparator(Room* room1, Room* room2)
{
    // Компаратор для сортировки комнат по
    // номеру.
    return room1->number > room2->number;
}

 */

int main()
{
    ListLink* orders;
    ListLink* products;
    ListLink* relations;
    orders = NULL;
    products = NULL;
    relations = NULL;

    int operationCode;
    while (true)
    {
        int subOperationCode;
        printf("\n1. Load data from savefile.\n"
               "2. Save current data to savefile.\n"
               "3. Add new element.\n"
               "4. Print information.\n"
               "5. Delete elements.\n"
               "6. Sort orders.\n"
               "7. Sort products.\n"
               "8. Quit without saving.\n\n");
        operationCode = CycleInputInt(
                "Choose the command and enter its number",
                MainMenuInputChecker);

        // Загрузка базы из документа "savefile.txt"
        /*
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

        // Сохранение текущей базы данных в документ "savefile.txt"
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
                printf("Saved!\n");
            }
            fclose(savefile);
        }
*/
        // Добавление комнаты
        if (operationCode == ADD)
        {
            subOperationCode = CycleInputInt("\n1. Add order."
                                             "\n2. Add product."
                                             "\n3. Add relation."
                                             "\n4. Back.\n",
                                             AddMenuInputChecker);
            if (subOperationCode == ADD_ORDER)
            {
                ListLink* addReturn;
                char* owner;
                int ordDay;
                int ordMonth;
                int ordYear;
                int shipDay;
                int shipMonth;
                int shipYear;

                owner = CycleInputString("Enter owner's name");
                ordMonth = CycleInputInt(
                        "Enter month (number) of order application",
                        PositiveIntInputChecker);
                ordDay = CycleInputInt("Enter day of order application",
                                       PositiveIntInputChecker);
                ordYear = CycleInputInt("Enter year of order application",
                                        PositiveIntInputChecker);
                shipMonth = CycleInputInt("Enter month (number) of shipment",
                                          PositiveIntInputChecker);
                shipDay = CycleInputInt("Enter day of shipment",
                                        PositiveIntInputChecker);
                shipYear = CycleInputInt("Enter year of shipment",
                                         PositiveIntInputChecker);
                addReturn = AddOrder(orders, owner, ordDay, ordMonth, ordYear,
                                     shipDay, shipMonth, shipYear);
                if (addReturn != NULL)
                {
                    orders = addReturn;
                }
                else
                {
                    printf("Order by such person already exists!");
                }
                free(owner);
            }
            if (subOperationCode == ADD_PRODUCT)
            {
                ListLink* addReturn;
                char* name;
                int price;
                int weight;

                name = CycleInputString("Enter product's name");
                price = CycleInputInt("Enter product's price", NULL);
                weight = CycleInputInt("Enter product's weight",
                                       PositiveIntInputChecker);
                addReturn = AddProduct(products, name, price, weight);
                if (addReturn != NULL)
                {
                    products = addReturn;
                    printf("Added!");
                }
                else
                {
                    printf("Product with such name already exists!");
                }
                free(name);
            }
            if (subOperationCode == ADD_RELATION)
            {
                ListLink* addReturn;
                ListLink* order;
                ListLink* product;
                char* orderName;
                char* productName;
                orderName = CycleInputString("Enter owner's name");
                productName = CycleInputString("Enter product's name");
                order = FindOrder(orders, orderName);
                product = FindProduct(products, productName);

                if (order != NULL && product != NULL)
                {
                    relations = AddRelation(relations, order, product);
                    printf("Added!");
                }
                else
                {
                    printf("Such relation already exists, or product"
                           "or order wasn't found!");
                }
                free(orderName);
                free(productName);

            }
        }

        if (operationCode == PRINT)
        {
            subOperationCode = CycleInputInt("\n1. Print all orders."
                                             "\n2. Print all products."
                                             "\n3. Print order's content."
                                             "\n4. Print product's related orders.\n"
                                             "\n5. Back.\n",
                                             PrintMenuInputChecker);
            if (subOperationCode == PRINT_ORDERS)
            {
                PrintAllOrders(orders);
            }
            if (subOperationCode == PRINT_PRODUCTS)
            {
                PrintAllProducts(products);
            }
            if (subOperationCode == PRINT_BY_ORDER)
            {
                char* name;
                name = CycleInputString("Enter owner's name");
                ListLink* order;
                order = FindOrder(orders, name);
                if (order == NULL)
                {
                    printf("No such order!");
                }
                else
                {
                    PrintProductsByOrder(order->content, relations);
                }
            }
            if (subOperationCode == PRINT_BY_PRODUCT)
            {
                char* name;
                name = CycleInputString("Enter product's name");
                ListLink* product;
                product = FindOrder(products, name);
                if (product == NULL)
                {
                    printf("No such product!");
                }
                else
                {
                    PrintOrdersByProduct(product->content, relations);
                }
            }
        }

        if (operationCode == DELETE)
        {
            printf("\n1. Delete order.\n"
                   "2. Delete product.\n"
                   "3. Add new element.\n"
                   "4. Print information.\n"
                   "5. Delete elements.\n"
                   "6. Sort orders.\n"
                   "7. Sort products.\n"
                   "8. Quit without saving.\n\n");
            subOperationCode = CycleInputInt(
                    "Choose the command and enter its number",
                    DeleteMenuInputChecker);
            if (subOperationCode == DELETE_ORDER)
            {
                char* name;
                name = CycleInputString("Enter owner's name");
                orders = DeleteOrder(FindOrder(orders, name), relations);
                free(name);
            }
            if (subOperationCode == DELETE_PRODUCT)
            {
                char* name;
                name = CycleInputString("Enter product's name");
                products = DeleteProduct(FindProduct(products, name), relations);
                free(name);
            }
        }
        /*
        // Удаление комнаты
        if (operationCode == DELETE_ROOM)
        {

            int room;
            room = CycleInputInt("Enter room's number",
                                 PositiveIntInputChecker);
            DeleteRoom(&object, room);
        }

        // Вывод набора комнат с учётом фильтров
        if (operationCode == PRINT)
        {
            PrintHotel(&object, filter);
        }

        // Вход в меню сортировки
        if (operationCode == SORT)
        {
            subOperationCode = CycleInputInt("\n1. Sort by price."
                                             "\n2. Sort by capacity."
                                             "\n3. Sort by type."
                                             "\n4. Sort by room's number."
                                             "\n5. Back.\n",
                                             SortMenuInputChecker);

            // Сортировка по цене
            if (subOperationCode == SORT_BY_PRICE)
            {
                ListLinkBubbleSort(object.rooms, PriceComparator);
            }

            // Сортировка по вместимости
            if (subOperationCode == SORT_BY_CAPACITY)
            {
                ListLinkBubbleSort(object.rooms, CapacityComparator);
            }

            // Сортирока по комфортабельности
            if (subOperationCode == SORT_BY_TYPE)
            {
                ListLinkBubbleSort(object.rooms, TypeComparator);
            }

            // Сортировка по номеру
            if (subOperationCode == SORT_BY_NUMBER)
            {
                ListLinkBubbleSort(object.rooms, NumberComparator);
            }

            // Выход из меню сортировки без действий
            if (subOperationCode == SORT_CANCEL)
            {
                printf("Sorting cancelled!\n\n");
                continue;
            }

            // Вывод отсортированной базы данных
            printf("Sorted!\n");
            PrintHotel(&object, filter);
        }

        // Вход в меню настройки фильтров
        if (operationCode == FILTERS)
        {
            subOperationCode = CycleInputInt("1. Set new filters."
                                             "\n2. Reset all filters."
                                             "\n3. Back.",
                                             FilterMenuInputChecker);

            // Установка нового набора фильтра
            if (subOperationCode == FILTERS_SET)
            {
                filter.minCapacity = CycleInputInt(
                        "Enter room's minimal capacity"
                        "\nEnter number < 1 to skip",
                        NULL);
                filter.roomType = CycleInputInt("\nChoose room's type:"
                                                "\n1. Junior suite."
                                                "\n2. Studio."
                                                "\n3. Lux."
                                                "\n4. Apartment."
                                                "\n5. Suite."
                                                "\n Different number to"
                                                " skip",
                                                NULL);

                filter.maxPrice = CycleInputInt(
                        "Enter room's maximum price"
                        "\nEnter number < 1 to skip",
                        NULL);
                filter.minPrice = CycleInputInt(
                        "Enter room's minimum price"
                        "\nEnter number < 1 to skip",
                        NULL);

                // Фиксация факта включения фильтров, функция вывода теперь
                // будет проходить этап фильтрации
                filter.isFiltered = true;
            }

            // Отключение фильтрации
            if (subOperationCode == FILTERS_RESET)
            {
                // Для отключения фильтрации достаточно просто задать
                // этой переменной false. Т.к. при этом этап фильтрации будет
                // пропускаться функцией вывода. Если фильтры понадобится
                // включить - значения остальныйх полей Filter filter в любом
                // случае будут заданы заново.
                filter.isFiltered = false;
                printf("\nFilters are reset!\n");
            }
        }

        // Вывод информации о комнате по её номеру
        if (operationCode == FIND_BY_NUMBER)
        {
            int number;
            number = CycleInputInt("Enter room's number",
                                   PositiveIntInputChecker);
            ListLink* result;
            result = FindRoom(&object, number);
            if (result == NULL)
            {
                printf("\nNo such room!\n\n");
            }
            else
            {
                PrintRoom(result->roomData);
            }

        }

        // Инверсия списка
        if (operationCode == REVESE)
        {
            ListLinkReverse(&object);
            printf("Reversed!\n");
            PrintHotel(&object, filter);
        }

        // Выход из программы
        if (operationCode == QUIT)
        {
            break;
        }
    }
    ListLinkFree(object.rooms);
         *///return 0;
    }
    return 0;
}
