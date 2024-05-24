#include <limits.h> // For INT_MIN and INT_MAX
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "piece.h"
#include "board.h"
#include "list.h"
#include "item.h"

#define DEPTH 3 // Profondeur de recherche pour l'IA

piece findPieceByName (Item * node, char * nomP) { //trouve la piece suivant son nom.
    for (int i = 0; i<21; i++) {
        if ((*(node->pieceJoueur+i))->nom_piece == nomP) {
            return *(node->pieceJoueur+i);
        }
    }
    return NULL;
}



Item * tourJoueur (Item * curNode, int isTurnOne) {
    printBoard(curNode);

    //PIECE A JOUER
    printf("\nQuelle Pièce voulez-vous jouer ?\n");
    for (int i = 0; i<21; i++) {
        printPiece(*(curNode->pieceJoueur+i));
    }
    char * quellePiece;
    scanf("%s", &quellePiece);
    piece p = findPieceByName(curNode, quellePiece);
    if (p == NULL) {
        printf ("Pièce invalide.\n");
        return NULL;
    }
    printPiece(p);

    //DISPOSITION CHOISIE
    printf("Quelle disposition souhaitez-vous utiliser ? (Attention : la première disposition est la disposition 0. \n");
    int p_disp;
    scanf("%d", p_disp);
    printf("Vous avez choisi: \n");
    if (p_disp >= p->nb_dispos) {
        printf ("Disposition invalide.\n");
        return NULL;
    }
    printDispo(*(p->dispositions+p_disp));

    if (isTurnOne) {
        int larg = (*(p->dispositions+p_disp))->largeur;
        int haut = (*(p->dispositions+p_disp))->hauteur;
        if (*((*(p->dispositions+p_disp))->cases + (larg*haut-1))==0) {
            printf("Pièce invalide.\n");
            return NULL;
        }
        int posX = WH_BOARD-1 - (*(p->dispositions+p_disp))->largeur;
        int posY = WH_BOARD-1 - (*(p->dispositions+p_disp))->hauteur;
        Item * newNode = getChildBoard(curNode, posX*WH_BOARD+posY, p, p_disp);
        return newNode;
    }

    // COORDONNEES DU COIN SUPERIEUR GAUCHE DE LA PIECE
    printf ("Ligne de la position de référence de la pièce ? (Attention : la première ligne est la ligne 0. La position de référence est en haut à gauche de la pièce.)");
    int posX;
    scanf("%d", &posX);
    printf ("Colonne de la position de référence de la pièce ? (Attention : la première colonne est la ligne 0. La position de référence est en haut à gauche de la pièce.)");
    int posY;
    scanf("%d", &posY);
    if (posX*WH_BOARD+posY >= MAX_BOARD || !isValidPosition(curNode->board, posX*WH_BOARD+posY, p, p_disp, 1)) {
        printf ("Position invalide.\n");
        return NULL;
    }

    // Affichage du board apres coup
    Item *newNode = getChildBoard(curNode, posX*WH_BOARD+posY, p, p_disp);
    printBoard(newNode);
    return newNode;
}




