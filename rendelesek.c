#include "rendelesek.h"

RendelesLista* rendelesListaLetrehozas(void) {
    RendelesLista* lista = (RendelesLista*)malloc(sizeof(RendelesLista));
    if (lista == NULL) return NULL;

    lista->elso = NULL;
    lista->meret = 0;
    return lista;
}

void rendelesListaFelszabadit(RendelesLista* lista) {
    if (lista == NULL) return;

    Rendeles* aktualis = lista->elso;
    while (aktualis != NULL) {
        Rendeles* kov = aktualis->kov;

        // Tételek felszabadítása
        RendelesTetel* tetel = aktualis->tetelek;
        while (tetel != NULL) {
            RendelesTetel* kovTetel = tetel->kov;
            free(tetel);
            tetel = kovTetel;
        }

        free(aktualis);
        aktualis = kov;
    }

    free(lista);
}

Rendeles* rendelesKeres(RendelesLista* lista, int asztalAzonosito) {
    if (lista == NULL) return NULL;

    Rendeles* aktualis = lista->elso;
    while (aktualis != NULL) {
        if (aktualis->asztalAzonosito == asztalAzonosito)
            return aktualis;
        aktualis = aktualis->kov;
    }
    return NULL; // Nincs ilyen rendelés
}

int rendelesHozzaad(RendelesLista* lista, int asztalAzonosito) {
    if (lista == NULL) return 0;

    // Ha már van rendelés az asztalnál
    if (rendelesKeres(lista, asztalAzonosito) != NULL)
        return 0;

    Rendeles* uj = (Rendeles*)malloc(sizeof(Rendeles));
    if (uj == NULL) return 0;

    uj->asztalAzonosito = asztalAzonosito;
    uj->tetelek = NULL;
    uj->kov = lista->elso;
    lista->elso = uj;
    lista->meret++;

    return 1; // Sikeres hozzáadás
}

int rendelesTetelHozzaad(Rendeles* rendeles, int menuElemAzonosito, int mennyiseg) {
    if (rendeles == NULL || mennyiseg <= 0) return 0;

    // Ha már létezik a tétel, növeljük a mennyiséget
    RendelesTetel* tetel = rendeles->tetelek;
    while (tetel != NULL) {
        if (tetel->menuElemAzonosito == menuElemAzonosito) {
            tetel->mennyiseg += mennyiseg;
            return 1; // Mennyiség növelve
        }
        tetel = tetel->kov;
    }

    // Új tétel hozzáadása
    RendelesTetel* uj = (RendelesTetel*)malloc(sizeof(RendelesTetel));
    if (uj == NULL) return 0;

    uj->menuElemAzonosito = menuElemAzonosito;
    uj->mennyiseg = mennyiseg;
    uj->kov = rendeles->tetelek;
    rendeles->tetelek = uj;

    return 1; // Sikeres hozzáadás
}

int rendelesTetelModosit(Rendeles* rendeles, int menuElemAzonosito, int ujMennyiseg) {
    if (rendeles == NULL) return 0;
    if (ujMennyiseg <= 0) {
        return rendelesTetelTorol(rendeles, menuElemAzonosito);
    }

    RendelesTetel* tetel = rendeles->tetelek;
    while (tetel != NULL) {
        if (tetel->menuElemAzonosito == menuElemAzonosito) {
            tetel->mennyiseg = ujMennyiseg;
            return 1; // Mennyiség módosítva
        }
        tetel = tetel->kov;
    }

    // Új tétel hozzáadása, ha nem létezik
    RendelesTetel* uj = (RendelesTetel*)malloc(sizeof(RendelesTetel));
    if (uj == NULL) return 0;

    uj->menuElemAzonosito = menuElemAzonosito;
    uj->mennyiseg = ujMennyiseg;
    uj->kov = rendeles->tetelek;
    rendeles->tetelek = uj;

    return 1; // Sikeres hozzáadás
}

