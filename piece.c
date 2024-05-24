#include <stdio.h>
#include <assert.h>
#include "piece.h"

#define NB_PIECES 21

piece piecesJ [21];
piece piecesIA [21];

// piece c5, f5, i5, l5, n5, p5, t5, v5, w5, x5, y5, z5, i4, l4, n4, o4, t4, i3, v3, domino, carre;





disposition init_dispo(int _h, int _l, short* p)
{
    disposition d = malloc(sizeof(disposition));
    d->cases = malloc(_h*_l*sizeof(short));
    d->hauteur = _h;
    d->largeur = _l;

    for(int i = 0; i < _h*_l; i++)
    {
        d->cases[i] = p[i];
    }


    return d;
}

disposition mirror_disp (disposition d_old)
{
    disposition d_s = init_dispo(d_old->hauteur,d_old->largeur,d_old->cases);
    for(int i = 0; i < d_old->hauteur; i++)
    {
        for(int j = 0; j < d_old->largeur; j++)
            d_s->cases[d_old->largeur*i+j] = d_old->cases[d_old->largeur*i+d_old->largeur-1-j];
    }


    return d_s;
}



disposition rotate90(disposition disp)
{
    disposition d_r = init_dispo(disp->largeur,disp->hauteur,disp->cases); // pour un pion qui n'est pas "contenu dans un carré", mais plutôt dans un "rectangle"
    for(int i = 0; i < disp->hauteur; i++) // pour chaque case de d
    {
        for(int j = 0; j < disp->largeur; j++)
            d_r->cases[j*disp->hauteur+disp->hauteur-1-i] = disp->cases[i*disp->largeur+j];
    }

    return d_r;
}







// Fonction pour comparer deux dispositions
int compareDispositions(disposition disp1, disposition disp2) {
    if (disp1->hauteur != disp2->hauteur || disp1->largeur != disp2->largeur) {
        return 0; // Les dimensions sont différentes
    }
    
    // Comparer les cases
    for (int i = 0; i < disp1->hauteur * disp1->largeur; i++) {
        if (disp1->cases[i] != disp2->cases[i]) {
            return 0; // Les cases sont différentes
        }
    }
    
    return 1; // Les dimensions et les cases sont identiques
}

// Fonction pour vérifier si une disposition est présente dans un tableau de dispositions
int appartientAuTableau(disposition *tab, int taille, disposition disp) {
    for (int i = 0; i < taille; i++) {
        if (compareDispositions(tab[i], disp)) {
            return 1; // La disposition est trouvée dans le tableau
        }
    }
    
    return 0; // La disposition n'est pas trouvée dans le tableau
}








disposition createDispositionSet(int h, int l, short cases_tab1)
{
    disposition* new_d_tab = malloc(8*sizeof(disposition));
    new_d_tab[0] = init_dispo(h,l,cases_tab1);

    disposition temp = init_dispo(new_d_tab[0]->hauteur,new_d_tab[0]->largeur, new_d_tab[0]->cases);
    int taille = 1;
    printf("Rotate\n");
    for(int i = 1; i < 4; i++)
    {
        temp = rotate90(new_d_tab[i-1]);
        if(!appartientAuTableau(new_d_tab,taille,temp))
        {
            new_d_tab[taille] = rotate90(new_d_tab[taille-1]);
            taille++;
        }
        
    }
    int j = taille;

    printf("Mirror\n");
    for(int i = 0; i < j; i++)
    {
        temp = mirror_disp(new_d_tab[i]);
        if(!appartientAuTableau(new_d_tab,taille,temp))
        {
            new_d_tab[taille] = mirror_disp(new_d_tab[i]);
            taille++;
        }
    }

    if(taille < 8)
        realloc(new_d_tab,taille*sizeof(disposition)); // falcutatif
    
    return new_d_tab;
}



//Affiche une disposition
void printDispo(disposition disp) {
    for (int i = 0; i < disp->hauteur; i++) {
        for (int j = 0; j < disp->largeur; j++) {
            printf("%d ", disp->cases[i * disp->largeur + j]);
        }
        printf("\n");
    }
}

