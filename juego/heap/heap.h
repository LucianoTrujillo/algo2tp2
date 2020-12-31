#ifndef __HEAP_H__
#define __HEAP_H__
#include <stdlib.h>
/*
 * Comparador de elementos. Recibe dos elementos del heap y devuelve
 * 0 en caso de ser iguales, numero menor a 0 si el primer elemento es mayor al
 * segundo o numero mayor a 0 si el primer elemento es menor al segundo.
 */
typedef int (*heap_comparador)(void*, void*);

/*
 * Destructor de elementos. Cada vez que un elemento deja el heap
 * se invoca al destructor pasandole el elemento.
 */
typedef void (*heap_destructor)(void*);

/*
  Heap minimal genérico implementado con vector dinámico
*/
typedef struct heap {
  void** elementos;
  size_t cantidad;
  heap_comparador comparador;
  heap_destructor heap_destructor;
} heap_t;

/*
*
*/
heap_t* heap_crear(heap_comparador comparador, heap_destructor destructor);


/*
*
*/
void heap_destruir(heap_t* heap);

/*
*
*/
int heap_insertar(heap_t* heap, void* elemento);

/*
*
*/
void heap_mostrar(int* elementos, int cantidad);

int probar_heap();

void* heap_extraer_raiz(heap_t* heap);

#endif /* __HEAP_H__ */
