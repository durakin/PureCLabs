#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

#include "Constants.h"
#include "Input.h"
#include "ListLink.h"
#include "Data.h"

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
                   "2. Delete product."
                   "3.Back\n\n");
            subOperationCode = CycleInputInt(
                    "Choose the command and enter its number",
                    DeleteMenuInputChecker);
            if (subOperationCode == DELETE_ORDER)
            {
                char* name;
                name = CycleInputString("Enter owner's name");
                orders = DeleteOrder(FindOrder(orders, name), &relations);
                free(name);
            }
            if (subOperationCode == DELETE_PRODUCT)
            {
                char* name;
                name = CycleInputString("Enter product's name");
                products = DeleteProduct(FindProduct(products, name), &relations);
                free(name);
            }
        }

        if (operationCode == QUIT)
        {
            break;
        }
    }

    return 0;
}
