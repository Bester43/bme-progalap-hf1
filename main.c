#include "main.h"

// Program inicializálása és adatok betöltése
void inicializalProgram(ProgramAllapot* allapot) {
    allapot->asztalok = asztalListaLetrehozas();
    allapot->menu = menuListaLetrehozas();
    allapot->rendelesek = rendelesListaLetrehozas();

    if (allapot->asztalok == NULL || allapot->menu == NULL || allapot->rendelesek == NULL) {
        printf("Hiba: Nem sikerült inicializálni a programot!\n");
        exit(1);
    }

    // Adatok betöltése
    if (!asztalokBetoltese(allapot->asztalok) ||
        !menuBetoltese(allapot->menu) ||
        !rendelesBetoltese(allapot->rendelesek)) {
        printf("Figyelmeztetés: Nem sikerült betölteni minden mentett adatot.\n");
    }
}

void foMenu(ProgramAllapot* allapot) {
    int valasztas;
    do {
        printf("\nFőmenü:\n");
        printf("1. Foglalások kezelése\n");
        printf("2. Étlap kezelése\n");
        printf("3. Rendelés felvétele\n");
        printf("4. Számla készítése\n");
        printf("5. Térkép megjelenítése\n");
        printf("6. Asztalok kezelése\n");
        printf("7. Kilépés\n");

        valasztas = bekerSzam("Válasszon", 1, 7);

        switch (valasztas) {
            case 1: asztalFoglalasKezeles(allapot->asztalok, allapot->rendelesek); break;
            case 2: menuKezelese(allapot->menu); break;
            case 3: rendelesekKezelese(allapot->rendelesek, allapot->asztalok, allapot->menu); break;
            case 4: szamlaKeszites(allapot->rendelesek, allapot->menu, allapot->asztalok); break;
            case 5: terkepMegjelenit(allapot->asztalok); break;
            case 6: asztalKezeloMenu(allapot->asztalok); break;
            case 7:
                if (!mentes(allapot)) {
                    printf("Hiba: Nem sikerült menteni minden adatot!\n");
                }
                printf("Viszontlátásra!\n");
                break;
        }
    } while (valasztas != 7);
}

bool mentes(ProgramAllapot* allapot) {
    return asztalokMentese(allapot->asztalok) &&
           menuMentese(allapot->menu) &&
           rendelesMentese(allapot->rendelesek);
}

void felszabaditProgram(ProgramAllapot* allapot) {
    if (allapot == NULL) return;
    asztalListaFelszabadit(allapot->asztalok);
    menuListaFelszabadit(allapot->menu);
    rendelesListaFelszabadit(allapot->rendelesek);
}

int main() {
    ProgramAllapot allapot;

    inicializalProgram(&allapot);
    foMenu(&allapot);
    felszabaditProgram(&allapot);

    return 0;
}
