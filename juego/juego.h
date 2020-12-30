#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"

#define MAX_NOMBRE 100
#define PUNTOS_RECOMPENSA_COMBATE 1
#define MAX_PUNTOS 63

typedef int (*funcion_batalla)(void*, void*);

typedef struct pokemon{
int velocidad;
int defensa;
int ataque; 
} pokemon_t;

typedef struct entrenador {
  const char nombre[MAX_NOMBRE];
  pokemon_t* pokemones;
} entrenador_t;

typedef struct gimnasio {
  const char nombre[MAX_NOMBRE];
  size_t dificultad;
  size_t batalla_id;
  entrenador_t lider;
  lista_t* entrenadores;
} gimnasio_t;

typedef struct personaje {
  const char nombre[MAX_NOMBRE];
  pokemon_t* pokemones_reserva;
  pokemon_t* pokemones_combate;
} personaje_t;

typedef struct heap {
  int cantidad;
} heap_t;

typedef struct juego {
  heap_t* gimnasios;
  personaje_t personaje;
  bool simulacion;
} juego_t;

int jugar();

#endif /* __JUEGO_H__ */
