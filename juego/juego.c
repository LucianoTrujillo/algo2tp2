#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "juego.h"
#include "interfaz/interfaz.h"
#include "inicio/inicio.h"
#include "gimnasio/gimnasio.h"
#include "batallas/batallas.h"

#define ACTUALIZAR_PERSONAJE 0
#define AGREGA_GIMNASIO 1
#define COMIENZA_PARTIDA_INTERACTIVA 2
#define SIMULA_PARTIDA 3

#define VER_PERSONAJE 0
#define VER_GIMNASIO 1
#define CAMBIAR_POKEDEX 2
#define IR_BATALLA 3

#define CAMBIAR_POKEDEX_VICTORIA 0
#define PROXIMO_GIMNASIO 1
#define ROBAR_POKEMON 2

#define CAMBIAR_POKEDEX_DERROTA 0
#define REINTENTAR_GIMNASIO 1
#define FINALIZAR_PARTIDA 2

typedef enum { INICIO, GYM, BATALLA, VICTORIA, DERROTA, FIN, CANT_MENUS } menu_t;
typedef menu_t funcion_menu(juego_t* juego);

menu_t tratar_comenzar(juego_t* juego){
  if(lista_elementos(juego->personaje.pokemones_combate) == 0){
    imprimir_consola("debes tener al menos 1 pokemon para combatir");
    return INICIO;
  }

  if(heap_cantidad(juego->gimnasios) == 0){
    imprimir_consola("debe haber al menos 1 gimnasio para combatir");
    return INICIO;
  }
  
  imprimir_consola("empieza la aventura, mucha suerte");
  return GYM;
}

menu_t proximo_gimnasio_o_terminar(juego_t* juego){
  if(heap_cantidad(juego->gimnasios) == 0){
    imprimir_consola("felicidades! Sos troesma pokemon");
    return FIN;
  }

  heap_extraer_raiz(juego->gimnasios);
  if(heap_cantidad(juego->gimnasios) == 0){
    imprimir_consola("felicidades! Sos troesma pokemon");
    return FIN;
  } else {
    return GYM;
  }
}

bool pudo_tomar_prestado_pokemon(juego_t* juego){
  gimnasio_t* gim_actual = (gimnasio_t*)heap_raiz(juego->gimnasios);
  pokemon_t* pokemones_lider[MAX_POKEMONES];
  size_t cantidad_pokemones_lider = llenar_vector_pokemon_lista(gim_actual->lider.pokemones, pokemones_lider);
  int id;
  system("clear");
  printf("\n\n\n\n");
  imprimir_pokemones(pokemones_lider, cantidad_pokemones_lider);
  printf("\n\nIngrese el ID del pokemon que desea robar. Digo... Tomar prestado. (-1 si se arrepiente): ");
  scanf("%i", &id);
  while ((getchar()) != '\n');

  if(id == SIN_ID)
    return false;
  
  int posicion = posicion_pokemon_con_id(gim_actual->lider.pokemones, id);
  while((posicion < 0 || posicion >= cantidad_pokemones_lider)){
    printf("Asegurate que el ID exista e ingresalo de nuevo: ");
    scanf("%i", &id);
    posicion = posicion_pokemon_con_id(gim_actual->lider.pokemones, id);
    while ((getchar()) != '\n');
  }
  
  pokemon_t* pokemon_prestado = lista_elemento_en_posicion(gim_actual->lider.pokemones,(size_t)posicion);
  arbol_insertar(juego->personaje.pokemones_reserva, pokemon_prestado);
  imprimir_consola("pokemon robado exitosamente... Digo tomado prestado");
  return true;
}

menu_t menu_inicio(juego_t* juego){
  char* opciones[MAX_OPCIONES] = {
    "Ingresar entrenador principal",
    "Agregar un gimnasio",
    "Comenzar la partida",
    "Simular la partida" };

  int opcion_elegida = elegir_opcion("INICIO", opciones, 4, 0);

  switch (opcion_elegida){
    case ACTUALIZAR_PERSONAJE:
      actualizar_personaje(juego);
      return INICIO;
      break;
    case AGREGA_GIMNASIO:
      agregar_gimnasio(juego);
      return INICIO;
    case COMIENZA_PARTIDA_INTERACTIVA:
      return tratar_comenzar(juego);
    case SIMULA_PARTIDA:
      juego->simulacion = true;
      return tratar_comenzar(juego);
    default:
      return INICIO;
  }

}

