#include <stdlib.h>
#include <stdio.h>
#include "abb.h"

#define EXITO 0
#define ERROR 1

#define IGUALES 0


/*
  -----------------------------------------------------------------------
  Funciones privadas. Sus firmas se encuentran arriba de su declaración |
  -----------------------------------------------------------------------
*/


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


/*
  Inserta el elemento en el sub arbol donde nodo es su raíz.
  Devuelve el nuevo arbol con el elemento insertado.
  Si no se pudo insertar el elemento, cambia el valor de estado a ERROR
  y deja el árbol intacto.
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

  if(comparador(nodo->elemento, elemento) < 0){
    nodo->derecha = insertar_elemento(nodo->derecha, elemento, comparador, estado);
  } else {
    nodo->izquierda = insertar_elemento(nodo->izquierda, elemento, comparador, estado);
  }
  
  return nodo;
}

/*
  Busca un elemento mediante el comparador.
  Devuelve el primer nodo que contiene a ese elemento.
  Si no encuentra al nodo, devuelve NULL.
*/
nodo_abb_t* buscar_nodo(nodo_abb_t* nodo, void* elemento, abb_comparador comparador){
  if(!nodo)
    return NULL;
    
  if(comparador(nodo->elemento, elemento) == IGUALES)
    return nodo;
  
  if(comparador(nodo->elemento, elemento) > 0)
      return buscar_nodo(nodo->izquierda, elemento, comparador);
  
  if(comparador(nodo->elemento, elemento) < 0)
    return buscar_nodo(nodo->derecha, elemento, comparador);

  return NULL;
}

/*
  Si existe el destructor, lo aplica sobre el elemento del nodo pasado por parametros.
  Libera la memoria alocada por el nodo.
*/
void destruir_nodo(nodo_abb_t* nodo, abb_liberar_elemento destructor){
  if(nodo && destructor)
      destructor(nodo->elemento);
  free(nodo);
}

/*
  Recorre el arbol inorden y agrega los elementos al array.
  Si la cantidad de elementos recorridos llega al tamanio del array, para el recorrido.
*/
void inorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad_actual){
  
  if(nodo->izquierda)
    inorden(nodo->izquierda, array, tamanio_array, cantidad_actual);

  if(!nodo || *cantidad_actual >= tamanio_array)
    return;

  array[*cantidad_actual] = nodo->elemento;
  (*cantidad_actual)++;

  if(nodo->derecha)
    inorden(nodo->derecha, array, tamanio_array, cantidad_actual);
}

/*
  Recorre el arbol preorden y agrega los elementos al array.
  Si la cantidad de elementos recorridos llega al tamanio del array, para el recorrido.
*/
void preorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad_actual){
  if(!nodo || *cantidad_actual == tamanio_array)
    return;
  
  array[(*cantidad_actual)++] = nodo->elemento;

  if(nodo->izquierda)
    preorden(nodo->izquierda, array, tamanio_array, cantidad_actual);


  if(nodo->derecha)
    preorden(nodo->derecha, array, tamanio_array, cantidad_actual);
}

/*
  Recorre el arbol postorden y agrega los elementos al array.
  Si la cantidad de elementos recorridos llega al tamanio del array, para el recorrido.
*/
void postorden(nodo_abb_t* nodo, void** array, size_t tamanio_array, size_t* cantidad_actual){

  if(nodo->izquierda)
    postorden(nodo->izquierda, array, tamanio_array, cantidad_actual);

  if(nodo->derecha)
    postorden(nodo->derecha, array, tamanio_array, cantidad_actual);

  if(!nodo || *cantidad_actual == tamanio_array)
    return;
  array[(*cantidad_actual)++] = nodo->elemento;
}

/*
  Recorre el arbol inorden aplicando sobre cada elemento la función pasado por parametros.
  Si la función devuelve true, se frena el recorrido.
*/
bool recorrer_cada_elemento_inorden(nodo_abb_t* nodo, size_t* cantidad_recorridos, bool (*funcion)(void*, void*), void* extra){
  if(!nodo)
    return true;

  if(nodo->izquierda)
    if(recorrer_cada_elemento_inorden(nodo->izquierda, cantidad_recorridos, funcion, extra))
      return true;

  (*cantidad_recorridos)++;
  if(funcion(nodo->elemento, extra))
    return true;

  if(nodo->derecha)
    if(recorrer_cada_elemento_inorden(nodo->derecha, cantidad_recorridos, funcion, extra))
      return true;

  return false;
}

/*
  Recorre el arbol preorden aplicando sobre cada elemento la función pasado por parametros.
  Si la función devuelve true, se frena el recorrido.
*/
bool recorrer_cada_elemento_preorden(nodo_abb_t* nodo, size_t* cantidad_recorridos, bool (*funcion)(void*, void*), void* extra){
  if(!nodo)
    return true;

  (*cantidad_recorridos)++;
  if(funcion(nodo->elemento, extra))
    return true;

  if(nodo->izquierda)
    if(recorrer_cada_elemento_preorden(nodo->izquierda, cantidad_recorridos, funcion, extra))
      return true;

  if(nodo->derecha)
    if(recorrer_cada_elemento_preorden(nodo->derecha, cantidad_recorridos, funcion, extra))
      return true;

  return false;
}

/*
  Recorre el arbol postorden aplicando sobre cada elemento la función pasado por parametros.
  Si la función devuelve true, se frena el recorrido.
*/
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

/*
  Llama a destruir_nodo por cada nodo del arbol en postorden.
*/
void destruir_recursivamente(nodo_abb_t* nodo, abb_liberar_elemento destructor){
  if(nodo){
    destruir_recursivamente(nodo->izquierda, destructor);
    destruir_recursivamente(nodo->derecha, destructor);
    destruir_nodo(nodo, destructor);
  }
}

/*
  Devuelve el predecesor inorden del nodo actual. (mayor de los menores).
*/
nodo_abb_t* obtener_predecesor_inorden(nodo_abb_t* nodo){
  if(!nodo || !nodo->derecha)
    return nodo;
  
  return obtener_predecesor_inorden(nodo->derecha);
}

/*
  Borra el primer nodo del arbol con el elemento pasado por parametros.
  Si no encuentra ningún nodo con ese elemento, deja el arbol intacto.
*/
nodo_abb_t* borrar_elemento(nodo_abb_t* nodo, void* elemento, abb_comparador comparador, abb_liberar_elemento destructor){
  if(!nodo) return NULL;

  //navegar hasta el nodo
  if(comparador(nodo->elemento, elemento) > 0){
    nodo->izquierda = borrar_elemento(nodo->izquierda, elemento, comparador, destructor);

  } else if(comparador(nodo->elemento, elemento) < 0){
    nodo->derecha = borrar_elemento(nodo->derecha, elemento, comparador, destructor);
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
    nodo_abb_t* predecesor_inorden = obtener_predecesor_inorden(nodo->izquierda);
    void* aux = nodo->elemento;
    nodo->elemento = predecesor_inorden->elemento;
    predecesor_inorden->elemento = aux;
    nodo->izquierda = borrar_elemento(nodo->izquierda, predecesor_inorden->elemento, comparador, destructor);
  }
  
  return nodo;
}

/*
  -------------------------------------------------------
  Funciones públicas. Sus firmas se encuentran el abb.h |
  -------------------------------------------------------
*/

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

int arbol_borrar(abb_t* arbol, void* elemento){
  if(!arbol)
    return ERROR;

  if(!arbol_buscar(arbol, elemento)) return ERROR;

  arbol->nodo_raiz = borrar_elemento(arbol->nodo_raiz, elemento, arbol->comparador, arbol->destructor);
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
