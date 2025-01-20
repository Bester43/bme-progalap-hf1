#include "asztalfoglalas.h"

int automatikusAsztalFoglalas(AsztalLista* asztalok, int vendegekSzama) {
    if (asztalok == NULL || vendegekSzama <= 0) {
        printf("Érvénytelen paraméterek!\n");
        return 0;
    }

    // Best fit keresés: a legkisebb, még elegendő méretű asztalt választjuk
    // hogy elkerüljük a nagy asztalok pazarlását kis csoportokra
    Asztal* aktualis = asztalok->elso;
    Asztal* legjobbAsztal = NULL;
    int legkisebbKulonbseg = INT_MAX;

    while (aktualis != NULL) {
        if (!aktualis->foglalt && aktualis->ferohely >= vendegekSzama) {
            int kulonbseg = aktualis->ferohely - vendegekSzama;
            if (kulonbseg < legkisebbKulonbseg) {
                legkisebbKulonbseg = kulonbseg;
                legjobbAsztal = aktualis;
            }
        }
        aktualis = aktualis->kov;
    }

    if (legjobbAsztal != NULL) {
        legjobbAsztal->foglalt = 1;
        printf("Automatikus asztalfoglalás: %d. asztal (%d fő)\n",
               legjobbAsztal->azonosito, legjobbAsztal->ferohely);
        return legjobbAsztal->azonosito;
    }

    printf("Nincs megfelelő méretű szabad asztal %d fő részére!\n", vendegekSzama);
    return 0;
}

int asztalFoglalas(AsztalLista* asztalok, int asztalAzonosito) {
    if (asztalok == NULL) {
        printf("Érvénytelen asztal lista!\n");
        return 0;
    }

    // Asztal keresése azonosító alapján a láncolt listában
    Asztal* asztal = asztalKeres(asztalok, asztalAzonosito);
    if (asztal == NULL) {
        printf("Nem létező asztal azonosító: %d\n", asztalAzonosito);
        return 0;
    }

    if (asztal->foglalt) {
        printf("Az asztal már foglalt!\n");
        return 0;
    }

    asztal->foglalt = 1;
    printf("A(z) %d. asztal sikeresen lefoglalva.\n", asztalAzonosito);
    return 1;
}

int asztalFelszabaditas(AsztalLista* asztalok, RendelesLista* rendelesek, int asztalAzonosito) {
    if (asztalok == NULL || asztalAzonosito <= 0) {
        return 0;
    }

    // Először az asztalt szabadítjuk fel egy külön függvénnyel
    int eredmeny = asztalFelszabadit(asztalok, asztalAzonosito);
    if (!eredmeny) {
        return 0;
    }

    // Memória felszabadítás kétszintű láncolt listában:
    // 1. Rendelés tételek (belső lista)
    // 2. Maga a rendelés (külső lista)
    if (rendelesek != NULL) {
        Rendeles* elozo = NULL;
        Rendeles* aktualis = rendelesek->elso;

        while (aktualis != NULL) {
            if (aktualis->asztalAzonosito == asztalAzonosito) {
                // 1. Belső lista (tételek) felszabadítása
                RendelesTetel* aktualisTetel = aktualis->tetelek;
                while (aktualisTetel != NULL) {
                    RendelesTetel* kovTetel = aktualisTetel->kov;
                    free(aktualisTetel);
                    aktualisTetel = kovTetel;
                }

                // 2. Külső lista (rendelés) elem törlése
                if (elozo == NULL) {
                    rendelesek->elso = aktualis->kov;  // Lista elejéről törlés
                } else {
                    elozo->kov = aktualis->kov;  // Lista közepéről/végéről törlés
                }

                Rendeles* torlendo = aktualis;
                aktualis = aktualis->kov;
                free(torlendo);
                rendelesek->meret--;
            } else {
                elozo = aktualis;
                aktualis = aktualis->kov;
            }
        }
    }

    return 1;
}

void asztalFoglalasKezeles(AsztalLista* asztalok, RendelesLista* rendelesek) {
    int valasztas;
    do {
        printf("\nFoglalások kezelése:\n");
        printf("1. Automatikus asztalfoglalás\n");
        printf("2. Manuális asztalfoglalás\n");
        printf("3. Asztal felszabadítása\n");
        printf("4. Vissza\n");
        printf("Válasszon (1-4): ");
        if (scanf("%d", &valasztas) != 1) {
            printf("Érvénytelen bemenet!\n");
            while (getchar() != '\n');  // Input buffer tisztítása a következő beolvasás előtt
            continue;
        }
        switch (valasztas) {
            case 1: {
                int vendegekSzama;
                printf("Vendégek száma: ");
                if (scanf("%d", &vendegekSzama) == 1) {
                    automatikusAsztalFoglalas(asztalok, vendegekSzama);
                } else {
                    printf("Érvénytelen szám!\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 2: {
                int asztalAzonosito;
                asztalListaz(asztalok);
                printf("Asztal azonosító: ");
                if (scanf("%d", &asztalAzonosito) == 1) {
                    asztalFoglalas(asztalok, asztalAzonosito);
                } else {
                    printf("Érvénytelen azonosító!\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 3: {
                int asztalAzonosito;
                asztalListaz(asztalok);
                printf("Asztal azonosító: ");
                if (scanf("%d", &asztalAzonosito) == 1) {
                    if (asztalFelszabaditas(asztalok, rendelesek, asztalAzonosito)) {
                        printf("Asztal sikeresen felszabadítva!\n");
                    } else {
                        printf("Az asztal felszabadítása sikertelen!\n");
                    }
                } else {
                    printf("Érvénytelen azonosító!\n");
                    while (getchar() != '\n');
                }
                break;
            }
            case 4:
                printf("Visszalépés a főmenübe...\n");
                break;
            default:
                printf("Érvénytelen választás!\n");
        }
    } while (valasztas != 4);
}
