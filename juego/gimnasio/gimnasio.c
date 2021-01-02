#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h> //para obtener dimension de la consola
#include "../juego.h"
#include "gimnasio.h"
#include "../interfaz/interfaz.h"
#include "../heap/heap.h"

#define SIN_POSICION -1
#define EXITO 0
#define ERROR -1

#define ANCHO_TABLA 82
#define MAX_ANCHO_NOMBRE 26
#define MAX_ANCHO_ID 4
#define MAX_ANCHO_ATRIBUTO 15
#define PADDING 2


typedef enum {
  PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_COMBATE,
  PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_RESERVA,
  PANTALLA_INTERCAMBIANDO_POKEMON,
  VOLVER,
  CANT_PANTALLAS } pantalla_t;
typedef pantalla_t funcion_pantalla(juego_t* juego, int* id_combate, int* id_reserva);

void imprimir_pokemones(pokemon_t* pokemones[MAX_POKEMONES], size_t cantidad){
  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  int ancho_media_pantalla = (int)((w.ws_col - ANCHO_TABLA) / 2);

  if(!pokemones){
      return;
  }

  printf("%*s", ancho_media_pantalla, " ");
  printf("╔═══════════════════════════════════════════════════════════════════════════════╗\n");
  printf("%*s", ancho_media_pantalla, " ");
  printf("║%-*s %-*s %-*s %-*s %-*s║\n",
  MAX_ANCHO_ID, "ID",
  MAX_ANCHO_NOMBRE, "Nombre",
  MAX_ANCHO_ATRIBUTO, "Velocidad",
  MAX_ANCHO_ATRIBUTO, "Ataque",
  MAX_ANCHO_ATRIBUTO, "Defensa");
  printf("%*s", ancho_media_pantalla, " ");
  printf("║═══════════════════════════════════════════════════════════════════════════════║\n");
  for(int i = 0; i < cantidad; i++){
    printf("%*s", ancho_media_pantalla, " ");
    printf("║%-*i %-*s %-*i %-*i %-*i║\n",
    MAX_ANCHO_ID, pokemones[i]->id,
    MAX_ANCHO_NOMBRE, pokemones[i]->nombre,
    MAX_ANCHO_ATRIBUTO, pokemones[i]->velocidad,
    MAX_ANCHO_ATRIBUTO, pokemones[i]->ataque,
    MAX_ANCHO_ATRIBUTO, pokemones[i]->defensa);
  }
  printf("%*s", ancho_media_pantalla, " ");
  printf("╚═══════════════════════════════════════════════════════════════════════════════╝\n");
}

size_t llenar_vector_pokemon_lista(lista_t* lista, pokemon_t* pokemones[MAX_POKEMONES]){
  lista_iterador_t* iterador = lista_iterador_crear(lista);
  size_t cantidad = 0;
  while(lista_iterador_tiene_siguiente(iterador)){
    pokemon_t* pokemon_actual = (pokemon_t*)lista_iterador_elemento_actual(iterador);
    pokemones[cantidad++] = pokemon_actual;
    lista_iterador_avanzar(iterador);
  }
  return cantidad;
}


int posicion_pokemon_con_id(lista_t* pokemones, int id){
  lista_iterador_t* iterador = lista_iterador_crear(pokemones);
  int posicion = 0;
  while(lista_iterador_tiene_siguiente(iterador)){
    if(((pokemon_t*)lista_iterador_elemento_actual(iterador))->id == id){
      return posicion;
    }
    posicion++;
    lista_iterador_avanzar(iterador);
  }
  return SIN_POSICION;
}

pantalla_t poke_combate(juego_t* juego, int* id_combate, int* id_reserva){
  system("clear");
  lista_t* pokemones_combate = juego->personaje.pokemones_combate;
  pokemon_t* vector_pokemones[MAX_POKEMONES];
  int input = SIN_POSICION;
  size_t cantidad = llenar_vector_pokemon_lista(pokemones_combate, vector_pokemones);
  printf("\n\n\n\n");
  imprimir_pokemones(vector_pokemones, cantidad);
  printf("\n\nIngresa el ID del pokemon de coombate que queres intercambiar (-1 para volver): ");
  scanf("%i", &input);
  while ((getchar()) != '\n');
  int posicion_lista = posicion_pokemon_con_id(juego->personaje.pokemones_combate, input);
  while(input != SIN_POSICION && (posicion_lista < 0 || posicion_lista >= cantidad)){
    printf("Asegurate que el ID exista e ingresalo de nuevo: ");
    scanf("%i", &input);
    posicion_lista = posicion_pokemon_con_id(juego->personaje.pokemones_combate, input);
    while ((getchar()) != '\n');
  }

  if(posicion_lista == SIN_POSICION){
    return VOLVER;
  }
  
  *id_combate = input;
  return PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_RESERVA;
}

