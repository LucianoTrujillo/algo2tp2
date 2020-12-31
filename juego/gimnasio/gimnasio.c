#include "../juego.h"
#include "gimnasio.h"
#include "../interfaz/interfaz.h"

#define SIN_POSICION -1

typedef enum {
  PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_COMBATE,
  PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_RESERVA,
  PANTALLA_INTERCAMBIANDO_POKEMON,
  VOLVER,
  CANT_PANTALLAS } pantalla_t;
typedef pantalla_t funcion_pantalla(juego_t* juego, int* posicion_batalla, char nombre_reserva[MAX_NOMBRE]);

pantalla_t poke_combate(juego_t* juego, int* posicion_batalla, char nombre_reserva[MAX_NOMBRE]){
  lista_t* pokemones_combate = juego->personaje.pokemones_combate;
  char* nombres_pokemones[MAX_POKEMONES_BATALLA + 1];
  int i = 0;

  lista_iterador_t* iterador = lista_iterador_crear(pokemones_combate);
  while(lista_iterador_tiene_siguiente(iterador)){
    pokemon_t* pokemon_actual = (pokemon_t*)lista_iterador_elemento_actual(iterador);
    nombres_pokemones[i++] = pokemon_actual->nombre;
    lista_iterador_avanzar(iterador);
  }

  nombres_pokemones[i] = "Volver";
  lista_iterador_destruir(iterador);

  int pos_pokemon_remover = elegir_opcion(
        "ELEGI AL POKEMON QUE QUIERAS REMOVER DE BATALLA",
        nombres_pokemones,
        (int)lista_elementos(juego->personaje.pokemones_combate) + 1,
        0);
  
  if(pos_pokemon_remover == i){
    return VOLVER;
  }

  *posicion_batalla = pos_pokemon_remover;
  return PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_RESERVA;
}

pantalla_t poke_reserva(juego_t* juego, int* posicion_batalla, char nombre_reserva[MAX_NOMBRE]){
  /*
  Imprimir pokemones de reserva
  dejar elegir uno o volver pa tras o salir
  */
 imprimir_consola("mostrando reserva...");
 return PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_COMBATE;
}

pantalla_t poke_intercambio(juego_t* juego, int* posicion_batalla, char nombre_reserva[MAX_NOMBRE]){
  return VOLVER;
}

funcion_pantalla* const nuevo_pantalla[CANT_PANTALLAS] = {
    poke_combate,
    poke_reserva,
    poke_intercambio,
};

pantalla_t mostrar_pantalla(pantalla_t pantalla_actual, juego_t *juego, int* posicion_batalla, char nombre_reserva[MAX_NOMBRE]) {
    return nuevo_pantalla[pantalla_actual](juego, posicion_batalla, nombre_reserva);
}

int cambiar_pokedex(juego_t* juego){
  pantalla_t pantalla_actual = PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_COMBATE;
  int posicion_pokemon_batalla_intercambiar = SIN_POSICION;
  char nombre_pokemon_reserva_intercambiar[MAX_NOMBRE] = "\0";

  while(pantalla_actual != VOLVER){
    mostrar_pantalla(pantalla_actual, juego, &posicion_pokemon_batalla_intercambiar, nombre_pokemon_reserva_intercambiar);
  }
  return 0;
}

int ver_personaje(juego_t* juego){
  return 0;
}
 
int ver_gimnasio(juego_t* juego){
  return 0;
}
