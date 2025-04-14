#include "gaddag.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cctype>

Gaddag::Gaddag()
{
    racine = new NoeudGaddag;
    racine->symbole = ' ';
    racine->estFinMot = false;
}

void Gaddag::insererMotDictionnaire(string mot)
{
    string prefixe = "";
    for (size_t i = 0; i < mot.size(); i++)
    {
        prefixe = mot[i] + prefixe;
        string suffixe = mot.substr(i + 1);
        ajouterMot(prefixe + "+" + suffixe);
    }
    ajouterMot(mot);
}

void Gaddag::ajouterMot(string mot)
{
    NoeudGaddag *courant = racine;
    for (size_t i = 0; i < mot.size(); i++)
    {
        char c = mot[i];
        bool trouve = false;
        for (size_t j = 0; j < courant->fils.size(); j++)
        {
            if (courant->fils[j]->symbole == c)
            {
                courant = courant->fils[j];
                trouve = true;
                break;
            }
        }
        if (!trouve)
        {
            NoeudGaddag *nouveau = new NoeudGaddag;
            nouveau->symbole = c;
            nouveau->estFinMot = false;
            courant->fils.push_back(nouveau);
            courant = nouveau;
        }
    }
    courant->estFinMot = true;
}

void Gaddag::chargerDictionnaire()
{
    ifstream fichier("../data/dico.txt");
    string mot;
    if (!fichier)
    {
        cout << "Erreur : fichier dictionnaire introuvable." << endl;
        return;
    }
    while (getline(fichier, mot))
    {
        insererMotDictionnaire(mot);
    }
    fichier.close();
}

void Gaddag::visualiserArbre(NoeudGaddag *noeud, string prefixe)
{
    if (noeud == NULL)
        return;
    cout << prefixe << noeud->symbole;
    if (noeud->estFinMot)
        cout << " (fin)";
    cout << endl;
    for (size_t i = 0; i < noeud->fils.size(); i++)
    {
        visualiserArbre(noeud->fils[i], prefixe + "  ");
    }
}

bool Gaddag::estMotValide(string mot)
{
    NoeudGaddag *courant = racine;
    for (size_t i = 0; i < mot.size(); i++)
    {
        bool trouve = false;
        for (size_t j = 0; j < courant->fils.size(); j++)
        {
            if (courant->fils[j]->symbole == mot[i])
            {
                courant = courant->fils[j];
                trouve = true;
                break;
            }
        }
        if (!trouve)
            return false;
    }
    return courant->estFinMot;
}

Gaddag *Gaddag::extraireSousArbre(string mot)
{
    Gaddag *g = new Gaddag();
    NoeudGaddag *original = racine;
    NoeudGaddag *copie = new NoeudGaddag();
    g->racine = copie;
    for (int i = (int)mot.size() - 1; i >= 0; i--)
    {
        char c = mot[i];
        bool trouve = false;
        for (size_t j = 0; j < original->fils.size(); j++)
        {
            if (original->fils[j]->symbole == c)
            {
                NoeudGaddag *nouveau = new NoeudGaddag();
                nouveau->symbole = c;
                nouveau->estFinMot = false;
                copie->fils.push_back(nouveau);
                copie = nouveau;
                original = original->fils[j];
                trouve = true;
                break;
            }
        }
        if (!trouve)
        {
            delete g;
            return NULL;
        }
    }
    copie->estFinMot = true;
    copie->fils = original->fils;
    return g;
}

void explorerProfondeur(NoeudGaddag *noeud, string mot, vector<string> &liste)
{
    if (noeud->estFinMot)
        liste.push_back(mot);
    for (size_t i = 0; i < noeud->fils.size(); i++)
    {
        explorerProfondeur(noeud->fils[i], mot + noeud->fils[i]->symbole, liste);
    }
}

