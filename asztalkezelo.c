#include "asztalkezelo.h"

void asztalHozzaadasMenu(AsztalLista* lista) {
    if (lista == NULL) return;

    printf("\nÚj asztal hozzáadása\n");
    printf("--------------------\n");

    // Egyedi azonosító bekérése ismétléssel, amíg nem lesz érvényes
    int azonosito = bekerSzam("Asztal azonosító", 1, 999);
    while (!asztalAzonositoErvenyesE(lista, azonosito)) {
        printf("Ez az azonosító már foglalt!\n");
        azonosito = bekerSzam("Asztal azonosító", 1, 999);
    }

    int ferohely = bekerSzam("Férőhely", 1, 9);

    // Érvényes pozíció bekérése: nem ütközhet fallal vagy másik asztallal
    int x, y;
    do {
        x = bekerSzam("X koordináta", 1, TERKEP_SZELESSEG - 2);
        y = bekerSzam("Y koordináta", 1, TERKEP_MAGASSAG - 2);

        if (!koordinatakErvenyesekE(x, y, TERKEP_SZELESSEG, TERKEP_MAGASSAG)) {
            printf("Érvénytelen koordináták!\n");
            continue;
        }
    } while (terkepUtkozesVizsgalat(lista, x, y, 0));  // 0 azonosító: új asztal

    if (asztalHozzaad(lista, azonosito, ferohely, x, y)) {
        printf("Asztal sikeresen hozzáadva!\n");
    } else {
        printf("Hiba történt az asztal hozzáadása során!\n");
    }
}

void asztalTorlesMenu(AsztalLista* lista) {
    if (lista == NULL) return;

    printf("\nAsztal törlése\n");
    printf("-------------\n");

    asztalListaz(lista);
    int azonosito = bekerSzam("Törlendő asztal azonosítója", 1, 999);

    // Ellenőrzések: létezik-e az asztal és nincs-e foglalva
    Asztal* asztal = asztalKeres(lista, azonosito);
    if (asztal == NULL) {
        printf("Nem létezik ilyen azonosítójú asztal!\n");
        return;
    }

    if (asztal->foglalt) {
        printf("Foglalt asztal nem törölhető!\n");
        return;
    }

    if (asztalTorol(lista, azonosito)) {
        printf("Asztal sikeresen törölve!\n");
    } else {
        printf("Hiba történt az asztal törlése során!\n");
    }
}

void asztalModositasMenu(AsztalLista* lista) {
    if (lista == NULL) return;

    printf("\nAsztal módosítása\n");
    printf("----------------\n");

    asztalListaz(lista);
    int azonosito = bekerSzam("Módosítandó asztal azonosítója", 1, 999);

    // Ellenőrzések: létezik-e az asztal és nincs-e foglalva
    Asztal* asztal = asztalKeres(lista, azonosito);
    if (asztal == NULL) {
        printf("Nem létezik ilyen azonosítójú asztal!\n");
        return;
    }

    if (asztal->foglalt) {
        printf("Foglalt asztal nem módosítható!\n");
        return;
    }

    int ferohely = bekerSzam("Új férőhely", 1, 9);

    // Új pozíció bekérése ütközésvizsgálattal
    // Az azonosítót átadjuk, hogy a saját pozícióját ne tekintse ütközésnek
    int x, y;
    do {
        x = bekerSzam("Új X koordináta", 1, TERKEP_SZELESSEG - 2);
        y = bekerSzam("Új Y koordináta", 1, TERKEP_MAGASSAG - 2);

        if (!terkepPozicioErvenyesE(x, y)) {
            printf("Érvénytelen koordináták! Az asztalnak az éttermen belül kell lennie.\n");
            continue;
        }

        if (terkepUtkozesVizsgalat(lista, x, y, azonosito)) {
            printf("Ezen a helyen már van egy asztal!\n");
        }
    } while (!terkepPozicioErvenyesE(x, y) || terkepUtkozesVizsgalat(lista, x, y, azonosito));

    if (asztalModosit(lista, azonosito, ferohely, x, y)) {
        printf("Asztal sikeresen módosítva!\n");
    } else {
        printf("Hiba történt az asztal módosítása során!\n");
    }
}

void asztalKezeloMenu(AsztalLista* lista) {
    if (lista == NULL) return;

    int valasztas;
    do {
        printf("\nAsztalkezelő menü:\n");
        printf("1. Új asztal hozzáadása\n");
        printf("2. Asztal törlése\n");
        printf("3. Asztal módosítása\n");
        printf("4. Asztalok listázása\n");
        printf("5. Vissza\n");

        valasztas = bekerSzam("Válasszon", 1, 5);

        switch (valasztas) {
            case 1: asztalHozzaadasMenu(lista); break;
            case 2: asztalTorlesMenu(lista); break;
            case 3: asztalModositasMenu(lista); break;
            case 4: asztalListaz(lista); break;
            case 5: printf("Visszalépés a főmenübe...\n"); break;
        }
    } while (valasztas != 5);
}
