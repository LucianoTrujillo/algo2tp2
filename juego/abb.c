#include <stdlib.h>
#include <stdio.h>
#include "abb.h"
#include "lista.h"

#define PRIMER_ELEMENTO_MAYOR 1
#define IGUALES 0
#define SEGUNDO_ELEMENTO_MAYOR -1

#define IZQ -1
#define DER 1

typedef struct cosita{
    float clave;
    char contenido[10];
}cosita_t;

/*
    Crea un nodo cuyo elemento es el pasado por parámetros.
    En caso de no poder crearlo devuelve NULL.
*/
nodo_abb_t* crear_nodo(void* elemento){
    nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));

    if(!nodo)
        return NULL;
    nodo->derecha = NULL;
    nodo->izquierda = NULL;
    nodo->elemento = elemento;
    return nodo;
}

/*typedef struct cosita {
    float clave;
    char contenido[10];
}cosita_t;
*/
void imprimir_arbol(abb_t* arbol){
  lista_t* cola = lista_crear();
  nodo_abb_t* corriente = arbol->nodo_raiz;
  lista_encolar(cola, corriente);

  if(!arbol) return;
  
  while(!lista_vacia(cola)){
    corriente = lista_primero(cola);
    printf("%i ", ((cosa*)(corriente->elemento))->clave);
    lista_desencolar(cola);

    if(corriente->izquierda)
      lista_encolar(cola, corriente->izquierda);
    
    if(corriente->derecha)
      lista_encolar(cola, corriente->derecha);
  }

  lista_destruir(cola);
  printf("\n\n");
}

/*

*/
void* insertar_elemento(nodo_abb_t* nodo, void* elemento, abb_comparador comparador, int* estado){
  if(!nodo){
    nodo_abb_t* nuevo_nodo = crear_nodo(elemento);
    if(!nuevo_nodo){
      *estado = ERROR;
      return NULL;
    } 
      
    return nuevo_nodo;
  }

  if(comparador(nodo->elemento, elemento) == SEGUNDO_ELEMENTO_MAYOR){
    nodo->derecha = insertar_elemento(nodo->derecha, elemento, comparador, estado);
  } else {
    nodo->izquierda = insertar_elemento(nodo->izquierda, elemento, comparador, estado);
  }
  
  return nodo;
}

/*

*/
nodo_abb_t* buscar_nodo(nodo_abb_t* nodo, void* elemento, abb_comparador comparador){
  if(!nodo)
    return NULL;
    
  if(comparador(nodo->elemento, elemento) == IGUALES)
    return nodo;
  
  if(comparador(nodo->elemento, elemento) == PRIMER_ELEMENTO_MAYOR)
      return buscar_nodo(nodo->izquierda, elemento, comparador);
  
  if(comparador(nodo->elemento, elemento) == SEGUNDO_ELEMENTO_MAYOR)
    return buscar_nodo(nodo->derecha, elemento, comparador);

  return NULL;
}

/*

*/
void destruir_nodo(nodo_abb_t* nodo, abb_liberar_elemento destructor){
  if(nodo && destructor)
      destructor(nodo->elemento);
  free(nodo);
}

void inorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad_actual){
  if(!nodo || *cantidad_actual == tamanio_array)
    return;
  
  if(nodo->izquierda)
    inorden(nodo->izquierda, array, tamanio_array, cantidad_actual);

  array[(*cantidad_actual)++] = nodo->elemento;

  if(nodo->derecha)
    inorden(nodo->derecha, array, tamanio_array, cantidad_actual);
}

void preorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad_actual){
  if(!nodo || *cantidad_actual == tamanio_array)
    return;
  
  array[(*cantidad_actual)++] = nodo->elemento;

  if(nodo->izquierda)
    preorden(nodo->izquierda, array, tamanio_array, cantidad_actual);


  if(nodo->derecha)
    preorden(nodo->derecha, array, tamanio_array, cantidad_actual);
}

void postorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad_actual){
  if(!nodo || *cantidad_actual == tamanio_array)
    return;

  if(nodo->izquierda)
    postorden(nodo->izquierda, array, tamanio_array, cantidad_actual);

  if(nodo->derecha)
    postorden(nodo->derecha, array, tamanio_array, cantidad_actual);

  array[(*cantidad_actual)++] = nodo->elemento;
}


bool  recorrer_cada_elemento_inorden(nodo_abb_t* nodo, size_t* cantidad_recorridos, bool (*funcion)(void*, void*), void* extra){
  if(!nodo)
    return true;

  if(nodo->izquierda){
    if(recorrer_cada_elemento_inorden(nodo->izquierda, cantidad_recorridos, funcion, extra))
      return true;
  }

  (*cantidad_recorridos)++;
  if(funcion(nodo->elemento, extra)){
    return true;
  }

  if(nodo->derecha)
    if(recorrer_cada_elemento_inorden(nodo->derecha, cantidad_recorridos, funcion, extra))
      return true;
  return false;
}

bool recorrer_cada_elemento_preorden(nodo_abb_t* nodo, size_t* cantidad_recorridos, bool (*funcion)(void*, void*), void* extra){
  if(!nodo)
    return true;

  (*cantidad_recorridos)++;
  if(funcion(nodo->elemento, extra)){
    return true;
  }

  if(nodo->izquierda){
    if(recorrer_cada_elemento_preorden(nodo->izquierda, cantidad_recorridos, funcion, extra))
      return true;
  }

  if(nodo->derecha)
    if(recorrer_cada_elemento_preorden(nodo->derecha, cantidad_recorridos, funcion, extra))
      return true;
  return false;
}

