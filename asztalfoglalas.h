#ifndef ASZTALFOGLALAS_H
#define ASZTALFOGLALAS_H

#include "asztalok.h"
#include "rendelesek.h"
#include <stdio.h>
#include <limits.h>

// Asztalfoglalás kezelése (1: siker, 0: hiba)
// Automatikus: legmegfelelőbb szabad asztal kiválasztása
int automatikusAsztalFoglalas(AsztalLista* asztalok, int vendegekSzama);

// Manuális foglalás és felszabadítás azonosító alapján
int asztalFoglalas(AsztalLista* asztalok, int asztalAzonosito);
int asztalFelszabaditas(AsztalLista* asztalok, RendelesLista* rendelesek, int asztalAzonosito);

// Felhasználói menü az asztalfoglaláshoz
void asztalFoglalasKezeles(AsztalLista* asztalok, RendelesLista* rendelesek);

#endif
