
//
// Exemples d'utilisation des structures et de tableaux pouvant les contenir
//

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Nombres de cacas que je vais initialiser dans le tableau
// tu peux mettre presque ce que tu veux sauf 0 ou des valeurs plus
// grandes que 650 000 ou 700 000 sinon la pile va exploser
static const size_t NOMBRE_CACA = 5;

//
// La structure d'un caca, permet de conserver ses caractéristiques
//
struct caca
{
    bool dur;              // Mon caca est dur ou mou ?
    unsigned int masse;    // La masse de mon caca, au moins en kilogrammes
    unsigned int longueur; // La taille de mon caca, au moins en mètres
};

//
// Fonction qui fait caca dans le pointeur que tu lui donnes
//
static void faire_caca(struct caca *ptr, size_t index)
{
    ptr->dur = rand() % 2 == 0 ? true : false;
    ptr->masse = rand();
    ptr->longueur = rand();

    printf("    caca n°%04zu : adresse=0x%p dur=%s masse=%010ukg longueur=%010um\n", index, ptr, ptr->dur ? "vrai" : "faux", ptr->masse, ptr->longueur);
}

//
// Suite d'exemples d'utilisation avec des tableaux statiques ou dynamiques
// et des cacas statiques ou dynamiques
//
// Chaque exemple, a un titre, renseigne des cacas en les affichant et vérifie ensuite
// l'accès au premier caca. On le voit bien sur la sortie avec un décalage sur la droite
// lorsqu'on renseigne les cacas
//
int main()
{
    // Au minimum 1 caca, faut bien chier quoi !
    assert(NOMBRE_CACA > 1);

    printf("Tableau statique avec des cacas :\n");
    {
        struct caca tableau[NOMBRE_CACA];

        // On peut directement initialiser les cacas en donnant chacune des adresses où ils sont
        for (size_t i = 0; i < NOMBRE_CACA; i++)
            faire_caca(&tableau[i], i);

        // On peut aussi directement accéder aux cacas, tu peux vérifier que c'est la même valeur
        printf("caca n°%04zu : dur=%s masse=%010ukg longueur=%010um\n", 0ul, tableau[0].dur ? "vrai" : "faux", tableau[0].masse, tableau[0].longueur);
    }
    printf("\n");

    printf("Tableau dynamique avec des cacas :\n");
    {
        struct caca *tableau;

        // On doit d'abord réserver une zone mémoire continue qui contienne des cacas
        // dans le cas précédent, cette zone était allouée toute seule sur la pile
        // dans ce cas, c'est à nous de l'allouer sur le tas
        tableau = malloc(NOMBRE_CACA * sizeof(struct caca));

        // On peut ensuite initialiser les cacas exactement comme précédemment
        for (size_t i = 0; i < NOMBRE_CACA; i++)
            faire_caca(&tableau[i], i);

        // On peut aussi directement accéder aux cacas, tu peux vérifier que c'est la même valeur
        printf("caca n°%04zu : dur=%s masse=%010ukg longueur=%010um\n", 0ul, tableau[0].dur ? "vrai" : "faux", tableau[0].masse, tableau[0].longueur);

        // On doit maintenant libérer le tableau car on ne l'utilisera plus
        free(tableau);
    }
    printf("\n");

    printf("Tableau statique avec des pointeurs vers des cacas :\n");
    {
        struct caca *tableau[NOMBRE_CACA];

        // Dans ce cas on a un tableau de 15 pointeurs, mais ils ne pointent vers rien
        // il faut donc renseigner ces pointeurs avec une adresse vers une zone qui contient
        // assez d'espace pour stocker un caca
        for (size_t i = 0; i < NOMBRE_CACA; i++)
            tableau[i] = malloc(sizeof(struct caca));

        // On peut ensuite initialiser les cacas en donnant les pointeurs qu'on vient de créer
        // tu noteras la différence avec précédemment, ici on n'utilise pas l'opérateur '&' pour
        // récupérer l'adresse de l'objet, l'objet est directement un pointeur
        for (size_t i = 0; i < NOMBRE_CACA; i++)
            faire_caca(tableau[i], i);

        // On peut aussi directement accéder aux cacas, tu peux vérifier que c'est la même valeur
        // mais dans ce cas, on est obligé d'utiliser une flèche !
        printf("caca n°%04zu : dur=%s masse=%010ukg longueur=%010um\n", 0ul, tableau[0]->dur ? "vrai" : "faux", tableau[0]->masse, tableau[0]->longueur);

        // On doit maintenant libérer tous les pointeurs qu'on a alloués dans le tableau car on ne les utilisera plus
        for (size_t i = 0; i < NOMBRE_CACA; i++)
            free(tableau[i]);
    }
    printf("\n");

    printf("Tableau dynamique avec des pointeurs vers des cacas :\n");
    {
        struct caca **tableau;

        // Dans ce cas, on doit d'abord réserver la zone mémoire qui va contenir les pointeurs
        // qui eux-mêmes iront pointer vers les cacas
        tableau = malloc(NOMBRE_CACA * sizeof(struct caca *));

        // Maintenant, on est comme dans le cas précédent, on a de l'espace pour les pointeurs
        // mais ils ne pointent vers rien. Il faut donc renseigner ces pointeurs avec une adresse
        // vers une zone qui contient assez d'espace pour stocker un caca
        for (size_t i = 0; i < NOMBRE_CACA; i++)
            tableau[i] = malloc(sizeof(struct caca));

        // Ensuite c'est exactement pareil qu'avant
        for (size_t i = 0; i < NOMBRE_CACA; i++)
            faire_caca(tableau[i], i);

        // On peut aussi directement accéder aux cacas, tu peux vérifier que c'est la même valeur
        printf("caca n°%04zu : dur=%s masse=%010ukg longueur=%010um\n", 0ul, tableau[0]->dur ? "vrai" : "faux", tableau[0]->masse, tableau[0]->longueur);

        // Comme précédemment, on doit maintenant libérer tous les pointeurs qu'on a alloués dans le tableau car on ne les utilisera plus
        for (size_t i = 0; i < NOMBRE_CACA; i++)
            free(tableau[i]);

        // Mais maintenant, on doit aussi libérer le tableau qui contenait les pointeurs
        free(tableau);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
