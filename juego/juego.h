#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <stdlib.h>
#include <stdbool.h>
#include "lista/lista.h"
#include "arbol/abb.h"
#include "heap/heap.h"

#define MAX_POKEMONES 1000
#define MAX_ENTRENADORES 1000

#define MAX_NOMBRE 100
#define PUNTOS_RECOMPENSA_COMBATE 1
#define MAX_PUNTOS 63
#define MAX_POKEMONES_BATALLA 6

typedef struct pokemon{
  char nombre[MAX_NOMBRE];
  int velocidad;
  int defensa;
  int ataque;
  int id;
} pokemon_t;

typedef struct entrenador {
  char nombre[MAX_NOMBRE];
  lista_t* pokemones;
} entrenador_t;

typedef struct gimnasio {
  char nombre[MAX_NOMBRE];
  size_t dificultad;
  size_t batalla_id;
  entrenador_t lider;
  lista_t* entrenadores;
} gimnasio_t;

typedef struct personaje {
  char nombre[MAX_NOMBRE];
  abb_t* pokemones_reserva;
  lista_t* pokemones_combate;
} personaje_t;

typedef struct juego {
  heap_t* gimnasios;
  personaje_t personaje;
  bool simulacion;
  bool tomo_prestado_pokemon;
} juego_t;

int jugar();
void destruir_pokemon(void* pokemon);
void destruir_entrenador(void* trainer);
void destruir_gimnasio(void* gim);
int comparar_pokemones(void* pokemon_1, void* pokemon_2);
int comparar_gimnasios(void* gimnasio_1, void* gimnasio_2);

#endif /* __JUEGO_H__ */
