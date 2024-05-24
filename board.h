#ifndef BOARD_H
#define BOARD_H
#include "list.h"

#define RANDINIT()  srand(time(NULL))
#define RANDOM()  ((float)rand() / (float)RAND_MAX)
#define RANDMAX(x)  (int)((float)(x)*rand()/(RAND_MAX+1.0))

#define WH_BOARD 20
#define MAX_BOARD WH_BOARD*WH_BOARD

Item *initGame(); //cree le premier Item de la partie, correspondant à la situation initiale
void initBoard(Item *node, char* board); //On donne à node un nouveau board qui est la copie de celui en entree
void printBoard( Item *board ); // Affiche le board
int evaluateBoard(Item *node, int nb_joueur); // Evalue le score du joueur nb_joueur (1 : humain ; 2 : IA), le score renvoyé est le nombre de cases des pièces qui ne sont pas encore placées. En fin de partie, donne le score final d'un joueur.
int isValidPosition(char *board, int pos, piece p, int nb_disp, int player); // pour une pièce et une disposition données, dit si la position pos du tableau est valide. (Comme précédemment, pos coincide avec la première case du tableau qui représente la disposition de la pièce.)
int isEqual_p (piece p1, piece p2); // Si deux pièces ont le meme nom, renvoie 1. Sinon, renvoie 0.
Item *getChildBoard( Item *node, int pos, piece p, int nb_disp ); // Renvoie un node mis à jour où le board contient la piece p. Les changements de disponibilité des pieces sont faits.

#endif