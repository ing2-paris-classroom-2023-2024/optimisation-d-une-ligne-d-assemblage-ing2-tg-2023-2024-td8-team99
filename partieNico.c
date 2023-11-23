#include <stdio.h>
#include <stdlib.h>

// Structure représentant une paire d'opérations interdites ensemble
typedef struct {
    int operation1;
    int operation2;
} ExclusionConstraint;

// Fonction pour vérifier si une paire d'opérations est interdite ensemble
int isExcluded(const ExclusionConstraint exclusions[], int numExclusions, int op1, int op2) {
    for (int i = 0; i < numExclusions; ++i) {
        if ((exclusions[i].operation1 == op1 && exclusions[i].operation2 == op2) ||
            (exclusions[i].operation1 == op2 && exclusions[i].operation2 == op1)) {
            return 1; // Les opérations sont interdites ensemble
        }
    }
    return 0; // Les opérations ne sont pas interdites ensemble
}