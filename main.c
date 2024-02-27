#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct {
    int val;
    int est_mine;
    int est_explosee;
    int vu;
    int est_marquee;
} Cellule;

void ajouter_matrice(Cellule **tab, int l, int c);
void ajouter_valeurs_adjacentes(Cellule **tab, int l, int c);
void afficher_tout(Cellule **tab, int l, int c);
void changer_etat_vu_de_la_cellule(Cellule **tab, int l, int c, int x, int y);
int visiter_cellule(Cellule **tab, int l, int c, int x, int y);
void ajouter_marque(Cellule **tab, int x, int y);
void supprimer_marque(Cellule **tab, int x, int y);
void jouer_partie(Cellule **tab, int l, int c);

// Fonction pour initialiser la matrice avec des cellules du jeu
void ajouter_matrice(Cellule **tab, int l, int c) {
    // Remplir la matrice avec des valeurs nulles
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            // Initialiser chaque cellule avec des valeurs par défaut
            tab[i][j].val = 0;
            tab[i][j].est_mine = 0;
            tab[i][j].vu = 0;
            tab[i][j].est_marquee = 0;
            tab[i][j].est_explosee = 0;
        }
    }
    // Remplir aléatoirement certaines cellules avec des mines
    int ind1, ind2;
    srand(time(NULL));
    for (int i = 0; i < l + 10; i++) {
        ind1 = rand() % l;
        ind2 = rand() % c;
        tab[ind1][ind2].val = -1; // Indiquer qu'il y a une mine dans cette cellule
        tab[ind1][ind2].est_mine = 1;
    }
}

// Fonction pour ajouter les valeurs des cellules adjacentes
void ajouter_valeurs_adjacentes(Cellule **tab, int l, int c) {
    // Parcourir chaque cellule de la matrice
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            // Si la cellule actuelle contient une mine, incrémenter la valeur des cellules adjacentes
            if (tab[i][j].est_mine == 1) {
                for (int k = i - 1; k <= i + 1; k++) {
                    for (int z = j - 1; z <= j + 1; z++) {
                        // Vérifier si la cellule adjacente est dans les limites de la matrice
                        if (k >= 0 && k < l && z >= 0 && z < c && (k != i || z != j)) {
                            // Incrémenter la valeur de la cellule adjacente
                            tab[k][z].val++;
                        }
                    }
                }
            }
        }
    }
}

// Fonction pour afficher l'état actuel du jeu
void afficher_tout(Cellule **tab, int l, int c) {
    // Afficher l'état actuel de chaque cellule de la matrice
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            if (tab[i][j].est_explosee == 1)
                printf("[ * ] "); // Afficher une '*' si la cellule est explosée
            else if (tab[i][j].est_marquee == 1)
                printf("[ D ] "); // Afficher un 'M' si la cellule est marquée
            else if (tab[i][j].vu == 1) {
                if (tab[i][j].est_mine == 1)
                    printf("[ X ] "); // Afficher un 'X' si la cellule contient une mine
                else
                    printf("[ %d ] ", tab[i][j].val); // Afficher la valeur de la cellule
            } else
                printf("[%d:%d] ", i, j); // Afficher les coordonnées de la cellule
        }
        printf("\n");
    }
}

// Fonction pour changer récursivement l'état visité des cellules adjacentes
void changer_etat_vu(Cellule **tab, int l, int c, int x, int y) {
    // Marquer la cellule actuelle comme visitée
    tab[x][y].vu = 1;
    // Si la valeur de la cellule actuelle est 0, changer récursivement l'état visité des cellules adjacentes
    if (tab[x][y].val == 0) {
        for (int k = x - 1; k <= x + 1; k++) {
            for (int z = y - 1; z <= y + 1; z++) {
                if (k >= 0 && k < l && z >= 0 && z < c) {
                    if (tab[k][z].est_mine != 1 && tab[k][z].vu == 0) {
                        changer_etat_vu(tab, l, c, k, z);
                    }
                }
            }
        }
    }
}

// Fonction pour gérer la visite d'une cellule
int visiter_cellule(Cellule **tab, int l, int c, int x, int y) {
    // Si la cellule visitée contient une mine, la partie est perdue
    if (tab[x][y].est_mine == 1) {
        printf("Vous avez perdu :( \n");
        tab[x][y].est_explosee = 1;
        for (int i = 0; i < l; i++) {
            for (int j = 0; j < c; j++)
                tab[i][j].vu = 1;
        }
        return 0;
    } else if (tab[x][y].vu == 1) {
        printf("Vous êtes déjà passé ici. Choisissez un autre endroit!\n");
    } else {
        printf("Bon travail :) \n");
        // Changer récursivement l'état visité des cellules adjacentes
        changer_etat_vu(tab, l, c, x, y);
    }
    return 1;
}

// Fonction pour ajouter une marque à une cellule
void ajouter_marque(Cellule **tab, int x, int y) {
    tab[x][y].est_marquee = 1;
}

// Fonction pour supprimer une marque d'une cellule
void supprimer_marque(Cellule **tab, int x, int y) {
    tab[x][y].est_marquee = 0;
}

// Fonction pour jouer au jeu
void jouer_partie(Cellule **tab, int l, int c) {
    int x, y, est_perdu, choix;
    do {
        printf("\n");
        printf("*************************Mini Menu *************************\n");
        printf("1 - Ajouter une marque \n");
        printf("2 - Visiter la case \n");
        printf("3 - Supprimer la marque \n");
        printf("*************************Fin du Mini Menu *************************\n");
        printf("Entrez votre choix (1/2): ");
        scanf("%d", &choix);
        printf("\nEntrez les coordonnées x et y de votre choix: ");
        scanf("%d %d", &x, &y);

        switch (choix) {
            case 1:
                ajouter_marque(tab, x, y);
                break;
            case 2:
                est_perdu = visiter_cellule(tab, l, l, x, y);
                break;
            case 3:
                supprimer_marque(tab, x, y);
                break;
            default:
                printf("Essayez à nouveau avec 1 ou 2 :) \n");
                break;
        }

        afficher_tout(tab, l, l);
    } while (est_perdu != 0);
}

int main() {
    printf("Salut! Bienvenue dans le jeu du Démineur :) \n");
    int l;
    do {
        printf("Choisissez le niveau de difficulté :\n");
        printf("1 - Facile \n");
        printf("2 - Moyen \n");
        printf("3 - Difficile \n");
        printf("Choisissez entre 1, 2 ou 3 :  \n");
        scanf("%d", &l);
    } while (l > 3 || l < 1);
    l = l * 10;

    // Créer la matrice en utilisant des pointeurs et malloc
    Cellule **tab = malloc(l * sizeof(Cellule *));
    for (int i = 0; i < l; i++) {
        tab[i] = malloc(l * sizeof(Cellule));
    }

    ajouter_matrice(tab, l, l);
    ajouter_valeurs_adjacentes(tab, l, l);
    afficher_tout(tab, l, l);
    jouer_partie(tab, l, l);

    // Libérer la mémoire allouée dynamiquement
    for (int i = 0; i < l; i++) {
        free(tab[i]);
    }
    free(tab);
    return 0;
}
