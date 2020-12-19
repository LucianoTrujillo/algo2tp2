#include <stdio.h>
#include "pa2mm.h"

/*
 * Pruebas del TP2
 * Primero se ejecutan las pruebas unitarias del heap
 * Luego, se ejecutan las pruebas de integración del programa.
*/
void test() {
    pa2m_nuevo_grupo("Se testea iinsta");
    pa2m_afirmar(4 < 5, "3 es menor que 5")
    pa2m_mostrar_reporte();
}

int main() {
    test();
    printf("Hello, World!\n");
    return 0;
}
