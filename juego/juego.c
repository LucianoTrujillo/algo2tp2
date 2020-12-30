#include "interfaz/interfaz.h"
#include "juego.h"
#include <stdio.h>
#include <stdbool.h>

#define INGRESA_ENTRENADOR_PRINCIPAL 0
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

menu_t menu_inicio(juego_t* juego){
  char* opciones[MAX_OPCIONES] = {
    "Ingresar entrenador principal",
    "Agregar un gimnasio",
    "Comenzar la partida",
    "Simular la partida" };

  int opcion_elegida = elegir_opcion(opciones, 4, 0);

  char msj[30] = "\0";
  sprintf(msj, "Elegiste %s.", opciones[opcion_elegida]);
  mostrar_mensaje_fluido(msj);

  switch (opcion_elegida){
    case INGRESA_ENTRENADOR_PRINCIPAL:
      /* agregar personaje principal medainte lectura de archivo */
      return INICIO;
      break;
    case AGREGA_GIMNASIO:
      /* agregar gimansio a heap medainte lectura de archivo */
      return INICIO;
    case COMIENZA_PARTIDA_INTERACTIVA:
      /*mensje de que comienza la aventura interactiva*/
      return GYM;
    case SIMULA_PARTIDA:
      /*mensaje empieza la simulación*/
      juego->simulacion = true;
      return GYM;
    default:
      return INICIO;
  }
}

menu_t menu_gym(juego_t* juego){
  char* opciones[MAX_OPCIONES] = {
    "Ver personaje y pokedex",
    "Ver gimasio a combatir",
    "Cambiar Pokemon de batalla",
    "Ir a la próxima batalla" };

  int opcion_elegida = elegir_opcion(opciones, 4, 0);

  char msj[30] = "\0";
  sprintf(msj, "Elegiste %s.", opciones[opcion_elegida]);
  mostrar_mensaje_fluido(msj);

  switch (opcion_elegida){
    case VER_PERSONAJE:
      /* mostrar pj y pokemones */
      return GYM;
      break;
    case VER_GIMNASIO:
      /* ver data de gim actual */
      return GYM;
    case CAMBIAR_POKEDEX:
      /* Interfaz copadísima para el cambio de pokmeones de reserva a batallar y viceversa*/
      return GYM;
    case IR_BATALLA:
      /*mensaje empieza la batalla*/
      return BATALLA;
    default:
      return GYM;
  }
}

menu_t menu_batalla(juego_t* juego){
  bool gano_gimnasio = false;
  /*
    Interfaz copadísima donde se ve los dos pokemones que pelean
    mostrar quien gana, y abajo un mensajito para avanzar con enter

    Acá va a estar la lógica para ir desapilando los entrenadores,
    y acordarse que al pelean con los pokemones, ir avanzando con iteradores
  */

  if(gano_gimnasio){ //ganó el gimnasio entero...
    return VICTORIA;
  } else {
    return DERROTA;
  }
}

menu_t menu_victoria(juego_t* juego){
  char* opciones[MAX_OPCIONES] = {
    "Cambiar Pokemon de batalla",
    "Ir a próximo gimnasio"
    "Robar pókemon de lider"};

  int cantidad_opciones = 3;

  int opcion_elegida = elegir_opcion(opciones, cantidad_opciones, 0);

  char msj[30] = "\0";
  sprintf(msj, "Elegiste %s.", opciones[opcion_elegida]);
  mostrar_mensaje_fluido(msj);

  switch (opcion_elegida){
    case CAMBIAR_POKEDEX_VICTORIA:
      /* misma Interfaz copadísima que usé para el inicio, DRY CODE PLEASE para el cambio de pokmeones de reserva a batallar y viceversa*/
      return VICTORIA;
    case PROXIMO_GIMNASIO:
      /* Si no hay mas gimnasios, mostrar que se convirtió en Troesma pokemon y return FIN*/
      return GYM;
    case ROBAR_POKEMON:
      /* interfaz bonita para elegir pokemon del lider y coopiarse el pokemon y agregarlo a la lista nuestra
         remover opción del array de opciones. Podrá usarse la lógica de cambiar pokedex en cuanto al MOSTRADO? maybe...
      */
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
    "Reintentar el gimnasio"
    "Finalizar la partida" };

  int opcion_elegida = elegir_opcion(opciones, 3, 0);

  char msj[30] = "\0";
  sprintf(msj, "Elegiste %s.", opciones[opcion_elegida]);
  mostrar_mensaje_fluido(msj);

  switch (opcion_elegida){
    case CAMBIAR_POKEDEX_DERROTA:
      /* misma Interfaz copadísima que usé para el inicio, DRY CODE PLEASE para el cambio de pokmeones de reserva a batallar y viceversa*/
      return GYM;
      break;
    case REINTENTAR_GIMNASIO:
      /* Ir al gym again, quizá cambio los pokemones... */
      return GYM;
    case FINALIZAR_PARTIDA:
      /* Chau chau adiós!*/
      return FIN;
    default:
      return DERROTA;
  }
}

funcion_menu* const nuevo_menu[ CANT_MENUS ] = {
    menu_inicio,
    menu_gym
};

menu_t mostrar_menu( menu_t menu_actual, juego_t *juego ) {
    return nuevo_menu[menu_actual](juego);
}

int jugar(){
  menu_t menu_actual = INICIO;
  juego_t* juego = NULL;

  while(menu_actual != FIN){
    menu_actual = mostrar_menu(menu_actual, juego);
  }

  mostrar_mensaje_fluido("Muchas gracias por formar parte de esta hermosa aventura! Esperamos verte pronto...");
  
  return 0;
}




