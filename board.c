#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "board.h"
// #include "pion.h"
#include "piece.h"

#define WH_BOARD 20
#define BOARD_AREA (WH_BOARD * WH_BOARD)
#define PLAYER 1
#define AI 2



Item *initGame() // OK. remplace le initGame
{

    Item *node;

	char *initial = (char*)malloc(MAX_BOARD*sizeof(char));
	for (int i=0; i<MAX_BOARD; i++) initial[i] = 0;

  node = nodeAlloc();
	initBoard(node, initial);
    
  node->depth = 0;
  node->f = 0;
  node->g = 0;
  node->h = 0;

  for (int i = 0; i<21; i++) {
  node->pieceIA[i] = createAllPieces(2);
  node->pieceJoueur[i] = createAllPieces(1);
  }

  return node;
}






void printBoard( Item *node ) // OK. Pour afficher la grille.
{
  assert(node);
	printf("\n");
	for (int j=0; j<WH_BOARD; j++) if (j==0) printf(" ___"); else printf("____"); printf("\n");
	for (int i = 0 ; i < MAX_BOARD ; i++) {
    if (i%WH_BOARD == 0) printf("|");
    if (node->board[i] == 0) printf("   |");
    else printf("%2d |", node->board[i]);
    if (((i+1)%WH_BOARD) == 0) {
			printf("\n");
			for (int j=0; j<WH_BOARD; j++) if (j==0) printf(" ___"); else printf("____"); printf("\n");
		}
  }
	printf("\n");
}


void initBoard(Item *node, char *board) { // OK. On donne a node un nouveau board qui est la copie du board en entree
	assert( node );
	
	node->size = MAX_BOARD;
  node->board = calloc(MAX_BOARD, sizeof(char));
  for(int i=0;i<(MAX_BOARD);i++){
    node->board[i]=board[i];
  }
}

int evaluateBoard(Item *node, int nb_joueur) { // OK. Renvoie le nombre de cases des pieces qu'il reste a placer.
  int nb_cases_left=0;
  piece * ensbl_pieces;
  if (nb_joueur == 1) {
    ensbl_pieces = node->pieceJoueur;
  }
  else {
    ensbl_pieces = node->pieceIA;
  }
    for(int i=0;i<21;i++){
      if(ensbl_pieces[i]->utilisable == 1) {  
        nb_cases_left+=ensbl_pieces[i]->nb_cases; 
      } 
    }
  return nb_cases_left;
}





int isValidPosition(char *board, int pos, piece p, int nb_disp, int player) { //OK. Doit verifier si un deplacement est autorise (si on peut poser une piece p a pos)
    // Vérifier si la pièce est encore jouable
    if (!p->utilisable) {
        return 0;
    }

    disposition d = p->dispositions[nb_disp];
    int row = pos / WH_BOARD;
    int col = pos % WH_BOARD;

    // Vérifier que la pièce ne dépasse pas du plateau
    if (row + d->hauteur > WH_BOARD || col + d->largeur > WH_BOARD) {
        return 0;
    }

    int has_corner_contact = 0;

    for (int i = 0; i < d->hauteur; i++) {
        for (int j = 0; j < d->largeur; j++) {
            if (d->cases[i * d->largeur + j] == 1) {
                int board_pos = (row + i) * WH_BOARD + (col + j);

                // Vérifier que la pièce ne chevauche pas une autre
                if (board[board_pos] != 0) {
                    return 0;
                }

                // Vérifier les cases adjacentes pour le même joueur
                int adj_sides = 0;
                // Gauche
                if (col + j > 0 && board[board_pos - 1] == player) {
                    adj_sides++;
                }
                // Droite
                if (col + j < WH_BOARD - 1 && board[board_pos + 1] == player) {
                    adj_sides++;
                }
                // Haut
                if (row + i > 0 && board[board_pos - WH_BOARD] == player) {
                    adj_sides++;
                }
                // Bas
                if (row + i < WH_BOARD - 1 && board[board_pos + WH_BOARD] == player) {
                    adj_sides++;
                }
                if (adj_sides > 0) {
                    return 0;
                }

                // Vérifier les coins adjacents pour le même joueur
                // Haut-gauche
                if (row + i > 0 && col + j > 0 && board[board_pos - WH_BOARD - 1] == player) {
                    has_corner_contact = 1;
                }
                // Haut-droit
                if (row + i > 0 && col + j < WH_BOARD - 1 && board[board_pos - WH_BOARD + 1] == player) {
                    has_corner_contact = 1;
                }
                // Bas-gauche
                if (row + i < WH_BOARD - 1 && col + j > 0 && board[board_pos + WH_BOARD - 1] == player) {
                    has_corner_contact = 1;
                }
                // Bas-droit
                if (row + i < WH_BOARD - 1 && col + j < WH_BOARD - 1 && board[board_pos + WH_BOARD + 1] == player) {
                    has_corner_contact = 1;
                }
            }
        }
    }

    // Vérifier si la pièce a au moins un coin commun avec une pièce du même joueur
    if (!has_corner_contact) {
        return 0;
    }

    return 1;
}





int isEqual_p (piece p1, piece p2) {
  if (p1->nom_piece == p2->nom_piece) {
    return 1;
  }
  return 0;
};




Item *getChildBoard( Item *node, int pos, piece p, int nb_disp, int player ) // OK. On renvoie un nouvel item dont le board est le board de node avec le placement de piece souhaite effectue.
{
  printf("pos : %d\n",pos);
  Item *child_p = NULL;
  if ( isValidPosition(node->board, pos, p, nb_disp, player) ) {
    printf("Node alloc\n");
    child_p =nodeAlloc();
    initBoard(child_p,node->board);
    child_p->depth=node->depth+1;
    


    int row = pos / WH_BOARD;
    int col = pos % WH_BOARD; 
    int larg = p->dispositions[nb_disp]->largeur; //(*p->dispositions+nb_disp)->largeur; 
    int haut = p->dispositions[nb_disp]->hauteur; //(*p->dispositions+nb_disp)->hauteur;
    for (int i = 0; i<larg; i++) {
      for (int j = 0; j<haut; j++) {
        if (child_p->board[(row+i)*WH_BOARD+(col+j)]==0) {
          child_p->board[(row+i)*WH_BOARD+(col+j)] = (char)p->dispositions[nb_disp]->cases[i*larg+j];
        }
      }
    }
    
    if (player = 1) {
      //Desactiver la piece du joueur
      for (int disable = 0; disable < 21; disable++)
      {
        if (isEqual_p (p, *(node->pieceJoueur+disable))) {
          (*(child_p->pieceJoueur+disable))->utilisable = 0;
        }
      }
    }
    else {
      //Desactiver la piece de l'ia
      for (int disable = 0; disable < 21; disable++)
      {
        if (isEqual_p (p, *(node->pieceIA+disable))) {
          (*(child_p->pieceIA+disable))->utilisable = 0;
        }
      }
    }
    

    child_p->parent=node;
  }
  return child_p;
}


