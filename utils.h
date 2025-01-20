#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>

// Általános segédfüggvények
void inputTisztitas(void); // Bemeneti puffer tisztítása
int bekerSzam(const char* uzenet, int min, int max); // Szám bekérése
bool koordinatakErvenyesekE(int x, int y, int maxX, int maxY); // Koordináták érvényességének ellenőrzése

#endif
