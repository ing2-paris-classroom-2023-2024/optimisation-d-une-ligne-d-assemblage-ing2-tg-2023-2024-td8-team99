#include <stdio.h>
#include <stdlib.h>

#define TAILLE_MAX 100

typedef struct {
    int numeroSommet;
    float tempsExecution;
    int degre_entrant;
    int numero_couleur;
    int traite;
} Sommet;
typedef struct {
    float poidsMax; // Poids maximum de la machine (temps de cycle)
    float poidsActuel; // Poids actuel de la machine
    int* contenu; // Tableau de sommets contenus dans la machine
    int nbSommets_machine;
    int numero_cycle;
    // Nombre de sommets actuellement dans la machine
} Machine;


// Structure pour stocker les paires de sommets
typedef struct {
    int sommet1;
    int sommet2;
} PairedSommets;


// Fonction pour lire le fichier et créer la matrice de précédence
void traiterFichier(int **triTopologiqueResult);

// Fonction pour le tri topologique
void triTopologique(int **matricePrecedence, int nombreSommets, int *triTopologiqueResult);
// Fonction de parcours en profondeur pour le tri topologique
void dfs(int sommet, int **matricePrecedence, int *visited, int nombreSommets, int *triTopologiqueResult, int *currentIndex);

void traiterFichier(int **triTopologiqueResult) {
    FILE *fichier;
    fichier = fopen("precedences.txt", "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int maxSommet = 0;
    PairedSommets paire;
    while (fscanf(fichier, "%d %d", &paire.sommet1, &paire.sommet2) == 2) {
        if (paire.sommet1 > maxSommet) maxSommet = paire.sommet1;
        if (paire.sommet2 > maxSommet) maxSommet = paire.sommet2;
    }

    int nombreSommets = maxSommet;

    int **matricePrecedence = (int **)malloc(nombreSommets * sizeof(int *));
    for (int i = 0; i < nombreSommets; ++i) {
        matricePrecedence[i] = (int *)calloc(nombreSommets, sizeof(int));
    }

    rewind(fichier);

    while (fscanf(fichier, "%d %d", &paire.sommet1, &paire.sommet2) == 2) {
        matricePrecedence[paire.sommet1 - 1][paire.sommet2 - 1] = 1;
    }

    if (!feof(fichier)) {
        printf("Erreur de lecture du fichier.\n");
    } else {
        // Affichage de la matrice de précédence
        /* printf("Matrice de précédence :\n");
          for (int i = 0; i < nombreSommets; ++i) {
              for (int j = 0; j < nombreSommets; ++j) {
                  printf("%d ", matricePrecedence[i][j]);
              }
              printf("\n");
          } */

        // Tri topologique
        *triTopologiqueResult = malloc(nombreSommets * sizeof(int));
        triTopologique(matricePrecedence, nombreSommets, *triTopologiqueResult);

        // Affichage du résultat du tri topologique
        printf("Ordre des operations :\n");
        for (int i = 0; i < nombreSommets; ++i) {
            if ((*triTopologiqueResult)[i] != 0) {
                printf("%d ", (*triTopologiqueResult)[i]);
            }
        }
        printf("\n");
    }

    for (int i = 0; i < nombreSommets; ++i) {
        free(matricePrecedence[i]);
    }
    free(matricePrecedence);

    fclose(fichier);
}

void triTopologique(int **matricePrecedence, int nombreSommets, int *triTopologiqueResult) {
    int *visited = (int *)calloc(nombreSommets, sizeof(int));
    int *sommetExiste = (int *)calloc(nombreSommets, sizeof(int));
    int currentIndex = nombreSommets - 1;

    // Marquer les sommets qui existent dans le fichier
    for (int i = 0; i < nombreSommets; ++i) {
        for (int j = 0; j < nombreSommets; ++j) {
            if (matricePrecedence[i][j] == 1) {
                sommetExiste[i] = 1;
                sommetExiste[j] = 1;
            }
        }
    }

    for (int i = 0; i < nombreSommets; ++i) {
        if (!visited[i] && sommetExiste[i]) {
            dfs(i, matricePrecedence, visited, nombreSommets, triTopologiqueResult, &currentIndex);
        }
    }

    free(visited);
    free(sommetExiste);
}

void dfs(int sommet, int **matricePrecedence, int *visited, int nombreSommets, int *triTopologiqueResult, int *currentIndex) {
    visited[sommet] = 1;

    for (int i = 0; i < nombreSommets; ++i) {
        if (matricePrecedence[sommet][i] == 1 && !visited[i]) {
            dfs(i, matricePrecedence, visited, nombreSommets, triTopologiqueResult, currentIndex);
        }
    }

    triTopologiqueResult[*currentIndex] = sommet + 1;
    (*currentIndex)--;
}


int lireChiffreDansFichier() {
    FILE *f = fopen("temps_cycle.txt", "r");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return -1; // Code d'erreur
    }

    int chiffre = 0;
    if (fscanf(f, "%d", &chiffre) != 1) {
        printf("Erreur lors de la lecture du chiffre dans le fichier.\n");
        fclose(f);
        return -1; // Code d'erreur
    }

    fclose(f);
    return chiffre; // Retourne le nombre lu
}


