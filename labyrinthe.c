#include "interface_simulation_labyrinthe.h"
#include "navigation.h"
#include <stdio.h>

int main()
{
    Position depart, arrivee;
    Direction dir_initiale;

    // Initialisation du labyrinthe
    dir_initiale = initialiser_labyrinthe(SANS_BIFURCATION, &depart, &arrivee, 1);
    printf("Direction initiale à l'init = %d\n", dir_initiale);

    // Affichage des positions de départ et d'arrivée
    printf("Position de départ : (%d, %d)\n", depart.x, depart.y);
    printf("Position d'arrivée : (%d, %d)\n", arrivee.x, arrivee.y);

    // Résolution du labyrinthe
    resoudre_labyrinthe_sans_bifurcation(depart, arrivee, dir_initiale);

    return 0;
}
