#include "navigation.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Pour la fonction usleep()
#include <stdlib.h> // Pour system()

/* Fonction pour avancer d'une case dans une direction donnée
 *
 * Paramètres :
 *  - pos : Un pointeur vers la position actuelle de l'objet (struct Position).
 *  - dir : La direction dans laquelle l'objet doit avancer. Les valeurs possibles sont :
 *    - NORD : déplace l'objet vers le haut (augmentation de y).
 *    - SUD : déplace l'objet vers le bas (diminution de y).
 *    - EST : déplace l'objet vers la droite (augmentation de x).
 *    - OUEST : déplace l'objet vers la gauche (diminution de x).
 *
 * Effet : La fonction met à jour les coordonnées de l'objet en fonction de la direction donnée.
 *
 * Si le flag DEBUG est activé, la fonction affiche des messages de débogage concernant
 * l'avancée de l'objet.
 *
 * Exemple d'utilisation :
 * Position pos = {0, 0};
 * avancer(&pos, NORD); // Avance l'objet vers le haut, nouvelle position (0, 1)
 */
void avancer(Position *pos, Direction dir)
{
#ifdef DEBUG
    printf("[DEBUG] Avancer depuis (%d, %d) dans la direction : %d\n", pos->x, pos->y, dir);
#endif

    if (dir == NORD)
    {
        pos->y += 1;
    }
    else if (dir == SUD)
    {
        pos->y -= 1;
    }
    else if (dir == EST)
    {
        pos->x += 1;
    }
    else if (dir == OUEST)
    {
        pos->x -= 1;
    }

#ifdef DEBUG
    printf("[DEBUG] Nouvelle position après avancée : (%d, %d)\n", pos->x, pos->y);
#endif
}

/* Fonction pour déterminer la prochaine direction sans bifurcation
 *
 * Paramètres :
 *  - pos : Un pointeur vers la position actuelle de l'objet (struct Position).
 *  - dir_precedente : La direction précédente dans laquelle l'objet a avancé. Les valeurs possibles sont :
 *    - NORD : direction opposée au bas.
 *    - SUD : direction opposée au haut.
 *    - EST : direction opposée à l'ouest.
 *    - OUEST : direction opposée à l'est.
 *
 * Effet : La fonction détermine la prochaine direction possible sans bifurcation,
 *         en évitant de revenir en arrière (en excluant la direction opposée à la précédente).
 *         Elle parcourt les directions possibles et retourne celle qui est libre.
 *
 * Si le flag DEBUG est activé, la fonction affiche des messages de débogage concernant
 * la recherche de la direction valide, le test des directions et les directions bloquées.
 *
 * Valeur de retour : La direction valide trouvée (NORD, SUD, EST, OUEST), ou la direction précédente si aucune direction n'est possible.
 *
 * Exemple d'utilisation :
 * Position pos = {2, 3};
 * Direction dir = determiner_prochaine_direction(&pos, NORD); // Retourne la prochaine direction valide après NORD
 */
Direction determiner_prochaine_direction(Position *pos, Direction dir_precedente)
{
    Direction directions[] = {NORD, SUD, EST, OUEST}; // NORD, SUD, EST, OUEST
    Position temp;

    // Calcul de la direction opposée
    Direction dir_opposee;
    if (dir_precedente == NORD)
    {
        dir_opposee = SUD;
    }
    else if (dir_precedente == SUD)
    {
        dir_opposee = NORD;
    }
    else if (dir_precedente == EST)
    {
        dir_opposee = OUEST;
    }
    else
    { // dir_precedente == OUEST
        dir_opposee = EST;
    }

#ifdef DEBUG
    printf("[DEBUG] Position actuelle : (%d, %d)\n", pos->x, pos->y);
    printf("[DEBUG] Direction précédente : %d\n", dir_precedente);
#endif

    for (int i = 0; i < 4; i++)
    {
#ifdef DEBUG
        if (directions[i] == dir_opposee)
        {
            printf("[DEBUG] Ignorer direction opposée : %d\n", directions[i]);
        }
#endif
        if (directions[i] == dir_opposee)
        {
            continue;
        }

#ifdef DEBUG
        printf("[DEBUG] Tester direction : %d\n", directions[i]);
#endif
        if (est_voie_libre(pos, directions[i]))
        {
#ifdef DEBUG
            printf("[DEBUG] Voie libre trouvée dans la direction : %d\n", directions[i]);
#endif
            afficher_avancer_labyrinthe(pos, directions[i]);

            // Effacer l'écran avant d'afficher le labyrinthe
            system("clear");

            // Temporisation (0.5 secondes)
            // usleep(500000); // 500000 microsecondes = 0.5 secondes

            return directions[i];
        }
        else
        {
#ifdef DEBUG
            printf("[DEBUG] Direction bloquée : %d\n", directions[i]);
#endif
        }
    }

#ifdef DEBUG
    printf("[DEBUG] Aucune direction valide trouvée, retour à la précédente : %d\n", dir_precedente);
#endif
    return dir_precedente;
}