vector<string> Gaddag::recupererTousMots()
{
    vector<string> mots;
    for (size_t i = 0; i < racine->fils.size(); i++)
    {
        explorerProfondeur(racine->fils[i], string(1, racine->fils[i]->symbole), mots);
    }
    return mots;
}
vector<Coup> Gaddag::trouverMotsPlateau(Board &plateau)
{
    vector<Coup> mots;
    string mot;
    int debut;
    bool directionHorizontale = true;

    // Recherche des mots horizontaux
    for (int ligne = 0; ligne < 15; ++ligne)
    {
        for (int colonne = 0; colonne < 15; ++colonne)
        {
            if (plateau(ligne, colonne).letter != 0 &&
                (colonne == 0 || plateau(ligne, colonne - 1).letter == 0))
            {
                debut = colonne;
                mot = "";
                while (colonne < 15 && plateau(ligne, colonne).letter != 0)
                {
                    mot += plateau(ligne, colonne).letter;
                    colonne++;
                }
                Coup coup;
                coup.ligneDebut = ligne;
                coup.colonneDebut = debut;
                coup.direction = directionHorizontale; // true = horizontal
                coup.texte = mot;
                mots.push_back(coup);
            }
        }
    }

    // Recherche des mots verticaux
    directionHorizontale = false;
    for (int colonne = 0; colonne < 15; ++colonne)
    {
        for (int ligne = 0; ligne < 15; ++ligne)
        {
            if (plateau(ligne, colonne).letter != 0 &&
                (ligne == 0 || plateau(ligne - 1, colonne).letter == 0))
            {
                debut = ligne;
                mot = "";
                while (ligne < 15 && plateau(ligne, colonne).letter != 0)
                {
                    mot += plateau(ligne, colonne).letter;
                    ligne++;
                }
                Coup coup;
                coup.ligneDebut = debut;
                coup.colonneDebut = colonne;
                coup.direction = directionHorizontale; // false = vertical
                coup.texte = mot;
                mots.push_back(coup);
            }
        }
    }
    return mots;
}

int Gaddag::calculerValeurMot(const string &mot,
                              const string &motExistant,
                              const vector<Lettre> &lettresJoueur)
{
    int score = 0;
    vector<Lettre> lettresRestantes = lettresJoueur;

    if (motExistant.empty() || motExistant == " ")
    {
        for (size_t i = 0; i < mot.size(); i++)
        {
            for (size_t j = 0; j < lettresRestantes.size(); j++)
            {
                if (lettresRestantes[j].lettre == mot[i])
                {
                    score += lettresRestantes[j].point;
                    lettresRestantes.erase(lettresRestantes.begin() + j);
                    break;
                }
            }
        }
        return score;
    }

    size_t pos = mot.find(motExistant);
    if (pos == string::npos)
        return 0;

    for (size_t i = 0; i < pos; i++)
    {
        for (size_t j = 0; j < lettresRestantes.size(); j++)
        {
            if (lettresRestantes[j].lettre == mot[i])
            {
                score += lettresRestantes[j].point;
                lettresRestantes.erase(lettresRestantes.begin() + j);
                break;
            }
        }
    }

    for (size_t i = pos + motExistant.size(); i < mot.size(); i++)
    {
        for (size_t j = 0; j < lettresRestantes.size(); j++)
        {
            if (lettresRestantes[j].lettre == mot[i])
            {
                score += lettresRestantes[j].point;
                lettresRestantes.erase(lettresRestantes.begin() + j);
                break;
            }
        }
    }
    return score;
}

string Gaddag::convertirMotGaddag(const string &motGaddag)
{
    size_t posSeparateur = motGaddag.find('+');
    if (posSeparateur == string::npos)
        return motGaddag;

    string prefixe = motGaddag.substr(0, posSeparateur);
    string revPrefixe = "";
    for (int i = (int)prefixe.size() - 1; i >= 0; i--)
    {
        revPrefixe += prefixe[i];
    }
    string suffixe = motGaddag.substr(posSeparateur + 1);
    return revPrefixe + suffixe;
}

