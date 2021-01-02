//
// Created by luciano on 22/12/20.
//

#ifndef TP2_INTERFAZ_H
#define TP2_INTERFAZ_H

#define MAX_OPCIONES 10
#define MAX_MENSAJE 200
#define LINEAS_DISPLAY 17
#define MAX_CARACTERES_LINEA_DISPLAY 55

#define TITULO "\x1b[37;4m"
#define CERRAR_TITULO "\x1b[37;0m"
#define AMARILLO "\x1b[33;1m"
#define BLANCO "\x1b[37;1m"

int elegir_opcion(char* menu, char* opciones[MAX_OPCIONES], int cantidad_opciones, int opcion_seleccionada);
void imprimir_consola(char* mensaje);
void imprimir_consola_inmediato(char* mensaje);

#endif /*TP2_INTERFAZ_H*/
