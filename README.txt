Malheureusement, par manque de temps, le code contient au moins une erreur :


- Dans blokus.c, l'une des fonctions suivantes parmi regardeAutour, PoarcoursCases et checkColor contient une erreur (encore indéterminée) qui fait que la liste movesPossibles reste vide pendant toute l'exécution. Une erreur sur une fonction aussi cruciale empêche l'exécution et donc, le débogage des fonctions de main.c.


//Structure du projet//

item.h implémente la structure item qui est le maillon de liste chainee.
list.h/list.c implémente la structure de liste chainee.
board.h/board.c implémente les modifications du board au fil de la partie.
piece.h/piece.c implémente la structure de pièce de Blokus.
blokus.h/blokus.c implémente la recherche de tous les coups possibles dans le cas spécifique du jeu de Blokus.
main.c contient les fonctions gérant les tours de l'IA et de l'humain et permet de lancer la partie.