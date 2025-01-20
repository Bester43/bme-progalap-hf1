#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "debugmalloc.h"
#include <string.h>

#include "asztalok.h"
#include "menu.h"
#include "asztalkezelo.h"
#include "rendelesek.h"
#include "file.h"
#include "terkep.h"
#include "szamla.h"
#include "asztalfoglalas.h"
#include "utils.h"

// Konstansok
#define TERKEP_SZELESSEG 20
#define TERKEP_MAGASSAG 10

// Program állapot struktúra
typedef struct {
    AsztalLista* asztalok;
    MenuLista* menu;
    RendelesLista* rendelesek;
} ProgramAllapot;

// Főprogram függvények
void inicializalProgram(ProgramAllapot* allapot);
void foMenu(ProgramAllapot* allapot);
void felszabaditProgram(ProgramAllapot* allapot);
bool mentes(ProgramAllapot* allapot);

#endif
