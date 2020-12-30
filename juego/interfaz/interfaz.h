//
// Created by luciano on 22/12/20.
//

#ifndef TP2_INTERFAZ_H
#define TP2_INTERFAZ_H

#define MAX_OPCIONES 10

void mostrar_opciones(char* opciones[MAX_OPCIONES], int cantidad_opciones, int opcion_seleccionada);
int elegir_opcion(char* opciones[MAX_OPCIONES], int cantidad_opciones, int opcion_seleccionada);
void mostrar_mensaje_fluido(char* mensaje);
#endif /*TP2_INTERFAZ_H*/
