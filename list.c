#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

/* Item Functions */

Item *nodeAlloc() // cree un nouvel item
{
  Item *node=(Item *) malloc(sizeof(Item) );
  assert(node);
  node->board = NULL;
  node->parent = NULL;
  node->prev = NULL;
  node->next = NULL;
  node->f = node->g = node->h = 0;
  return node;
}

void freeItem( Item *node ) // libere la memoire allouee a un item
{
  if (node && node->board) free(node->board);
	if (node) free(node);
}


void initList( list_t *list_p ) // Cree une liste vide
{
	assert(list_p);

  list_p->numElements = 0;
  list_p->first = NULL;
  list_p->last = NULL;
}

int listCount( list_t *list ) // Donne le nombre d'elements
{
	return list->numElements;
}

Item* onList( list_t *list, char *board) // On cherche un item de la liste avec le meme board et on le renvoie ; s'il n'y en a pas, on renvoie NULL
{
  Item *nex=list->first;
  while(nex!= NULL){
    int b=1;
    for(int i=0;i<nex->size;i++){
      if(nex->board[i] != board[i]) {  
        b=0; 
      }
    }
    if(b){
      return nex;
    }
    nex=nex->next;
  }
  return NULL;
}


Item *popFirst( list_t *list ) //On retire le premier element de la liste et on le renvoie
{
  Item *item =list->first;
  delList(list,item);
	return item;
}


Item *popLast( list_t *list ) //On retire le dernier element de la liste et on le renvoie
{
	Item *item =list->last;
  delList(list,item);
	return item;
}


void delList( list_t *list, Item *node) { //On retire l'element node de la liste
  if(node->next)node->next->prev=node->prev;
  if(node->prev)node->prev->next=node->next;
  if(list->last==node)list->last=list->last->prev;
  if(list->first==node)list->first=list->first->next;
  list->numElements--;
}


Item *popBest( list_t *list ) // On parcourt la liste pour trouver l'element de poids f le plus faible ; on le retire de la liste et on le renvoie
{
  Item *item =list->first;
  Item *nex=list->first;
  while(nex){
    if(nex->f<item->f)item=nex;
    nex=nex->next;
  }
  delList(list,item);
  return item;
}


void addFirst( list_t *list, Item *node ) // Pour ajouter un element au debut de la liste
{
  if(!listCount(list))list->last=node;
  else{
    list->first->prev=node;
    node->next=list->first;
  }
  list->first=node;
  list->numElements++;
}

 // add item in queue
void addLast( list_t *list, Item *node ) // Pour ajouter un element a la fin de la liste
{
  if(!listCount(list))list->first=node;
  else{
    list->last->next=node;
    node->prev=list->last;
  }
  list->last=node;
  list->numElements++;
}

void cleanupList( list_t *list ) // On vide la liste et libere la memoire occupee par chaque item
{
	while(listCount(list))freeItem(popFirst(list));
}