int rendelesTetelTorol(Rendeles* rendeles, int menuElemAzonosito) {
    if (rendeles == NULL) return 0;

    RendelesTetel* elozo = NULL;
    RendelesTetel* aktualis = rendeles->tetelek;

    while (aktualis != NULL) {
        if (aktualis->menuElemAzonosito == menuElemAzonosito) {
            if (elozo == NULL) {
                rendeles->tetelek = aktualis->kov; // Az első tétel törlése
            } else {
                elozo->kov = aktualis->kov; // Törlés a listából
            }
            free(aktualis);
            return 1; // Sikeres törlés
        }
        elozo = aktualis;
        aktualis = aktualis->kov;
    }
    return 0; // Nincs mit törölni
}

void rendelesListaz(const RendelesLista* lista, const MenuLista* menu) {
    if (lista == NULL || menu == NULL) return;

    printf("\nAktív rendelések:\n");
    Rendeles* rendeles = lista->elso;
    while (rendeles != NULL) {
        printf("\nAsztal %d:\n", rendeles->asztalAzonosito);
        printf("----------------------------------------\n");

        RendelesTetel* tetel = rendeles->tetelek;
        while (tetel != NULL) {
            MenuElem* menuElem = menuElemKeres(menu, tetel->menuElemAzonosito);
            if (menuElem != NULL) {
                printf("%-40s %3d db\n", menuElem->nev, tetel->mennyiseg);
            }
            tetel = tetel->kov;
        }

        printf("----------------------------------------\n");
        rendeles = rendeles->kov;
    }
}

void rendelesekKezelese(RendelesLista* lista, AsztalLista* asztalok, MenuLista* menu) {
    int valasztas;
    do {
        printf("\nRendelések kezelése:\n");
        printf("1. Új rendelés felvétele\n");
        printf("2. Rendelés módosítása\n");
        printf("3. Rendelések listázása\n");
        printf("4. Vissza\n");
        printf("Válasszon (1-4): ");

        if (scanf("%d", &valasztas) != 1) {
            printf("Érvénytelen bemenet!\n");
            while (getchar() != '\n'); // Bemenet tisztítása
            continue;
        }

        switch (valasztas) {
            case 1: {
                printf("Asztal azonosítója: ");
                int asztalId;
                scanf("%d", &asztalId);

                Asztal* asztal = asztalKeres(asztalok, asztalId);
                if (asztal == NULL || !asztal->foglalt) {
                    printf("Az asztal nem létezik vagy nincs foglalva!\n");
                    break;
                }

                if (rendelesHozzaad(lista, asztalId)) {
                    Rendeles* rendeles = rendelesKeres(lista, asztalId);
                    if (rendeles != NULL) {
                        do {
                            menuListaz(menu);
                            printf("\nVálasszon ételt (0 = befejezés): ");
                            int menuId;
                            scanf("%d", &menuId);

                            if (menuId == 0) break;

                            if (menuElemKeres(menu, menuId) == NULL) {
                                printf("Érvénytelen menü azonosító!\n");
                                continue;
                            }

                            printf("Mennyiség: ");
                            int mennyiseg;
                            scanf("%d", &mennyiseg);

                            if (rendelesTetelHozzaad(rendeles, menuId, mennyiseg)) {
                                printf("Tétel hozzáadva!\n");
                            } else {
                                printf("Hiba a tétel hozzáadásakor!\n");
                            }
                        } while (1);
                    }
                } else {
                    printf("Hiba a rendelés létrehozásakor!\n");
                }
                break;
            }
            case 2: {
                printf("Módosítandó rendelés asztal azonosítója: ");
                int asztalId;
                scanf("%d", &asztalId);

                Rendeles* rendeles = rendelesKeres(lista, asztalId);
                if (rendeles == NULL) {
                    printf("Nincs rendelés ennél az asztalnál!\n");
                    break;
                }

                menuListaz(menu);
                printf("\nMódosítandó tétel azonosítója: ");
                int menuId;
                scanf("%d", &menuId);

                printf("Új mennyiség (0 = törlés): ");
                int ujMennyiseg;
                scanf("%d", &ujMennyiseg);

                if (rendelesTetelModosit(rendeles, menuId, ujMennyiseg)) {
                    printf("Tétel módosítva!\n");
                } else {
                    printf("Hiba a tétel módosításakor!\n");
                }
                break;
            }
            case 3:
                rendelesListaz(lista, menu);
                break;
            case 4:
                printf("Visszalépés a főmenübe...\n");
                break;
            default:
                printf("Érvénytelen választás!\n");
        }
    } while (valasztas != 4);
}
