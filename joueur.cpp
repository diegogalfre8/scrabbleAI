#include "joueur.hpp"

// Piocher des lettres tant que le joueur en a moins de 7 et que le sac n'est pas vide
void Joueur::piocherLettre(SacDeLettres &sac)
{
    while (lettres.size() < 7 && sac.total > 0)  // Vérification directe si le sac contient des lettres
    {
        Lettre lettrePiochée = sac.piocherLettre();
        lettres.push_back(lettrePiochée);
    }
}
// Vérifier si le joueur dispose des lettres nécessaires pour former le mot.
// On copie d'abord le vecteur de lettres et on retire à chaque correspondance trouvée.
bool Joueur::verifierMot(std::string mot)
{
    std::vector<Lettre> tempLettres = lettres;
    for (size_t i = 0; i < mot.size(); i++)
    {
        char c = mot[i];
        bool trouve = false;
        for (size_t j = 0; j < tempLettres.size(); j++)
        {
            // On considère que la lettre dans tempLettres peut être utilisée si elle correspond à 'c'
            // ou si c'est un joker représenté par l'espace ' '
            if (tempLettres[j].lettre == c || tempLettres[j].lettre == ' ')
            {
                trouve = true;
                // Retirer la lettre trouvée pour ne pas la réutiliser
                tempLettres.erase(tempLettres.begin() + j);
                break;
            }
        }
        // Si la lettre n'a pas été trouvée, le mot ne peut être formé
        if (!trouve)
            return false;
    }
    return true;
}

// Retirer toutes les occurrences de la lettre donnée dans le vecteur de lettres du joueur
void Joueur::retirerLettre(char lettre)
{
    // Parcours en utilisant une boucle while pour gérer la modification du vecteur
    size_t i = 0;
    while (i < lettres.size())
    {
        if (lettres[i].lettre == lettre)
        {
            lettres.erase(lettres.begin() + i);
        }
        else
        {
            i++;
        }
    }
}
