#include "file.h"

// Asztalok mentése
int asztalokMentese(const AsztalLista* asztalok) {
    if (asztalok == NULL) {
        printf("Hiba: Az asztalok listája üres.\n");
        return 0;
    }

    char fajlNev[256] = ASZTALOK_FAJL; // Alapértelmezett fájlnév
    FILE* fajl = fopen(fajlNev, "wb"); // Fájl megnyitása írásra
    while (fajl == NULL) { // Hibaellenőrzés a fájl megnyitásakor
        printf("Hiba: Nem sikerült megnyitni a '%s' fájlt írásra.\n", fajlNev);
        printf("Adjon meg egy új fájlnevet: ");
        if (scanf("%255s", fajlNev) != 1) {
            printf("Hiba: Érvénytelen fájlnév!\n");
            return 0;
        }
        fajl = fopen(fajlNev, "wb"); // Új fájlnevet próbálunk
    }

    // Asztalok kiírása a fájlba
    Asztal* aktualis = asztalok->elso;
    while (aktualis != NULL) {
        if (fwrite(aktualis, sizeof(Asztal), 1, fajl) != 1) {
            printf("Hiba: Nem sikerült kiírni az adatokat a fájlba.\n");
            fclose(fajl);
            return 0;
        }
        aktualis = aktualis->kov; // Következő asztal
    }
    fclose(fajl);
    printf("Az asztalok adatai sikeresen mentve a '%s' fájlba.\n", fajlNev);
    return 1;
}

// Asztalok betöltése
int asztalokBetoltese(AsztalLista* asztalok) {
    if (asztalok == NULL) {
        printf("Hiba: Az asztalok listája nem inicializált.\n");
        return 0;
    }

    FILE* fajl = fopen(ASZTALOK_FAJL, "rb"); // Fájl megnyitása olvasásra
    if (fajl == NULL) {
        printf("Figyelmeztetés: Az '%s' fájl nem található. Üres asztaladatbázissal indul a program.\n", ASZTALOK_FAJL);
        return 1; // Folytatás üres adatbázissal
    }

    while (1) {
        Asztal temp; // Ideiglenes asztal a beolvasáshoz
        if (fread(&temp, sizeof(Asztal), 1, fajl) != 1) {
            if (feof(fajl)) break; // Elérte a fájl végét
            printf("Figyelmeztetés: Hiba történt az '%s' fájl olvasása közben.\n", ASZTALOK_FAJL);
            fclose(fajl);
            return 0;
        }

        // Asztal hozzáadása a listához
        if (!asztalHozzaad(asztalok, temp.azonosito, temp.ferohely, temp.x, temp.y)) {
            printf("Figyelmeztetés: Érvénytelen vagy ütköző asztal adat a fájlban (azonosító: %d).\n", temp.azonosito);
        } else if (temp.foglalt) {
            asztalFoglal(asztalok, temp.azonosito); // Ha foglalt, foglaljuk le
        }
    }
    fclose(fajl);
    return 1;
}