vector<string> Gaddag::filtrerMotsJouables(const vector<string> &candidats,
                                           const string &motAncre,
                                           const vector<Lettre> &lettresJoueur)
{
    vector<string> resultat;
    for (size_t i = 0; i < candidats.size(); i++)
    {
        string motComplet = convertirMotGaddag(candidats[i]);
        if (motAncre == " " || motAncre.empty())
        {
            vector<char> disponibles;
            for (size_t j = 0; j < lettresJoueur.size(); j++)
                disponibles.push_back(lettresJoueur[j].lettre);
            bool possible = true;
            for (size_t k = 0; k < motComplet.size(); k++)
            {
                bool found = false;
                for (size_t l = 0; l < disponibles.size(); l++)
                {
                    if (disponibles[l] == motComplet[k])
                    {
                        disponibles.erase(disponibles.begin() + l);
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    possible = false;
                    break;
                }
            }
            if (possible)
                resultat.push_back(motComplet);
            continue;
        }

        size_t pos = motComplet.find(motAncre);
        if (pos == string::npos)
            continue;

        string avant = motComplet.substr(0, pos);
        string apres = motComplet.substr(pos + motAncre.size());
        if (avant.empty() && apres.empty())
            continue;

        vector<char> disponibles;
        for (size_t j = 0; j < lettresJoueur.size(); j++)
            disponibles.push_back(lettresJoueur[j].lettre);
        bool possible = true;
        for (size_t k = 0; k < avant.size(); k++)
        {
            bool found = false;
            for (size_t l = 0; l < disponibles.size(); l++)
            {
                if (disponibles[l] == avant[k])
                {
                    disponibles.erase(disponibles.begin() + l);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                possible = false;
                break;
            }
        }
        if (!possible)
            continue;
        for (size_t k = 0; k < apres.size(); k++)
        {
            bool found = false;
            for (size_t l = 0; l < disponibles.size(); l++)
            {
                if (disponibles[l] == apres[k])
                {
                    disponibles.erase(disponibles.begin() + l);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                possible = false;
                break;
            }
        }
        if (possible)
            resultat.push_back(motComplet);
    }
    return resultat;
}

vector<ResultatMot> Gaddag::trierParPoints(const vector<string> &mots,
                                           const string &motAncre,
                                           const vector<Lettre> &lettresJoueur)
{
    vector<ResultatMot> resultat;
    for (size_t i = 0; i < mots.size(); i++)
    {
        int score = calculerValeurMot(mots[i], motAncre, lettresJoueur);
        ResultatMot r;
        r.texte = mots[i];
        r.valeur = score;
        resultat.push_back(r);
    }
    sort(resultat.begin(), resultat.end(), [](const ResultatMot &a, const ResultatMot &b)
         { return a.valeur > b.valeur; });
    return resultat;
}

bool Gaddag::poserMotSurPlateau(Board &plateau,
                                Joueur &joueur,
                                const std::string &mot,
                                int ligne, int colonne,
                                const std::string &motAncre,
                                bool direction)
{
    if (motAncre.empty() || motAncre == " ")
    {
        if (direction && colonne + (int)mot.size() > 15)
            return false;
        if (!direction && ligne + (int)mot.size() > 15)
            return false;
        for (size_t i = 0; i < mot.size(); i++)
        {
            int r = ligne, c = colonne;
            if (direction)
                c += i;
            else
                r += i;
            if (plateau(r, c).letter == 0)
            {
                plateau(r, c).letter = mot[i];
                joueur.retirerLettre(mot[i]);
            }
            else if (plateau(r, c).letter != mot[i])
            {
                return false;
            }
        }
        return true;
    }

    size_t pos = mot.find(motAncre);
    if (pos == string::npos)
        return false;

    int debutLigne = ligne;
    int debutColonne = colonne;
    if (pos > 0)
    {
        if (direction)
            debutColonne -= pos;
        else
            debutLigne -= pos;
    }
    if (direction && (debutColonne < 0 || debutColonne + (int)mot.size() > 15))
        return false;
    if (!direction && (debutLigne < 0 || debutLigne + (int)mot.size() > 15))
        return false;

    for (size_t i = 0; i < mot.size(); i++)
    {
        int r = debutLigne, c = debutColonne;
        if (direction)
            c += i;
        else
            r += i;
        if (r < 0 || r >= 15 || c < 0 || c >= 15)
            return false;
        if (plateau(r, c).letter != 0 && plateau(r, c).letter != mot[i])
            return false;
    }

    for (size_t i = 0; i < mot.size(); i++)
    {
        int r = debutLigne, c = debutColonne;
        if (direction)
            c += i;
        else
            r += i;
        if (plateau(r, c).letter == 0)
        {
            plateau(r, c).letter = mot[i];
            joueur.retirerLettre(mot[i]);
        }
    }
    return true;
}

bool Gaddag::verifierCoupValide(Board &plateau, Joueur &joueur,
                                const string &mot, int ligne, int colonne,
                                const string &motAncre, bool direction)
{
    if (!estMotValide(mot))
        return false;

    Board plateauTemp = plateau;
    Joueur joueurTemp = joueur;
    if (!poserMotSurPlateau(plateauTemp, joueurTemp, mot, ligne, colonne, motAncre, direction))
        return false;

    vector<Coup> motsFormes = trouverMotsPlateau(plateauTemp);
    for (size_t i = 0; i < motsFormes.size(); i++)
    {
        if (motsFormes[i].texte.size() >= 2 && !estMotValide(motsFormes[i].texte))
            return false;
    }
    return true;
}
