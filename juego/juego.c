#include "interfaz/interfaz.h"
#include "juego.h"
#include <stdio.h>

#define INGRESA_ENTRENADOR_PRINCIPAL 0
#define AGREGA_GIMNASIO 1
#define COMIENZA_PARTIDA_INTERACTIVA 2
#define SIMULA_PARTIDA 3

typedef enum { INICIO, GYM, BATALLA, VICTORIA, DERROTA, CANT_MENUS } menu_t;
typedef menu_t funcion_menu(juego_t* juego);

menu_t menu_inicio(juego_t* juego){
  char* opciones[4] = {"Ingresa entrenador principal", "Agrega un gimnasio", "Comienza la partida", "Simula la partida"};
  int opcion_elegida = elegir_opcion(opciones, 4, 0);

  char msj[30] = "\0";
  sprintf(msj, "Elegiste la opcion %s", opciones[opcion_elegida]);
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
      return GYM;
    default:
      return INICIO;
  }
}

menu_t menu_gym(juego_t* juego);
menu_t menu_batalla(juego_t* juego);
menu_t menu_victoria(juego_t* juego);
menu_t menu_derrota(juego_t* juego);

funcion_menu* const nuevo_menu[ CANT_MENUS ] = {
    menu_inicio,
};

menu_t mostrar_menu( menu_t menu_actual, juego_t *juego ) {
    return nuevo_menu[menu_actual](juego);
}

int jugar(){
  menu_t menu_actual = INICIO;
  juego_t* juego = NULL;

  while(menu_actual != DERROTA){
    mostrar_menu(menu_actual, juego);
  }
  
  return 0;
}




