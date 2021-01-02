#include "inicio.h"
#include "../juego.h"
#include "../interfaz/interfaz.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


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

int leer_identificador_linea(FILE* archivo, char* identificador){
  return fscanf(archivo, FORMATO_IDENTIFICADOR_LINEA, identificador) == 1 ? EXITO : ERROR;
}

int leer_identificador_linea_esperada(FILE* archivo, char identificador_esperado){
  char identificador_linea;
  int estado;

  estado = leer_identificador_linea(archivo, &identificador_linea);

  if(estado == ERROR || identificador_linea != identificador_esperado)
    return ERROR;
  
  return EXITO;
}

int leer_nombre_entrenador(FILE* archivo, char* nombre){
  return fscanf(archivo, FORMATO_NOMBRE, nombre) == 1 ? EXITO : ERROR;
}

int leer_nombre_personaje(FILE* archivo, char* nombre, char caracter_identificador){
  if(leer_identificador_linea_esperada(archivo, caracter_identificador) == EXITO)
    return leer_nombre_entrenador(archivo, nombre);

  return ERROR;
}

int leer_pokemon(FILE* archivo, pokemon_t* pokemon){
  if(fscanf(archivo,
        FORMATO_POKEMON,
        pokemon->nombre,
        &(pokemon->velocidad),
        &(pokemon->ataque),
        &(pokemon->defensa),
        &(pokemon->id)) 
        == CANTIDAD_LEIDOS_POKEMON){
    return EXITO;
    } else {
      return ERROR;
    }

}

int leer_pokemon_personaje(FILE* archivo, pokemon_t* pokemon){
  if(leer_identificador_linea_esperada(archivo, CHAR_POKEMON) == EXITO)
    return leer_pokemon(archivo, pokemon);
  
  return ERROR;
}