void printPiece(piece p)
{
    printf("Nom: %s\n", p->nom_piece);
    printf("Disponible: %d\n(1: disponible. 0: Non disponible.)", p->utilisable);
    for(int i = 0; i < p->nb_dispos; i++)
	{
		printDispo(p->dispositions[i]);
		printf("____________\n");
	}
}



//Cree une piece
piece createPiece(char *nom_piece, int nb_cases,int nb_dispos, disposition *dispositions) {
    piece new_p = (piece) malloc(sizeof(struct piece_)); //struct piece_ ou piece ?
    assert(new_p);
    new_p->nom_piece = nom_piece;
    new_p->utilisable = 1;
    new_p->nb_cases = nb_cases;
    new_p->nb_dispos = nb_dispos;
    new_p->dispositions = dispositions;
    return new_p;
};



piece * createAllPieces (char player) {

piece * allPieces = malloc(NB_PIECES*sizeof(piece));

char * tabc5 = malloc(3*2*sizeof(char));
*(tabc5)= player;
*(tabc5+1)= player;

*(tabc5+2)= player;
*(tabc5+3)= 0;

*(tabc5+4)= player;
*(tabc5+5)= player;
piece c5 = createPiece("c5", 5, 4, createDispositionSet(3, 2, tabc5));

// printf("c5 done\n");
*(allPieces) = c5;


char * tabf5 = malloc(3*3*sizeof(char));
*(tabf5)= 0;
*(tabf5+1)= player;
*(tabf5+2)= player;

*(tabf5+3)= player;
*(tabf5+4)= player;
*(tabf5+5)= 0;

*(tabf5+6)= 0;
*(tabf5+7)= player;
*(tabf5+8)= 0;


piece f5 = createPiece("f5", 5, 8, createDispositionSet(3, 3, tabf5));

// printf("f5 done\n");
*(allPieces+1) = f5;

// { 1, 0, 0, 1, 1, 1, 0, 1, 0 };
// { 0, 1, 1, 1, 1, 0, 0, 1, 0 };
// { 0, 1, 0, 1, 1, 1, 0, 0, 1 };
// { 0, 1, 0, 0, 1, 1, 1, 1, 0 };


char * tabi5 = malloc(5*sizeof(char)); // répliquer pour les autres ix pièces
*(tabi5)= player;
*(tabi5+1)= player;
*(tabi5+2)= player;
*(tabi5+3)= player;
*(tabi5+4)= player;

disposition* i5_d = malloc(2*sizeof(disposition));
i5_d[0] = init_dispo(1,5,tabi5);
i5_d[1] = init_dispo(5,1,tabi5);

piece i5 = createPiece("i5", 5, 2, i5_d);

// printf("i5 done\n");
*(allPieces+2) = i5;


char * tabl5 = malloc(4*2*sizeof(char));
*(tabl5)= player;
*(tabl5+1)= 0;

*(tabl5+2)= player;
*(tabl5+3)= 0;

*(tabl5+4)= player;
*(tabl5+5)= 0;

*(tabl5+6)= player;
*(tabl5+7)= player;

piece l5 = createPiece("l5", 5, 8, createDispositionSet(4, 2, tabl5));

// printf("l5 done\n");
*(allPieces+3) = l5;

char * tabn5 = malloc(4*2*sizeof(char));
*(tabn5)= 0;
*(tabn5+1)= player;

*(tabn5+2)= player;
*(tabn5+3)= player;

*(tabn5+4)= player;
*(tabn5+5)= 0;

*(tabn5+6)= player;
*(tabn5+7)= 0;

piece n5 = createPiece("n5", 5, 8, createDispositionSet(4, 2, tabn5));

// printf("n5 done\n");
*(allPieces+4) = n5;

char * tabp5 = malloc(3*2*sizeof(char));
*(tabp5)= player;
*(tabp5+1)= player;

*(tabp5+2)= player;
*(tabp5+3)= player;

*(tabp5+4)= player;
*(tabp5+5)= 0;

piece p5 = createPiece("p5", 5, 8, createDispositionSet(3, 2, tabp5));

// printf("p5 done\n");
*(allPieces+5) = p5;

char * tabt5 = malloc(3*3*sizeof(char));
*(tabt5)= player;
*(tabt5+1)= player;
*(tabt5+2)= player;

*(tabt5+3)= 0;
*(tabt5+4)= player;
*(tabt5+5)= 0;

*(tabt5+6)= 0;
*(tabt5+7)= player;
*(tabt5+8)= 0;


piece t5 = createPiece("t5", 5, 4, createDispositionSet(3, 3, tabt5));

// printf("t5 done\n");
*(allPieces+6) = t5;

char * tabv5 = malloc(3*3*sizeof(char));
*(tabv5)= player;
*(tabv5+1)= 0;
*(tabv5+2)= 0;

*(tabv5+3)= player;
*(tabv5+4)= 0;
*(tabv5+5)= 0;

*(tabv5+6)= player;
*(tabv5+7)= player;
*(tabv5+8)= player;

piece v5 = createPiece("v5", 5, 4, createDispositionSet(3, 3,  tabv5));

// printf("v5 done\n");
*(allPieces+7) = v5;

char * tabw5 = malloc(3*3*sizeof(char));
*(tabw5)= player;
*(tabw5+1)= 0;
*(tabw5+2)= 0;

*(tabw5+3)= player;
*(tabw5+4)= player;
*(tabw5+5)= 0;

*(tabw5+6)= 0;
*(tabw5+7)= player;
*(tabw5+8)= player;

piece w5 = createPiece("w5", 5, 4, createDispositionSet(3, 3, tabw5));

// printf("w5 done\n");
*(allPieces+8) = w5;

char * tabx5 = malloc(3*3*sizeof(char)); // attendtion : une seule disposition !
*(tabx5)= 0;
*(tabx5+1)= player;
*(tabx5+2)= 0;

*(tabx5+3)= player;
*(tabx5+4)= player;
*(tabx5+5)= player;

*(tabx5+6)= 0;
*(tabx5+7)= player;
*(tabx5+8)= 0;

disposition* x5_d = malloc(sizeof(disposition));
x5_d[0] = init_dispo(3,3,tabx5);

piece x5 = createPiece("x5", 5, 1, x5_d);

// printf("x5 done\n");
*(allPieces+9) = x5;



char * taby5 = malloc(4*2*sizeof(char));
*(taby5)= 0;
*(taby5+1)= player;

*(taby5+2)= player;
*(taby5+3)= player;

*(taby5+4)= 0;
*(taby5+5)= player;

*(taby5+6)= 0;
*(taby5+7)= player;


piece y5 = createPiece("y5", 5, 8, createDispositionSet(4, 2, taby5));

// printf("y5 done\n");
*(allPieces+10) = y5;

// char * tabz5 = malloc(3*3*sizeof(char));

// *(tabz5)= 1;
// *(tabz5+1)= 1;
// *(tabz5+2)= 0;

// *(tabz5+3)= 0;
// *(tabz5+4)= 1;
// *(tabz5+5)= 0;

// *(tabz5+6)= 0;
// *(tabz5+7)= 1;
// *(tabz5+8)= 1;.

char tabz5[9] = { player, player, 0,
                   0, player, 0,
                   0, player, player };

disposition* z5_d = malloc(4*sizeof(disposition));
z5_d[0] = init_dispo(3,3,tabz5);
z5_d[1] = rotate90(z5_d[0]);
z5_d[2] = mirror_disp(z5_d[0]);
z5_d[3] = rotate90(z5_d[2]);


piece z5 = createPiece("z5", 5, 4, z5_d);

// printf("z5 done\n");
*(allPieces+11) = z5;

/////// 4 cases

char * tabi4 = malloc(4*sizeof(char));
*(tabi4)= player;
*(tabi4+1)= player;
*(tabi4+2)= player;
*(tabi4+3)= player;

disposition* i4_d = malloc(2*sizeof(disposition));
i4_d[0] = init_dispo(1,4,tabi4);
i4_d[1] = init_dispo(4,1,tabi4);

piece i4 = createPiece("i4", 4, 2, i4_d);

// printf("i4 done\n");
*(allPieces+12) = i4;

char * tabl4 = malloc(3*2*sizeof(char));
*(tabl4)= player;
*(tabl4+1)= 0;

*(tabl4+2)= player;
*(tabl4+3)= 0;

*(tabl4+4)= player;
*(tabl4+5)= player;

piece l4 = createPiece("l4", 4, 8, createDispositionSet(3, 2, tabl4));

// printf("l4 done\n");
*(allPieces+13) = l4;

// char * tabn4 = malloc(3*2*sizeof(char));
// *(tabn4)= 0;
// *(tabn4+1)= 1;

// *(tabn4+2)= 1;
// *(tabn4+3)= 1;

// *(tabn4+4)= 1;
// *(tabn4+5)= 0;

char tabn4[6] = { 0, player,
                  player, player,
                  player, 0 };

disposition* n4_d = malloc(4*sizeof(disposition));
n4_d[0] = init_dispo(3,2,tabn4);
n4_d[1] = rotate90(n4_d[0]);
n4_d[2] = mirror_disp(n4_d[0]);
n4_d[3] = rotate90(n4_d[2]);

// piece n4 = createPiece("n4", 4, 4, createDispositionSet(3, 2, tabn4));
piece n4 = createPiece("n4", 4, 4, n4_d);

// printf("n4 done\n");
*(allPieces+14) = n4;

char * tabo4 = malloc(2*2*sizeof(char));
*(tabo4)= player;
*(tabo4+1)= player;

*(tabo4+2)= player;
*(tabo4+3)= player;

disposition* o4_d = malloc(sizeof(disposition));
o4_d[0] = init_dispo(2,2,tabo4);

piece o4 = createPiece("o4", 4, 1, o4_d);

// printf("o4 done\n");
*(allPieces+15) = o4;

// char * tabt4 = malloc(2*3*sizeof(char));
// *(tabt4)= 1;
// *(tabt4+1)= 1;
// *(tabt4+2)= 1;

// *(tabt4+3)= 0;
// *(tabt4+4)= 1;
// *(tabt4+5)= 0;

char tabt4[6] = { player, 0,
                   player, player,
                   player, 0 };

disposition* t4_d = malloc(4*sizeof(disposition));
t4_d[0] = init_dispo(3,2,tabt4);
t4_d[1] = rotate90(t4_d[0]);
t4_d[2] = mirror_disp(t4_d[0]);
t4_d[3] = rotate90(t4_d[2]);

// piece t4 = createPiece("t4", 4, 4, createDispositionSet(3, 2, tabt4));
piece t4 = createPiece("t4", 4, 4, t4_d);
// printf("t4 done\n");
*(allPieces+16) = t4;

//////// 3 cases

char * tabi3 = malloc(3*sizeof(char));
*(tabi3)= player;
*(tabi3+1)= player;
*(tabi3+2)= player;

disposition* i3_d = malloc(2*sizeof(disposition));
i3_d[0] = init_dispo(1,3,tabi3);
i3_d[1] = init_dispo(3,1,tabi3);

piece i3 = createPiece("i3", 3, 2, i3_d);

// printf("i3 done\n");
*(allPieces+17) = i3;

char * tabv3 = malloc(2*2*sizeof(char));
*(tabv3)= player;
*(tabv3+1)= 0;

*(tabv3+2)= player;
*(tabv3+3)= player;


piece v3 = createPiece("v3", 3, 4, createDispositionSet(2, 2, tabv3));
// printf("v3 done\n");
*(allPieces+18) = v3;


/////// 2 cases

char * tabdomino = malloc(2*sizeof(char));
*(tabdomino)= player;
*(tabdomino+1)= player;

disposition* dom_d = malloc(2*sizeof(disposition));
dom_d[0] = init_dispo(1,2,tabdomino);
dom_d[1] = init_dispo(2,1,tabdomino);

piece domino = createPiece("domino", 2, 2, dom_d);

// printf("domino done\n");
*(allPieces+19) = domino;

//////// 1 case

char * tabcarre = malloc(sizeof(char));
*tabcarre = player;

disposition* carre_d = malloc(sizeof(disposition));
carre_d[0] = init_dispo(1,1,tabcarre);

piece carre = createPiece("carre", 1, 1, carre_d);

// printf("carre done\n");
*(allPieces+20) = carre;

return allPieces;
}

// int main () {
//     piece *allPieces = createAllPieces();
//     disposition * dispTab = malloc(8*sizeof (disposition));
//     for (int i; i<21; i++) {
        
//     }
//     return 0;
// }