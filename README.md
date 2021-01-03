# TP 2 - Pokemon - Algoritmos y Programacion 2 FIUBA
[![N|Solid](https://upload.wikimedia.org/wikipedia/commons/thumb/9/98/International_Pok%C3%A9mon_logo.svg/1200px-International_Pok%C3%A9mon_logo.svg.png)](https://nodesource.com/products/nsolid)

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
Esto permitirá jugar normalmente, y al finalizar, mostrará el resultado del uso de memoria y si hubo pérdidas.

## Implementación del Juego

El juego está hecho en C, sin utilizar librerías gráficas, toda la interacción es mediante la terminal.
Se puso en práctica la utilización de estructuras de datos convenientes para un código sólido, que se 
adecúe a las necesidades del funcionamiento.

### Organizacion del Proyecto y Estructuras

Como todo programa en C, cuenta con un main.c como punto de entrada donde luego se llama al juego en sí.
El código del juego se encuentra en la carpeta `/juego`. 
En esta, se encuentran varias carpetas, cada una contiene la lógica de una parte del juego:

  - El `juego.c` y `juego.h` conforman el TDA Juego, el cual contiene las primitivas crear, jugar y destruir.
    Este TDA se encarga de manejar el flujo del juego, decidiendo que opciones puede tomar el usuario
    y delegando la funcionalidad a otros lugares.

    este TDA Es muy imporante, dado que le da la "forma" al juego, guiando al usuario a través de los menús
    sin tener la lógica de que es lo que se hace en cada uno.

  - Dentro de la carpeta ` /inicio` se encuentra la lógica de lectura de archivos correspondiente al
    entrenador que usa el usuario (el personaje principal), además de la correspondiente a los gimnasios.

  - Dentro de la carpeta `/gimnasio` se encuentra la lógica relacionada a lo que se puede hacer previo a combatir con un entrenador:
    ver la información de tu entrenador, ver la información del gimnasio y reorganizar tu pokedex.

  - Dentro de la carpeta `/batalla` se encuentra la lógica relacionada a los enfrentamientos con los entrenadores y sus pokemon:
    contiene la lógica de los combates entre pokemones y Se encarga de controlar el flujo de una batalla, cuando pierde, y cuando gana. 
  
  - Dentro de la carpeta `/interfaz` se encuentra la lógica relacionada a la interacción entre el usuario y el juego:
    imprimir los mensajes, mostrar los menús, manejo de elección de opciones. 

### Utilizacion y explicacion del uso de los TDAS

Las estructuras usadas fueron: **Lista, Cola, Pila, Árbol Binario de Búsqueda, Árbol Heap básico**.

Aclaracion: Dado que la implementación la Lista, Cola, Pila estan en un mismo TDA, con lo cual hay solapamiento.

  - **Heap**: El Heap fue usado para guardar los gimnasios a los cuales se enfrenta el entrenador.
    Dado que tienen que ir apareciendo de mas fácil a más dificil, es lógico usar una estructura de este tipo,
    porque el heap asegura que la raiz siempre sera el minimo (o maximo) de nuestro conjunto de elementos. 

  - **Arbol Binario de Busqueda**: El ABB se utilizo para guardar los pokemones de reserva. 
    Es decir, los obtenidos pero que no se utilizan para combatir. 
    Se decidio utilizar esta estructura teniendo en cuenta los siguientes factores:
    - Pueden haber una cantidad considerablemente grande de pokemones almacenados
    - Los pokemones tienen un atributo clave (ID) para poder ser comparados
    - Se supone que el conjunto de pokemones que se conserva es bastante cambiante

    Dada estas características del problema a resolver, el ABB viene a resolver todos los problemas, dado que:
    - Soporta una gran cantidad de "ilimitada" elementos, dado que usa memoria dinámica
    - Al poder comparar pokemones por ID, puede ordenarlos.
    - Al mantenerlos ordenados y dado la estructura del ABB, la inserción y eliminación es más eficiente que una lista (si no se deforma en una).

  - **Pila**: La Pila fue usada para guardar los entrenadores. Cuando se va leyendo del archivo la información del gimnasio, los entrenadores se van leyendo en orden del mas dificil al mas facil (Asi están escritos estos archivos). Pero, se requiere que las batallas con los entrenadores vayan incrementando en dificultad a medida que avanza, significando que se deben combatir en orden facil a dificil. A diferencia del gimnasio, cada entrenador no tiene una dificultad definida la cual pueda ser comparada, asi que solo nos basamos en la lectura del archivo. Por esta razon, es necesaria usar una pila, accediendo siempre al tope de esta. 
  
  - **Lista**: La Lista fue utilizada para guardar los pokemones de combate tanto del entrenador principal como los de los entrenadores y lider. Esto se debe a que se requiere recorrerlos sin eliminarlos (al batallar), con lo cual la opcion mas facil es usar el iterador externo que provee nuestro TDA Lista para recorrerlos sin modificar la lista, y poder recorrerla cuantas veces se desee. 


### Modificaciones de los TDAs Existentes

**Lista**: 
-   Se le invirtio la la condicion de corte en el iterador interno para que sea coherente con el de arbol. En ambos casos, la funcion que se aplica sobre los elementos devuelve true si quiere parar y false de lo contrario.
-   Se le agrego una funcion destructora opcional para poder liberar la memoria que ocupan los pokemones en el heap. Para agregar un atributo extra, el destructor se le pasa cuando se llama a la funcion `destruir_lista` en vez de al crearla.
- tal vez lo de opcional bool en destruir del arbol

### Analisis de complejidad interesantes

Hay ciertas operaciones que se realizan que me llamaron la atencion. Por ejemplo, la destruccion de los gimnasios al finalizar el juego.

_g_: numero de gimnasios
_e_: numero de entrenadores
6: numero maximo de pokemones que puede tener un entrenador

para liberal el heap de gimnasios, se recorren todos los gimnasios O(g). Por cada gimnasio, se liberan todos sus entrenadores O(g\*e). Por cada entrenador, se liberan sus pokemones O(g\*e\*6) => O(g*e).


Licencia
----

Derechos reservados © Luchito