menu_t menu_gym(juego_t* juego){
  char* opciones[MAX_OPCIONES] = {
    "Ver personaje y pokedex",
    "Ver gimasio a combatir",
    "Cambiar Pokemon de batalla",
    "Ir a proxima batalla" };

  int opcion_elegida = elegir_opcion("GIMNASIO", opciones, 4, 0);

  switch (opcion_elegida){
    case VER_PERSONAJE:
      ver_personaje(juego);
      return GYM;
      break;
    case VER_GIMNASIO:
      ver_gimnasio(juego);
      return GYM;
    case CAMBIAR_POKEDEX:
      cambiar_pokedex(juego);
      return GYM;
    case IR_BATALLA:
      return BATALLA;
    default:
      return GYM;
  }
}

menu_t menu_batalla(juego_t* juego){
  gimnasio_t* gimnasio = (gimnasio_t*)heap_raiz(juego->gimnasios);
  estado_combate_t estado_combate = batallar(juego->personaje, gimnasio);

  if(estado_combate == GANO){
    imprimir_consola("le ganaste la batalla a este entrenador");
    return GYM;
  }
  
  if(estado_combate == PERDIO){
    imprimir_consola("perdiste. Probar cambiando tus pokemones de batalla");
    return DERROTA;
  }

  if(estado_combate == COMPLETO_GIMNASIO){
    imprimir_consola("ganaste el gimnasio completo");
    return VICTORIA;
  }
  
  return GYM;
}

menu_t menu_victoria(juego_t* juego){
  char* opciones[MAX_OPCIONES] = {
    "Cambiar Pokemon de batalla",
    "Ir a proximo gimnasio",
    "Robar pokemon de lider"};

  int cantidad_opciones = 3;

  int opcion_elegida = elegir_opcion("VICTORIA", opciones, cantidad_opciones, 0);

  switch (opcion_elegida){
    case CAMBIAR_POKEDEX_VICTORIA:
      cambiar_pokedex(juego);
      return VICTORIA;
    case PROXIMO_GIMNASIO:
      return proximo_gimnasio_o_terminar(juego);
    case ROBAR_POKEMON:
      if(pudo_tomar_prestado_pokemon(juego))
        cantidad_opciones--;
      return VICTORIA;
      break;
    default:
      return VICTORIA;
  }
}

menu_t menu_derrota(juego_t* juego){
  char* opciones[MAX_OPCIONES] = {
    "Cambiar Pokemon de batalla",
    "Reintentar el gimnasio",
    "Finalizar la partida" };

  int opcion_elegida = elegir_opcion("DERROTA", opciones, 3, 0);

  switch (opcion_elegida){
    case CAMBIAR_POKEDEX_DERROTA:
      cambiar_pokedex(juego);
      return DERROTA;
      break;
    case REINTENTAR_GIMNASIO:
      return GYM;
    case FINALIZAR_PARTIDA:
      return FIN;
    default:
      return DERROTA;
  }
}

funcion_menu* const nuevo_menu[CANT_MENUS] = {
    menu_inicio,
    menu_gym,
    menu_batalla,
    menu_victoria,
    menu_derrota,
};

menu_t mostrar_menu(menu_t menu_actual, juego_t *juego) {
    return nuevo_menu[menu_actual](juego);
}

int comparar_pokemones(void* pokemon_1, void* pokemon_2){
  if(!pokemon_1 || !pokemon_2)
    return 1;

  return (int)(((pokemon_t*)(pokemon_1))->id) - (int)(((pokemon_t*)(pokemon_2))->id);
}

int comparar_gimnasios(void* gimnasio_1, void* gimnasio_2){
  gimnasio_t* gim_1 = (gimnasio_t*)gimnasio_1;
  gimnasio_t* gim_2 = (gimnasio_t*)gimnasio_2;
  return (int)(gim_2->dificultad - gim_1->dificultad);
}

void destruir_pokemon(void* pokemon){
    free(pokemon);
}

int inicializar_juego(juego_t* juego){
  juego->simulacion = false;
  
  memset(juego->personaje.nombre, '\0', MAX_NOMBRE);

  juego->personaje.pokemones_reserva = arbol_crear(comparar_pokemones, destruir_pokemon);
  if(!juego->personaje.pokemones_reserva)
    return ERROR;
  
  juego->personaje.pokemones_combate = lista_crear();

  if(!juego->personaje.pokemones_combate){
    arbol_destruir(juego->personaje.pokemones_reserva);
    return ERROR;
  }
  
  juego->gimnasios = heap_crear(comparar_gimnasios, NULL);

  if(!juego->gimnasios){
    arbol_destruir(juego->personaje.pokemones_reserva);
    lista_destruir(juego->personaje.pokemones_combate);
    return ERROR;
  }

  return EXITO;
}

int jugar(){
  menu_t menu_actual = INICIO;
  juego_t juego;
  int resultado = inicializar_juego(&juego);
  if(resultado == ERROR)
    return ERROR;

  while(menu_actual != FIN){
    menu_actual = mostrar_menu(menu_actual, &juego);
  }

  imprimir_consola("Chau! Esperamos verte pronto...");

  return EXITO;
}
