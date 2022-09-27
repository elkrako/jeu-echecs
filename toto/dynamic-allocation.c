
//
// Ce fichier a pour but de montrer des cas où l'allocation dynamique est utile
// bien que la taille du tableau soit connue au moment de la compilation
//

#include <stdlib.h>
#include <stdio.h>

//
// Ce tableau est instancié sur le tas et non la pile.
// Au retour de la fonction, l'adresse est toujours valide. Le tas reste inchangé.
//
int *tableau_alloue_ok()
{
    int *tableau;

    tableau = malloc(150 * sizeof(int));

    for (int i = 0; i < 150; ++i)
    {
        tableau[i] = i;
    }

    return tableau;
}

void tableau_libere_ok(int *tableau)
{
    free(tableau);
}

//
// Ce tableau est instancié sur la pile lors de l'appel à la fonction.
// Cependant, dès le retour de la fonction, la pile ne contient plus le tableau car elle s'est déplacée.
// Tu as d'ailleurs un avertissement lors de la compilation.
// Utiliser ce retour pour accéder aux éléments est un comportement indéfini. Dans mon cas j'ai eu une erreur de segmentation.
//
int *tableau_alloue_ko()
{
    int tableau[150];

    for (int i = 0; i < 150; ++i)
    {
        tableau[i] = i;
    }

    return tableau;
}

//
// Ce tableau fait plus de 8Mo mais sur le tas on s'en fiche tant qu'il y a de la place
//
void tableau_utiliser_ok()
{
    int *tableau;

    tableau = malloc(5 * 1000 * 1000 * sizeof(int));

    for (int i = 0; i < 150; ++i)
    {
        tableau[i] = i;
    }

    free(tableau);
}

//
// Ce tableau fait plus de 8Mo, il est trop gros pour être conservé sur la pile
// telle qu'elle est configurée par défaut. Tu peux exécuter `ulimit -a | grep "stack size"` pour
// voir la valeur de cette limite.
//
void tableau_utiliser_ko()
{
    int tableau[5 * 1000 * 1000];

    for (int i = 0; i < 150; ++i)
    {
        tableau[i] = i;
    }
}

int main()
{
    int *tableau;

    tableau = tableau_alloue_ok();
    printf("tableau_alloue_ok:\n");
    for (int i = 0; i < 10; ++i)
    {
        printf("  - %d\n", tableau[i]);
    }
    tableau_libere_ok(tableau);

    tableau_utiliser_ok();
    printf("tableau_utiliser_ok\n");

    // Tu peux commenter cet appel si tu veux voir la suite du programme
    tableau_utiliser_ko();
    printf("tableau_utiliser_ko\n");

    tableau = tableau_alloue_ko();
    printf("Tableau KO:\n");
    for (int i = 0; i < 10; ++i)
    {
        printf("  - %d\n", tableau[i]);
    }

    return EXIT_SUCCESS;
}
