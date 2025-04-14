#include "board.hpp"
#include "gaddag.hpp"
#include "joueur.hpp"
#include "sacdelettres.hpp"
#include "lettre.hpp"

#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>

std::string lettresEnTexte(const std::vector<Lettre> &lettres)
{
    std::ostringstream oss;
    for (const auto &l : lettres)
        oss << l.lettre << " ";
    return oss.str();
}

bool jouerMeilleurCoup(Board &plateau, Gaddag &gaddag, Joueur &joueur, SacDeLettres &sac, std::string &motPlace, int &pointsGagnes)
{
    std::vector<Coup> coups = gaddag.trouverMotsPlateau(plateau);
    int meilleurScore = 0;
    Coup meilleurCoup;
    std::string meilleurMot;

    if (coups.empty())
    {
        std::vector<std::string> mots = gaddag.recupererTousMots();
        std::vector<std::string> motsJouables = gaddag.filtrerMotsJouables(mots, "", joueur.lettres);
        std::vector<ResultatMot> motsTries = gaddag.trierParPoints(motsJouables, "", joueur.lettres);

        int centre = 7;
        for (const auto &resultatMot : motsTries)
        {
            int colStart = centre - (resultatMot.texte.length() / 2);
            if (!gaddag.verifierCoupValide(plateau, joueur, resultatMot.texte, centre, colStart, "", true))
                continue;

            if (resultatMot.valeur > meilleurScore)
            {
                meilleurScore = resultatMot.valeur;
                meilleurCoup = {centre, colStart, true, ""};
                meilleurMot = resultatMot.texte;
            }
        }
    }
    else
    {
        for (auto &coup : coups)
        {
            Gaddag *sousArbre = gaddag.extraireSousArbre(coup.texte);
            if (!sousArbre)
                continue;

            std::vector<std::string> motsGaddag = sousArbre->recupererTousMots();
            std::vector<std::string> motsJouables = gaddag.filtrerMotsJouables(motsGaddag, coup.texte, joueur.lettres);
            std::vector<ResultatMot> motsTries = gaddag.trierParPoints(motsJouables, coup.texte, joueur.lettres);

            for (const auto &resultatMot : motsTries)
            {
                if (!gaddag.verifierCoupValide(plateau, joueur, resultatMot.texte, coup.ligneDebut, coup.colonneDebut, coup.texte, coup.direction))
                    continue;

                if (resultatMot.valeur > meilleurScore)
                {
                    meilleurScore = resultatMot.valeur;
                    meilleurCoup = coup;
                    meilleurMot = resultatMot.texte;
                }
            }
        }
    }

    if (!meilleurMot.empty())
    {
        gaddag.poserMotSurPlateau(plateau, joueur,
                                  meilleurMot,
                                  meilleurCoup.ligneDebut,
                                  meilleurCoup.colonneDebut,
                                  meilleurCoup.texte,
                                  meilleurCoup.direction);
        joueur.ajouterScore(meilleurScore);
        motPlace = meilleurMot;
        pointsGagnes = meilleurScore;
        return true;
    }
    else
    {
        motPlace = "aucun";
        pointsGagnes = 0;
        return false;
    }
}

int main()
{
    srand(time(0));
    Board plateau;
    Gaddag gaddag;
    std::cout << "chargement ..." << std::endl;
    gaddag.chargerDictionnaire();

    SacDeLettres sac;
    Joueur joueur("IA");
    joueur.score = 0;

    // Pioche initiale
    for (int i = 0; i < 7 && !sac.estVide(); i++)
    {
        joueur.piocherLettre(sac);
    }

    std::cout << plateau << std::endl;

    int tour = 1;
    bool coupJoue;
    std::string lettresPioches;
    do
    {
        std::cout << "\n----- TOUR " << tour << " -----" << std::endl;

        std::string motPlace;
        int pointsGagnes = 0;

        // Jouer le coup
        coupJoue = jouerMeilleurCoup(plateau, gaddag, joueur, sac, motPlace, pointsGagnes);

        // Piocher après le coup
        std::vector<Lettre> nouvelles;
        while (joueur.lettres.size() < 7 && !sac.estVide())
        {
            Lettre l = sac.piocherLettre();
            joueur.lettres.push_back(l);
            nouvelles.push_back(l);
        }

        lettresPioches = lettresEnTexte(nouvelles);

        // Affichage
        std::cout << plateau << std::endl;
        std::cout << "Lettres piochees precedement: " << lettresPioches << std::endl;
        std::cout << "Lettres joueur: " << lettresEnTexte(joueur.lettres) << std::endl;
        std::cout << "MOT place: " << motPlace << std::endl;
        std::cout << "score: " << joueur.score << std::endl;

        tour++;
    } while (coupJoue);

    // Fin de partie
    if (!joueur.lettres.empty())
    {
        int pointsRestants = 0;
        for (const auto &lettre : joueur.lettres)
        {
            pointsRestants += int(lettre.point);
        }
        joueur.ajouterScore(int(-pointsRestants));
    }

    std::cout << "\n===== RESULTAT =====" << std::endl;
    std::cout << "score: " << joueur.score << std::endl;
    std::cout << plateau << std::endl;

    return 0;
}
