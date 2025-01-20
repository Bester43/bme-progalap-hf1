#ifndef FILE_H
#define FILE_H

#include "asztalok.h"
#include "menu.h"
#include "rendelesek.h"
#include "debugmalloc.h"

// Fájlnevek definiálása
#define ASZTALOK_FAJL "asztalok.dat"
#define MENU_FAJL "menu.dat"
#define RENDELESEK_FAJL "rendelesek.dat"

// Fájl műveletek
int asztalokMentese(const AsztalLista* asztalok);
int asztalokBetoltese(AsztalLista* asztalok);

int menuMentese(const MenuLista* menu);
int menuBetoltese(MenuLista* menu);

int rendelesMentese(const RendelesLista* rendelesek);
int rendelesBetoltese(RendelesLista* rendelesek);

#endif
