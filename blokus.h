#ifndef BLOKUS_H
#define BLOKUS_H

#include <stdio.h>
#include "piece.h"
#include "board.h"
#include "list.h"
#include "item.h"

int sup0 (int val); // renvoie le maximum entre 0 et la valeur val
list_t * regardeAutour (Item * node, int pos, piece p, list_t * movesPossibles, int player); // on regarde autour de la position pos données en entrée pour voir si on peut placer la pièce p. L'idée est que la pièce donnée en entrée peut peut-être être placée de manière à ce que l'une de ses cases se trouve à pos dans le board final, mais que la case en question n'est pas forcément la position de référence de la pièce (i.e. le coin supérieur gauche, à partir duquel sont faits tous les tests de positionnement. Il faut donc "regarder autour" de pos, c'est à dire supposer que l'on place la pièce sur diverses cases du board telles que l'une de ses cases soit à pos, afin de vérifier correctement si une pièce peut être placée à une certaine case.
void addFinTab (int * tableau, int val); // Le tableau tab contient un certain nombre de valeurs >=0, suivies de -1. On cherche le premier endroit dans le tableau où on trouve un -1 et on y met la valeur val.
int existsInArray(int* arr, int pos); // On regarde si l'entier pos est dans le tableau arr (qui a la meme configuration que dans addFinTab)
int checkColor (Item * node, int pos, int player, int * dejaVu); //Compte les cases adjacentes qui sont de la meme "couleur" (i.e. de la valeur de player càd 1 ou 2) et les met dans le tableau "dejaVu"
int posHG (int pos); // renvoie l'entier qui correspond à la position en haut à gauche de pos
int posHD (int pos); // renvoie l'entier qui correspond à la position en haut à droite de pos
int posBG (int pos); // renvoie l'entier qui correspond à la position en bas à gauche de pos
int posBD (int pos); // renvoie l'entier qui correspond à la position en bas à droite de pos
void parcoursCases (Item * node, piece p, int player, int pos, int * dejaVu, int ou_on_en_est, list_t * movesPossibles); //Le principe est de suivre le "chemin" forme par les pieces du joueur player sur le plateau et de chercher tous les endroits ou on peut placer la piece p. dejaVu contient les cases traitees ou a traiter dans le tableau. ou_on_en_est est la limite entre les cases traitees et a traiter dans le tableau.
list_t * allMoves (Item * node, piece p, int quel_joueur); // cherche tous les coups possibles pour le joueur quel_joueur (1 ou 2) pour une pièce donnée, dans la configuration du board donnée par node.

// Remarque : quelque part dans CheckColor, regardeAutour ou ParcoursCases, une erreur fait que movesPossibles reste vide à la fin de parcoursCases... Erreur non identifiée, qui empeche l'exécution et donc le debogage des fonctions de main.c.

#endif