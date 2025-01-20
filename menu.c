#include "menu.h"

MenuLista* menuListaLetrehozas(void) {
    MenuLista* lista = (MenuLista*)malloc(sizeof(MenuLista));
    if (lista == NULL) return NULL;
    lista->elso = NULL;
    lista->meret = 0;
    return lista;
}

void menuListaFelszabadit(MenuLista* lista) {
    if (lista == NULL) return;
    MenuElem* aktualis = lista->elso;
    while (aktualis != NULL) {
        MenuElem* kov = aktualis->kov;
        free(aktualis->nev); // Memória felszabadítása az étel nevéhez
        free(aktualis);
        aktualis = kov;
    }
    free(lista); // Felszabadítjuk a lista struktúrát
}

MenuElem* menuElemKeres(const MenuLista* lista, int azonosito) {
    if (lista == NULL || azonosito <= 0) return NULL;
    MenuElem* aktualis = lista->elso;
    int i = 1;
    while (aktualis != NULL) {
        if (i == azonosito) return aktualis;
        aktualis = aktualis->kov;
        i++;
    }
    return NULL; // Nincs ilyen azonosító
}

int menuElemHozzaad(MenuLista* lista, const char* nev, int ar) {
    if (lista == NULL || nev == NULL || ar <= 0) return 0;

    MenuElem* uj = (MenuElem*)malloc(sizeof(MenuElem));
    if (uj == NULL) return 0;

    size_t nevHossz = strlen(nev) + 1;
    uj->nev = (char*)malloc(nevHossz * sizeof(char));
    if (uj->nev == NULL) {
        free(uj);
        return 0;
    }

    strncpy(uj->nev, nev, nevHossz - 1);
    uj->nev[nevHossz - 1] = '\0';

    uj->ar = ar;
    uj->kov = lista->elso;
    lista->elso = uj;
    lista->meret++;
    return 1; // Sikeres hozzáadás
}

int menuElemTorol(MenuLista* lista, int azonosito) {
    if (lista == NULL || lista->elso == NULL || azonosito <= 0) return 0;

    MenuElem* elozo = NULL;
    MenuElem* aktualis = lista->elso;
    int i = 1;

    while (aktualis != NULL && i != azonosito) {
        elozo = aktualis;
        aktualis = aktualis->kov;
        i++;
    }

    if (aktualis == NULL) return 0; // Nincs mit törölni

    if (elozo == NULL) {
        lista->elso = aktualis->kov; // Az első elem törlése
    } else {
        elozo->kov = aktualis->kov; // Törlés a listából
    }

    free(aktualis->nev); // Memória felszabadítása
    free(aktualis);
    lista->meret--;
    return 1; // Sikeres törlés
}

int menuElemModosit(MenuLista* lista, int azonosito, const char* ujNev, int ujAr) {
    if (lista == NULL || ujNev == NULL || ujAr <= 0 || azonosito <= 0) return 0;

    MenuElem* elem = menuElemKeres(lista, azonosito);
    if (elem == NULL) return 0;

    size_t ujNevHossz = strlen(ujNev) + 1;
    char* ujNevMemoria = (char*)malloc(ujNevHossz * sizeof(char));
    if (ujNevMemoria == NULL) return 0;

    strncpy(ujNevMemoria, ujNev, ujNevHossz - 1);
    ujNevMemoria[ujNevHossz - 1] = '\0';

    free(elem->nev); // Régi név felszabadítása
    elem->nev = ujNevMemoria;
    elem->ar = ujAr;
    return 1; // Sikeres módosítás
}

void menuListaz(const MenuLista* lista) {
    if (lista == NULL) {
        printf("A menü nem létezik!\n");
        return;
    }
    if (lista->elso == NULL) {
        printf("A menü üres.\n");
        return;
    }

    printf("\nÉtlap:\n");
    printf("----------------------------------------\n");
    printf("%-3s | %-40s | %s\n", "ID", "Név", "Ár");
    printf("----------------------------------------\n");

    MenuElem* aktualis = lista->elso;
    int id = 1;
    while (aktualis != NULL) {
        printf("%-3d | %-40s | %d Ft\n", id, aktualis->nev, aktualis->ar);
        id++;
        aktualis = aktualis->kov;
    }
    printf("----------------------------------------\n");
}

void menuKezelese(MenuLista* lista) {
    int valasztas;
    char* nev = NULL;
    size_t bufferMeret = 0;

    do {
        printf("\nÉtlap kezelése:\n");
        printf("1. Új étel hozzáadása\n");
        printf("2. Étel törlése\n");
        printf("3. Étel módosítása\n");
        printf("4. Étlap listázása\n");
        printf("5. Vissza\n");

        valasztas = bekerSzam("Válasszon", 1, 5);

        switch (valasztas) {
            case 1:
                printf("Étel neve: ");
                if (getline(&nev, &bufferMeret, stdin) != -1) {
                    size_t len = strlen(nev);
                    if (len > 0 && nev[len-1] == '\n') {
                        nev[len-1] = '\0'; // Új sor karakter eltávolítása
                    }
                    int ar = bekerSzam("Ár (Ft)", 1, 99999);
                    if (menuElemHozzaad(lista, nev, ar)) {
                        printf("Étel sikeresen hozzáadva!\n");
                    }
                }
                break;

            case 2:
                menuListaz(lista);
                if (lista->meret > 0) {
                    int azonosito = bekerSzam("Törlendő azonosító", 1, lista->meret);
                    if (menuElemTorol(lista, azonosito)) {
                        printf("Étel sikeresen törölve!\n");
                    }
                }
                break;

            case 3:
                menuListaz(lista);
                if (lista->meret > 0) {
                    int modAzonosito = bekerSzam("Módosítandó azonosító", 1, lista->meret);
                    MenuElem* elem = menuElemKeres(lista, modAzonosito);
                    if (elem != NULL) {
                        printf("Új név (Enter az eredeti megtartásához): ");
                        if (getline(&nev, &bufferMeret, stdin) != -1) {
                            if (strlen(nev) > 0 && nev[strlen(nev)-1] == '\n') {
                                nev[strlen(nev)-1] = '\0'; // Új sor karakter eltávolítása
                            }
                            const char* modNev = (strlen(nev) > 0) ? nev : elem->nev;
                            int ujAr = bekerSzam("Új ár (Ft)", 1, 99999);
                            if (menuElemModosit(lista, modAzonosito, modNev, ujAr)) {
                                printf("Étel sikeresen módosítva!\n");
                            }
                        }
                    }
                }
                break;

            case 4:
                menuListaz(lista);
                break;
        }
    } while (valasztas != 5);
}
