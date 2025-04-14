#pragma once

#include <iostream>

struct Lettre
{
public:
    char lettre;
    int point;

    Lettre() : lettre(' '), point(0) {} // Joker ou lettre non définie
    Lettre(char l, int p) : lettre(l), point(p) {}
};