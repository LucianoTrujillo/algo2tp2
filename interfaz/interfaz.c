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
#define ANCHO_OPCION 32
#define ALTO_OPCION 3
#define ANCHO_BORDE 1
#define ANCHO_BARRA_CARGA 100
#define ALTO_BARRA_CARGA 1
#define DELAY_CARGA 150
#define MILISEGUNDOS_EN_SEGUNDO 1000

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
    srand(time(0));
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
            printf("█");
        }
        for(;carga <= 100; carga++){
            printf("░");
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

    if(seleccionada){
        printf(AMARILLO);
        printf("%*s", ancho_media_pantalla, " ");
        printf("╔══════════════════════════════╗\n");
        printf("%*s", ancho_media_pantalla, " ");
        printf("║%*s%s%*s║\n",
               padding_opcion, "",
               opcion,
               padding_opcion, "");
        printf("%*s", ancho_media_pantalla, " ");
        printf("╚══════════════════════════════╝\n");
        printf(BLANCO);
    } else {
        printf("%*s", ancho_media_pantalla, " ");
        printf("┌──────────────────────────────┐\n");
        printf("%*s", ancho_media_pantalla, " ");
        printf("│%*s%s%*s│\n",
               padding_opcion, "",
               opcion,
               padding_opcion, "");
        printf("%*s", ancho_media_pantalla, " ");
        printf("└──────────────────────────────┘\n");
    }
}

void mostrar_opciones(char* opciones[10], int cantidad_opciones, int opcion_seleccionada){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    system("clear");
    int alto_media_pantalla = (int)((w.ws_row - ALTO_OPCION * cantidad_opciones) / 2);
    for(int i = 0; i < alto_media_pantalla; i++) printf("\n");
    printf(BLANCO);
    for(int i = 0; i < cantidad_opciones; i++){
        mostrar_opcion(opciones[i], i == opcion_seleccionada);
    }

    for(int i = 0; i < alto_media_pantalla; i++) printf("\n");
    mostrar_barra_carga(3000);
}