/* Fonction principale pour résoudre le labyrinthe sans bifurcation
 *
 * Paramètres :
 *  - depart : La position de départ dans le labyrinthe (struct Position).
 *  - arrivee : La position d'arrivée dans le labyrinthe (struct Position).
 *  - dir_initiale : La direction initiale dans laquelle l'objet commence à avancer.
 *                   Les valeurs possibles sont : NORD, SUD, EST, OUEST.
 *
 * Effet : La fonction résout le labyrinthe en avançant à partir du point de départ vers
 *         l'arrivée sans bifurcation, en suivant une seule direction possible à chaque
 *         étape et en évitant de revenir sur ses pas.
 *
 * La fonction utilise la fonction `determiner_prochaine_direction` pour déterminer
 * la direction suivante à chaque étape, puis met à jour la position de l'objet avec la fonction `avancer`.
 * Elle s'arrête lorsque la position actuelle est égale à la position d'arrivée.
 *
 * Si le flag DEBUG est activé, la fonction affiche des messages de débogage concernant
 * la position actuelle, la direction choisie, et les étapes du chemin parcouru.
 *
 * Exemple d'utilisation :
 * Position depart = {1, 1};
 * Position arrivee = {5, 5};
 * Direction dir_initiale = NORD;
 * resoudre_labyrinthe_sans_bifurcation(depart, arrivee, dir_initiale); // Résout le labyrinthe depuis (1, 1) vers (5, 5)
 */
void resoudre_labyrinthe_sans_bifurcation(Position depart, Position arrivee, Direction dir_initiale)
{
    Position actuelle = depart;
    Direction direction = dir_initiale;

#ifdef DEBUG
    printf("[DEBUG] ** Point de départ ** \n");
    printf("[DEBUG] Position actuelle : (%d, %d)\n", actuelle.x, actuelle.y);
    printf("[DEBUG] Direction : %d\n", dir_initiale);
#endif

    while (!(actuelle.x == arrivee.x && actuelle.y == arrivee.y))
    {
#ifdef DEBUG
        printf("[DEBUG] Dans la boucle while\n");
        printf("[DEBUG] Nouvelle position après affichage : (%d, %d)\n", actuelle.x, actuelle.y);
#endif

        // Déterminer la prochaine direction
        direction = determiner_prochaine_direction(&actuelle, direction);
        avancer(&actuelle, direction);

#ifdef DEBUG
        printf("[DEBUG] Position : (%d, %d), %d\n", actuelle.x, actuelle.y, direction);
#endif
    }

#ifdef DEBUG
    if (actuelle.x == arrivee.x && actuelle.y == arrivee.y)
    {
        printf("[DEBUG] Arrivée atteinte en (%d, %d)\n", arrivee.x, arrivee.y);
    }
    else
    {
        printf("[DEBUG] Impossible d'atteindre l'arrivée (%d, %d) depuis (%d, %d)\n", arrivee.x, arrivee.y, actuelle.x, actuelle.y);
    }
#endif
}
