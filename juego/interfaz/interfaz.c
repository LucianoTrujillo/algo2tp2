//
// Created by luciano on 22/12/20.
//

#include "interfaz.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/ioctl.h> //para obtener dimension de la consola
#include <time.h>
#include <stdlib.h>

#define AMARILLO "\x1b[33;1m"
#define BLANCO "\x1b[37;1m"
#define TITULO "\x1b[37;4m"
#define CERRAR_TITULO "\x1b[37;0m"

#define ANCHO_OPCION 42
#define ALTO_OPCION 3
#define ANCHO_BORDE 1
#define ANCHO_BARRA_CARGA 100
#define ALTO_BARRA_CARGA 1
#define DELAY_CARGA 150
#define MILISEGUNDOS_EN_SEGUNDO 1000

#define ANCHO_DISPLAY 95
#define ALTO_DISPLAY 25
#define LINEAS_DISPLAY 10
#define MAX_CARACTERES_LINEA_DISPLAY 55
#define PADDING_DISPLAY 2
#define VELOCIDAD_LETRAS 25
#define VELOCIDAD_PAUSAS 400

#define MAX_OPCIONES 10

#define ENTER 13
#define ARRIBA 'w' 
#define ABAJO 's'
#define DERECHA 'd'
#define IZQUIERDA 'a'
#define CANCELAR 3

void delay(int milisegundos)
{
    long pausa;
    clock_t ahora, antes;

    pausa = milisegundos*(CLOCKS_PER_SEC/MILISEGUNDOS_EN_SEGUNDO);
    ahora = antes = clock();
    while((ahora-antes) < pausa)
        ahora = clock();
}

void mostrar_barra_carga(int maximos_milisegundos_carga){
    srand((unsigned int)time((time_t*)0));
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    int ancho_media_pantalla = (int)((w.ws_col - ANCHO_BARRA_CARGA) / 2);
    int alto_media_pantalla = (int)((w.ws_row - ALTO_BARRA_CARGA) / 2);

    system("clear");
    for(int i = 0; i <= 100; i++){
        for(int i = 0; i < alto_media_pantalla; i++) printf("\n");
        printf("%*s", ancho_media_pantalla, " ");
        int carga = 0;
        for(; carga <= i; carga++){
            printf("▰");
        }
        for(;carga <= 100; carga++){
            printf("▱");
        }
        printf(" %i%%\n", i);
        delay(rand() %  (maximos_milisegundos_carga / 100));
        for(int i = 0; i < alto_media_pantalla; i++) printf("\n");

        system("clear");
    }

}


void mostrar_opcion(char* opcion, bool seleccionada){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    int ancho_media_pantalla = (int)((w.ws_col - ANCHO_OPCION) / 2);
    int padding_opcion = (int)((ANCHO_OPCION - 2 * ANCHO_BORDE - strlen(opcion)) / 2);
    int padding_extra = strlen(opcion) % 2 != 0;
  
    printf(BLANCO);
    if(seleccionada){
        printf(AMARILLO);
        printf("%*s", ancho_media_pantalla, " ");
        printf("╔════════════════════════════════════════╗\n");
        printf("%*s", ancho_media_pantalla, " ");
        printf("║%*s%s%*s║\n",
               padding_opcion, "",
               opcion,
               padding_opcion + padding_extra, "");
        printf("%*s", ancho_media_pantalla, " ");
        printf("╚════════════════════════════════════════╝\n");
        printf(BLANCO);
    } else {
        printf("%*s", ancho_media_pantalla, " ");
        printf("┌────────────────────────────────────────┐\n");
        printf("%*s", ancho_media_pantalla, " ");
        printf("│%*s%s%*s│\n",
               padding_opcion, "",
               opcion,
               padding_opcion + padding_extra, "");
        printf("%*s", ancho_media_pantalla, " ");
        printf("└────────────────────────────────────────┘\n");
    }
}

