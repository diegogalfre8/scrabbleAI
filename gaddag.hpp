#pragma once

#include "lettre.hpp" 
#include "joueur.hpp" 
#include "board.hpp"
#include "sacdelettres.hpp"
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct NoeudGaddag {
    char symbole;
    std::vector<NoeudGaddag*> fils;
    bool estFinMot;
};

struct Coup {
    int ligneDebut;
    int colonneDebut;
    bool direction;
    std::string texte;
};

struct ResultatMot {
    string texte;
    int valeur;
};

struct Gaddag {
    NoeudGaddag* racine;

    Gaddag();

    void insererMotDictionnaire(string mot);
    void ajouterMot(string mot);
    void chargerDictionnaire();
    void visualiserArbre(NoeudGaddag* noeud, string prefixe = "");

    bool estMotValide(string mot);
    Gaddag* extraireSousArbre(string mot);
    vector<string> recupererTousMots();
    vector<Coup> trouverMotsPlateau(Board &plateau);
    int calculerValeurMot(const string& mot, const string& motExistant, const vector<Lettre>& lettresJoueur);
    string convertirMotGaddag(const string& motGaddag);
    vector<string> filtrerMotsJouables(const vector<string>& candidats, const string& motAncre, const vector<Lettre>& lettresJoueur);
    vector<ResultatMot> trierParPoints(const vector<string>& mots, const string& motAncre, const vector<Lettre>& lettresJoueur);
    bool poserMotSurPlateau(Board& plateau, Joueur& joueur, const string& mot, int ligne, int colonne, const string& motAncre, bool direction);
    bool verifierCoupValide(Board& plateau, Joueur& joueur, const string& mot, int ligne, int colonne, const string& motAncre, bool direction);
};