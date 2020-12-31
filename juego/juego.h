#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <stdlib.h>
#include <stdbool.h>
#include "lista/lista.h"
#include "arbol/abb.h"
#include "heap/heap.h"

#define MAX_NOMBRE 100
#define PUNTOS_RECOMPENSA_COMBATE 1
#define MAX_PUNTOS 63

typedef int (*funcion_batalla)(void*, void*);

typedef struct pokemon{
  char nombre[MAX_NOMBRE];
  int velocidad;
  int defensa;
  int ataque; 
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
} juego_t;

int jugar();

#endif /* __JUEGO_H__ */
