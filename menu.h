#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <string.h>
#include "debugmalloc.h"
#include "utils.h"

// Menüelem struktúra
typedef struct MenuElem {
    char* nev; // Étel neve
    int ar;    // Étel ára
    struct MenuElem* kov; // Következő elem
} MenuElem;

// Menü lista struktúra
typedef struct {
    MenuElem* elso; // Első elem
    int meret;      // Lista mérete
} MenuLista;

// Alap műveletek
MenuLista* menuListaLetrehozas(void);
void menuListaFelszabadit(MenuLista* lista);
MenuElem* menuElemKeres(const MenuLista* lista, int azonosito);

// Menü műveletek
int menuElemHozzaad(MenuLista* lista, const char* nev, int ar);
int menuElemModosit(MenuLista* lista, int azonosito, const char* ujNev, int ujAr);
int menuElemTorol(MenuLista* lista, int azonosito);
void menuListaz(const MenuLista* lista);
void menuKezelese(MenuLista* lista);

#endif
