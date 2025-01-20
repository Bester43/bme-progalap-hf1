#ifndef RENDELESEK_H
#define RENDELESEK_H

#include "asztalok.h"
#include "debugmalloc.h"
#include "menu.h"

// Rendelés tétel struktúra
typedef struct RendelesTetel {
    int menuElemAzonosito; // Étel azonosító
    int mennyiseg;         // Mennyiség
    struct RendelesTetel* kov; // Következő tétel
} RendelesTetel;

// Rendelés struktúra
typedef struct Rendeles {
    int asztalAzonosito;   // Asztal azonosító
    RendelesTetel* tetelek; // Tételek lista
    struct Rendeles* kov;   // Következő rendelés
} Rendeles;

// Rendelések lista struktúra
typedef struct {
    Rendeles* elso; // Első rendelés
    int meret;      // Lista mérete
} RendelesLista;

// Alap műveletek
RendelesLista* rendelesListaLetrehozas(void);
void rendelesListaFelszabadit(RendelesLista* lista);
Rendeles* rendelesKeres(RendelesLista* lista, int asztalAzonosito);

// Rendelés műveletek
int rendelesHozzaad(RendelesLista* lista, int asztalAzonosito);
int rendelesTetelHozzaad(Rendeles* rendeles, int menuElemAzonosito, int mennyiseg);
int rendelesTetelModosit(Rendeles* rendeles, int menuElemAzonosito, int ujMennyiseg);
int rendelesTetelTorol(Rendeles* rendeles, int menuElemAzonosito);
void rendelesListaz(const RendelesLista* lista, const MenuLista* menu);
void rendelesekKezelese(RendelesLista* lista, AsztalLista* asztalok, MenuLista* menu);

#endif
