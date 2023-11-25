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