// Menü mentése
int menuMentese(const MenuLista* menu) {
    if (menu == NULL) {
        printf("Hiba: A menü lista üres.\n");
        return 0;
    }

    char fajlNev[256] = MENU_FAJL; // Alapértelmezett fájlnév
    FILE* fajl = fopen(fajlNev, "wb"); // Fájl megnyitása írásra
    while (fajl == NULL) { // Hibaellenőrzés
        printf("Hiba: Nem sikerült megnyitni a '%s' fájlt írásra.\n", fajlNev);
        printf("Adjon meg egy új fájlnevet: ");
        if (scanf("%255s", fajlNev) != 1) {
            printf("Hiba: Érvénytelen fájlnév!\n");
            return 0;
        }
        fajl = fopen(fajlNev, "wb"); // Új fájlnevet próbálunk
    }

    // Menü méretének írása
    if (fwrite(&menu->meret, sizeof(int), 1, fajl) != 1) {
        printf("Hiba: Nem sikerült kiírni a menü méretét.\n");
        fclose(fajl);
        return 0;
    }

    // Menüelemek kiírása
    MenuElem* aktualis = menu->elso;
    while (aktualis != NULL) {
        size_t nevHossz = strlen(aktualis->nev) + 1; // Név hossza (null terminátorral)
        if (fwrite(&nevHossz, sizeof(size_t), 1, fajl) != 1 ||
            fwrite(aktualis->nev, sizeof(char), nevHossz, fajl) != nevHossz ||
            fwrite(&aktualis->ar, sizeof(int), 1, fajl) != 1) {
            printf("Hiba: Nem sikerült kiírni a menüelemet.\n");
            fclose(fajl);
            return 0;
        }
        aktualis = aktualis->kov; // Következő menüelem
    }
    fclose(fajl);
    printf("A menü adatai sikeresen mentve a '%s' fájlba.\n", fajlNev);
    return 1;
}

// Menü betöltése
int menuBetoltese(MenuLista* menu) {
    if (menu == NULL) {
        printf("Hiba: A menü lista nem inicializált.\n");
        return 0;
    }

    FILE* fajl = fopen(MENU_FAJL, "rb"); // Fájl megnyitása olvasásra
    if (fajl == NULL) {
        printf("Figyelmeztetés: A '%s' fájl nem található. Üres menüvel indul a program.\n", MENU_FAJL);
        return 1; // Folytatás üres menüvel
    }

    int meret;
    if (fread(&meret, sizeof(int), 1, fajl) != 1) {
        printf("Figyelmeztetés: Hiba történt a '%s' fájl olvasása közben.\n", MENU_FAJL);
        fclose(fajl);
        return 0;
    }

    // Menüelemek beolvasása
    for (int i = 0; i < meret; i++) {
        size_t nevHossz;
        if (fread(&nevHossz, sizeof(size_t), 1, fajl) != 1 || nevHossz == 0) {
            printf("Figyelmeztetés: Sérült adat a '%s' fájlban.\n", MENU_FAJL);
            fclose(fajl);
            return 0;
        }

        char* nev = (char*)malloc(nevHossz * sizeof(char)); // Név memóriafoglalása
        if (nev == NULL) {
            printf("Hiba: Memóriafoglalási hiba.\n");
            fclose(fajl);
            return 0;
        }

        // Név beolvasása
        if (fread(nev, sizeof(char), nevHossz, fajl) != nevHossz) {
            printf("Figyelmeztetés: Sérült adat a '%s' fájlban.\n", MENU_FAJL);
            free(nev);
            fclose(fajl);
            return 0;
        }

        int ar;
        if (fread(&ar, sizeof(int), 1, fajl) != 1) {
            printf("Figyelmeztetés: Sérült adat a '%s' fájlban.\n", MENU_FAJL);
            free(nev);
            fclose(fajl);
            return 0;
        }

        // Menüelem hozzáadása
        if (!menuElemHozzaad(menu, nev, ar)) {
            printf("Figyelmeztetés: Nem sikerült hozzáadni a menüelemet (%s).\n", nev);
            free(nev);
            fclose(fajl);
            return 0;
        }
        free(nev);
    }
    fclose(fajl);
    return 1;
}