void welshPowell(int **matriceAdjacence, int nbStations, int *stationsExistantes, Sommet *tableauSommets, int *nombreCouleursUtilisees) {
    int *couleur = (int *)malloc((nbStations + 1) * sizeof(int));
    int *degre = (int *)malloc((nbStations + 1) * sizeof(int));
    int *ordre = (int *)malloc((nbStations + 1) * sizeof(int));
    int nbCouleurs = 0;

    // Initialisation des tableaux
    for (int i = 1; i <= nbStations; i++) {
        couleur[i] = 0;
        degre[i] = 0;
        for (int j = 1; j <= nbStations; j++) {
            degre[i] += matriceAdjacence[i][j];
        }
        ordre[i] = i;
    }

    // Tri des stations par degré décroissant
    for (int i = 1; i <= nbStations; i++) {
        for (int j = i + 1; j <= nbStations; j++) {
            if (degre[i] < degre[j]) {
                int temp = degre[i];
                degre[i] = degre[j];
                degre[j] = temp;
                temp = ordre[i];
                ordre[i] = ordre[j];
                ordre[j] = temp;
            }
        }
    }

    // Algorithme de coloration de Welsh-Powell
    for (int i = 1; i <= nbStations; i++) {
        int station = ordre[i];

        // Ignorer les stations qui n'existent pas
        if (!stationsExistantes[station]) continue;

        int couleurActuelle = 1;

        while (1) {
            int possible = 1;
            for (int j = 1; j <= nbStations; j++) {
                if (matriceAdjacence[station][j] && tableauSommets[j].numero_couleur == couleurActuelle) {
                    possible = 0;
                    break;
                }
            }

            if (possible) {
                tableauSommets[station].numero_couleur = couleurActuelle;
                if (couleurActuelle > nbCouleurs) nbCouleurs = couleurActuelle;
                *nombreCouleursUtilisees=couleurActuelle;
                break;
            }

            couleurActuelle++;
        }
    }

    // Libération de la mémoire
    free(couleur);
    free(degre);
    free(ordre);
}
Sommet* traiterFichier_temps(int* nombre) {
    int nb = 0;
    FILE *fichier;
    fichier = fopen("operations.txt", "r");

    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        exit(1);
    }

    char ligne[20]; // Adapter la taille si nécessaire

    // Compter le nombre de lignes pour obtenir le nombre de sommets
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        nb++;
    }

    // Retourner au début du fichier pour le lire à nouveau
    rewind(fichier);
    // Allouer dynamiquement un tableau de sommets
    Sommet *tableauSommets = malloc(nb * sizeof(Sommet));
    if (tableauSommets == NULL) {
        printf("Erreur d'allocation pour le tableau de sommets.\n");
        exit(1);
    }
    int index = 0;
    // Lire le fichier et remplir les structures
    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        // Utiliser sscanf pour extraire les données du fichier
        int numeroSommet;
        float tempsExecution;
        sscanf(ligne, "%d %f", &numeroSommet, &tempsExecution);

        // Remplir le tableau avec les données extraites
        tableauSommets[index].numeroSommet = numeroSommet;
        tableauSommets[index].tempsExecution = tempsExecution;

        index++;
    }


    // Fermer le fichier après utilisation
    fclose(fichier);
    // Afficher chaque sommet et son temps d'exécution
    *nombre= nb;
    return  tableauSommets;
    // Libérer la mémoire allouée dynamiquement

}
Machine *creerMachines(float temp_machine,int nombreCouleurs,int nbcase_station) {

    Machine* machines = malloc(nombreCouleurs * sizeof(Machine));
    for (int i = 0; i <nombreCouleurs; ++i) {
        // Initialisation des machines avec un poids maximum (temps de cycle)
        /// Vous pouvez assigner le temps de cycle à partir du fichier temps_cycle.txt
        machines[i].poidsMax = temp_machine;
        machines[i].poidsActuel = 0;
        machines[i].contenu = malloc(nbcase_station * sizeof(int));
        if (machines[i].contenu == NULL)
        {
            printf("erreur allocation ");
            exit(0);
        }
        machines[i].nbSommets_machine = 0;
        machines[i].numero_cycle = 1;
    }
    return machines;
}


