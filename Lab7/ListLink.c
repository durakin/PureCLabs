#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ListLink.h"

ListLink* ListLinkHead(ListLink* objectLink)
{
    ListLink* position;
    position = objectLink;
    if (objectLink == NULL)
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
    free(linkToDelete->content);
    free(linkToDelete);
    return linkToReturn;
}


void ListLinkSwap(ListLink* firstLink, ListLink* secondLink)
{
    void* contentBox;
    contentBox = firstLink->content;
    firstLink->content = secondLink->content;
    secondLink->content = contentBox;
}


int ListLinkSize(ListLink* objectLink)
{
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


void
ListLinkBubbleSort(ListLink* objectLink, bool(* comparator)(void*, void*))
{
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


void ListLinkFree(ListLink* objectLink)
{
    ListLink* i;
    ListLink* linkToFree;
    i = ListLinkHead(objectLink);
    while (i != NULL)
    {
        linkToFree = i;
        i = i->next;
        free(linkToFree->content);
        free(linkToFree);
    }

}