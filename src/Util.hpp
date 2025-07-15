#pragma once


static void SwapInt(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}