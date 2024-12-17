Pour implémenter une compilation conditionnelle afin que les messages de débogage s'affichent uniquement lorsqu'un flag `DEBUG` est défini, il faut modifier votre code et la ligne de compilation. Voici comment procéder :

### 1. Dans le code : Utilisation de macros conditionnelles

Modifiez le code pour entourer les sections de débogage avec une directive conditionnelle `#ifdef DEBUG`. Cela signifie que les messages de débogage ne seront compilés que si le flag `DEBUG` est défini.

Modifie le code comme suit :

```c
#include "chat1.h"
#include <stdio.h>
#include <stdlib.h>

// Fonction pour avancer d'une case dans une direction donnée
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

// Fonction pour déterminer la prochaine direction sans bifurcation
Direction determiner_prochaine_direction(Position *pos, Direction dir_precedente)
{
    Direction directions[] = {NORD, SUD, EST, OUEST}; // NORD, SUD, EST, OUEST
    Position temp;

    // Calcul de la direction opposée
    Direction dir_opposite;
    if (dir_precedente == NORD)
    {
        dir_opposite = SUD;
    }
    else if (dir_precedente == SUD)
    {
        dir_opposite = NORD;
    }
    else if (dir_precedente == EST)
    {
        dir_opposite = OUEST;
    }
    else
    { // dir_precedente == OUEST
        dir_opposite = EST;
    }

#ifdef DEBUG
    printf("[DEBUG] Position actuelle : (%d, %d)\n", pos->x, pos->y);
    printf("[DEBUG] Direction précédente : %d\n", dir_precedente);
#endif

    for (int i = 0; i < 4; i++)
    {
#ifdef DEBUG
        if (directions[i] == dir_opposite)
        {
            printf("[DEBUG] Ignorer direction opposée : %d\n", directions[i]);
        }
#endif
        if (directions[i] == dir_opposite)
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

// Fonction principale pour résoudre le labyrinthe sans bifurcation
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
        printf("[DEUBG] Impossible d'atteindre l'arrivée (%d, %d) depuis (%d, %d)\n", arrivee.x, arrivee.y, actuelle.x, actuelle.y);
    }
#endif
}
```

### 2. Dans la ligne de compilation : Définir le flag `DEBUG`

Lors de la compilation, tu peux définir une macro `DEBUG` en utilisant l'option `-D` de `gcc`. Cela activera toutes les sections de code entourées par `#ifdef DEBUG`.

Voici la commande de compilation avec le flag `DEBUG` :

```bash
gcc -DDEBUG mainchat.c -lsimulation_labyrinthe_linux -L. -o mainchat
```

Cette commande définira la macro `DEBUG` pour que les sections de débogage soient incluses dans la compilation. Si tu ne veux pas que les messages de débogage soient affichés, il te suffit de ne pas ajouter le flag `-DDEBUG` lors de la compilation :

```bash
gcc mainchat.c -lsimulation_labyrinthe_linux -L. -o mainchat
```

### Explication des changements :

- **`#ifdef DEBUG`** : Ce préprocesseur va vérifier si la macro `DEBUG` est définie avant d'inclure le code qui suit. Si le flag `DEBUG` est passé à `gcc` lors de la compilation, le code entre `#ifdef DEBUG` et `#endif` sera inclus.
- **`-DDEBUG`** : Ce flag indique à `gcc` de définir la macro `DEBUG` pendant la compilation.
