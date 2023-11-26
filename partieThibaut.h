//
// Created by thiba on 26/11/2023.
//

#ifndef OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_TD8_TEAM99_PARTIETHIBAUT_H
#define OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_TD8_TEAM99_PARTIETHIBAUT_H


#include <stdio.h>
#include <stdlib.h>

// Structure d'un arc
typedef struct Arc
{
    int numSommet; // numero de sommet d'un arc adjacent au sommet initial.
    int valeur;
    struct t_arc* arc_suivant;
    int sommetInit;
    int marquer;
}t_arc;

typedef struct Sommet
{
    t_arc* arc;
    int valeur;
    int couleur;
}*t_sommet;

typedef struct Graphe
{
    int taille;
    int orientation;
    int ordre;
    t_sommet* t_sommet;
}t_graphe;

t_graphe* CreerGraphe(int ordre);
t_graphe* lire_graphe(char* nomFichier);
t_sommet* CreerArete(t_sommet* sommet, int sommet1, int sommet2, int poids);
void afficher_successeurs(t_sommet* sommet, int num);
void graphe_afficher(t_graphe* graphe);

#endif //OPTIMISATION_D_UNE_LIGNE_D_ASSEMBLAGE_ING2_TG_2023_2024_TD8_TEAM99_PARTIETHIBAUT_H
