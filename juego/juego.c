#include "interfaz/interfaz.h"
#include "juego.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

#define MAX_MENSAJE 200
#define MAX_NOMBRE_ARCHIVO 100

#define FORMATO_NOMBRE "%1[^;];%100[^\n]\n"
#define FORMATO_POKEMON "%1[^;];%100[^;];%i;%i;%i\n"
#define MODO_LECTURA "r"
#define CHAR_ENTRENADOR "E"
#define CHAR_POKEMON "P"
#define CANTIDAD_LEIDOS_NOMBRE 2
#define CANTIDAD_LEIDOS_POKEMON 5
#define LECTURA_INCOMPLETA 2
#define ERROR -1
#define EXITO 0

#define FORMATO_GIMNASIO "%1[^;];%100[^;];%lu;%lu\n"
#define CHAR_LIDER "L"
#define CHAR_GIMNASIO "G"
#define CANTTIDAD_LEIDOS_GIMNASIO 4

#define MAX_POKEMONES_BATALLA 6

bool imprimir_pokemon(void* pokemon, void* extra){
  printf("%s\n",((pokemon_t*)pokemon)->nombre);
  return false;
}

bool imprimir_entrenador(void* entrenador, void* extra){
  printf("Nombre entrenador: %s\n",((entrenador_t*)entrenador)->nombre);
  printf("Pokemones entrenador: \n");
  lista_con_cada_elemento(((entrenador_t*)entrenador)->pokemones, imprimir_pokemon, NULL);
  return false;
}

int leer_nombre(FILE* archivo, char* nombre, char* caracter_identificador){
  char identificardor_linea[2];
  int res = fscanf(archivo, FORMATO_NOMBRE, identificardor_linea, nombre);

  if(res != CANTIDAD_LEIDOS_NOMBRE || strcmp(identificardor_linea, caracter_identificador) != 0)
    return ERROR;
  
  return EXITO;
}
int leer_pokemones(FILE* archivo, void* entrenador, int (*insertar)(void* entrenador, pokemon_t* pokemon, int* contador)){
  pokemon_t* pokemon = malloc(sizeof(pokemon_t));
  if(!pokemon){
    return ERROR;
  }
  char identificardor_linea[2];
  int contador = 0;

  while(fscanf(
              archivo,
              FORMATO_POKEMON,
              identificardor_linea,
              pokemon->nombre, 
              &(pokemon->velocidad), 
              &(pokemon->ataque), 
              &(pokemon->defensa)) == CANTIDAD_LEIDOS_POKEMON &&
              strcmp(identificardor_linea, CHAR_POKEMON) == 0){
    
    if(insertar(entrenador, pokemon, &contador) == ERROR){
      return ERROR;
    }

    pokemon = malloc(sizeof(pokemon_t));
    if(!pokemon)
      return ERROR;
  }
  printf("pokemones leidos: %i\n", contador);
  return EXITO;
}

int insertar_pokemon_personaje(void* entrenador_generico, pokemon_t* pokemon, int* contador){
  personaje_t* entrenador_principal = (personaje_t*)entrenador_generico;

  int resultado_insercion = EXITO;
  if(*contador < MAX_POKEMONES_BATALLA){
    resultado_insercion = lista_insertar(entrenador_principal->pokemones_combate, pokemon);
  } else {
    resultado_insercion = arbol_insertar(entrenador_principal->pokemones_reserva, pokemon);
  }

  if(resultado_insercion == EXITO)
    (*contador)++;

  return resultado_insercion;
}

int insertar_pokemon_entrenador(void* entrenador_generico, pokemon_t* pokemon, int* contador){
  entrenador_t* entrenador = (entrenador_t*)entrenador_generico;

  if(lista_insertar(entrenador->pokemones, pokemon) == EXITO){
    (*contador)++;
    return EXITO;
  }

  return ERROR;
}

int leer_entrenador(FILE* archivo, entrenador_t* entrenador, char* identificador){
  int res = leer_nombre(archivo, entrenador->nombre, identificador);
  printf("resultado: %i, nombre: %s\n", res, entrenador->nombre);
  if(leer_nombre(archivo, entrenador->nombre, identificador) == ERROR){
    return ERROR;
  }
  printf("hola");
  return leer_pokemones(archivo, (void*)(entrenador), insertar_pokemon_entrenador);
}

int leer_entrenador_principal(juego_t* juego){
  char nombre_archivo[MAX_NOMBRE_ARCHIVO];
  printf("Ingresa la ruta al archivo donde se encuentra la información del entrenador principal: ");
  scanf("%s", nombre_archivo);
  while ((getchar()) != '\n'); 

  FILE* archivo = fopen(nombre_archivo, MODO_LECTURA);
  
  if(!archivo){
    imprimir_consola("No se pudo abrir el archivo del entrenador");
    return ERROR;
  }
  
  if(leer_nombre(archivo, (juego->personaje.nombre), CHAR_ENTRENADOR) == ERROR){
    imprimir_consola("No se pudo leer el nombre del entrenador");
    fclose(archivo);
    return ERROR;
  }

  int resultado = leer_pokemones(archivo, (void*)(&(juego->personaje)), insertar_pokemon_personaje);

  if(resultado == ERROR){
    imprimir_consola("No se pudo leer correctamente todos los pokemones del personaje");
  } else {
    imprimir_consola("Informacion del entrenador actualizada correctamente");
  }

  fclose(archivo);
  return resultado;
}