bool recorrer_cada_elemento_postorden(nodo_abb_t* nodo, size_t* cantidad_recorridos, bool (*funcion)(void*, void*), void* extra){
  if(!nodo)
    return true;

  if(nodo->izquierda){
    if(recorrer_cada_elemento_postorden(nodo->izquierda, cantidad_recorridos, funcion, extra))
      return true;
  }

  if(nodo->derecha)
    if(recorrer_cada_elemento_postorden(nodo->derecha, cantidad_recorridos, funcion, extra))
      return true;

  (*cantidad_recorridos)++;
  if(funcion(nodo->elemento, extra)){
    return true;
  }

  return false;
}

void destruir_recursivamente(nodo_abb_t* nodo, abb_liberar_elemento destructor){
  if(nodo){
    destruir_recursivamente(nodo->izquierda, destructor);
    destruir_recursivamente(nodo->derecha, destructor);
    destruir_nodo(nodo, destructor);
  }
}

abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
    if(!comparador)
        return NULL;

    abb_t* arbol = malloc(sizeof(abb_t));

    if(!arbol)
        return NULL;

    arbol->comparador = comparador;
    arbol->destructor = destructor;
    arbol->nodo_raiz = NULL;

    return arbol;
}


int arbol_insertar(abb_t* arbol, void* elemento){
  int estado = EXITO;
  if(!arbol){
    estado = ERROR;
  } else {
    arbol->nodo_raiz = insertar_elemento(arbol->nodo_raiz, elemento, arbol->comparador, &estado);
  }

  return estado;
}

void* arbol_buscar(abb_t* arbol, void* elemento){
  if(!arbol)
    return NULL;

  nodo_abb_t* nodo = buscar_nodo(arbol->nodo_raiz, elemento, arbol->comparador);

  if(!nodo) return NULL;
  return nodo->elemento;
}

nodo_abb_t* sucesor_inorden(nodo_abb_t* nodo){
  if(!nodo || !nodo->derecha)
    return nodo;
  
  return sucesor_inorden(nodo->derecha);
}

nodo_abb_t* borrar_rec(nodo_abb_t* nodo, void* elemento, abb_comparador comparador, abb_liberar_elemento destructor){
  if(!nodo) return nodo;

  //navegar hasta el nodo
  if(comparador(nodo->elemento, elemento) == PRIMER_ELEMENTO_MAYOR){
    nodo->izquierda = borrar_rec(nodo->izquierda, elemento, comparador, destructor);

  } else if(comparador(nodo->elemento, elemento) == SEGUNDO_ELEMENTO_MAYOR){
    nodo->derecha = borrar_rec(nodo->derecha, elemento, comparador, destructor);
  } else {
    //nodo tiene una rama
    if(!nodo->izquierda){
      nodo_abb_t* rama_derecha = nodo->derecha;
      destruir_nodo(nodo, destructor);
      return rama_derecha;

    } else if(!nodo->derecha){
      nodo_abb_t* rama_izquierda = nodo->izquierda;
      destruir_nodo(nodo, destructor);
      return rama_izquierda;

    }

    //nodo tiene 2 ramas
    nodo_abb_t* sucesor_inorden_izquierdo = sucesor_inorden(nodo->izquierda);
    nodo_abb_t* aux = nodo->elemento;
    nodo->elemento = sucesor_inorden_izquierdo->elemento;
    sucesor_inorden_izquierdo->elemento = aux;
    nodo->izquierda = borrar_rec(nodo->izquierda, sucesor_inorden_izquierdo->elemento, comparador, destructor);
  }
  
  return nodo;
}

int arbol_borrar(abb_t* arbol, void* elemento){
  if(!arbol)
    return ERROR;

  arbol->nodo_raiz = borrar_rec(arbol->nodo_raiz, elemento, arbol->comparador, arbol->destructor);

  return EXITO;
}



void* arbol_raiz(abb_t* arbol){
    if(!arbol || !arbol->nodo_raiz)
        return NULL;
    
    return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol){
    return !arbol || !arbol->nodo_raiz;
}

size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){
  if(!arbol || !arbol->nodo_raiz || !array)
    return 0;
  
  size_t cantidad_actual = 0;
  inorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_actual);
  return cantidad_actual;
}

size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){
  if(!arbol || !arbol->nodo_raiz || !array)
    return 0;
  
  size_t cantidad_actual = 0;
  preorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_actual);
  return cantidad_actual;
}

size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){
  if(!arbol || !arbol->nodo_raiz || !array)
    return 0;
  
  size_t cantidad_actual = 0;
  postorden(arbol->nodo_raiz, array, tamanio_array, &cantidad_actual);
  return cantidad_actual;
}

size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
  if(!arbol || !funcion)
    return 0;

  size_t cantidad_recorridos = 0;

  switch (recorrido){
  case ABB_RECORRER_INORDEN:
    recorrer_cada_elemento_inorden(arbol->nodo_raiz, &cantidad_recorridos, funcion, extra);
    break;
  case ABB_RECORRER_PREORDEN:
    recorrer_cada_elemento_preorden(arbol->nodo_raiz, &cantidad_recorridos, funcion, extra);
    break;
  case ABB_RECORRER_POSTORDEN:
    recorrer_cada_elemento_postorden(arbol->nodo_raiz, &cantidad_recorridos, funcion, extra);
    break;
  }

  return cantidad_recorridos;
}

void arbol_destruir(abb_t* arbol){
  if(arbol){
    destruir_recursivamente(
      arbol->nodo_raiz,
      arbol->destructor);
    free(arbol);
  }
}
