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
    int nbStations = 0;
    int nbContraintes = 0;
    int station1 , station2;

    while (fscanf(fichier, "%d %d", &station1, &station2)== 2) {
        nbContraintes++;
        if (station1 > nbStations) nbStations = station1;
        if (station2 > nbStations) nbStations = station2;
    }

    fclose(fichier);

    //Allocation de la matrice d'adjacence
    int**matriceAdjacence = (int**) malloc((nbStations + 1) * sizeof(int *));
    for (int i = 0; i<= nbStations;i++ ) {
        matriceAdjacence[i] = (int*) calloc((nbStations + 1), sizeof (int));
    }
    // Remplissage de la matrice d'adjacence
    fichier = fopen("exclusions.txt", "r");
    while (fscanf(fichier, "%d %d", &station1, &station2) == 2) {
        matriceAdjacence[station1][station2] = 1;
        matriceAdjacence[station2][station1] = 1;
    }
    fclose(fichier);

    // Stations existantes dans le fichier
    int *stationsExistantes = (int *)calloc((nbStations + 1), sizeof(int));

    fichier = fopen("exclusions.txt", "r");
    while (fscanf(fichier, "%d %d", &station1, &station2) == 2) {
        stationsExistantes[station1] = 1;
        stationsExistantes[station2] = 1;
    }

    fclose(fichier);

    // Exemple d'utilisation
    welshPowell(matriceAdjacence, nbStations, stationsExistantes);

    // Libération de la mémoire
    for (int i = 0; i <= nbStations; i++) {
        free(matriceAdjacence[i]);
    }
    free(matriceAdjacence);
    free(stationsExistantes);

    return 0;
}
