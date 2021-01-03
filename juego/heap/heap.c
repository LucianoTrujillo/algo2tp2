#include "heap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../juego.h"
#define ERROR -1
#define EXITO 0


void heap_mostrar(int* buffer, int n){
  for(int i = 0; i < n; i++){
    printf("%i ", buffer[i]);
  }

  printf("\n");
  int niveles = 0;
  for(int restantes=n;restantes>0;restantes>>=1)
    niveles++;
  
  int n_nodo=0;
  for(int nivel=0;nivel<niveles;nivel++){
    int posicion = 1<<(niveles-nivel-1);
    
    for(int nodo=0;nodo<(1<<nivel);nodo++){
      for(int i=1;i<posicion;i++){
        if(i<=posicion/2-1 || 2*n_nodo+1 >= n || buffer[2*n_nodo+1]==0)
          printf("  ");
        else{
          if(i == posicion/2)
            printf(" ┌");
          else printf("――");
        }
      }

      if(n_nodo<n && buffer[n_nodo]!=0){
        int valor = (int)((buffer[n_nodo]<100?buffer[n_nodo]:99));
        printf("%2d", valor);
      }
      else{
        printf("  ");
      }
      
      for(int i=0;i<posicion;i++){
        if(i>=posicion/2 || 2*n_nodo+2 >= n ||buffer[2*n_nodo+2]==0)
          printf("  ");
        else{
          if(i == posicion/2-1)
            printf("―┐");
          else printf("――");
        }
      }
      n_nodo++;
    }
    printf("\n");
  }

    printf("\n\n");
}

heap_t* heap_crear(heap_comparador comparador, heap_destructor destructor){
  if(!comparador)
    return NULL;

  heap_t* heap = calloc(1, sizeof(heap_t));

  if(!heap)
    return NULL;

  heap->comparador = comparador;
  heap->heap_destructor = destructor;
  return heap;

}

void heap_destruir(heap_t* heap){
  if(!heap)
    return;
  
  if(heap->heap_destructor){
    for(int i = 0; i < heap->cantidad; i++)
      heap->heap_destructor(heap->elementos[i]);
  }
  free(heap->elementos);
  free(heap);
}

void swap(heap_t* heap, size_t pos_1, size_t pos_2){
  void* aux = heap->elementos[pos_1];
  heap->elementos[pos_1] = heap->elementos[pos_2];
  heap->elementos[pos_2] = aux;
}

size_t posicion_padre(size_t posicion_elemento){
  return posicion_elemento / 2;
}

size_t posicion_hijo_izquierdo(size_t posicion_elemento){
  return (posicion_elemento * 2) + 1;
}

size_t posicion_hijo_derecho(size_t posicion_elemento){
  return (posicion_elemento * 2) + 2;
}

void sift_up(heap_t* heap, size_t posicion_elemento){
  if(posicion_elemento == 0) // es raiz
    return;
  
  size_t pos_padre = posicion_padre(posicion_elemento);

  if(heap->comparador(heap->elementos[posicion_elemento], heap->elementos[pos_padre]) > 0){
    swap(heap, posicion_elemento, pos_padre);
    sift_up(heap, pos_padre);
  }
}

void sift_down(heap_t* heap, size_t posicion_elemento){
  size_t pos_hijo_izq = posicion_hijo_izquierdo(posicion_elemento);
  size_t pos_hijo_der = posicion_hijo_derecho(posicion_elemento);
  size_t pos_menor = pos_hijo_izq;

  if(pos_hijo_izq >= heap->cantidad)
    return;
  
  if(pos_hijo_der < heap->cantidad){
    if(heap->comparador(heap->elementos[pos_hijo_izq], heap->elementos[pos_hijo_der]) < 0){
      pos_menor = pos_hijo_der;
    }
  } 

  if(heap->comparador(heap->elementos[posicion_elemento], heap->elementos[pos_menor]) < 0){
    swap(heap, posicion_elemento, pos_menor);
    sift_down(heap, pos_menor);
  }
}

int heap_insertar(heap_t* heap, void* elemento){
  if(!heap)
    return ERROR;

  void* aux = realloc(heap->elementos, sizeof(void*) * (heap->cantidad + 1));

  if(!aux)
    return ERROR;
  
  heap->cantidad++;
  heap->elementos = aux;

  heap->elementos[heap->cantidad - 1] = elemento;
  sift_up(heap, heap->cantidad - 1);
  return 0;
}

void* heap_extraer_raiz(heap_t* heap){
  if(!heap || heap->cantidad == 0)
    return NULL;

  void* raiz = heap->elementos[0];
  heap->elementos[0] = heap->elementos[heap->cantidad - 1];
  heap->cantidad--;

  if(heap->cantidad > 0)
    sift_down(heap, 0);
  
  return raiz;
}

void* heap_raiz(heap_t* heap){
  if(!heap || heap->cantidad == 0)
    return NULL;

  return heap->elementos[0];
}

size_t heap_cantidad(heap_t* heap){
  return heap->cantidad;
}
