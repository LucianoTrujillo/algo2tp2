#ifndef __INICIO_H_
#define __INICIO_H_
#include "../juego.h"

#define MAX_NOMBRE_ARCHIVO 100
#define FORMATO_IDENTIFICADOR_LINEA "%c;"
#define FORMATO_NOMBRE "%100[^\n]\n"
#define FORMATO_NOMBRE_PERSONAJE "%c;%100[^\n]\n"
#define FORMATO_POKEMON "%100[^;];%i;%i;%i\n"
#define FORMATO_POKEMON_PERSONAJE "%c;%100[^;];%i;%i;%i\n"
#define MODO_LECTURA "r"
#define CHAR_ENTRENADOR 'E'
#define CHAR_POKEMON 'P'
#define CANTIDAD_LEIDOS_NOMBRE 1
#define CANTIDAD_LEIDOS_NOMBRE_PERSONAJE 2
#define CANTIDAD_LEIDOS_POKEMON_PERSONAJE 5
#define CANTIDAD_LEIDOS_POKEMON 4
#define LECTURA_INCOMPLETA 2
#define ERROR -1
#define EXITO 0

#define FORMATO_GIMNASIO "%100[^;];%lu;%lu\n"
#define CHAR_LIDER 'L'
#define CHAR_GIMNASIO 'G'
#define CANTIDAD_LEIDOS_GIMNASIO 3

int actualizar_personaje(juego_t* juego);
int agregar_gimnasio(juego_t* juego);
#endif /* __INICIO_H_ */
