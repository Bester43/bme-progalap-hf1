#ifndef ASZTALKEZELO_H
#define ASZTALKEZELO_H

#include "asztalok.h"
#include "debugmalloc.h"
#include "main.h"
#include "utils.h"

// Asztalok kezelése: létrehozás, törlés, módosítás, listázás
void asztalKezeloMenu(AsztalLista* lista);
void asztalHozzaadasMenu(AsztalLista* lista);
void asztalTorlesMenu(AsztalLista* lista);
void asztalModositasMenu(AsztalLista* lista);

#endif
