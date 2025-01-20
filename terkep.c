#include "terkep.h"

// Térkép megjelenítése az asztalok helyzetével
void terkepMegjelenit(const AsztalLista* asztalok) {
    char terkep[TERKEP_MAGASSAG][TERKEP_SZELESSEG];

    // Térkép inicializálása: határok és üres helyek beállítása
    for (int y = 0; y < TERKEP_MAGASSAG; ++y) {
        for (int x = 0; x < TERKEP_SZELESSEG; ++x) {
            if (x == 0 || x == TERKEP_SZELESSEG - 1 || y == 0 || y == TERKEP_MAGASSAG - 1) {
                terkep[y][x] = '#'; // Határ
            } else {
                terkep[y][x] = ' '; // Üres hely
            }
        }
    }

    // Asztalok elhelyezése a térképen
    Asztal* aktualis = asztalok->elso;
    while (aktualis != NULL) {
        // Ellenőrizzük, hogy az asztal pozíciója érvényes-e
        if (aktualis->x >= 0 && aktualis->x < TERKEP_SZELESSEG &&
            aktualis->y >= 0 && aktualis->y < TERKEP_MAGASSAG) {
            if (aktualis->foglalt) {
                terkep[aktualis->y][aktualis->x] = 'X'; // Foglalt asztal
            } else {
                terkep[aktualis->y][aktualis->x] = '0' + aktualis->ferohely; // Férőhely száma
            }
        }
        aktualis = aktualis->kov; // Következő asztal
    }

    // Térkép kiíratása
    for (int y = 0; y < TERKEP_MAGASSAG; ++y) {
        for (int x = 0; x < TERKEP_SZELESSEG; ++x) {
            putchar(terkep[y][x]); // Karakter kiírása
        }
        putchar('\n'); // Új sor
    }
}

// Ellenőrzi, hogy van-e ütközés egy adott pozícióval az asztalok között
int terkepUtkozesVizsgalat(const AsztalLista* asztalok, int x, int y, int kihagyAzonosito) {
    if (asztalok == NULL) return 0;

    Asztal* aktualis = asztalok->elso;
    while (aktualis != NULL) {
        // Ütközés vizsgálat (azonosító figyelmen kívül hagyásával)
        if (aktualis->x == x && aktualis->y == y && aktualis->azonosito != kihagyAzonosito) {
            return 1; // Ütközés történt
        }
        aktualis = aktualis->kov; // Következő asztal
    }
    return 0; // Nincs ütközés
}

// Ellenőrzi, hogy a megadott pozíció érvényes-e a térképen
int terkepPozicioErvenyesE(int x, int y) {
    return x > 0 && x < TERKEP_SZELESSEG - 1 && y > 0 && y < TERKEP_MAGASSAG - 1;
}
