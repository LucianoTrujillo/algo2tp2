#include "batallas.h"
#include "../interfaz/interfaz.h"
#include "../gimnasio/gimnasio.h"
#include <string.h>
#include <stdio.h>

#define CANTIDAD_TIPOS_BATALLA 5

int funcion_batalla_1(void* pkm_1, void* pkm_2){
  pokemon_t* pokemon_1 = (pokemon_t*)pkm_1;
  pokemon_t* pokemon_2 = (pokemon_t*)pkm_2;
  return ((pokemon_1->ataque + pokemon_1->defensa) > (pokemon_2->velocidad)) ? GANO_PRIMERO : GANO_SEGUNDO; 
}

int funcion_batalla_2(void* pkm_1, void* pkm_2){
  pokemon_t* pokemon_1 = (pokemon_t*)pkm_1;
  pokemon_t* pokemon_2 = (pokemon_t*)pkm_2;
  return ((pokemon_1->defensa) > (pokemon_2->velocidad)) ? GANO_PRIMERO : GANO_SEGUNDO; 
}

int funcion_batalla_3(void* pkm_1, void* pkm_2){
  pokemon_t* pokemon_1 = (pokemon_t*)pkm_1;
  pokemon_t* pokemon_2 = (pokemon_t*)pkm_2;
  return (strcmp(pokemon_1->nombre, pokemon_2->nombre)) < 0 ? GANO_PRIMERO : GANO_SEGUNDO; 
}

int funcion_batalla_4(void* pkm_1, void* pkm_2){
  pokemon_t* pokemon_1 = (pokemon_t*)pkm_1;
  pokemon_t* pokemon_2 = (pokemon_t*)pkm_2;
  return (pokemon_1->id > pokemon_2->id) ? GANO_PRIMERO : GANO_SEGUNDO; 
}

int funcion_batalla_5(void* pkm_1, void* pkm_2){
  pokemon_t* pokemon_1 = (pokemon_t*)pkm_1;
  pokemon_t* pokemon_2 = (pokemon_t*)pkm_2;
  return (strlen(pokemon_1->nombre) > strlen(pokemon_2->nombre)) ? GANO_PRIMERO : GANO_SEGUNDO; 
}

typedef int funcion_batalla(void* pkm_1, void* pkm_2);

funcion_batalla* const batallas[CANTIDAD_TIPOS_BATALLA] = {
    funcion_batalla_1,
    funcion_batalla_2,
    funcion_batalla_3,
    funcion_batalla_4,
    funcion_batalla_5
};
bool gano_combate(void* pokemon_pj, void* pokemon_enemigo, size_t batalla_id, bool simulacion){
  pokemon_t* pokemon_1 = (pokemon_t*)pokemon_pj;
  pokemon_t* pokemon_2 = (pokemon_t*)pokemon_enemigo;

  int ganador = batallas[batalla_id - 1](pokemon_pj, pokemon_enemigo);

  if(!simulacion){
    char texto[LINEAS_DISPLAY * MAX_CARACTERES_LINEA_DISPLAY];
    sprintf(texto, 
      "Tu pokemon: %-43s"
      "Velocidad: %-44i"
      "Ataque: %-47i"
      "Defensa: %-46i"
      "                                                       "
      "Pokemon enemigo: %-38s"
      "Velocidad: %-44i"
      "Ataque: %-47i"
      "Defensa: %-46i"
      "                                                       "
      "-------------------------------------------------------"
      "                                                       "
      "GANADOR: %-46s"
      ,
      pokemon_1->nombre,
      pokemon_1->velocidad,
      pokemon_1->ataque,
      pokemon_1->defensa,
      pokemon_2->nombre,
      pokemon_2->velocidad,
      pokemon_2->ataque,
      pokemon_2->defensa,
      ganador == GANO_PRIMERO ? pokemon_1->nombre : pokemon_2->nombre
    );

    imprimir_consola_inmediato(texto);
  }
  
  
  return ganador == GANO_PRIMERO;
}

bool gano_batalla(personaje_t personaje, entrenador_t* entrenador, size_t batalla_id, bool simulacion){
  lista_iterador_t* iterador_pokemones_pj = lista_iterador_crear(personaje.pokemones_combate);
  lista_iterador_t* iterador_pokemones_enemigo = lista_iterador_crear(entrenador->pokemones);

  while(lista_iterador_tiene_siguiente(iterador_pokemones_pj) && 
        lista_iterador_tiene_siguiente(iterador_pokemones_enemigo)){
    if(gano_combate(
        lista_iterador_elemento_actual(iterador_pokemones_pj),
        lista_iterador_elemento_actual(iterador_pokemones_enemigo),
        batalla_id,
        simulacion)){
      lista_iterador_avanzar(iterador_pokemones_enemigo);
    } else {
      lista_iterador_avanzar(iterador_pokemones_pj);
    }
  }

  bool gano = false;
  if(lista_iterador_tiene_siguiente(iterador_pokemones_pj))
    gano = true;

  lista_iterador_destruir(iterador_pokemones_pj);
  lista_iterador_destruir(iterador_pokemones_enemigo);
  return gano;
}

estado_combate_t batallar(personaje_t personaje, gimnasio_t* gim, bool simulacion){
  if(!gim)
    return COMPLETO_GIMNASIO;

  lista_t* entrenadores = gim->entrenadores;

  if(!lista_vacia(entrenadores)){
    if(gano_batalla(personaje, lista_tope(entrenadores), gim->batalla_id, simulacion)){
      lista_desapilar(entrenadores);
      return GANO;
    }
  } else if(gano_batalla(personaje, &(gim->lider), gim->batalla_id, simulacion)){
    return COMPLETO_GIMNASIO;
  }

  return PERDIO;
}
