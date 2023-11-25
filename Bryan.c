//
// Created by Romaric on 20/11/2023.
//
#include <stdio.h>
#include <stdlib.h>

struct Noeud {
    int dest;
    struct Noeud* suivant;
};

struct Graphe {
    int V;
    struct Noeud** listesAdj;
    int* degreEntrant;
};

struct Noeud* creerNoeud(int dest) {
    struct Noeud* nouveauNoeud = (struct Noeud*)malloc(sizeof(struct Noeud));
    nouveauNoeud->dest = dest;
    nouveauNoeud->suivant = NULL;
    return nouveauNoeud;
}

struct Graphe* creerGraphe(int V) {
    struct Graphe* graphe = (struct Graphe*)malloc(sizeof(struct Graphe));
    graphe->V = V;
    graphe->listesAdj = (struct Noeud**)malloc(V * sizeof(struct Noeud*));
    graphe->degreEntrant = (int*)malloc(V * sizeof(int));

    for (int i = 0; i < V; i++) {
        graphe->listesAdj[i] = NULL;
        graphe->degreEntrant[i] = 0;
    }

    return graphe;
}
void ajouterContraintePrecedence(struct Graphe* graphe, int src, int dest) {
    struct Noeud* nouveauNoeud = creerNoeud(src);
    nouveauNoeud->suivant = graphe->listesAdj[dest];
    graphe->listesAdj[dest] = nouveauNoeud;

    graphe->degreEntrant[src]++;
}
void triTopologique(struct Graphe* graphe, int* tableauResultats) {
    int* resultat = (int*)malloc(graphe->V * sizeof(int));
    for (int i = 0; i < graphe->V; i++) {
        resultat[i] = -1;
    }
    int indexResultat = 0;

    int* file = (int*)malloc(graphe->V * sizeof(int));
    int debutFile = 0, finFile = -1;

    for (int i = 0; i < graphe->V; i++) {
        if (graphe->degreEntrant[i] == 0) {
            file[++finFile] = i;
        }
    }

    while (debutFile <= finFile) {
        int u = file[debutFile++];
        resultat[indexResultat++] = u;

        struct Noeud* temp = graphe->listesAdj[u];
        while (temp != NULL) {
            if (--graphe->degreEntrant[temp->dest] == 0) {
                file[++finFile] = temp->dest;
            }
            temp = temp->suivant;
        }
    }

    if (indexResultat != graphe->V) {
        printf("Le graphe contient un cycle, le tri topologique n'est pas possible.\n");
        free(resultat);
        free(file);
        return;
    }

    for (int i = 0, j = 0; i < graphe->V; i++) {
        if (resultat[i] != -1) {
            tableauResultats[j++] = resultat[i];
        }
    }

    free(resultat);
    free(file);
}
void inverserTableau(int tableau[], int taille) {
    int debut = 0;
    int fin = taille - 1;

    while (debut < fin) {
        int temp = tableau[debut];
        tableau[debut] = tableau[fin];
        tableau[fin] = temp;

        debut++;
        fin--;
    }
}