void ver_datos_gim(gimnasio_t gim){
  printf("Nombre: %s\n", gim.nombre);
  printf("Dificultad: %lu\n", gim.dificultad);
  printf("ID de batalla: %lu\n", gim.batalla_id);

  printf("Nombre lider: %s\n", gim.lider.nombre);
  printf("Pokemones del lider: \n");
  lista_con_cada_elemento(gim.lider.pokemones, imprimir_pokemon, NULL);
  
  printf("Entrenadores: \n");
  lista_con_cada_elemento(gim.entrenadores, imprimir_entrenador, NULL);
}

int agregar_gimnasio(juego_t* juego){
  char nombre_archivo[MAX_NOMBRE_ARCHIVO];
  printf("Ingresa la ruta al archivo donde se encuentra la información del gimnasio: ");
  scanf("%s", nombre_archivo);
  while ((getchar()) != '\n'); 
  FILE* archivo = fopen(nombre_archivo, MODO_LECTURA);
  
  if(!archivo){
    imprimir_consola("No se pudo abrir el archivo del gimnasio");
    return ERROR;
  }
  
  char identificardor_linea[2];
  gimnasio_t* nuevo_gim = calloc(1, sizeof(gimnasio_t));
  if(!nuevo_gim){
    fclose(archivo);
    imprimir_consola("No se pudo crear el nuevo gimnasio");
    return ERROR;
  }

  int res = fscanf(archivo, FORMATO_GIMNASIO, identificardor_linea, nuevo_gim->nombre, &(nuevo_gim->dificultad), &(nuevo_gim->batalla_id));
  if(res != CANTTIDAD_LEIDOS_GIMNASIO || strcmp(identificardor_linea, CHAR_GIMNASIO) != 0){
    imprimir_consola("El formato del gimnasio es incorrecto. Visitar el README para mas informacion");
    fclose(archivo);
    return ERROR;
  }
  ver_datos_gim(*nuevo_gim);
  if(leer_entrenador(archivo, &(nuevo_gim->lider), CHAR_LIDER) == ERROR){
    imprimir_consola("No se pudo leer la informacion del lider");
    fclose(archivo);
    return ERROR;
  }

  int resultado_lectura = EXITO;
  entrenador_t* nuevo_entrenador = NULL;

  do {
    nuevo_entrenador = calloc(1, sizeof(entrenador_t));
    if(!nuevo_entrenador){
      fclose(archivo);
      return ERROR;
    }

    resultado_lectura = leer_entrenador(archivo, nuevo_entrenador, CHAR_ENTRENADOR);
    if(resultado_lectura == EXITO)
      lista_insertar(nuevo_gim->entrenadores, nuevo_entrenador);
  } while(resultado_lectura == EXITO);

  ver_datos_gim(*nuevo_gim);
  heap_insertar(juego->gimnasios, (void*)nuevo_gim);
  fclose(archivo);
  return EXITO;
}

typedef enum { INICIO, GYM, BATALLA, VICTORIA, DERROTA, FIN, CANT_MENUS } menu_t;
typedef menu_t funcion_menu(juego_t* juego);

menu_t menu_inicio(juego_t* juego){
  char* opciones[MAX_OPCIONES] = {
    "Ingresar entrenador principal",
    "Agregar un gimnasio",
    "Comenzar la partida",
    "Simular la partida" };

  int opcion_elegida = elegir_opcion("INICIO", opciones, 4, 0);

  char msj[MAX_MENSAJE];
  memset(msj, '\0', MAX_MENSAJE);
  sprintf(msj, "Elegiste %s.", opciones[opcion_elegida]);
  imprimir_consola(msj);

  switch (opcion_elegida){
    case INGRESA_ENTRENADOR_PRINCIPAL:
      leer_entrenador_principal(juego);
      return INICIO;
      break;
    case AGREGA_GIMNASIO:
      agregar_gimnasio(juego);
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
    "Ir a proxima batalla" };

  int opcion_elegida = elegir_opcion("GIMNASIO", opciones, 4, 0);

  char msj[MAX_MENSAJE];
  memset(msj, '\0', MAX_MENSAJE);
  sprintf(msj, "Elegiste %s.", opciones[opcion_elegida]);
  imprimir_consola(msj);

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
    "Ir a proximo gimnasio"
    "Robar pókemon de lider"};

  int cantidad_opciones = 3;

  int opcion_elegida = elegir_opcion("VICTORIA", opciones, cantidad_opciones, 0);

  char msj[MAX_MENSAJE];
  memset(msj, '\0', MAX_MENSAJE);
  sprintf(msj, "Elegiste %s.", opciones[opcion_elegida]);
  imprimir_consola(msj);

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
    "Reintentar el gimnasio",
    "Finalizar la partida" };

  int opcion_elegida = elegir_opcion("DERROTA", opciones, 3, 0);

  char msj[MAX_MENSAJE];
  memset(msj, '\0', MAX_MENSAJE);
  sprintf(msj, "Elegiste %s.", opciones[opcion_elegida]);
  imprimir_consola(msj);

  switch (opcion_elegida){
    case CAMBIAR_POKEDEX_DERROTA:
      /* misma Interfaz copadísima que usé para el inicio, DRY CODE PLEASE para el cambio de pokmeones de reserva a batallar y viceversa*/
      return DERROTA;
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

  return strcmp(((pokemon_t*)(pokemon_1))->nombre, ((pokemon_t*)(pokemon_2))->nombre);
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

  if(!juego->personaje.pokemones_combate)
    return ERROR;

  juego->gimnasios = NULL; //falta inicializarle el heap postaa
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

  return 0;
}




