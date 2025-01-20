#include "asztalok.h"

#define TERKEP_SZELESSEG 20
#define TERKEP_MAGASSAG 10

// Üres asztallista létrehozása
AsztalLista* asztalListaLetrehozas(void) {
    AsztalLista* lista = (AsztalLista*)malloc(sizeof(AsztalLista));
    if (lista == NULL) return NULL;

    lista->elso = NULL;
    lista->meret = 0;
    return lista;
}

// Teljes lista és minden asztal felszabadítása
void asztalListaFelszabadit(AsztalLista* lista) {
    if (lista == NULL) return;

    Asztal* aktualis = lista->elso;
    while (aktualis != NULL) {
        Asztal* kov = aktualis->kov;
        free(aktualis);
        aktualis = kov;
    }
    free(lista);
}

// Asztal keresése azonosító alapján
Asztal* asztalKeres(const AsztalLista* lista, int azonosito) {
    if (lista == NULL) return NULL;

    Asztal* aktualis = lista->elso;
    while (aktualis != NULL) {
        if (aktualis->azonosito == azonosito) {
            return aktualis;
        }
        aktualis = aktualis->kov;
    }
    return NULL;
}

// Validációs függvények
int asztalPozicioErvenyesE(int x, int y) {
    return x >= 0 && x < TERKEP_SZELESSEG-1 &&
           y >= 0 && y < TERKEP_MAGASSAG-1;
}

int asztalAzonositoErvenyesE(const AsztalLista* lista, int azonosito) {
    if (azonosito <= 0) return 0;
    return asztalKeres(lista, azonosito) == NULL;
}

// Ütközésvizsgálat: kihagyAzonosito != -1 esetén azt az asztalt figyelmen kívül hagyja
int asztalUtkozes(const AsztalLista* lista, int x, int y, int kihagyAzonosito) {
    if (lista == NULL) return 0;

    Asztal* aktualis = lista->elso;
    while (aktualis != NULL) {
        if (aktualis->azonosito != kihagyAzonosito &&
            aktualis->x == x && aktualis->y == y) {
            return 1;
        }
        aktualis = aktualis->kov;
    }
    return 0;
}

// Új asztal hozzáadása a listához
int asztalHozzaad(AsztalLista* lista, int azonosito, int ferohely, int x, int y) {
    if (!asztalPozicioErvenyesE(x, y)) return 0;
    if (asztalUtkozes(lista, x, y, 0)) return 0;
    if (lista == NULL || ferohely <= 0 || ferohely > 9 ||
        !asztalPozicioErvenyesE(x, y) ||
        asztalUtkozes(lista, x, y, -1) ||
        asztalKeres(lista, azonosito) != NULL) {
        return 0;
    }

    Asztal* uj = (Asztal*)malloc(sizeof(Asztal));
    if (uj == NULL) return 0;

    uj->azonosito = azonosito;
    uj->ferohely = ferohely;
    uj->x = x;
    uj->y = y;
    uj->foglalt = 0;

    uj->kov = lista->elso;
    lista->elso = uj;
    lista->meret++;

    return 1;
}

// Asztal törlése azonosító alapján
int asztalTorol(AsztalLista* lista, int azonosito) {
    if (lista == NULL || lista->elso == NULL) return 0;

    Asztal* elozo = NULL;
    Asztal* aktualis = lista->elso;

    while (aktualis != NULL && aktualis->azonosito != azonosito) {
        elozo = aktualis;
        aktualis = aktualis->kov;
    }

    if (aktualis == NULL) return 0;

    if (elozo == NULL) {
        lista->elso = aktualis->kov;
    } else {
        elozo->kov = aktualis->kov;
    }

    free(aktualis);
    lista->meret--;
    return 1;
}

// Alapműveletek asztalokkal
int asztalFoglal(AsztalLista* lista, int azonosito) {
    Asztal* asztal = asztalKeres(lista, azonosito);
    if (asztal == NULL || asztal->foglalt) return 0;

    asztal->foglalt = 1;
    return 1;
}

int asztalFelszabadit(AsztalLista* lista, int azonosito) {
    Asztal* asztal = asztalKeres(lista, azonosito);
    if (asztal == NULL || !asztal->foglalt) return 0;

    asztal->foglalt = 0;
    return 1;
}

// Asztalok listázása táblázatos formában
void asztalListaz(const AsztalLista* lista) {
    if (lista == NULL) {
        printf("A lista nem létezik!\n");
        return;
    }

    if (lista->elso == NULL) {
        printf("Nincsenek asztalok a rendszerben.\n");
        return;
    }

    printf("\nAsztalok listája:\n");
    printf("ID\tFérőhely\tX\tY\tÁllapot\n");
    printf("----------------------------------------\n");

    Asztal* aktualis = lista->elso;
    while (aktualis != NULL) {
        printf("%d\t%d\t\t%d\t%d\t%s\n",
               aktualis->azonosito,
               aktualis->ferohely,
               aktualis->x,
               aktualis->y,
               aktualis->foglalt ? "Foglalt" : "Szabad");
        aktualis = aktualis->kov;
    }
}

// Asztal adatainak módosítása
int asztalModosit(AsztalLista* lista, int azonosito, int uj_ferohely, int uj_x, int uj_y) {
    if (lista == NULL || azonosito <= 0 || uj_ferohely <= 0) {
        return 0;
    }

    if (!asztalPozicioErvenyesE(uj_x, uj_y)) {
        return 0;
    }

    if (asztalUtkozes(lista, uj_x, uj_y, azonosito)) {
        return 0;
    }

    Asztal* asztal = asztalKeres(lista, azonosito);
    if (asztal == NULL) {
        return 0;
    }

    asztal->ferohely = uj_ferohely;
    asztal->x = uj_x;
    asztal->y = uj_y;

    return 1;
}
