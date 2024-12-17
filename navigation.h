#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "interface_simulation_labyrinthe.h"
#include <stdio.h>
#include <stdlib.h>

// Fonction pour avancer d'une case dans une direction donnée
void avancer(Position *pos, Direction dir);

// Fonction pour déterminer la prochaine direction sans bifurcation
Direction determiner_prochaine_direction(Position *pos, Direction dir_precedente);

// Fonction principale pour résoudre le labyrinthe sans bifurcation
void resoudre_labyrinthe_sans_bifurcation(Position depart, Position arrivee, Direction dir_initiale);

#endif
