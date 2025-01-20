#ifndef TERKEP_H
#define TERKEP_H

#include "asztalok.h"
#include "main.h"

// Térkép műveletek
void terkepMegjelenit(const AsztalLista* asztalok); // Térkép megjelenítése
int terkepUtkozesVizsgalat(const AsztalLista* asztalok, int x, int y, int kihagyAzonosito); // Ütközés vizsgálat
int terkepPozicioErvenyesE(int x, int y); // Pozíció érvényességének ellenőrzése

#endif