void afficherContenuMachines(Machine *machines, int index) {

    printf("Machine %d - cycle:%d Contenu:\n",index,machines[index].numero_cycle); //
    printf("Nombre de Sommet %d - Temps d'excution totale: %.2f\n", machines[index].nbSommets_machine, machines[index].poidsActuel);
    for (int j = 0; j < machines[index].nbSommets_machine; ++j) {
        printf("Sommet %d \n", machines[index].contenu[j]);
    }
    printf("\n");

}
void vider_M(Machine* M,int couleur){
    M[couleur].poidsActuel = 0;
    M[couleur].nbSommets_machine = 0;
    M[couleur].numero_cycle++;
}
void remplirMachines(Sommet *sommets, int nbSommets, Machine *Machine_remplir, float temps,int nbCouLeurs) {
    int sommetsTraites = 0;

    while (sommetsTraites < nbSommets) {
        for (int i = 0; i < nbSommets; i++) {
            if (sommets[i].traite == 0) {
                int couleur = sommets[i].numero_couleur;

                /* printf("Sommet %d avec couleur %d\n", sommets[i].numeroSommet, couleur);

                 // Les couleurs commencent à partir de 1, mais les indices de tableau commencent à partir de 0
                 if (couleur == 1) {
                     printf("Sommet %d attribué à la Machine 1\n", sommets[i].numeroSommet);
                 }
 */
                if (Machine_remplir[couleur].poidsActuel + sommets[i].tempsExecution <= temps) {
                    int N = Machine_remplir[couleur].nbSommets_machine;
                    Machine_remplir[couleur].contenu[N] = sommets[i].numeroSommet;
                    Machine_remplir[couleur].poidsActuel += sommets[i].tempsExecution;
                    Machine_remplir[couleur].nbSommets_machine++;
                    sommets[i].traite = 1;
                    sommetsTraites++;
                } else {
                    printf("La Machine numero %d est pleine au cycle %d.\n", couleur, Machine_remplir[couleur].numero_cycle);
                    afficherContenuMachines(Machine_remplir, couleur);
                    vider_M(Machine_remplir, couleur);
                }
            }
        }
    }
    for (int i = 0; i <nbCouLeurs; i++) {

        afficherContenuMachines(Machine_remplir,i);
    }
}

