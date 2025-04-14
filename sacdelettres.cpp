#include "sacdelettres.hpp"

SacDeLettres::SacDeLettres()
{
    // Initialisation du sac par groupe d'éléments dans une seule affectation.
    lettres = {
        {Lettre('A', 1), 9},
        {Lettre('B', 3), 2},
        {Lettre('C', 3), 2},
        {Lettre('D', 2), 3},
        {Lettre('E', 1), 15},
        {Lettre('F', 4), 2},
        {Lettre('G', 2), 2},
        {Lettre('H', 4), 2},
        {Lettre('I', 1), 8},
        {Lettre('J', 8), 1},
        {Lettre('K', 10), 1},
        {Lettre('L', 1), 5},
        {Lettre('M', 2), 3},
        {Lettre('N', 1), 6},
        {Lettre('O', 1), 6},
        {Lettre('P', 3), 2},
        {Lettre('Q', 8), 1},
        {Lettre('R', 1), 6},
        {Lettre('S', 1), 6},
        {Lettre('T', 1), 6},
        {Lettre('U', 1), 6},
        {Lettre('V', 4), 2},
        {Lettre('W', 10), 1},
        {Lettre('X', 10), 1},
        {Lettre('Y', 10), 1},
        {Lettre('Z', 10), 1}};
    total = 102;
}

Lettre SacDeLettres::piocherLettre()
{
    if (total == 0)
    {
        return Lettre(' ', 0); // Retourne un espace quand le sac est vide
    }
    int r = rand() % total;
    int i = 0;
    while (r >= lettres[i].second)
    {
        r -= lettres[i].second;
        i++;
    }
    total--;
    lettres[i].second--;
    return lettres[i].first;
}

void SacDeLettres::reinitialiser()
{
    lettres.clear();
    total = 0;
    *this = SacDeLettres(); // Réinitialise le sac avec les valeurs initiales
}
