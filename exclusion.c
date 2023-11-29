#include <stdio.h>
#include <stdlib.h>

#define TAILLE_MAX 100
void welshPowell(int **matriceAdjacence, int nbStations, int *stationsExistantes);

int main() {
    //ouverture du fichier
    FILE *fichier = fopen("exclusion.txt", "r");

    if (fichier == NULL) {
        perror("erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    //Determination du nombre de stations et de contraintes
}