int leer_pokemones_personaje(FILE* archivo, void* entrenador, int (*insertar)(void* entrenador, pokemon_t* pokemon, int* contador)){
  pokemon_t* pokemon = calloc(1, sizeof(pokemon_t));
  if(!pokemon)
    return ERROR;
  int contador = 0;
  while(leer_pokemon_personaje(archivo, pokemon) == EXITO){
    if(insertar(entrenador, pokemon, &contador) == ERROR){
      free(pokemon);
      return ERROR;
    }
    pokemon = calloc(1, sizeof(pokemon_t));
    if(!pokemon)
      return ERROR;
  }

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

FILE* obtener_archivo(char* mensaje){
  char nombre_archivo[MAX_NOMBRE_ARCHIVO];
  printf("%s", mensaje);
  scanf("%s", nombre_archivo);
  while ((getchar()) != '\n');
  FILE* archivo = fopen(nombre_archivo, MODO_LECTURA);
  if(!archivo)
    imprimir_consola("No se pudo abrir el archivo");
  return archivo;
}

FILE* obtener_archivo_2(char* nombre){
  FILE* archivo = fopen(nombre, MODO_LECTURA);
  if(!archivo)
    imprimir_consola("No se pudo abrir el archivo");
  return archivo;
}

int actualizar_personaje(juego_t* juego){
  FILE* archivo = obtener_archivo_2("entrenador/test.txt");
  if(!archivo){
    return ERROR;
  }
  
  if(leer_nombre_personaje(archivo, (juego->personaje.nombre), CHAR_ENTRENADOR) == ERROR){
    imprimir_consola("No se pudo leer el nombre del entrenador");
    fclose(archivo);
    return ERROR;
  }

  //hacer que libere memoria de elementos
  lista_destruir(juego->personaje.pokemones_combate, destruir_pokemon);
  juego->personaje.pokemones_combate = lista_crear();

  if(!juego->personaje.pokemones_combate){
    return ERROR;
  }

  arbol_destruir(juego->personaje.pokemones_reserva);
  juego->personaje.pokemones_reserva = arbol_crear(comparar_pokemones, destruir_pokemon);
  if(!juego->personaje.pokemones_reserva)
    return ERROR;

  int resultado = leer_pokemones_personaje(archivo, (void*)(&(juego->personaje)), insertar_pokemon_personaje);

  if(resultado == ERROR){
    imprimir_consola("No se pudo leer correctamente todos los pokemones del personaje");
  } else {
    imprimir_consola("Informacion del entrenador actualizada correctamente");
  }

  lista_con_cada_elemento(juego->personaje.pokemones_combate, imprimir_pokemon, NULL);
  if(juego->personaje.pokemones_combate->cantidad == 0){
    fclose(archivo);
    return ERROR;
  }

  fclose(archivo);
  return EXITO;
}


void ver_datos_gim(gimnasio_t gim){
  printf("Nombre: %s\n", gim.nombre);
  printf("Dificultad: %lu\n", gim.dificultad);
  printf("ID de batalla: %lu\n\n", gim.batalla_id);

  printf("Nombre lider: %s\n", gim.lider.nombre);
  printf("Pokemones del lider: \n");
  lista_con_cada_elemento(gim.lider.pokemones, imprimir_pokemon, NULL);
  
  printf("\nEntrenadores: \n");
  lista_con_cada_elemento(gim.entrenadores, imprimir_entrenador, NULL);
  printf("\n\n\n");
}

gimnasio_t* crear_gimnasio(){
  gimnasio_t* nuevo_gim = calloc(1, sizeof(gimnasio_t));
  if(!nuevo_gim){
    imprimir_consola("No se pudo crear el nuevo gimnasio");
    return NULL;
  }

  nuevo_gim->entrenadores = lista_crear();
  if(!nuevo_gim->entrenadores){
    free(nuevo_gim);
    imprimir_consola("No se pudo crear el nuevo gimnasio");
    return NULL;
  }

  return nuevo_gim;
}

int leer_metadata_gim(FILE* archivo, gimnasio_t* gim){
  if(leer_identificador_linea_esperada(archivo, CHAR_GIMNASIO) == EXITO){
    int res = fscanf(archivo, FORMATO_GIMNASIO, gim->nombre, &(gim->dificultad), &(gim->batalla_id));
    if(res == CANTIDAD_LEIDOS_GIMNASIO)
      return EXITO;
  }
  imprimir_consola("El formato del gimnasio es incorrecto. Visitar el README para mas informacion");
  return ERROR;
}

void agregar_entrenador_gimnasio(FILE* archivo, entrenador_t** entrenador_actual, gimnasio_t* gim){
  (*entrenador_actual) = calloc(1, sizeof(entrenador_t));
  if((*entrenador_actual)){
    (*entrenador_actual)->pokemones = lista_crear();
    if(!(*entrenador_actual)->pokemones){
      free((*entrenador_actual));
      (*entrenador_actual) = NULL;
    } else if(leer_nombre_entrenador(archivo, (*entrenador_actual)->nombre) == ERROR){
      (*entrenador_actual) = NULL;
    } else if(lista_insertar(gim->entrenadores, (*entrenador_actual)) == ERROR) {
        (*entrenador_actual) = NULL;
    }
  }
}

void agregar_pokemon_gimnasio(FILE* archivo, pokemon_t* pokemon, entrenador_t** entrenador){
  pokemon = calloc(1, sizeof(pokemon_t));

  if(!pokemon){
    *entrenador = NULL;
    return;
  }
    
  if(leer_pokemon(archivo, pokemon) == EXITO)
    if(lista_insertar((*entrenador)->pokemones, pokemon) == EXITO)
      return;

  *entrenador = NULL;
}

int agregar_gimnasio(juego_t* juego){
  FILE* archivo = obtener_archivo_2("gimnasio/test.txt");
  if(!archivo){
    return ERROR;
  }
  
  gimnasio_t* nuevo_gim = crear_gimnasio();
  if(!nuevo_gim){
    fclose(archivo);
    return ERROR;
  }

  if(leer_metadata_gim(archivo, nuevo_gim) == ERROR){
    free(nuevo_gim);
    fclose(archivo);
    return ERROR;
  }

 if(leer_nombre_personaje(archivo, (nuevo_gim->lider.nombre), CHAR_LIDER) == ERROR){
    imprimir_consola("No se pudo leer el nombre del lider");
    free(nuevo_gim);
    fclose(archivo);
    return ERROR;
  }

  entrenador_t* entrenador_actual = &(nuevo_gim->lider);
  entrenador_actual->pokemones = lista_crear();
  if(!entrenador_actual->pokemones){
    free(nuevo_gim);
    fclose(archivo);
  }

  char identificador_linea;
  pokemon_t* pokemon_nuevo = NULL;

  do {
    if(leer_identificador_linea(archivo, &identificador_linea) == ERROR){
      entrenador_actual = NULL;
    }
    if(entrenador_actual){
      switch(identificador_linea){
        case CHAR_ENTRENADOR:
          agregar_entrenador_gimnasio(archivo, &entrenador_actual, nuevo_gim);
          break;
        case CHAR_POKEMON: 
          agregar_pokemon_gimnasio(archivo, pokemon_nuevo, &entrenador_actual);
          break;
       default:
        entrenador_actual = NULL;
        break;
      }
    }
  } while(entrenador_actual);
  heap_insertar(juego->gimnasios, (void*)nuevo_gim);
  fclose(archivo);
  imprimir_consola("gimnasio agregado correctamente");
  return EXITO;
}
