#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define JUGADORES 10

//CADA PARTIDA ES INDEPENDIETE DE LA ANTERIOR

struct apuesta {
    float dinerocolor;
    float dineronumero;
    float dinerozona;
    char color[7];
    int numero;
    int zona ;
};

struct jugador {
    char nombre[30];
    char apellidos[100];
    int edad;
    char dni[9]; //Maximo 8 caracteres
    float dinero;
    float puntuacion;
    float dineroganado;
    struct apuesta apuesta;
};

struct combinacion {
    char color[20];
    int numero;
    int zona;
};

struct ruleta {
    float dinerorecaudado;
    float dineroperdido;
    struct combinacion combinacion;
};

void apuestaNumero(struct jugador jugadores[], int i);
void apuestaColor(struct jugador jugadores[], int i);
void apuestaZona(struct jugador jugadores[], int i);
int ruletaNumero(struct ruleta ruleta);
int ruletaZona(struct ruleta ruleta);
void calculoJugadores(struct jugador jugadores[], struct ruleta ruleta, int numJugadores, int numeroRuleta, int zonaRuleta);
void Salir();


int main(void) {
    struct ruleta ruleta;
    struct jugador jugadores[JUGADORES];

    int numJugadores;
    char respuesta;
    char respuesta2[5];
    float ganancia = 0;
    float totalganado = 0;
    float totalperdido = 0;
    float apuestaMedia;
    float dineroTotal;
    float apuestaTotal = 0;
    int numeroRuleta;
    int zonaRuleta;
    

    srand(time(NULL));

    printf("Buen dia, bienvenido a la ruleta.\n");


        do {
            //Reiniciamos las estadisticas para la siguiente partida
            totalganado = 0;
            totalperdido = 0;
            apuestaTotal = 0;

            if(strcasecmp(respuesta2, "si") == 0) {
                printf("\n\n-----------NUEVA PARTIDA------------\n");
            }

            do {
                printf("\nDime cuantos jugadores hay en la ruleta (0 - 10): ");
                scanf("%d", &numJugadores);

                if(numJugadores < 0 || numJugadores > JUGADORES) {
                    printf("Introduce un numero de jugadores valido.\n");
                }
            } while (numJugadores < 0 || numJugadores > JUGADORES);

            
            

            //Pedir datos

            for (int i = 0; i < numJugadores; i++) {
                //Reinicio todos los datos porque sino se solapan las apuestas de la primera con la segunda partida
                jugadores[i].dineroganado = 0;
                jugadores[i].apuesta.dineronumero = 0;
                jugadores[i].apuesta.dinerocolor = 0;
                jugadores[i].apuesta.dinerozona = 0;
                jugadores[i].apuesta.numero = -1; //Le damos un valor invalido asi reinicia la apuesta
                jugadores[i].apuesta.color[0] = '\0'; //Así se limpia el string
                jugadores[i].apuesta.zona = -1; //Le damos un valor invalido asi reinicia la apuesta

                printf("\n\n-----------JUGADOR %d-----------\n\n", i + 1); // +1, lo cual para un usuario hace mas entendible "Usuario 1" que "Usuario 0"

                printf("Introduce tu nombre: ");
                fpurge(stdin);
                scanf("%[^\n]", jugadores[i].nombre); //%[^\n] Se utiliza para poder recoger espacios, es decir, nombres compuestos, se recoge todo hasta pulsar intro

                printf("Introduce tus apellidos: ");
                fpurge(stdin);
                scanf("%[^\n]", jugadores[i].apellidos);

                do {
                    printf("Introduce tu edad: ");
                    scanf("%d", &jugadores[i].edad);

                    if (jugadores[i].edad < 18) {
                        printf("No tienes edad para jugar a la ruleta.\n\n");
                    }
                } while (jugadores[i].edad < 18);

                printf("Introduce tu DNI: ");
                fpurge(stdin);
                scanf("%s", jugadores[i].dni);

                do {
                    printf("Introduzca dinero disponible: ");
                    scanf("%f", &jugadores[i].dinero);

                    if (jugadores[i].dinero <= 0) {
                        printf("No tiene el dinero suficiente disponible.\n\n");
                    }
                } while (jugadores[i].dinero <= 0);

                jugadores[i].dineroganado = 0;

                do {
                    //I - Parte del menu

                    printf("\n\n-----------MENU-----------\n\n");
                    printf("A - Apuesta de numero\n");
                    printf("B - Apuesta de color\n");
                    printf("C - Apuesta de zona\n");
                    printf("S - Confirmar apuesta total\n");

                    //II - Parte del menú

                    printf("\n¿Que opcion quieres ejecutar? ");
                    fpurge(stdin);
                    scanf("%c", &respuesta);

                    //Paso a mayuscula la opcion que me dice el usuario, así siempre trabajo con mayusculas para el switch
                    respuesta = toupper(respuesta);

                    //III - Parte del menú
                    switch (respuesta) {
                        case 'A':
                            apuestaNumero(jugadores,i);
                        break;

                        case 'B':
                            apuestaColor(jugadores,i);
                        break;

                        case 'C':
                            apuestaZona(jugadores,i);
                        break;
                        case 'S':
                            Salir();
                        break;

                        default:
                            printf("Opcion no valida.\n");
                    }
                } while (respuesta != 'S');
            }

            numeroRuleta = ruletaNumero(ruleta);

             if (numeroRuleta % 2 == 0) { //Aqui si el resto de dividirlo entre 2 es 0 (basicamente el numero es par), se asigna el color rojo
                strcpy(ruleta.combinacion.color, "Rojo");
            } else {
                strcpy(ruleta.combinacion.color, "Negro"); //Asignamos negro
            }

            zonaRuleta = ruletaZona(ruleta);

            printf("\n\n-----------Resultados de la Ruleta-----------\n\n");
            printf("Numero ganador: %d\n", numeroRuleta);
            printf("Color ganador: %s\n", ruleta.combinacion.color);
            printf("Zona ganadora: %d\n", zonaRuleta);

            calculoJugadores(jugadores,ruleta,numJugadores,numeroRuleta,zonaRuleta);
            
            printf("\n\n-----------Resultados de los jugadores-----------\n");

            for (int i = 0; i < numJugadores; i++) {
                dineroTotal = jugadores[i].apuesta.dineronumero + jugadores[i].apuesta.dinerocolor + jugadores[i].apuesta.dinerozona;
                ganancia = jugadores[i].dineroganado - dineroTotal;
                printf("\nJugador %d: %s %s\n", i + 1, jugadores[i].nombre, jugadores[i].apellidos);
                printf("Numero: %d (%.2f)\n", jugadores[i].apuesta.numero, jugadores[i].apuesta.dineronumero);
                printf("Color: %s (%.2f)\n", jugadores[i].apuesta.color, jugadores[i].apuesta.dinerocolor);
                printf("Zona: %d (%.2f)\n", jugadores[i].apuesta.zona, jugadores[i].apuesta.dinerozona);
                printf("Dinero ganado/perdido: %.2f\n", ganancia);

                if (ganancia > 0) {
                    totalganado += ganancia;
                } else if (ganancia < 0) {
                    totalperdido += ganancia;
                }

                apuestaTotal += dineroTotal;
            }
            apuestaMedia = apuestaTotal / (float) numJugadores;

            printf("\n\n-----------Resumen Colectivo-----------\n\n");
            printf("Total de dinero ganado por los jugadores: %.2f\n",totalganado);
            printf("Total de dinero perdido por los jugadores: %.2f\n", totalperdido);
            printf("Media colectiva de ganancias/perdidas de los jugadores: %.2f\n", (totalganado + totalperdido) / (float) numJugadores);
            printf("Media colectiva de dinero apostado: %.2f\n", apuestaMedia);

            printf("\n\nDesea jugar de nuevo? (Si - No): ");
            fpurge(stdin);
            scanf("%s", respuesta2);

        } while (strcasecmp(respuesta2,"Si") == 0);

        printf("\n\nGracias por jugar a la ruleta!");

    return 0;
}

