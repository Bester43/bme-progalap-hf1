#ifndef ASZTALOK_H
#define ASZTALOK_H

#include <stdio.h>
#include "debugmalloc.h"

// Asztal struktúra: azonosító, férőhely, koordináták, foglaltság, láncolás
typedef struct Asztal {
    int azonosito;
    int ferohely;
    int x, y;
    int foglalt;
    struct Asztal* kov; // Következő asztal a listában
} Asztal;

// Asztal lista struktúra: első asztal, méret
typedef struct {
    Asztal* elso;
    int meret;
} AsztalLista;

// Alap műveletek
AsztalLista* asztalListaLetrehozas(void);
void asztalListaFelszabadit(AsztalLista* lista);
Asztal* asztalKeres(const AsztalLista* lista, int azonosito);

// Asztal műveletek
int asztalHozzaad(AsztalLista* lista, int azonosito, int ferohely, int x, int y);
int asztalModosit(AsztalLista* lista, int azonosito, int ferohely, int x, int y);
int asztalTorol(AsztalLista* lista, int azonosito);
int asztalFoglal(AsztalLista* lista, int azonosito);
int asztalFelszabadit(AsztalLista* lista, int azonosito);
void asztalListaz(const AsztalLista* lista);

// Validációs függvények
int asztalPozicioErvenyesE(int x, int y);
int asztalUtkozes(const AsztalLista* lista, int x, int y, int kihagyAzonosito);
int asztalAzonositoErvenyesE(const AsztalLista* lista, int azonosito);

#endif