void precedence_et_temp( Sommet *sommet,int* ordre, float temps,int NBSOMMET){
    float Somme=0;
    int cycle = 1;
    printf("Cycle 1 :  \n");
    for(int i =0;i<NBSOMMET;i++){
        for(int X=0;X<NBSOMMET;X++){
            if(sommet[X].numeroSommet==ordre[i]){
                if((sommet[X].tempsExecution+Somme)<=temps){
                    printf("sommet %d->",sommet[X].numeroSommet);
                    Somme+=sommet[X].tempsExecution;
                }
                if ((sommet[X].tempsExecution+Somme)>=temps){
                    printf(" \n");
                    printf(" temps de %.2f\n\n",Somme);
                    cycle++;
                    printf("Cycle %d :  \n",cycle);
                    printf("sommet %d->",sommet[X].numeroSommet);

                    Somme=sommet[X].tempsExecution;
                }

            }
        }
    }
    printf(" temps de %.2f\n",Somme);

}
void project(){

    // Ouverture du fichier
    FILE *fichier = fopen("exclusions.txt", "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    // Détermination du nombre de stations et de contraintes
    int nb_sommet_wesh = 0;
    int nbContraintes = 0;
    int station1, station2;

    while (fscanf(fichier, "%d %d", &station1, &station2) == 2) {
        nbContraintes++;
        if (station1 > nb_sommet_wesh) nb_sommet_wesh = station1;
        if (station2 > nb_sommet_wesh) nb_sommet_wesh = station2;
    }

    fclose(fichier);

    // Allocation de la matrice d'adjacence
    int **matriceAdjacence = (int **)malloc((nb_sommet_wesh + 1) * sizeof(int *));
    for (int i = 0; i <= nb_sommet_wesh; i++) {
        matriceAdjacence[i] = (int *)calloc((nb_sommet_wesh + 1), sizeof(int));
    }

    // Remplissage de la matrice d'adjacence
    fichier = fopen("exclusions.txt", "r");
    while (fscanf(fichier, "%d %d", &station1, &station2) == 2) {
        matriceAdjacence[station1][station2] = 1;
        matriceAdjacence[station2][station1] = 1;
    }

    fclose(fichier);

    // Stations existantes dans le fichier
    int *stationsExistantes = (int *)calloc((nb_sommet_wesh + 1), sizeof(int));

    fichier = fopen("exclusions.txt", "r");
    while (fscanf(fichier, "%d %d", &station1, &station2) == 2) {
        stationsExistantes[station1] = 1;
        stationsExistantes[station2] = 1;
    }

    fclose(fichier);
    int T= lireChiffreDansFichier();

    float TempsCycle=(float)T;
    printf("temps cycle %.2f\n",TempsCycle);
    printf("\n");
    printf("\n");

    // Création et initialisation du tableau de structures Sommet
    Sommet *tableauSommets = (Sommet *)malloc((nb_sommet_wesh + 1) * sizeof(Sommet));
    for (int i = 1; i <= nb_sommet_wesh; i++) {
        tableauSommets[i].numeroSommet = i;
        tableauSommets[i].numero_couleur = 0; // Initialisez la couleur à zéro pour chaque sommet
        tableauSommets[i].traite = 0; // Par défaut, le sommet n'est pas traité
        // Initialisez d'autres champs selon vos besoins
    }

    // Exemple d'utilisation
    int nombreDeCouleurs = 0;
    welshPowell(matriceAdjacence, nb_sommet_wesh, stationsExistantes, tableauSommets, &nombreDeCouleurs);

    // Affichage des sommets avec leur couleur
    printf("Exlusions \n");
    for (int i = 1; i <= nb_sommet_wesh; i++) {
        if(tableauSommets[i].numero_couleur!=0) {
            printf("Sommet %d - Couleur: %d\n", tableauSommets[i].numeroSommet, tableauSommets[i].numero_couleur);
        }
    }
    printf("\n");
    printf("\n");
    int nb_sommet_temps;
    Sommet *Tableau_sommet_temps= traiterFichier_temps(&nb_sommet_temps);
    printf("Temps \n");
    for (int i = 0; i < nb_sommet_temps; i++) {
        printf("Sommet %d - Temps: %.2f\n", Tableau_sommet_temps[i].numeroSommet, Tableau_sommet_temps[i].tempsExecution);
    }
    printf("\n");
    printf("\n");
    int k = 0;
    printf("nombre sommet %d\n",nb_sommet_temps);
    printf("\n");
    printf("\n");
    for (int i = 0; i < nb_sommet_wesh; i++) {

        if (Tableau_sommet_temps[k].numeroSommet == tableauSommets[i].numeroSommet) {
            Tableau_sommet_temps[k].numero_couleur = tableauSommets[i].numero_couleur;
            k++;
            if( k == nb_sommet_temps) {
                break;
            }
        }
    }
    printf("Numero de sommet , temps et numero de station:\n\n");
    for (int i = 0; i < nb_sommet_temps; i++) {
        //  if(Tableau_sommet_temps[i].numero_couleur!=0) {
        printf("Sommet %d - Couleur: %d - Temps: %.2f\n", Tableau_sommet_temps[i].numeroSommet,
               Tableau_sommet_temps[i].numero_couleur, Tableau_sommet_temps[i].tempsExecution);
        //  }
    }
    printf("\n");
    printf("\n");
    /* for (int i = 0; i < nb_sommet_temps; i++) {
         printf("sommet traite %d",Tableau_sommet_temps[i].traite);
     }
     */
    nombreDeCouleurs++;
    printf("Contrainte Exclusions et temps V1 \n");
    printf("Nombre de station differente %d\n",nombreDeCouleurs);
    printf("\n");
    for(int w =0;w <nombreDeCouleurs;w++){
        float Somme =0;
        printf(" Station %d\n",w);
        int C=1;
        printf(" Cycle %d\n",C);
        for(int l=0;l<=nb_sommet_temps;l++){
            if(Tableau_sommet_temps[l].numero_couleur==w){
                //printf("->");
                if((Somme + Tableau_sommet_temps[l].tempsExecution) <= TempsCycle){

                    printf(" operation %d->",Tableau_sommet_temps[l].numeroSommet);
                }
                if((Somme + Tableau_sommet_temps[l].tempsExecution) >= TempsCycle) {
                    C++;
                    printf("\n");
                    printf(" Temps du cycle  %.2f\n",Somme);
                    printf(" Cycle %d\n", C);

                    printf(" operation %d->",Tableau_sommet_temps[l].numeroSommet);
                    Somme = 0;
                }
                Somme += Tableau_sommet_temps[l].tempsExecution;
            }
        }
        printf("\n");
        printf(" Temps du cycle  %.2f\n",Somme);
        printf("\n");
        printf("\n");


    }
    float Somme=0;
    int C=1;
    printf("Contraite temps \n");
    printf("\n");

    printf(" Cycle %d\n", C);
    printf("\n");
    for(int l=0;l<nb_sommet_temps;l++){
        //printf("->");
        if((Somme + Tableau_sommet_temps[l].tempsExecution) <= TempsCycle){


            printf(" operation %d\n",Tableau_sommet_temps[l].numeroSommet);

        }
        if((Somme + Tableau_sommet_temps[l].tempsExecution) >= TempsCycle) {
            C++;

            printf(" Temps du cycle  %.2f\n",Somme);
            printf("\n");
            printf(" Cycle %d\n", C);
            printf("\n");
            printf(" operation %d\n",Tableau_sommet_temps[l].numeroSommet);
            Somme = 0;
        }
        Somme += Tableau_sommet_temps[l].tempsExecution;
    }
    int *triTopologiqueResult;
    printf("\n \n");
    printf("Precedence  \n");
    traiterFichier(&triTopologiqueResult);
    printf("  \n");


    precedence_et_temp(Tableau_sommet_temps,triTopologiqueResult,TempsCycle,nb_sommet_temps);
    printf(" Temps du cycle  %.2f\n",Somme);
    printf("\n");
    printf("\n");

    printf("exclusions et Temps  version  finale\n");
    Machine *Station_Couleur= creerMachines(TempsCycle, nombreDeCouleurs, nb_sommet_temps);
    remplirMachines(Tableau_sommet_temps, nb_sommet_temps, Station_Couleur,TempsCycle,nombreDeCouleurs);

    // Libération de la mémoire
    free(tableauSommets);
    free(Tableau_sommet_temps);
    for (int i = 0; i <= nombreDeCouleurs; i++) {
        free(Station_Couleur[i].contenu);
    }
    free(Station_Couleur);
    for(int i = 0; i <= nb_sommet_wesh; i++) {
        free(matriceAdjacence[i]);
    }
    free(matriceAdjacence);
    free(stationsExistantes);
    free(triTopologiqueResult);
}

int main() {

    project();

    return 0;
}


