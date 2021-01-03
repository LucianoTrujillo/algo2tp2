# TP 2 - Pokemon - Algoritmos y Programacion 2 FIUBA
[![N|Solid](https://res.cloudinary.com/practicaldev/image/fetch/s--y3j6aJXJ--/c_limit%2Cf_auto%2Cfl_progressive%2Cq_auto%2Cw_880/https://res.cloudinary.com/practicaldev/image/fetch/s--_PwtVEkJ--/c_limit%252Cf_auto%252Cfl_progressive%252Cq_auto%252Cw_880/https://www.educative.io/api/edpresso/shot/5077575695073280/image/5192456339456000)](https://nodesource.com/products/nsolid)

# El programa

## ¿Qúe es?

Es un juego de terminal interactivo cuyo objetivo es convertirse en maestro pokemon.
Para ello, se debe vencer a todos los gimnasios, haciendo un uso inteligente de tu pokedex para
derrotar a tus oponentes. 

Para vencer a un gimnasio se necesita vencer a todos los entrenadores y al líder.
Una vez que consigas derrotarlo, obtendrás una medalla.
Para llegar al líder, debes enfrentarte contra todos los entrenadores previos.

A medida que batalles:
  - Podras intercambiar los pokemones que usas para combatir con los que tienes en la reserva. 
  - Tus pokemones irán mejorando en sus atributos
  - podrás elegir un pokemon del líder para agregar a tu reserva y luego ser usando en combate.

Podrás repetir cuantas veces quieras los combates contra los entrenadores hasta poder derrotarlos.
Una vez derrotados, ya no podrás volver a luchar contra ellos.

El juego está organizado mediante menús, en cada uno tendrás diferentes acciones disponibles
que te irán guiando hacia el final.

## Compilación

Para compilarlo se puede hacer uso del siguiente comando dentro de la carpeta del proyecto.
```
$ make tp2
```

## Ejecución

La ejecución es muy sencilla, solo basta con el siguiente comando.
```
$ ./tp2
```

## Pruebas de Memoria

Para verificar que no haya pérdida de memoria con Valgrind, se puede hacer uso del siguiente comando.
```
$ make mem_test
```
Esto permitirá jugar normalmente, y al finalizar, mostrará el resultado del uso de memooria y si hubo pérdidas.

## Implementación del Juego

El juego está hecho en C, sin utilizar librerías gráficas, toda la interacción es mediante la terminal.
Se puso en práctica la utilización de estructuras de datos convenientes para un código sólido, que se 
adecúe a las necesidades del funcionamiento.

### Organizacion del Proyecto y Estructuras

Como todo programa en C, cuenta con un main.c como punto de entrada donde luego se llama al juego en sí.
El código del juego se encuentra en la carpeta _code_ /juego _code_. 
En esta, se encuentran varias carpetas, cada una contiene la lógica de una parte del juego:

  - El juego.c y juego.h conforman el TDA Juego, el cual contiene las primitivas crear, jugar y destruir.
    Este TDA se encarga de manejar el flujo del juego, decidiendo que opciones puede tomar el usuario
    y delegando la funcionalidad a otros lugares.

    este TDA Es muy imporante, dado que le da la "forma" al juego, guiando al usuario a través de los menús
    sin tener la lógica de que es lo que se hace en cada uno.

  - Dentro de la carpeta _code_ /inicio _code_ se encuentra la lógica de lectura de archivos correspondiente al
    entrenador que usa el usuario (el personaje principal), además de la correspondiente a los gimnasios.

  - Dentro de la carpeta _code_ /gimnasio _code_ se encuentra la lógica relacionada a lo que se puede hacer dentro de un gimnasio:
    ver la información de tu entrenador, ver la información del gimnasio y reorganizar tu pokedex.

  - Dentro de la carpeta _code_ /batalla _code_ se encuentra la lógica relacionada a los enfrentamientos con los entrenadores y sus pokemon:
    contiene la lógica de los combates entre pokemones y Se encarga de controlar el flujo de una batalla, cuando pierde, cuando gana. 
  
  - Dentro de la carpeta _code_ /interfaz _code_ se encuentra la lógica relacionada a la interacción entre el usuario y el juego:
    imprimir los mensajes, mostrar los menús, manejo de elección de opciones. 

### Utilizacion y explicacion del uso de los TDAS

Las estructuras usadas fueron: Lista, Cola, Pila, Árbol Binario de Búsqueda, Árbol Heap básico.

Aclaracion: Dado que la implementación la Lista, Cola, Pila estan en un mismo TDA, con lo cual hay solapamiento.

  - Heap: El Heap fue usado para guardar los gimnasios a los cuales se enfrenta el entrenador.
    Dado que tienen que ir apareciendo de mas fácil a más dificil, es lógico usar una estructura de este tipo,
    porque el heap asegura que la raiz siempre sera el minimo (o maximo) de nuestro conjunto de elementos.

  - Arbol Binario de Busqueda: El ABB se utilizo para guardar los pokemones de reserva. 
    Es decir, los obtenidos pero que no se utilizan para combatir. 
    Se decidio utilizar esta estructura teniendo en cuenta los siguientes factores:
    - Pueden haber una cantidad considerablemente grande de pokemones almacenados
    - Los pokemones tienen un atributo clave (ID) para poder ser comparados
    - Se supone que el conjunto de pokemones que se conserva es bastante cambiante

    Dada estas características del problema a resolver, el ABB viene a resolver todos los problemas, dado que:
    - Soporta una gran cantidad de elementos, dado que usa memoria dinámica
    - Al poder comparar pokemones por ID, puede ordenarlos.
    - Al mantenerlos ordenados y dado la estructura del ABB, la inserción y eliminación es más eficiente que una lista (si no se deforma en una).

  - Pila: La Pila fue usada para los entrenadores



# Conceptos teóricos

## Lista

### Concepto
A nivel macro, una lista es un **conjunto de elementos ordenados**. 
Este concepto es utilizado todo el tiempo en nuestra vida contidiana, por ejemplo:
   - Cuando queremos comprar ciertos alimentos en el supermercado.
   - Cuando queremos tener la información de todos los invitados a un cumpleaños.
   - Cuando queremos organizarnos para hacer ciertas tareas en un día.
   - Este elemento actual es parte de una lista.

### Informática
En la informática hay ciertas operaciones que se pueden realizar sobre una lista, aunque dependiendo del problema a ser resuelto, puede sufrir variaciones.

Las **operaciones** mas comunes que se hacen sobre una lista son:

   - Crear.
   - Destruir.
   - Insertar en posición *n*.
   - Eliminar en posición *n*.
   - ¿Esta vacía?.
   - Ver elemento en posición *n*.

Una lista puede tener diferentes implementaciones ya sea estática, dinámica, usando nodos, etc. Cada una ofrece diferentes pros y cons, pero ahora nos interesa en particular la lista dinámicamente usada con **nodos**.

Un **nodo** es tipo de dato que dependiendo del tipo de lista varía, pero contiene un dato y referencias a almenos un nodo que no sea el mismo. 

### Simplemente Enlazada
En el caso de las listas simplemente enlazadas, se consta de nodos conformados por:
   - un dato.
   - una *referencia* al siguiente elemento en la lista.

La estructura de una lista con nodos enlazados permite la gran **ventaja** de que la memoria no debe ser contigua, es decir no se necesita reservar un bloque de memoria que guarde toda la lista, si no que se va reservando de a "pedazitos" (bloque que ocupa un nodo, muy poco comparado con la lista entera). Además, las operaciones como agregar y eliminación solo requieren unos pocos pasos, para cambiar las referencias de los nodos. No se necesita mover los otros nodos de lugar.

La lista de nodos enlazados maneja de manera muy **eficiente** la memoria ocupada, ya que se va agrandando y reduciendo para ocupar solo lo que necesita. A diferencia del vector dinámico, la lista no tiene que preocuparse por no encontrar un bloque de memoria nuevo donde guardar la lista en caso de fallo (lo cual genera un *O(n)* para la insersión o remoción en el peor caso del vector dinámico, pues tiene que copiar todos los elementos al nuevo vector).

Sin embargo, la lista de nodos enlazados tiene sus *desventajas*. Al no tener la memoria contigua fisicamente, para acceder/agregar/modificar un elemento en la posición *n*, se debe **recorrer** la lista hasta llegar a ese elemento, dado que no se puede calcular la posición de memoria de antemano. Esto trae una deficiencia a nivel complejidad en estas operaciones básicas sobre una lista.

### Doblemente Enlazada
En el caso de las listas doblemente enlazadas, se consta de nodos conformados por:
   - un dato.
   - una *referencia* al siguiente elemento en la lista (si es que existe). 
   - una *referencia* al anterior elemento en la lista (si es que existe).

Esta implementación de lista permite una manipulación mas fácil para ciertas operaciones particulares como invertir una lista y es muy útil para cuando se requiere recorrer de manera bidireccional la lista.

### Comparando implementaciones
A continuación algunos datos para dimensionar la complejidad (del **peor** caso) de las operaciones más comunes entre las diferentes implementaciones de una lista: 

| Operación | Vector Estático | Vector Dinámico | Nodos Simplemente Enlazados |
| ------ | ------ | ------ | ------ |
| Crear | *O(1)* | *O(1)* | *O(1)*
| Destriur | *O(n)* | *O(n)* | *O(n)* |
| Insertar en lugar *n* | *O(n)* | *O(n)* | *O(n)* |
| Eliminar en lugar *n* | *O(n)* | *O(n)* | *O(n)* |
| Está vacía | *O(1)* | *O(1)* |*O(1)* |
| Ver Elemento en lugar *n* | *O(1)* | *O(1)* | *O(n)* |

*Nota 1: Se asume que en todos lo casos se usan en conjunto con una variable de cantidad actual.* <br />
*Nota 2: Se asume que insertar y destruir quieren mantener el bloque contiguo, es decir en el caso de los vectores se quiere una eliminación física, teniendo que mover a todos los elementos de lugar en el peor caso.*  <br />
*Nota 3: Se asume que las listas empiezan con 0 elementos.*  <br />
*Nota 4: La complejdiad no es el único factor que determina cual de los tres usar. El vector estático no tiene la posibilidad de agrandarse y se debe reservar de antemano todo el tamaño posible que pueda llegar a ocupar la lista.* 

## Pilas

### Concepto
Al igual que la lista, la pila es un conjunto de elementos ordenados. Sin embargo, lo que las diferencia son las **operaciones** que se pueden realizar sobre cada una. Su estructura es conocida como **LIFO** (Last in, first out), en el cual como explica el nombre, el útlimo en entrar es el primero en salir. Esto restringe las operaciones que nos permiten las listas, pero permiten modelar mde una manera mas adecuada ciertas estructuras y comportamientos:

   - Platos de concina.
   - Cartas del juego UNO (si es que nadie hace trampa).
   - El historial del navegador.

El factor intrínseco de las pilas es que la prioridad de salida se la da al que "acaba de llegar".

### Características Fundamentales

Al igual que las listas, las pilas tienen ciertas operaciones fundamentales:

   - Crear.
   - Destruir.
   - Apilar.
   - Desapilar.
   - Tope.
   - ¿Está vacía?

## Colas

### Concepto
Al igual que la lista, la cola es un conjunto de elementos ordenados. Sin embargo, lo que las diferencia son las **operaciones** que se pueden realizar sobre cada una. Su estructura es conocida como **FIFO** (first in, first out), en el cual como explica el nombre, el primero en entrar es el primero en salir. Al igual que las pilas, restringe las operaciones que nos permiten las listas, pero a la vez permite una manera de utilizarlas que es muy eficaz para resolver otros problemas de la vida contidiana:

   - Colas de colectivo.
   - Cola del supermercado.
   - Turno con la doctora.
   - Cola online de entradas a conciertos.

El factor intrínseco de las colas es que la prioridad de salida se la da al que estuvo más tiempo "esperando".

### Características Fundamentales

Al igual que las listas, las colas tienen ciertas operaciones fundamentales:

   - Crear.
   - Destruir.
   - Encolar.
   - Desencolar.
   - Obtener el primer elemento.
   - ¿Está vacía?

## Iteradores

Un iterador es un *herramienta* que permite **recorrer** de manera mas eficiente los elementos de una lista. Dependiendo del tipo, tiene más, o menos control sobre el recorrido de los elementos. En vez de tener que recorrer la lista desde el comienzo hasta el elemento actual siendo procesado (lo cual le da un orden de complejidad **O(n)** como peor caso), se accede directamente al elemento siguiente al anterior (cuyo orden de complejidad es **O(1)** para todos los casos).

### Iterador interno

Un iterador interno se encarga en absoluto de la lógica de iteración sobre la lista de elementos. Permite al usuario del iterador, pasar la acción que se quiere realizar sobre cada elemento de la lista.

Suele ser una sola función que se encarga de hacer la iteración, sin necesidad de crear estructuras extras.

Este tipo de iterador suele ser el más recomendado si su uso es posible, dado a que es menos propensa a errores de quien la utiliza. Además, requiere menos código de quien la utiliza y es más fácil de leer.

Como diría nuestro querido Edsger Dijkstra:

> “Simplicity is prerequisite for reliability.”

A continuación se muestra un ejemplo en pseudocódigo de como funciona un iterador interno:
```sh
funcion mostrar_personaje(personaje){
    ...
}

personajes = "Cutrulu", "Mamichula", "Ricardo Fort"

#iterador:
por_cada_personaje(lista_personajes, mostrar_personaje)

```

De esta manera, nos aseguramos que estamos recorriendo toda la lista de personajes y ejecutando la acción por cada uno de ellos.

### Iterador externo

Un iterador externo le permite al usuario el control sobre la iteración de los elementos. El iterador externo se encarga de pasar al siguiente elemento cuando el usuario lo desee. 

Es útil cuando se necesita un flujo de control complejo a la hora de recorrer una lista.

En general suelen ser menos recomendados porque al darle mas control al usuario, genera mas espacio para errores. Además, su uso es más tedioso por toda la preparación y desmontamiento del mismo.

A continuación se muestra un ejemplo en pseudocódigo de como funciona un iterador externo:
```sh
funcion mostrar_personaje(personaje){
    ...
}

personajes = "Cutrulu", "Mamichula", "Ricardo Fort"

# creación
iterador = crear_iterador(personajes)

# iteración
mientras_que(iterador_tiene_siguiente(iterador)){
    personaje = lista_elemento_actual(iterador)
    mostrar(personaje)
    lista_avanzar_elemento(iterador)
}

# destrucción
destruir_iterador(iterador)

```
Como se observa, su uso requiere de más líneas de código y es mas propenso a error.


Licencia
----

Derechos reservados © Luchito

