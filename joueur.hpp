#pragma once

#include <string>
#include <vector>
#include "lettre.hpp"
#include "sacdelettres.hpp"

struct Joueur {
    std::string nom;
    std::vector<Lettre> lettres;
    int score;

    Joueur(std::string n) : nom(n) {}

    void piocherLettre(SacDeLettres &sac);
    bool verifierMot(std::string mot);
    void retirerLettre(char l);
    void ajouterScore(int points) { score += points; }
};