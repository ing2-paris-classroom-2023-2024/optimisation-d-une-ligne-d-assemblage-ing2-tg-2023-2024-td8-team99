//
// Created by thiba on 20/11/2023.
//

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

//Creation d'aretes
t_sommet* CreerArete(t_sommet* sommet, int sommet1, int sommet2, int valeur)
{
    if(sommet[sommet1]->arc == NULL)
    {
        t_arc* nouveauArc=(t_arc*)malloc(sizeof(t_arc));
        nouveauArc->numSommet=sommet2;
        nouveauArc->sommetInit=sommet1;
        nouveauArc->arc_suivant=NULL;
        nouveauArc->valeur=valeur;
        sommet[sommet1]->arc=nouveauArc;
        return sommet;
    }

    else
    {
        t_arc* temp=sommet[sommet1]->arc;
        while( !(temp->arc_suivant==NULL))
        {
            temp=temp->arc_suivant;
        }
        t_arc* nouveauArc=(t_arc*)malloc(sizeof(t_arc));
        nouveauArc->numSommet=sommet2;
        nouveauArc->sommetInit=sommet1;
        nouveauArc->arc_suivant=NULL;
        nouveauArc->valeur=valeur;
        temp->arc_suivant=nouveauArc;
        return sommet;
    }
}

//Creation graphe.
t_graphe* CreerGraphe(int ordre)
{
    t_graphe* nouveauGraphe=(t_graphe*)malloc(sizeof(t_graphe));
    nouveauGraphe->t_sommet = (t_sommet*)malloc(ordre * sizeof(t_sommet));
    for(int i=0; i<ordre; i++)
    {
        nouveauGraphe->t_sommet[i]=(t_sommet)malloc(sizeof(struct Sommet));
        nouveauGraphe->t_sommet[i]->valeur=i;
        nouveauGraphe->t_sommet[i]->arc=NULL;
    }
    return nouveauGraphe;
}

//Lecture graphe a partir de fichier texte.
// Entree : Nom du fichier
// Sortie : Graphe
//Fichier avec : ordre, taille,orientation et liste des arcs.
t_graphe * lire_graphe(char * nomFichier)
{
    t_graphe* graphe;
    FILE* fs = fopen(nomFichier,"r");
    int taille, orientation, ordre, s1, s2,valeur;
    if (!fs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }
    fscanf(fs,"%d",&ordre);
    graphe=CreerGraphe(ordre);
    fscanf(fs,"%d",&taille);
    fscanf(fs,"%d",&orientation);
    graphe->orientation=orientation;
    graphe->ordre=ordre;
    graphe->taille = taille;

    for (int i=0; i<taille; ++i)
    {
        fscanf(fs,"%d%d%d",&s1,&s2,&valeur);
        graphe->t_sommet=CreerArete(graphe->t_sommet, s1, s2,valeur);
        if(!orientation)
            graphe->t_sommet=CreerArete(graphe->t_sommet, s2, s1,valeur);
    }
    return graphe;
}

// Affichage du graphe.
void graphe_afficher(t_graphe* graphe)
{
    if(graphe->orientation)
    {
        printf("Graphe Oriente\n");
    }
    else
    {
        printf("Graphe Non Oriente\n");
    }
    printf("Ordre = %d\n",graphe->ordre);
    printf("Taille : %d\n",graphe->taille);
    printf("Tableau Aretes :\n");
    for (int i=0; i<graphe->ordre; i++)
    {
        afficher_successeurs(graphe->t_sommet, i);
        printf("\n");
    }
}

int main() {
    t_graphe *graphe = lire_graphe("contraintes_exclusion.txt");
    int i = 0;
    int j = 0;
    int sommetInitial = 0;
    int quitter = 0;
    int choix = 0;
    int *numdeCC = (int *) malloc(graphe->ordre * sizeof(int));
    struct Arc **tabAdj = (struct Arc **) malloc(graphe->taille * sizeof(struct Arc *));
    struct Arc *temparc = NULL;
    while (j != graphe->taille) {
        temparc = graphe->t_sommet[i]->arc;
        while (temparc != NULL) {
            if (graphe->t_sommet[temparc->numSommet]->couleur == 0) {
                tabAdj[j] = temparc;
                tabAdj[j]->sommetInit = i;
                j++;
            }
            temparc = temparc->arc_suivant;
        }
        graphe->t_sommet[i]->couleur = 1;
        i++; //Passage au sommet suivant
    }
    for (i = 0; i < graphe->taille; i++) {
        tabAdj[i]->marquer = 0;
    }


    // MENU //
    while (quitter == 0) {
        printf("1 : Afficher graphe\n 2 : Quitter\n\nVotre choix : ");
        scanf("%d", &choix);
        if (choix == 1) {
            printf("\n");
            graphe_afficher(graphe);

        } else if (choix == 2) {
            quitter = 1;
        }
    }
    return 0;
}