void apuestaNumero(struct jugador jugadores[], int i) {


        printf("\n\n-----------NUMERO-----------\n\n");

        printf("\nElige un número (0 - 36): ");
        scanf("%d", &jugadores[i].apuesta.numero);
        if (jugadores[i].apuesta.numero < 0 || jugadores[i].apuesta.numero > 36) {
            printf("Numero no valido.\n");
            return;
        }

        printf("\nIntroduzca cuanto quiere apostar: ");
        scanf("%f", &jugadores[i].apuesta.dineronumero);
        if (jugadores[i].apuesta.dineronumero > jugadores[i].dinero) {
            printf("No puedes apostar más dinero del que tienes (%.2f).\n\n", jugadores[i].dinero);
        } else {
            printf("\nApuesta realizada!\n");
            jugadores[i].dinero -= jugadores[i].apuesta.dineronumero;
        }
}

void apuestaColor(struct jugador jugadores[], int i) {

        printf("\n\n-----------COLOR-----------\n\n");
        printf("\nElige un Color (Rojo/Negro): ");
        scanf("%s", jugadores[i].apuesta.color);
        if (strcasecmp(jugadores[i].apuesta.color, "Rojo") != 0 && strcasecmp(jugadores[i].apuesta.color, "Negro") != 0) {
            printf("Color no valido.\n");
            return;
        }

        printf("\nIntroduzca cuanto quiere apostar: ");
        scanf("%f", &jugadores[i].apuesta.dinerocolor);
        if (jugadores[i].apuesta.dinerocolor > jugadores[i].dinero) {
            printf("No puedes apostar más dinero del que tienes (%.2f).\n\n", jugadores[i].dinero);
        } else {
            printf("\nApuesta realizada!\n");
            jugadores[i].dinero -= jugadores[i].apuesta.dinerocolor;
        }
}

