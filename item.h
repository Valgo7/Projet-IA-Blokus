#ifndef ITEM_H
struct Item_s;
#include "piece.h"

typedef struct Item_s {
  int size; // size of board (400)
  char *board; // board is an array of small int
  float f, g, h; // cost, heuristic, ...
  int depth;
  piece* pieceJoueur;
  piece* pieceIA;
  struct Item_s *parent; // needed for tree structure in game search
  struct Item_s *prev, *next; // needed for chained list 
} Item ;

#endif 



