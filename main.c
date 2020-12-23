#include <stdio.h>
#include "pa2mm.h"
#include "interfaz/interfaz.h"

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
    char* opciones[10] = {"holaaaaa", "como", "va"};
    mostrar_opciones(opciones, 3, 1);
    return 0;
}
