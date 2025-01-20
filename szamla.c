#include "szamla.h"
#include <stdio.h>

// Számla listázása egy adott rendelés és menü alapján
void szamlaListaz(const Rendeles* rendeles, const MenuLista* menu) {
    if (rendeles == NULL || menu == NULL) {
        printf("Hiba: Érvénytelen számla adatok!\n");
        return;
    }
    printf("\nSzámla:\n");
    printf("Asztal: %d\n", rendeles->asztalAzonosito);
    printf("--------------------------------------------------------------\n");
    printf("Tétel                                 Mennyiség   Ár/db    Összeg\n");
    printf("--------------------------------------------------------------\n");

    int vegosszeg = 0; // Végösszeg
    RendelesTetel* tetel = rendeles->tetelek;

    while (tetel != NULL) {
        MenuElem* menuElem = menuElemKeres(menu, tetel->menuElemAzonosito);
        if (menuElem != NULL) {
            size_t nevHossz = strlen(menuElem->nev);
            size_t pozicio = 0;

            // Étel nevének kiírása
            while (pozicio < nevHossz) {
                char tempStr[36];
                strncpy(tempStr, menuElem->nev + pozicio, 35);
                tempStr[35] = '\0';
                if (pozicio == 0) {
                    printf("%-35s %3d db %6d Ft/db = %8d Ft\n",
                           tempStr,
                           tetel->mennyiseg,
                           menuElem->ar,
                           tetel->mennyiseg * menuElem->ar);
                } else {
                    printf("%-35s\n", tempStr);
                }
                pozicio += 35; // Következő rész
            }
            vegosszeg += tetel->mennyiseg * menuElem->ar;
        }
        tetel = tetel->kov; // Következő tétel
    }

    printf("--------------------------------------------------------------\n");
    printf("Végösszeg: %d Ft\n", vegosszeg);
}

// Számla készítése
void szamlaKeszites(RendelesLista* rendelesek, MenuLista* menu, AsztalLista* asztalok) {
    if (rendelesek == NULL || menu == NULL || asztalok == NULL) {
        printf("Hiba: Érvénytelen paraméterek!\n");
        return;
    }

    printf("\nSzámla készítése\n----------------\n");

    int asztalAzonosito;
    printf("Adja meg az asztal azonosítóját: ");
    if (scanf("%d", &asztalAzonosito) != 1) {
        printf("Hiba: Érvénytelen azonosító!\n");
        while (getchar() != '\n'); // Bemenet tisztítása
        return;
    }

    Asztal* asztal = asztalKeres(asztalok, asztalAzonosito);
    if (asztal == NULL || !asztal->foglalt) {
        printf("Hiba: Az asztal nem létezik vagy nincs foglalva!\n");
        return;
    }

    Rendeles* rendeles = rendelesKeres(rendelesek, asztalAzonosito);
    if (rendeles == NULL) {
        printf("Hiba: Nincs rendelés az asztalnál!\n");
        return;
    }

    szamlaListaz(rendeles, menu); // Számla listázása

    // Fizetés megerősítése
    if (bekerSzam("\nFizetés sikeres? (1-Igen, 0-Nem)", 0, 1)) {
        szamlaFizet(rendelesek, asztalok, asztalAzonosito);
    }
}

// Fizetés végrehajtása
int szamlaFizet(RendelesLista* rendelesek, AsztalLista* asztalok, int asztalAzonosito) {
    if (rendelesek == NULL || asztalok == NULL || asztalAzonosito <= 0) {
        return 0; // Érvénytelen paraméterek
    }
    if (asztalFelszabaditas(asztalok, rendelesek, asztalAzonosito)) {
        printf("Sikeres fizetés! Asztal felszabadítva!\n");
        return 1; // Sikeres fizetés
    }
    printf("Hiba történt a fizetés során!\n");
    return 0; // Fizetés hiba
}
