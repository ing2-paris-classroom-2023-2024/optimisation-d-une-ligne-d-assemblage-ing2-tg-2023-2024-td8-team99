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

