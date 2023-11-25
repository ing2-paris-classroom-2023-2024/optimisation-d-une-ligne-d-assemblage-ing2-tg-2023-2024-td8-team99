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