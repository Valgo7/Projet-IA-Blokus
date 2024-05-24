#include <stdio.h>
#include "piece.h"
#include "blokus.h"
#include "board.h"
#include "list.h"
#include "item.h"


int sup0 (int val) { // Renvoie la valeur si elle est positive ou 0 sinon.
    if (val>0) return val;
    return 0;
};



list_t * regardeAutour (Item * node, int pos, piece p, list_t * movesPossibles, int player)
{ //On cherche les endroits où on peut mettre la piece p qui chevauchent la position de reference pos de la piece
    // printf("Enter regarde autour\n");
    Item * nodeMove;
    for (int i = 0; i < p->nb_dispos; i++)
    {
        disposition currtDisp = p->dispositions[i];
        int posX = pos / WH_BOARD;
        int posY = pos % WH_BOARD;
        for (int x = sup0(posX - currtDisp->largeur); x<=posX; x++) {
            for (int y = sup0(posY - currtDisp->hauteur); y<=posY; y++) {
                if (isValidPosition(node->board, x*WH_BOARD+y, p, i, player)==1)
                {
                //   printf("Add valid position\n");
                    nodeMove = getChildBoard(node, x*WH_BOARD+y, p, i, player);
                    addLast(movesPossibles, nodeMove);
                }
            }
        }
    }
    return movesPossibles;
};


void addFinTab (int * tableau, int val) {
    int i = 0;
    while (*(tableau+i)!=-1 && i<MAX_BOARD) {
        
        i++;
    }
    if (i<MAX_BOARD) {
        tableau[i+1]=val;
    }
    return;
}

int existsInArray(int* arr, int pos)
{
    int i = 0;
    while(arr[i] != -1)
    {
        if(arr[i] == pos)
            return 1;
        i++;
    }
    return 0;
}
int checkColor (Item * node, int pos, int player, int * dejaVu) {
    // Compte les cases adjacentes qui sont de la meme "couleur" (i.e. de la valeur de player) et les met dans deja vu

    int cpt = 0;
    int row = pos / WH_BOARD;
    int col = pos % WH_BOARD;

    // printf("row : %d\n",row);
    // printf("col : %d\n",col);

    //Cases de la rangee du dessus
    if (row !=0 && node->board[(row-1)*WH_BOARD+col]==player && !existsInArray(dejaVu,(row-1)*WH_BOARD+col)) {
        // printf("Dessus centre\n");
        
        addFinTab(dejaVu, (row-1)*WH_BOARD+col);
    
        cpt++;
    }
    if (row !=0 && col !=0 && node->board[(row-1)*WH_BOARD+col-1]==player && !existsInArray(dejaVu,(row-1)*WH_BOARD+col-1)) {
        // printf("Dessus gauche\n");
        addFinTab(dejaVu, (row-1)*WH_BOARD+col-1);
        cpt++;
    }
    if (row !=0 && col !=WH_BOARD-1 && node->board[(row-1)*WH_BOARD+col+1]==player && !existsInArray(dejaVu,(row-1)*WH_BOARD+col+1)) {
        // printf("Dessus droite\n");
        addFinTab(dejaVu, (row-1)*WH_BOARD+col+1);
        cpt++;
    }

    //case de gauche
    if (col !=0 && node->board[row*WH_BOARD+col-1]==player && !existsInArray(dejaVu,row*WH_BOARD+col-1)) {
        // printf("Gauche\n");
        addFinTab(dejaVu, row*WH_BOARD+col-1);
        cpt++;
    }
    //case de droite
    if (col !=WH_BOARD && node->board[row*WH_BOARD+col+1]==player && !existsInArray(dejaVu,row*WH_BOARD+col+1)) {
        // printf("Droite\n");
        addFinTab(dejaVu, row*WH_BOARD+col+1);
        cpt++;
    }

    //Cases de la rangee du dessous

    //desous centre
    if (row !=WH_BOARD-1 && node->board[(row+1)*WH_BOARD+col]==player && !existsInArray(dejaVu,(row+1)*WH_BOARD+col)) {
        addFinTab(dejaVu, (row+1)*WH_BOARD+col);
        cpt++;
    }

    //dessous gauche
    if (row !=WH_BOARD-1 && col !=0 && node->board[(row+1)*WH_BOARD+col-1]==player && !existsInArray(dejaVu,(row+1)*WH_BOARD+col-1)) {
        addFinTab(dejaVu, (row+1)*WH_BOARD+col-1);
        cpt++;
    }

    //dessous droite
    if (row !=WH_BOARD-1 && col !=WH_BOARD-1 && node->board[(row+1)*WH_BOARD+col+1]==player && !existsInArray(dejaVu,(row+1)*WH_BOARD+col+1)) {
        addFinTab(dejaVu, (row+1)*WH_BOARD+col+1);
        cpt++;
    }
    return cpt;
}

