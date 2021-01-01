#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "juego/juego.h"
#include "juego/inicio/inicio.h"
#include "juego/heap/heap.h"
#include "pa2mm.h"


static void destruir_pokemon(void* pokemon){
    free(pokemon);
}
static int comparar_pokemones(void* pokemon_1, void* pokemon_2){
  if(!pokemon_1 || !pokemon_2)
    return 1;

  return strcmp(((pokemon_t*)(pokemon_1))->nombre, ((pokemon_t*)(pokemon_2))->nombre);
}

static int comparar_gimnasios(void* gimnasio_1, void* gimnasio_2){
  gimnasio_t* gim_1 = (gimnasio_t*)gimnasio_1;
  gimnasio_t* gim_2 = (gimnasio_t*)gimnasio_2;
  return (int)(gim_2->dificultad - gim_1->dificultad);
}

static int inicializar_juego(juego_t* juego){
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

int probar_inicio(){
  pa2m_nuevo_grupo("Pruebas Inicio (manejo de archivos)");
  juego_t juego;
  inicializar_juego(&juego);
  actualizar_personaje(&juego);
  agregar_gimnasio(&juego);
  pa2m_mostrar_reporte();
  return EXITO;
}

int probar_gimnasio(){
  
  return EXITO;
}


static int comparador(void* elemento_1, void* elemento_2){
    if(*(int*)(elemento_1) == *(int*)elemento_2)
      return 0;
    
    if(*(int*)(elemento_1) > *(int*)(elemento_2))
      return -5;
    
    return 5;
}


int probar_heap(){
  pa2m_nuevo_grupo("Pruebas Heap");
  heap_t* heap = heap_crear(comparador, NULL);

  if(!heap){
    return -1;
  }
  pa2m_afirmar(heap->cantidad == 0, "se crea con 0 elementos");
  pa2m_afirmar(heap->comparador == comparador, "asigna correctamente el comparador");

  int elementos[] = {5, 3, 7, 9, 4, 23, 1, 2, 44, 12, 55, 22, 21, 90, 41};
  int elementos_esperados[] = {1, 2, 5, 3, 7, 22, 9, 4, 44, 12, 55, 23, 21, 90, 41};

  for(int i = 0; i < sizeof(elementos) / sizeof(int); i++){
    heap_insertar(heap, elementos+i);
  }

  int elementos_heap[sizeof(elementos) / sizeof(int)];

  for(int i = 0; i < heap->cantidad; i++){
    elementos_heap[i] = *(int*)(heap->elementos[i]);
  }

  bool posiciones_heap_correctas = true;
  for(int i = 0; i < heap->cantidad; i++){
    posiciones_heap_correctas = *(int*)(heap->elementos[i]) == (elementos_esperados[i]);
  }
  pa2m_afirmar(posiciones_heap_correctas, "Heap queda ordenado luego de insertar muchos elementos");
  heap_mostrar(elementos_heap, (int)(heap->cantidad));
  
  int* raiz = (int*)heap_extraer_raiz(heap);
  for(int i = 0; i < heap->cantidad; i++){
    elementos_heap[i] = *(int*)(heap->elementos[i]);
  }
  pa2m_afirmar(*raiz == 1, "La raiz se extrae correctamente");
  heap_mostrar(elementos_heap, (int)(heap->cantidad));

  while(heap->cantidad > 0){
    raiz = (int*)heap_extraer_raiz(heap);
  }

  pa2m_afirmar(heap->cantidad == 0, "Se remueven todos los elementos del arbol");


  heap_destruir(heap);
  pa2m_mostrar_reporte();
  return EXITO;
}

int probar(){
  probar_heap();
  probar_inicio();
  probar_gimnasio();
  return __pa2m_cantidad_de_pruebas_fallidas == 0;
}
