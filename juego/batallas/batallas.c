#include "batallas.h"
#include "../interfaz/interfaz.h"

bool gana_batalla(personaje_t personaje, entrenador_t* entrenador){
  imprimir_consola("peleando contra entrenador...");
  return true;
}

estado_combate_t batallar(personaje_t personaje, gimnasio_t* gim){
  if(!gim)
    return COMPLETO_GIMNASIO;

  lista_t* entrenadores = gim->entrenadores;

  if(!lista_vacia(entrenadores)){
    if(gana_batalla(personaje, lista_tope(entrenadores))){
      lista_desapilar(entrenadores);
      return GANO;
    }
  } else if(gana_batalla(personaje, &(gim->lider))){
    return COMPLETO_GIMNASIO;
  }

  return PERDIO;
}
