//
// Created by thiba on 20/11/2023.
//
//test
#include "partieThibaut.h"

// Affichage des successeurs de chaque sommet
void afficher_successeurs(t_sommet* sommet, int num)
{
    printf(" sommet %d :\n",num); //Affichage d un sommet
    t_arc* arc=sommet[num]->arc;
    while(arc!=NULL) //Permet de parcourir
    {
        printf("/%d/ ",arc->numSommet); // Affichage du successeur du sommet
        printf("distance: %d\n", arc->valeur); //Affichage du poids
        arc=arc->arc_suivant;
    }
}