void mostrar_mensaje(char* mensaje, bool mostrar_continuar){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    int ancho_media_pantalla = (int)((w.ws_col - ANCHO_DISPLAY) / 2);
    int alto_media_pantalla = (int)((w.ws_row - ALTO_DISPLAY) / 2);

    if(!mensaje || !*mensaje || strlen(mensaje) > (LINEAS_DISPLAY * MAX_CARACTERES_LINEA_DISPLAY)){
        return;
    }

    char texto[LINEAS_DISPLAY][MAX_CARACTERES_LINEA_DISPLAY + 1];

    int cant_lineas = (int)(1 + (strlen(mensaje) / MAX_CARACTERES_LINEA_DISPLAY));

    for(int i = 0; i < LINEAS_DISPLAY; i++){
        strcpy(texto[i], "\0");
    }

    for(int i = 0; i <= cant_lineas; i++){
        int cant_letras_linea = (int)(strlen(mensaje) > MAX_CARACTERES_LINEA_DISPLAY ? MAX_CARACTERES_LINEA_DISPLAY : strlen(mensaje));
        strncpy(texto[i], mensaje, cant_letras_linea);
        texto[i][cant_letras_linea] = '\0';
        mensaje+=cant_letras_linea;
    }
    for(int i = 0; i < alto_media_pantalla; i++) printf("\n");

    printf("%*s", ancho_media_pantalla, " ");
       printf("           ____________________________________________________________________\n ");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "         /                                                                    \\ \n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "          |    _____________________________________________________________   |\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "          |   |                                                            |   |\n");
        printf("%*s", ancho_media_pantalla, " ");
       for(int i = 0; i < LINEAS_DISPLAY; i++){
           printf( "          |   |  %-*s |   |\n",MAX_CARACTERES_LINEA_DISPLAY + PADDING_DISPLAY, texto[i]);
           printf("%*s", ancho_media_pantalla, " ");
       }
       if(mostrar_continuar) {
           printf("          |   |                       " AMARILLO "Presione Enter ↵  Para Continuar..." BLANCO "  |   |\n");
           printf("%*s", ancho_media_pantalla, " ");
       }
       printf( "          |   |____________________________________________________________|   |\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "          |                                                                    |\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "          \\____________________________________________________________________/\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "                           \\___________________________________/\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "                         _________________________________________\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "                    _-'-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. -- - ` - _\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "                  -_'_-'.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.--. .-.-.`-_\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "                _-'.-.-.-. .---.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-`__`. .-.-.-.`-\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "               -'.-.-.-.-. .-----.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-----. .-.-.-.-.`-_\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "            _-'.-.-.-.-.-. .---.-. .-------------------------. .-.---. .---.-.-.-.`-_\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "            :-------------------------------------------------------------------------:\n");
        printf("%*s", ancho_media_pantalla, " ");
       printf( "           `---._.-------------------------------------------------------------._.---'\n");

    for(int i = 0; i < alto_media_pantalla; i++) printf("\n");
}

void obtener_input(int* c){
    system("/bin/stty raw");
    *c = getchar();
    system("/bin/stty cooked");
}

void imprimir_consola(char* mensaje){

    if(!mensaje || !*mensaje || strlen(mensaje) > (LINEAS_DISPLAY * MAX_CARACTERES_LINEA_DISPLAY)){
        return;
    }

    char temp[strlen(mensaje) + 1];
    for(int i = 0; i <= strlen(mensaje); i++)
        temp[i] = '\0';

    for(int i = 0; i < strlen(mensaje); i++){
        temp[i] = mensaje[i];
        mostrar_mensaje(temp, i + 1 == strlen(mensaje));
        if(strchr(".,:;!?¡¿\t", temp[i]))
            delay(VELOCIDAD_PAUSAS);
        else
            delay(VELOCIDAD_LETRAS);
            
    }
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}


void mostrar_opciones(char* menu, char* opciones[MAX_OPCIONES], int cantidad_opciones, int opcion_seleccionada){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    system("clear");
    int alto_media_pantalla = (int)((w.ws_row - ALTO_OPCION * cantidad_opciones) / 2);
    for(int i = 0; i < alto_media_pantalla; i++) printf("\n");
    int ancho_media_pantalla = (int)((w.ws_col - strlen(menu)) / 2);
    printf("%*s", ancho_media_pantalla, " ");
    printf(TITULO);
    printf("%s\n\n\n", menu);
    printf(CERRAR_TITULO);

    for(int i = 0; i < cantidad_opciones; i++){
        mostrar_opcion(opciones[i], i == opcion_seleccionada);
    }

    for(int i = 0; i < alto_media_pantalla; i++) printf("\n");
  }


int elegir_opcion(char* menu, char* opciones[MAX_OPCIONES], int cantidad_opciones, int opcion_seleccionada){
  int c = 0;
  mostrar_opciones(menu, opciones, cantidad_opciones, opcion_seleccionada);

  while(c != ENTER && c != CANCELAR){
    obtener_input(&c);
    switch(c){
      case ARRIBA: {
        if(opcion_seleccionada > 0)
          opcion_seleccionada--;
				break;
			}
			case ABAJO: {
        if(opcion_seleccionada < cantidad_opciones - 1)
				  opcion_seleccionada++;
				break;
      }
      case CANCELAR: {
        exit(-1);
			}
    }
    mostrar_opciones(menu, opciones, cantidad_opciones, opcion_seleccionada);
  }

  return opcion_seleccionada;
}