int posHG (int pos) {
    int row = pos / WH_BOARD;
    int col = pos % WH_BOARD;
    if (row >= 0 && col >=0) {
        return (row-1)*WH_BOARD+col-1;
    }
    else {return -1;}
};
int posHD (int pos) {
    int row = pos / WH_BOARD;
    int col = pos % WH_BOARD;
    if (row >= 0 && col < WH_BOARD) {
        return (row-1)*WH_BOARD+col+1;
    }
    else {return -1;}
};
int posBG (int pos) {
    int row = pos / WH_BOARD;
    int col = pos % WH_BOARD;
    if (row < WH_BOARD && col >= 0) {
        return (row+1)*WH_BOARD+col-1;
    }
    else {return -1;}
};
int posBD (int pos) {
    int row = pos / WH_BOARD;
    int col = pos % WH_BOARD;
    if (row < WH_BOARD && col <= WH_BOARD) {
        return (row-1)*WH_BOARD+col-1;
    }
    else {return -1;}
};

void parcoursCases (Item * node, piece p, int player, int pos, int * dejaVu, int ou_on_en_est, list_t * movesPossibles) {
    //Le principe est de suivre le "chemin" forme par les pieces du joueur player sur le plateau et de chercher tous les endroits ou on peut placer la piece p.
    //dejaVu contient les cases traitees ou a traiter dans le tableau.
    //ou_on_en_est est la limite entre les cases traitees et a traiter dans le tableau.
    // printf("Ou on es est : %d\n", ou_on_en_est);
    // printf("pos : %d\n",pos);
    int poshg, poshd, posbg, posbd;
    //Cas terminal :
    //Si aucune case autour de pos n'est de la "couleur" player alors regardeAutour (positions en diagonale de pos)
    int check = checkColor(node, pos, player, dejaVu);
    // printf("checkColor(node, pos, player, dejaVu) == %d\n",check);
    if (checkColor(node, pos, player, dejaVu) == 0) {
        // int poshg, poshd, posbg, posbd = posHG(pos), posHD(pos), posBG(pos), posBD(pos);

        poshg = posHG(pos);
        poshd = posHD(pos);
        posbg = posBG(pos);
        posbd = posBD(pos);
        // printf("poshg, poshd, posbg, posbd : %d %d %d %d\n",poshg, poshd, posbg, posbd);
        if (poshg !=-1 && node->board[poshg]==0) {regardeAutour(node, poshg, p, movesPossibles, player);}
        if (poshd !=-1 && node->board[poshd]==0) {regardeAutour(node, poshd, p, movesPossibles, player);}
        if (posbg !=-1 && node->board[posbg]==0) {regardeAutour(node, posbg, p, movesPossibles, player);}
        if (posbd !=-1 && node->board[posbd]==0) {regardeAutour(node, posbd, p, movesPossibles, player);}
        
    }

    // Recursivite : on va regarder plus loin dans le tableau des cases traitees/a traiter pour vois s'il y a des positions valides autour
    else {
        while (*(dejaVu+ou_on_en_est)!=-1 && ou_on_en_est+1<MAX_BOARD) {
            // printf(" while (*(dejaVu+ou_on_en_est)!=-1 && ou_on_en_est+1<MAX_BOARD) -> parcoursCases");
            // printf("*(dejaVu+ou_on_en_est) : %d\n",dejaVu[ou_on_en_est]);
            parcoursCases (node, p, player, *(dejaVu+ou_on_en_est), dejaVu, ou_on_en_est+1, &movesPossibles);
            ou_on_en_est++;
        }
    }
    return;
};


list_t * allMoves (Item * node, piece p, int quel_joueur) {
    list_t * movesPossibles = malloc(sizeof(list_t));
    initList(movesPossibles);
    // printf("init moves possibles 2 : %d\n",movesPossibles->numElements);
    int * dejaVu = malloc (MAX_BOARD*sizeof(int));
    // printf("malloc dejaVu\n");
    for (int i = 0; i<MAX_BOARD; i++) {
        // printf("dejaVu[i]=-1\n");
        dejaVu[i]=-1;
    }
    if (quel_joueur == 1) {
        dejaVu[0]=MAX_BOARD-1;
    }
    else {
        dejaVu[0] = 0;
    }
    int ou_on_en_est = 0;
    parcoursCases(node, p, quel_joueur, *dejaVu, dejaVu, ou_on_en_est, movesPossibles);
    return movesPossibles;
};