pantalla_t poke_reserva(juego_t* juego, int* id_combate, int* id_reserva){
  pokemon_t* pokemones_reserva[MAX_POKEMONES];
  size_t cantidad = arbol_recorrido_inorden(juego->personaje.pokemones_reserva, (void**)pokemones_reserva, MAX_POKEMONES);
  printf("\n\n\n\n");
  imprimir_pokemones(pokemones_reserva, cantidad);
  int input = SIN_POSICION;
  printf("\n\nIngresa el ID del pokemon en reserva que queres intercambiar (-1 para volver): ");
  scanf("%i", &input);
  while ((getchar()) != '\n');


  if(input == SIN_ID){
    *id_combate = SIN_ID;
    return PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_COMBATE;
  }

  pokemon_t comparador;
  comparador.id = input;
  pokemon_t* pokemon = arbol_buscar(juego->personaje.pokemones_reserva, &comparador);

  while(!pokemon){
    printf("Asegurate que el ID exista e ingresalo de nuevo: ");
    scanf("%i", &input);
    while ((getchar()) != '\n');
    comparador.id = input;
    pokemon = arbol_buscar(juego->personaje.pokemones_reserva, &comparador);
  }

  
  *id_reserva = input;
  return PANTALLA_INTERCAMBIANDO_POKEMON;
}

