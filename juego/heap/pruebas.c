#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"
#include "../../pa2mm.h"
/*
 * Comparador de elementos. Recibe dos elementos del healp y devuelve
 * 0 en caso de ser iguales, numero menor a 0 si el primer elemento es mayor al
 * segundo o numero mayor a 0 si el primer elemento es menor al segundo.
 */
int comparador(void* elemento_1, void* elemento_2){
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
  return 0;
}
