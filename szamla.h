#ifndef SZAMLA_H
#define SZAMLA_H

#include "asztalok.h"
#include "menu.h"
#include "utils.h"
#include "rendelesek.h"
#include "asztalfoglalas.h"

// Számla készítése
void szamlaKeszites(RendelesLista* rendelesek, MenuLista* menu, AsztalLista* asztalok);

// Számla listázása
void szamlaListaz(const Rendeles* rendeles, const MenuLista* menu);

// Fizetés végrehajtása
int szamlaFizet(RendelesLista* rendelesek, AsztalLista* asztalok, int asztalAzonosito);

#endif