pantalla_t poke_intercambio(juego_t* juego, int* id_combate, int* id_reserva){
  lista_t* pokemones_combate = juego->personaje.pokemones_combate;
  abb_t* pokemones_reserva = juego->personaje.pokemones_reserva;

  int posicion_combate = posicion_pokemon_con_id(juego->personaje.pokemones_combate, *id_combate);
  pokemon_t* pokemon_combate = lista_elemento_en_posicion(pokemones_combate, (size_t)posicion_combate);
  if(!pokemon_combate){
    imprimir_consola("no se pudo realizar el intercambio");
    return VOLVER;
  }

  pokemon_t* pokemon_reserva;
  pokemon_t comparador;
  comparador.id = *id_reserva;
  pokemon_reserva = arbol_buscar(juego->personaje.pokemones_reserva, &comparador);
  if(!pokemon_reserva){
    imprimir_consola("no se pudo realizar el intercambio");
    return VOLVER;
  }

  char texto[MAX_MENSAJE];
  sprintf(texto, "Intercambiando %s con %s...", pokemon_combate->nombre, pokemon_reserva->nombre);
  imprimir_consola(texto);
  lista_borrar_de_posicion(pokemones_combate, (size_t)posicion_combate);
  arbol_borrar(pokemones_reserva, &comparador, false);

  if(arbol_insertar(pokemones_reserva, pokemon_combate) == EXITO && 
    lista_insertar_en_posicion(pokemones_combate, pokemon_reserva, (size_t)posicion_combate) == EXITO){
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

pantalla_t mostrar_pantalla(pantalla_t pantalla_actual, juego_t *juego, int* id_combate, int* id_reserva) {
    return nuevo_pantalla[pantalla_actual](juego, id_combate, id_reserva);
}

int cambiar_pokedex(juego_t* juego){
  pantalla_t pantalla_actual = PANTALLA_ELEGIR_POKEMON_INTERCAMBIAR_COMBATE;
  int id_combate = SIN_POSICION;
  int id_reserva = SIN_POSICION;

  while(pantalla_actual != VOLVER){
    pantalla_actual = mostrar_pantalla(pantalla_actual, juego, &id_combate, &id_reserva);
  }
  return 0;
}

int ver_personaje(juego_t* juego){
  pokemon_t* pokemones_combate[MAX_POKEMONES];
  pokemon_t* pokemones_reserva[MAX_POKEMONES];

  size_t cantidad_combate = llenar_vector_pokemon_lista(juego->personaje.pokemones_combate, pokemones_combate);
  size_t cantidad_reserva = arbol_recorrido_inorden(juego->personaje.pokemones_reserva, (void**)pokemones_reserva, MAX_POKEMONES);

  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  int ancho_media_pantalla = (int)((w.ws_col - ANCHO_TABLA) / 2);
  system("clear");
  printf("\n\n\n%*s", ancho_media_pantalla, " ");
  printf(TITULO);
  printf("Informacion del personaje\n\n");
  printf(CERRAR_TITULO);
  printf("%*s", ancho_media_pantalla, " ");
  printf("■ Nombre: %s\n\n", juego->personaje.nombre);
  printf("%*s", ancho_media_pantalla, " ");
  printf("■ Pokemones de combate:\n");
  imprimir_pokemones(pokemones_combate, cantidad_combate);
  printf("\n%*s", ancho_media_pantalla, " ");
  printf("■ Pokemones de reserva:\n");
  imprimir_pokemones(pokemones_reserva, cantidad_reserva);

  printf(AMARILLO "\n%*s", ancho_media_pantalla, " ");
  printf("presione enter para volver al gimnasio...");
  int c;
  while((c = getchar()) != '\n' && c != EOF);
  printf(BLANCO);
  return 0;
}
 
int ver_gimnasio(juego_t* juego){
  gimnasio_t* gim = heap_raiz(juego->gimnasios);
  if(!gim)
    return ERROR;

  pokemon_t* pokemones_lider[MAX_POKEMONES];
  size_t cantidad_pokemones_lider = llenar_vector_pokemon_lista(gim->lider.pokemones, pokemones_lider);
  
  entrenador_t* entrenadores[MAX_POKEMONES];
  lista_iterador_t* iterador = lista_iterador_crear(gim->entrenadores);
  size_t cantidad_entrenadores = 0;
  while(lista_iterador_tiene_siguiente(iterador)){
    entrenador_t* entrenador_actual = (entrenador_t*)lista_iterador_elemento_actual(iterador);
    entrenadores[cantidad_entrenadores++] = entrenador_actual;
    lista_iterador_avanzar(iterador);
  }

  pokemon_t* pokemones_entrenador[MAX_POKEMONES];

  struct winsize w;
  ioctl(0, TIOCGWINSZ, &w);
  int ancho_media_pantalla = (int)((w.ws_col - ANCHO_TABLA) / 2);
  system("clear");
  printf("\n\n\n%*s", ancho_media_pantalla, " ");
  printf(TITULO);
  printf("Informacion del gimnasio\n\n");
  printf(CERRAR_TITULO);

  printf("%*s", ancho_media_pantalla, " ");
  printf("■ Nombre: %s", gim->nombre);

  printf("\n\n%*s", ancho_media_pantalla, " ");
  printf("■ Dificultad: %lu", gim->dificultad);

  printf("\n\n%*s", ancho_media_pantalla, " ");
  printf("■ ID de batalla: %lu", gim->batalla_id);

  printf("\n\n%*s", ancho_media_pantalla, " ");
  printf("■ Pokemones del lider %s:\n", gim->lider.nombre);
  imprimir_pokemones(pokemones_lider, cantidad_pokemones_lider);

  if(cantidad_entrenadores > 0){  
    printf("\n\n%*s", ancho_media_pantalla, " ");
    printf("■ Entrenadores del gimnasio %s:", gim->nombre);
    for(size_t i = 0; i < cantidad_entrenadores; i++){
      printf("\n\n%*s", ancho_media_pantalla, " ");
      printf("\t■ Pokemones de %s:\n", entrenadores[i]->nombre);
      size_t cantidad_pokemones_entrenador = llenar_vector_pokemon_lista(entrenadores[i]->pokemones, pokemones_entrenador);
      imprimir_pokemones(pokemones_entrenador, cantidad_pokemones_entrenador);
    }
  }


  printf("\n%*s", ancho_media_pantalla, " ");

  printf(AMARILLO  "presione enter para volver al gimnasio..." BLANCO);
  int c;
  while((c = getchar()) != '\n' && c != EOF);

  return 0;
}
