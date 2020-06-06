#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "ListLink.h"
#include "Data.h"


void PrintOrder(Order* objectOrder)
{
    printf("ORDER by: %s\nDate of order: %d.%d.%d\nDate of shipment: "
           "%d.%d.%d\n", objectOrder->owner, objectOrder->ordMonth,
           objectOrder->ordDay, objectOrder->ordYear, objectOrder->shipMonth,
           objectOrder->shipDay, objectOrder->shipYear);
}


void PrintFileOrder(FILE* saveFile, Order* objectOrder)
{
    fprintf(saveFile, "%s\n%d\n%d\n%d\n%d\n%d\n%d\n", objectOrder->owner,
            objectOrder->ordMonth, objectOrder->ordDay, objectOrder->ordYear,
            objectOrder->shipMonth,
            objectOrder->shipDay, objectOrder->shipYear);
}

void PrintProduct(Product* objectProduct)
{
    printf("PRODUCT: %s\nPrice: %d\nWeight: %d\n", objectProduct->name,
           objectProduct->price, objectProduct->weight);
}


void PrintFileProduct(FILE* saveFile, Product* objectProduct)
{
    fprintf(saveFile, "%s\n%d\n%d\n", objectProduct->name,
            objectProduct->price,
            objectProduct->weight);
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


void PrintFileRelation(FILE* saveFile, Relation* relation)
{
    fprintf(saveFile, "%s\n%s\n", relation->product->name,
            relation->order->owner);
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
    if (relationToDelete == NULL)
    {
        return NULL;
    }
    return ListLinkDelete(relationToDelete);
}


ListLink* DeleteOrder(ListLink* orderLinkToDelete, ListLink** relations)
{
    if (orderLinkToDelete == NULL)
    {
        return NULL;
    }
    ListLink* i = ListLinkHead(*relations);
    while (i != NULL)
    {
        ListLink* relationLinkToDelete;
        Relation* relation;
        relation = i->content;
        relationLinkToDelete = i;
        i = i->next;
        if (relation->order == orderLinkToDelete->content)
        {
            *relations = DeleteRelation(relationLinkToDelete);
        }
    }
    return ListLinkDelete(orderLinkToDelete);
}


ListLink* DeleteProduct(ListLink* productLinkToDelete, ListLink** relations)
{
    ListLink* i = ListLinkHead(*relations);
    while (i != NULL)
    {
        ListLink* relationLinkToDelete;
        Relation* relation;
        relation = i->content;
        relationLinkToDelete = i;
        i = i->next;
        if (relation->product == productLinkToDelete->content)
        {
            *relations = DeleteRelation(relationLinkToDelete);
        }
    }
    return ListLinkDelete(productLinkToDelete);
}