// Rendelések mentése
int rendelesMentese(const RendelesLista* rendelesek) {
    if (rendelesek == NULL) {
        printf("Hiba: A rendelések listája üres.\n");
        return 0;
    }

    char fajlNev[256] = RENDELESEK_FAJL; // Alapértelmezett fájlnév
    FILE* fajl = fopen(fajlNev, "wb"); // Fájl megnyitása írásra
    while (fajl == NULL) { // Hibaellenőrzés
        printf("Hiba: Nem sikerült megnyitni a '%s' fájlt írásra.\n", fajlNev);
        printf("Adjon meg egy új fájlnevet: ");
        if (scanf("%255s", fajlNev) != 1) {
            printf("Hiba: Érvénytelen fájlnév!\n");
            return 0;
        }
        fajl = fopen(fajlNev, "wb"); // Új fájlnevet próbálunk
    }

    // Rendelések kiírása
    Rendeles* aktualis = rendelesek->elso;
    while (aktualis != NULL) {
        fwrite(&aktualis->asztalAzonosito, sizeof(int), 1, fajl); // Asztal azonosító
        int tetelSzam = 0;

        // Tételek számának meghatározása
        RendelesTetel* tetel = aktualis->tetelek;
        while (tetel != NULL) {
            tetelSzam++;
            tetel = tetel->kov;
        }
        fwrite(&tetelSzam, sizeof(int), 1, fajl); // Tételek számának írása

        // Tételek kiírása
        tetel = aktualis->tetelek;
        while (tetel != NULL) {
            fwrite(&tetel->menuElemAzonosito, sizeof(int), 1, fajl);
            fwrite(&tetel->mennyiseg, sizeof(int), 1, fajl);
            tetel = tetel->kov;
        }
        aktualis = aktualis->kov; // Következő rendelés
    }
    fclose(fajl); // Fájl bezárása
    printf("A rendelések adatai sikeresen mentve a '%s' fájlba.\n", fajlNev);
    return 1;
}

// Rendelések betöltése
int rendelesBetoltese(RendelesLista* rendelesek) {
    if (rendelesek == NULL) {
        printf("Hiba: A rendelések listája nem inicializált.\n");
        return 0;
    }

    FILE* fajl = fopen(RENDELESEK_FAJL, "rb"); // Fájl megnyitása olvasásra
    if (fajl == NULL) {
        printf("Figyelmeztetés: A '%s' fájl nem található. Üres rendelésadatbázissal indul a program.\n", RENDELESEK_FAJL);
        return 1; // Folytatás üres rendelésadatbázissal
    }

    while (1) {
        int asztalAzonosito;
        if (fread(&asztalAzonosito, sizeof(int), 1, fajl) != 1) {
            if (feof(fajl)) break; // Elérte a fájl végét
            printf("Figyelmeztetés: Hiba történt az '%s' fájl olvasása közben.\n", RENDELESEK_FAJL);
            fclose(fajl);
            return 0;
        }

        int tetelSzam;
        if (fread(&tetelSzam, sizeof(int), 1, fajl) != 1) {
            printf("Figyelmeztetés: Sérült adat az '%s' fájlban.\n", RENDELESEK_FAJL);
            fclose(fajl);
            return 0;
        }

        // Rendelés létrehozása
        if (!rendelesHozzaad(rendelesek, asztalAzonosito)) {
            printf("Figyelmeztetés: Nem sikerült létrehozni rendelést az asztalhoz: %d.\n", asztalAzonosito);
            continue;
        }

        // Tételek beolvasása
        Rendeles* rendeles = rendelesKeres(rendelesek, asztalAzonosito);
        for (int i = 0; i < tetelSzam; i++) {
            int menuElemAzonosito, mennyiseg;
            if (fread(&menuElemAzonosito, sizeof(int), 1, fajl) != 1 ||
                fread(&mennyiseg, sizeof(int), 1, fajl) != 1) {
                printf("Figyelmeztetés: Sérült adat az '%s' fájlban.\n", RENDELESEK_FAJL);
                fclose(fajl);
                return 0;
            }
            // Tétel hozzáadása a rendeléshez
            if (!rendelesTetelHozzaad(rendeles, menuElemAzonosito, mennyiseg)) {
                printf("Figyelmeztetés: Nem sikerült hozzáadni tételt a rendeléshez (asztal: %d).\n", asztalAzonosito);
            }
        }
    }
    fclose(fajl); // Fájl bezárása
    return 1;
}
