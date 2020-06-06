#ifndef LISTLINK_H
#define LISTLINK_H

#include <stdbool.h>

typedef struct ListLink
{
    struct ListLink* previous;
    struct ListLink* next;
    void* content;
} ListLink;

ListLink* ListLinkHead(ListLink* objectLink);

ListLink* ListLinkTail(ListLink* objectLink);

ListLink* ListLinkAdd(ListLink* objectList, void* content);

ListLink* ListLinkDelete(ListLink* linkToDelete);

void ListLinkSwap(ListLink* firstLink, ListLink* secondLink);

int ListLinkSize(ListLink* objectLink);

void
ListLinkBubbleSort(ListLink* objectLink, bool(* comparator)(void*, void*));

void ListLinkFree(ListLink* objectLink);


#endif //LISTLINK_H
