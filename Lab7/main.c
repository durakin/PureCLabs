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
        SORT_ORDERS_BACK = 3,
    SORT_PRODUCTS = 7,
        SORT_PRODUCTS_PRICE = 1,
        SORT_PRODUCTS_WEIGHT = 2,
        SORT_PRODUCTS_BACK = 3,
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


bool CompOrdersOrderDate(Order* first, Order* second)
{
    if (first->ordYear == second->ordYear)
    {
        if (first->ordMonth == second->ordMonth)
        {
            return  (first->ordDay > second->ordDay);
        }
        return (first->ordMonth > second->ordMonth);
    }
    return first->ordYear>second->ordYear;
}


bool CompOrdersShipDate(Order* first, Order* second)
{
    if (first->shipYear == second->shipYear)
    {
        if (first->shipMonth == second->shipMonth)
        {
            return  (first->shipDay > second->shipDay);
        }
        return (first->shipMonth> second->shipMonth);
    }
    return first->shipYear>second->shipYear;
}


bool CompProductsPrice(Product* first, Product* second)
{
    return first->price > second->price;
}


bool CompProductsWeight(Product* first, Product* second)
{
    return first->weight > second->weight;
}


void SaveData(FILE* saveFile, ListLink* orders, ListLink* products,
              ListLink* relations)
{
    ListLink* i;

    int ordersSize;
    ordersSize = ListLinkSize(orders);
    fprintf(saveFile,"%d\n", ordersSize);
    i = ListLinkHead(orders);
    while (i!=NULL)
    {
        PrintFileOrder(saveFile, i->content);
        i = i->next;
    }

    int productsSize;
    productsSize = ListLinkSize(products);
    fprintf(saveFile,"%d\n", productsSize);
    i = ListLinkHead(products);
    while (i!=NULL)
    {
        PrintFileProduct(saveFile, i->content);
        i = i->next;
    }

    int relationsSize;
    relationsSize = ListLinkSize(relations);
    fprintf(saveFile,"%d\n", relationsSize);
    i = ListLinkHead(relations);
    while (i!=NULL)
    {
        PrintFileRelation(saveFile, i->content);
        i = i->next;
    }
}


void LoadData(FILE* saveFile, ListLink** orders, ListLink** products,
              ListLink** relations)
{
    int ordersSize;
    fscanf(saveFile, "%d", &ordersSize);
    for (int i = 0; i < ordersSize; i++)
    {
        char owner[INPUT_SIZE];
        int ordDay;
        int ordMonth;
        int ordYear;
        int shipDay;
        int shipMonth;
        int shipYear;
        fscanf(saveFile, "%s", owner);
        fscanf(saveFile, "%d", &ordMonth);
        fscanf(saveFile, "%d", &ordDay);
        fscanf(saveFile, "%d", &ordYear);
        fscanf(saveFile, "%d", &shipMonth);
        fscanf(saveFile, "%d", &shipDay);
        fscanf(saveFile, "%d", &shipYear);
        *orders = AddOrder(*orders, owner, ordDay, ordMonth, ordYear, shipDay,
                          shipMonth, shipYear);

    }
    int productsSize;
    fscanf(saveFile, "%d", &productsSize);
    for (int i = 0; i < productsSize; i++)
    {
        char name[INPUT_SIZE];
        int price;
        int weight;
        fscanf(saveFile, "%s", name);
        fscanf(saveFile, "%d", &price);
        fscanf(saveFile, "%d", &weight);
        *products = AddProduct(*products, name, price, weight);
    }
    int relationsSize;
    fscanf(saveFile, "%d", &relationsSize);
    for (int i = 0; i < relationsSize; i++)
    {
        char owner[INPUT_SIZE];
        char product[INPUT_SIZE];
        fscanf(saveFile, "%s", product);
        fscanf(saveFile, "%s", owner);
        ListLink* ownerLink;
        ListLink* productLink;
        ownerLink = FindOrder(*orders, owner);
        productLink = FindProduct(*products, product);
        *relations = AddRelation(*relations, ownerLink, productLink);
    }
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
                ListLinkFree(orders);
                orders = NULL;
                ListLinkFree(products);
                products = NULL;
                ListLinkFree(relations);
                relations = NULL;
                LoadData(savefile, &orders, &products, &relations);
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
                SaveData(savefile, orders, products, relations);
                printf("Saved!\n");
            }
            fclose(savefile);
        }

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
                fflush(stdout);
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
                fflush(stdout);
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
                   "3.Back\n\n");
            subOperationCode = CycleInputInt(
                    "Choose the command and enter its number",
                    DeleteMenuInputChecker);
            if (subOperationCode == DELETE_ORDER)
            {
                char* name;
                name = CycleInputString("Enter owner's name");
                if(FindOrder(orders, name)!=NULL)
                {
                    orders = DeleteOrder(FindOrder(orders, name), &relations);
                    free(name);
                }
            }
            if (subOperationCode == DELETE_PRODUCT)
            {
                char* name;
                name = CycleInputString("Enter product's name");
                if (FindProduct(products, name) != NULL)
                {
                    products = DeleteProduct(FindProduct(products, name),
                                             &relations);
                    free(name);
                }
            }
        }

        if (operationCode == SORT_ORDERS)
        {
            subOperationCode = CycleInputInt("\n1. Sort by order date."
                                             "\n2. Sort by shipment date."
                                             "\n3. Back.\n", SortOrdersMenuInputChecker);
            if (subOperationCode == SORT_ORDERS_ORD)
            {
                ListLinkBubbleSort(orders, CompOrdersOrderDate);
            }

            if (subOperationCode == SORT_ORDERS_SHIP)
            {
                ListLinkBubbleSort(orders, CompOrdersShipDate);
            }
        }

        if (operationCode == SORT_PRODUCTS)
        {
            subOperationCode = CycleInputInt("\n1. Sort by products price."
                                             "\n2. Sort by products weight."
                                             "\n3. Back.\n", SortProductsMenuInputChecker);
            if (subOperationCode == SORT_PRODUCTS_PRICE)
            {
                ListLinkBubbleSort(products, CompProductsPrice);
            }
            if (subOperationCode == SORT_PRODUCTS_WEIGHT)
            {
                ListLinkBubbleSort(products, CompProductsWeight);
            }
        }

        if (operationCode == QUIT)
        {
            break;
        }
    }
    ListLinkFree(orders);
    ListLinkFree(products);
    ListLinkFree(relations);
    return 0;
}
