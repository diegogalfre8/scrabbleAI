#ifndef SACDELETTRES_HPP
#define SACDELETTRES_HPP

#include "lettre.hpp"
#include <vector>

struct SacDeLettres {
    std::vector<std::pair<Lettre, int>> lettres;
    int total;

    SacDeLettres();
    Lettre piocherLettre();
    bool estVide() const { return total == 0; }
    void reinitialiser();
    int nombre_lettres() const { return total; }
};

#endif // SACDELETTRES_HPP