#ifndef DATA_H
#define DATA_H

#include "Constants.h"


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


void PrintOrder(Order* objectOrder);

void PrintProduct(Product* objectProduct);

void PrintAllOrders(ListLink* objectList);

void PrintAllProducts(ListLink* objectList);

void PrintProductsByOrder(Order* objectOrder, ListLink* relations);

void PrintOrdersByProduct(Product* objectProduct, ListLink* relations);

ListLink* FindOrder(ListLink* objectList, char* owner);

ListLink* FindProduct(ListLink* objectList, char* name);

ListLink* FindRelation(ListLink* objectList, Order* order, Product* product);

ListLink* AddOrder(ListLink* objectList, char* owner, int ordDay,
                   int ordMonth, int ordYear, int shipDay, int shipMonth,
                   int shipYear);

ListLink* AddProduct(ListLink* objectList, char* name, int price, int weight);

ListLink* AddRelation(ListLink* objectList, ListLink* order,
                      ListLink* product);

ListLink* DeleteRelation(ListLink* relationToDelete);

ListLink* DeleteOrder(ListLink* orderLinkToDelete, ListLink** relations);

ListLink* DeleteProduct(ListLink* productLinkToDelete, ListLink** relations);

void PrintFileOrder(FILE* saveFile, Order* objectOrder);

void PrintFileProduct(FILE* saveFile, Product* objectProduct);

void PrintFileRelation(FILE* saveFile, Relation* relation);

#endif //DATA_H
