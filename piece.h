#ifndef PIECE_H
#define PIECE_H


typedef struct disp_ {
    int hauteur;
    int largeur;
    short * cases;  // Cases is a 1D array representing the 2D disposition
} * disposition;


typedef struct piece_ {
    char * nom_piece;
    short utilisable;
    int nb_cases;
    int nb_dispos;
    disposition * dispositions;  // Array of disposition pointers
} * piece;



disposition init_dispo(int _h, int _l, short* p); // Initialise une nouvelle disposition de piece qui correspond à sa symétrie axiale.
disposition mirror_disp (disposition d_old); // Renvoie la disposition symétrique à la disposition en entree
disposition rotate90(disposition disp); // renvoie la piece qui a subi une rotation de 90 degrés dans le sens horaire.
int compareDispositions(disposition disp1, disposition disp2); //Vérifie si deux dispositions en entree sont égales
int appartientAuTableau(disposition *tab, int taille, disposition disp); // vérifie si une disposition appartient à un tableau de dispositions
disposition createDispositionSet(int h, int l, short cases_tab1); // Crée l'ensemble des dispositions d'une pièce ; utile pour la création d'une pièce
void printDispo(disposition disp); //affiche une disposition
void printPiece(piece p); // affiche une pièce (son nom, si elle est utilisable, ses dispositions)
piece createPiece(char *nom_piece, int nb_cases,int nb_dispos, disposition *dispositions); // crée une pièce
piece * createAllPieces (char player); // crée toutes les pièces du joueur player. Les cases non vides de la disposition valent 1 pour le joueur humain et 2 pour l'ia.

#endif