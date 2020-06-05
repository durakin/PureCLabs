#ifndef LISTLINK_H
#define LISTLINK_H
#include <stdbool.h>

typedef struct ListLink
{
    // Структура для хранения двусвязного списка комнат.
    struct ListLink* previous;    // Указатель на предыдущий элемент
    struct ListLink* next;    // Указатель на следующий элемент
    void* content;    // Указатель на информацию о комнате
} ListLink;

ListLink* ListLinkHead(ListLink* objectLink);
ListLink* ListLinkTail(ListLink* objectLink);
ListLink* ListLinkAdd(ListLink* objectList, void* content);
ListLink* ListLinkDelete(ListLink* linkToDelete);
void ListLinkSwap(ListLink* firstLink, ListLink* secondLink);
int ListLinkSize(ListLink* objectLink);
void ListLinkBubbleSort(ListLink* objectLink, bool(* comparator)(void*, void*));
void ListLinkFree(ListLink* objectLink);


#endif //LISTLINK_H