void apuestaZona(struct jugador jugadores[], int i) {

        //I - Parte del menu
        printf("\n\n-----------MENU ZONA-----------\n\n");
        printf("1 - Vecinos del 0\n");
        printf("2 - Huerfanos\n");
        printf("3 - Tercios del Cilindro\n");


        //II - Parte del menú
        printf("\nElige una zona: ");
        scanf("%d", &jugadores[i].apuesta.zona);
        if (jugadores[i].apuesta.zona != 1 && jugadores[i].apuesta.zona != 2 && jugadores[i].apuesta.zona != 3) {
            printf("Zona no valida.\n");
            return;
        }

        printf("\nIntroduzca cuanto quiere apostar: ");
        scanf("%f", &jugadores[i].apuesta.dinerozona);
        if (jugadores[i].apuesta.dinerozona > jugadores[i].dinero) {
            printf("No puedes apostar más dinero del que tienes (%.2f).\n\n", jugadores[i].dinero);
        } else {
            printf("\nApuesta realizada!\n");
            jugadores[i].dinero -= jugadores[i].apuesta.dinerozona;

        }

}

int ruletaNumero(struct ruleta ruleta) {
    ruleta.combinacion.numero = rand() % 37; //Se acota por abajo, del 0 al 36
    return ruleta.combinacion.numero;

}



int ruletaZona(struct ruleta ruleta) {
    ruleta.combinacion.zona =  rand() % 3 + 1; //Delimitamos la zona entre 1 y 3
    return ruleta.combinacion.zona;

}

void calculoJugadores(struct jugador jugadores[], struct ruleta ruleta, int numJugadores,int numeroRuleta, int zonaRuleta) {
    for(int i = 0; i < numJugadores; i++) {
        if (jugadores[i].apuesta.numero == numeroRuleta && jugadores[i].apuesta.numero == 0) {
            jugadores[i].dineroganado += jugadores[i].apuesta.dineronumero * 7.2; //Ya que el 0 tiene un multiplicador mayor siempre
        } else if (jugadores[i].apuesta.numero == numeroRuleta && jugadores[i].apuesta.numero != 0) {
            jugadores[i].dineroganado += jugadores[i].apuesta.dineronumero * 3.6;
        }

        if(strcasecmp(jugadores[i].apuesta.color, ruleta.combinacion.color) == 0 ) { //Acierta color
            jugadores[i].dineroganado += jugadores[i].apuesta.dinerocolor * 2;
        }

        if (jugadores[i].apuesta.zona == zonaRuleta) {
            jugadores[i].dineroganado += jugadores[i].apuesta.dinerozona * 3;
        }
    }
}

void Salir() {
    printf("Apuestas confirmadas.\n");
}