//Implémentation de minimax avec un elagage alpha beta
int minimax(Item *node, int depth, int isMaximizingPlayer, int alpha, int beta) {
    if (depth == 0 || node == NULL) {
        return evaluateBoard(node, isMaximizingPlayer ? 2 : 1);
    }

    if (isMaximizingPlayer) {
        int maxEval = INT_MIN;
        for (int i = 0; i < 21; i++) {
            piece p = &(node->pieceIA[i]);
            if (p->utilisable) {
                list_t *moves = allMoves(node, p, 2);
                Item *move = moves->first;
                while (move != NULL) {
                    int eval = minimax(move, depth - 1, 0, alpha, beta);
                    maxEval = eval > maxEval ? eval : maxEval;
                    alpha = alpha > eval ? alpha : eval;
                    if (beta <= alpha) {
                        break;
                    }
                    move = move->next;
                }
                cleanupList(moves);
                free(moves);
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int i = 0; i < 21; i++) {
            piece p = &(node->pieceJoueur[i]);
            if (p->utilisable) {
                list_t *moves = allMoves(node, p, 1);
                Item *move = moves->first;
                while (move != NULL) {
                    int eval = minimax(move, depth - 1, 1, alpha, beta);
                    minEval = eval < minEval ? eval : minEval;
                    beta = beta < eval ? beta : eval;
                    if (beta <= alpha) {
                        break;
                    }
                    move = move->next;
                }
                cleanupList(moves);
                free(moves);
            }
        }
        return minEval;
    }
}




//Tour du joueur IA
Item *tourIA(Item *curNode, int depth) {
    int bestValue = INT_MIN;
    Item *bestMove = NULL;
    list_t possibleMoves;
    initList(&possibleMoves);

    // Generate all possible moves for the AI
    for (int i = 0; i < 21; i++) {
        piece p = &(curNode->pieceIA[i]);
        if (p->utilisable) {
            list_t *moves = allMoves(curNode, p, 2);
            Item *move = moves->first;
            while (move != NULL) {
                addLast(&possibleMoves, move);
                move = move->next;
            }
            // Free the temporary moves list after copying
            cleanupList(moves);
            free(moves);
        }
    }

    // Evaluate all possible moves using minimax
    Item *move = possibleMoves.first;
    while (move != NULL) {
        int moveValue = minimax(move, depth - 1, 0, INT_MIN, INT_MAX);
        if (moveValue > bestValue) {
            bestValue = moveValue;
            bestMove = move;
        }
        move = move->next;
    }

    if (bestMove != NULL) {
        curNode = bestMove;
    }

    cleanupList(&possibleMoves);
    return curNode;
}






int main() {
    // Initialisation du jeu
    Item *currentNode = initGame();
    int isGameOver = 0;
    int isPlayerTurn = 1; // 1 pour joueur humain, 0 pour IA
    int turnNumber = 0;

    // Boucle principale du jeu
    while (!isGameOver) {
        printBoard(currentNode);
        
        if (isPlayerTurn) {
            // Tour du joueur
            Item *newNode = tourJoueur(currentNode, turnNumber == 0);
            if (newNode == NULL) {
                printf("Mouvement invalide. Veuillez réessayer.\n");
                continue;
            } else {
                freeItem(currentNode);
                currentNode = newNode;
            }
        } else {
            // Tour de l'IA
            Item *newNode = tourIA(currentNode, DEPTH);
            if (newNode == NULL) {
                printf("L'IA ne peut pas effectuer de mouvement valide.\n");
                isGameOver = 1;
            } else {
                freeItem(currentNode);
                currentNode = newNode;
            }
        }

        // Vérifiez les conditions de fin de partie
        int playerScore = evaluateBoard(currentNode, 1);
        int iaScore = evaluateBoard(currentNode, 2);
        if (playerScore == 0 || iaScore == 0 || turnNumber >= WH_BOARD * WH_BOARD) {
            isGameOver = 1;
        }

        // Alternance des tours
        isPlayerTurn = !isPlayerTurn;
        turnNumber++;
    }

    // Affiche les scores finaux et déclare le gagnant
    int playerScore = evaluateBoard(currentNode, 1);
    int iaScore = evaluateBoard(currentNode, 2);

    printf("Partie terminée !\n");
    printf("Score du joueur : %d\n", playerScore);
    printf("Score de l'IA : %d\n", iaScore);

    if (playerScore > iaScore) {
        printf("Félicitations ! Vous avez gagné !\n");
    } else if (playerScore < iaScore) {
        printf("L'IA a gagné !\n");
    } else {
        printf("C'est une égalité !\n");
    }

    // Libère la mémoire allouée
    freeItem(currentNode);

    return 0;
}
