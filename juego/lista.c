#include "lista.h"
#include <stdio.h>
#include <stdlib.h>

/*
    Crea un nodo con el elemento pasa por parámetro.
    El nuevo nodo apunta en siguiente a NULL.
    Si falla, devuelve NULL.
*/
nodo_t* nodo_crear(void* elemento){
    nodo_t* nuevo_nodo = malloc(sizeof(nodo_t));
        
    if(!nuevo_nodo)
       return NULL;
      
    nuevo_nodo->elemento = elemento;
    nuevo_nodo->siguiente = NULL;
    
    return nuevo_nodo;
}

lista_t* lista_crear(){
    lista_t* nueva_lista = malloc(sizeof(lista_t));
    
    if(!nueva_lista)
        return NULL;
    
    nueva_lista->nodo_inicio = NULL;
    nueva_lista->nodo_fin = NULL;
    nueva_lista->cantidad = 0;
    
    return nueva_lista;
}

/*
    Devuelve el **nodo** que se encuentra en la posición de la lista.
    Si no encuentra ningún nodo, devuelve NULL.
*/
nodo_t* lista_nodo_en_posicion(lista_t* lista, size_t posicion){
    if(!lista || lista->cantidad == 0)
        return NULL;
    
    nodo_t* nodo_actual = lista->nodo_inicio;
    int contador = 0;
    
    while(contador < posicion && contador < (lista->cantidad) && nodo_actual->siguiente){
        nodo_actual = nodo_actual->siguiente;
        contador++;
    }
    
    if(contador == posicion)
        return nodo_actual;
    
    return NULL;
}

int lista_insertar(lista_t* lista, void* elemento){
    if(!lista)
        return ERROR;
    
    nodo_t* nuevo_nodo = nodo_crear(elemento);
    
    if(!nuevo_nodo)
        return ERROR;
    
    if(lista->cantidad == 0){
        lista->nodo_inicio = nuevo_nodo;
        lista->nodo_fin = nuevo_nodo;
        lista->cantidad = 1;
        return EXITO;
    }
    
    lista->nodo_fin->siguiente = nuevo_nodo;
    lista->nodo_fin = nuevo_nodo;
    (lista->cantidad)++;
    
    return EXITO;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
    if(!lista)
        return ERROR;
    
    if(lista->cantidad == 0 || posicion >= (lista->cantidad)){
        return lista_insertar(lista, elemento);
    }
    
    nodo_t* nuevo_nodo = nodo_crear(elemento);
    
    if(!nuevo_nodo)
        return ERROR;
    
    
    if(posicion == 0){
        nuevo_nodo->siguiente = lista->nodo_inicio;
        lista->nodo_inicio = nuevo_nodo;
    } else {
        nodo_t* nodo_anterior;
        nodo_anterior = lista_nodo_en_posicion(lista, posicion - 1);
        nuevo_nodo->siguiente = nodo_anterior->siguiente;
        nodo_anterior->siguiente  = nuevo_nodo;
    }
    
    (lista->cantidad)++;
    
    return EXITO;
}

int lista_borrar(lista_t* lista){
    if(!lista || lista->cantidad == 0)
        return ERROR;
        
    free(lista->nodo_fin);
    (lista->cantidad)--;
    lista->nodo_fin = lista_nodo_en_posicion(lista, lista->cantidad - 1);
    
    if(lista->cantidad == 0){
        lista->nodo_inicio = NULL;
        lista->nodo_fin = NULL;
    } else {
        lista->nodo_fin->siguiente = NULL;
    }
    return EXITO;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
    if(!lista || lista->cantidad == 0)
        return ERROR;
    
    if(posicion >= lista->cantidad - 1 || lista->cantidad == 1){
        return lista_borrar(lista);
    }
    
    nodo_t* aux;
    if(posicion == 0){
        aux = lista->nodo_inicio;
        lista->nodo_inicio = lista->nodo_inicio->siguiente;
    } else {
        nodo_t* nodo_anterior = lista_nodo_en_posicion(lista, posicion - 1);
        aux = nodo_anterior->siguiente;
        nodo_anterior->siguiente = nodo_anterior->siguiente->siguiente;
    }
    
    free(aux);
    (lista->cantidad)--;
    
    return EXITO;
}

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
    if(!lista || posicion >= (lista->cantidad))
        return NULL;
    
    nodo_t* nodo = lista_nodo_en_posicion(lista, posicion);
    
    if(!nodo)
        return NULL;
        
    return nodo->elemento;
}

size_t lista_elementos(lista_t* lista){
    if(!lista)
        return 0;
    
    return lista->cantidad;
}

void* lista_ultimo(lista_t* lista){
    if(!lista || !lista->nodo_fin)
        return NULL;
    
    
    return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t* lista){
    return !lista || lista->cantidad == 0;
}

int lista_apilar(lista_t* lista, void* elemento){
    return lista_insertar(lista, elemento);
}

int lista_desapilar(lista_t* lista){
    return lista_borrar(lista);
}

void* lista_tope(lista_t* lista){
    return lista_ultimo(lista);
}

int lista_encolar(lista_t* lista, void* elemento){
    return lista_apilar(lista, elemento);
}

int lista_desencolar(lista_t* lista){
    return lista_borrar_de_posicion(lista, 0);
}

void* lista_primero(lista_t* lista){
    return lista_elemento_en_posicion(lista, 0);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
    if(!lista)
        return NULL;
    
    lista_iterador_t* iterador = malloc(sizeof(lista_iterador_t));
    
    if(!iterador)
        return NULL;
    
    iterador->lista = lista;
    iterador->corriente = lista->nodo_inicio;
    
    return iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
    return iterador && iterador->corriente;
}


bool lista_iterador_avanzar(lista_iterador_t* iterador){
    if(iterador && iterador->corriente){
        bool pudo_avanzar = iterador->corriente;
        iterador->corriente = iterador->corriente->siguiente;
        return pudo_avanzar;
    }
    
    return false;
}

void* lista_iterador_elemento_actual(lista_iterador_t* iterador){
    if(iterador && iterador->corriente)
        return iterador->corriente->elemento;
    
    return NULL;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
    free(iterador);
}

/*
    Libera recursivamente la memoria de todos los nodos enlazados que le siguen, incluyéndose a él mismo.
*/
void lista_destruir_nodos(nodo_t* nodo){
    if(nodo){
        lista_destruir_nodos(nodo->siguiente);
        free(nodo);
    }
}

void lista_destruir(lista_t* lista){
    if(lista){
        lista_destruir_nodos(lista->nodo_inicio);
        free(lista);
    }
}

size_t lista_con_cada_elemento(lista_t* lista, bool (*funcion)(void*, void*), void *contexto){
    if(!lista || !funcion)
        return 0;

    nodo_t* nodo_actual = lista->nodo_inicio;
    size_t contador = 0;
    bool seguir_iterando = true;
     
    while(contador < (lista->cantidad) && seguir_iterando){
        seguir_iterando = funcion(nodo_actual->elemento, contexto);
        nodo_actual = nodo_actual->siguiente;
        contador++;
    }
        
    return contador;
}
