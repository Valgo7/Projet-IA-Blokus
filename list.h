#ifndef LIST_H
#define LIST_H

#include "item.h"

typedef struct {
  int numElements;
  char *name;
  Item *first;
  Item *last;
} list_t;


Item *nodeAlloc(); //Initialise un noeud
void freeItem( Item *node ); //Libère la mémoire utilisée pour un item
void initList( list_t *list_p ); //initialise la liste chainee en entree
int listCount( list_t *list ); //donne le nombre d'elements de la liste
Item* onList( list_t *list, char *board); // return an item with same board , or null
Item *popFirst( list_t *list ); // return and remove first item
Item *popLast( list_t *list ); // return and remove last item 
Item *popBest( list_t *list ); // return and remove best item with minimal f value
void addFirst( list_t *list, Item *node ); // add item in top
void addLast( list_t *list, Item *node ); // add item in queue
void delList( list_t *list, Item *node); // remove a node from list

void cleanupList( list_t *list ); // empty list and free items


#endif
