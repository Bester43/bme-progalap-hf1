#include "utils.h"

// Bemeneti puffer tisztítása
void inputTisztitas(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Szám bekérése a megadott tartományban
int bekerSzam(const char* uzenet, int min, int max) {
    int szam;
    do {
        printf("%s (%d-%d): ", uzenet, min, max);
        if (scanf("%d", &szam) != 1) {
            inputTisztitas();
            continue;
        }
        inputTisztitas();
    } while (szam < min || szam > max);
    return szam;
}

// Koordináták érvényességének ellenőrzése
bool koordinatakErvenyesekE(int x, int y, int maxX, int maxY) {
    return (x >= 0 && x < maxX && y >= 0 && y < maxY);
}
