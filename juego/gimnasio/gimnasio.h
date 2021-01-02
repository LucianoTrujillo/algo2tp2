#ifndef __GIMNASIO_H_
#define __GIMNASIO_H_
#include "../juego.h"
#define SIN_ID -1

int ver_personaje(juego_t* juego);
int ver_gimnasio(juego_t* juego);
int cambiar_pokedex(juego_t* juego);
size_t llenar_vector_pokemon_lista(lista_t* lista, pokemon_t* pokemones[MAX_POKEMONES]);
void imprimir_pokemones(pokemon_t* pokemones[MAX_POKEMONES], size_t cantidad);
int posicion_pokemon_con_id(lista_t* pokemones, int id);
#endif /* __GIMNASIO_H_ */
