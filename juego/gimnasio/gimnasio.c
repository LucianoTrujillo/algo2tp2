#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h> //para obtener dimension de la consola
#include "../juego.h"
#include "gimnasio.h"
#include "../interfaz/interfaz.h"
#define MAX_RESERVA 1000
#define SIN_POSICION -1
#define EXITO 0
#define ERROR -1

typedef enum {
  PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_COMBATE,
  PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_RESERVA,
  PANTALLA_INTERCAMBIANDO_POKEMON,
  VOLVER,
  CANT_PANTALLAS } pantalla_t;
typedef pantalla_t funcion_pantalla(juego_t* juego, int* posicion_combate, char nombre_reserva[MAX_NOMBRE]);

pantalla_t poke_combate(juego_t* juego, int* posicion_combate, char nombre_reserva[MAX_NOMBRE]){
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

  *posicion_combate = pos_pokemon_remover;
  return PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_RESERVA;
}
#define ANCHO_TABLA 82
#define MAX_ANCHO_NOMBRE 30
#define MAX_ANCHO_ATRIBUTO 15
#define PADDING 2

void imprimir_pokemones(pokemon_t* pokemones[MAX_RESERVA], size_t cantidad){
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  int ancho_media_pantalla = (int)((w.ws_col - ANCHO_TABLA) / 2);
  int alto_media_pantalla = (int)(w.ws_row - (cantidad)) / 2;

  if(!pokemones){
      return;
  }

  for(int i = 0; i < alto_media_pantalla; i++) printf("\n");

  printf("%*s", ancho_media_pantalla, " ");
  printf("╔══════════════════════════════════════════════════════════════════════════════╗\n");
  printf("%*s", ancho_media_pantalla, " ");
  printf("║%-*s %-*s %-*s %-*s║\n",
  MAX_ANCHO_NOMBRE, "Nombre",
  MAX_ANCHO_ATRIBUTO, "Velocidad",
  MAX_ANCHO_ATRIBUTO, "Ataque",
  MAX_ANCHO_ATRIBUTO, "Defensa");
  printf("%*s", ancho_media_pantalla, " ");
  printf("║══════════════════════════════════════════════════════════════════════════════║\n");
  for(int i = 0; i < cantidad; i++){
    printf("%*s", ancho_media_pantalla, " ");
    printf("║%-*s %-*i %-*i %-*i║\n",
    MAX_ANCHO_NOMBRE, pokemones[i]->nombre,
    MAX_ANCHO_ATRIBUTO, pokemones[i]->velocidad,
    MAX_ANCHO_ATRIBUTO, pokemones[i]->ataque,
    MAX_ANCHO_ATRIBUTO, pokemones[i]->defensa);
  }
  printf("%*s", ancho_media_pantalla, " ");
  printf("╚══════════════════════════════════════════════════════════════════════════════╝\n");

  for(int i = 0; i < alto_media_pantalla; i++) printf("\n");
}

pantalla_t poke_reserva(juego_t* juego, int* posicion_combate, char nombre_reserva[MAX_NOMBRE]){
  pokemon_t* pokemones_reserva[MAX_RESERVA];
  size_t cantidad = arbol_recorrido_inorden(juego->personaje.pokemones_reserva, (void**)pokemones_reserva, MAX_RESERVA);

  imprimir_pokemones(pokemones_reserva, cantidad);

  pokemon_t* pokemon_elegido = NULL;

  while(!pokemon_elegido){
    printf("\nIngresa 'V' si queres volver a elegir el pokemon intercambiado de combate.\n");
    printf("Ingresa el nombre del pokemon que quieras usar para combater: ");
    scanf("%s", nombre_reserva);
    while ((getchar()) != '\n');

    if(strcmp(nombre_reserva, "V") == 0){
      *posicion_combate = SIN_POSICION;
      return PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_COMBATE;
    } else {
      pokemon_t comparador;
      strcpy(comparador.nombre, nombre_reserva);
      pokemon_elegido = arbol_buscar(juego->personaje.pokemones_reserva, &comparador);
      if(!pokemon_elegido)
        printf("\nEse pokemon no se encuentra dentro del conjunto!\n");
    }
  }
  
  return PANTALLA_INTERCAMBIANDO_POKEMON;
}

pantalla_t poke_intercambio(juego_t* juego, int* posicion_combate, char nombre_reserva[MAX_NOMBRE]){
  imprimir_consola("realizando intercambio.....");
  size_t pos_combate = (size_t)(*posicion_combate);
  lista_t* pokemones_combate = juego->personaje.pokemones_combate;
  abb_t* pokemones_reserva = juego->personaje.pokemones_reserva;

  pokemon_t* pokemon_combate = lista_elemento_en_posicion(pokemones_combate, pos_combate);
  if(!pokemon_combate){
    imprimir_consola("no se pudo realizar el intercambio");
    return VOLVER;
  }

  pokemon_t* pokemon_reserva;
  pokemon_t comparador;
  strcpy(comparador.nombre, nombre_reserva);
  pokemon_reserva = arbol_buscar(juego->personaje.pokemones_reserva, &comparador);
  if(!pokemon_reserva){
    imprimir_consola("no se pudo realizar el intercambio");
    return VOLVER;
  }

  lista_borrar_de_posicion(pokemones_combate, pos_combate);
  arbol_borrar(pokemones_reserva, &comparador, false);

  if(arbol_insertar(pokemones_reserva, pokemon_combate) == EXITO && 
    lista_insertar_en_posicion(pokemones_combate, pokemon_reserva, pos_combate) == EXITO){
      imprimir_consola("intercambio realizado exitosamente");
    } else {
      imprimir_consola("no se pudo realizar el intercambio");
    }

  return VOLVER;
}

funcion_pantalla* const nuevo_pantalla[CANT_PANTALLAS] = {
    poke_combate,
    poke_reserva,
    poke_intercambio,
};

pantalla_t mostrar_pantalla(pantalla_t pantalla_actual, juego_t *juego, int* posicion_combate, char nombre_reserva[MAX_NOMBRE]) {
    return nuevo_pantalla[pantalla_actual](juego, posicion_combate, nombre_reserva);
}

int cambiar_pokedex(juego_t* juego){
  pantalla_t pantalla_actual = PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_COMBATE;
  int posicion_pokemon_combate_intercambiar = SIN_POSICION;
  char nombre_pokemon_reserva_intercambiar[MAX_NOMBRE] = "\0";

  while(pantalla_actual != VOLVER){
    pantalla_actual = mostrar_pantalla(pantalla_actual, juego, &posicion_pokemon_combate_intercambiar, nombre_pokemon_reserva_intercambiar);
  }
  return 0;
}

int ver_personaje(juego_t* juego){
  return 0;
}
 
int ver_gimnasio(juego_t* juego){
  return 0;
}
