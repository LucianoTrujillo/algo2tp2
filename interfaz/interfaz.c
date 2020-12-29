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

#define ANCHO_DISPLAY 80
#define ALTO_DISPLAY 25
#define LINEAS_DISPLAY 10
#define MAX_CARACTERES_LINEA_DISPLAY 55
#define PADDING_DISPLAY 2
#define VELOCIDAD_LETRAS 25
#define VELOCIDAD_PAUSAS 400

#define MAX_OPCIONES 10

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

    printf(BLANCO);
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

void mostrar_mensaje(char* mensaje, bool mostrar_continuar){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    int ancho_media_pantalla = (int)((w.ws_col - ANCHO_DISPLAY) / 2);
    int alto_media_pantalla = (int)((w.ws_row - ALTO_DISPLAY) / 2);

    if(!mensaje || !*mensaje || strlen(mensaje) > (LINEAS_DISPLAY * MAX_CARACTERES_LINEA_DISPLAY)){
        return;
    }

    char texto[LINEAS_DISPLAY][MAX_CARACTERES_LINEA_DISPLAY + 1];

    int cant_lineas = 1 + (strlen(mensaje) / MAX_CARACTERES_LINEA_DISPLAY);

    for(int i = 0; i < LINEAS_DISPLAY; i++){
        strcpy(texto[i], "\0");
    }

    for(int i = 0; i <= cant_lineas; i++){
        int cant_letras_linea = strlen(mensaje) > MAX_CARACTERES_LINEA_DISPLAY ? MAX_CARACTERES_LINEA_DISPLAY : strlen(mensaje);
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

void obtener_input(char* c){
    system ("/bin/stty raw");
    *c = getchar();
    system ("/bin/stty cooked");
}

void mostrar_mensaje_fluido(char* mensaje){

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
    char c;
    while((c = getchar()) != 10);
}


void mostrar_opciones(char* opciones[MAX_OPCIONES], int cantidad_opciones, int opcion_seleccionada){
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

   // mostrar_mensaje_fluido("a");
    //mostrar_mensaje_fluido("hola");
    //mostrar_mensaje_fluido("1 2 3 4 5 6 7 8 9 10 11 12 13 141 51 617 18 19 202 12 22 3 242 52 62 72 82 93 0 31 32 33 34 3536 47 3 839 40 ");
    mostrar_mensaje_fluido("Hola reyy como te va? en que andas? todo piola? jaajaj me re alegro, sabes que el otro dia estaba pensando en vos y me re colgue");
    mostrar_mensaje_fluido("Hola reyy como te va? en que andas? todo piola? jaajaj me re alegro, sabes que el otro dia estaba pensando en vos y me re colgue pero por suerte ya estoy de nuevo. Vivito y coleante como de costumbre, no vaya a ser que me mura jajaja pero anda por suerte todo re bien, un gustazo de verdad. Vos en que andas? todo bien? jajajaja. No bueno chau nos vemos! abzo!");
   mostrar_mensaje_fluido("On the other hand, we denounce with righteous indignation and dislike men who are so beguiled and demoralized by the charms of pleasure of the moment, so blinded by desire, that they cannot foresee the pain and trouble that are bound to ensue; and equal blame belongs to those who fail in their duty through weakness of will, which is the same as saying through shrinking from toil and pain. These cases are perfectly simple and easy to distinguish. In a free hour, when our power of choice is untrammelled and when nothing prevents our being able to do what we like best, every pleasure is to be welcomed and every pain avoided. But in certain circumstances and owing to the claims of duty or the obligations of business it will frequently occur that pleasures have to be repudiated and annoyances accepted. The wise man therefore always holds in these matters to this principle of selection: he rejects pleasures to secure other greater pleasures, or else he endures pains to avoid worse pains.");
}






