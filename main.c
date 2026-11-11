#include <stdio.h>
#include <stdlib.h>
#include "4 enratlla.h"
#include "Arbre.h"


int main2(){
    int victoriesJugador=0;
    int victoriesMaquina=0;
    int empats=0;
    int numPartides=0;
    double tempsJugador=-1;//sense record
    double tempsMaquina=-1;
    while(1){
        time_t inici=time(NULL);
        char guanyador=partida(JUGADOR);
        time_t fi=time(NULL);

        double durada=(double)(fi-inici);
        numPartides++;

        if(guanyador==JUGADOR) {
            victoriesJugador++;
            if(tempsJugador<0||durada<tempsJugador){
                tempsJugador=durada;
                printf("Nou record.\n");
            }
        }

        else if(guanyador==MAQUINA) {
            victoriesMaquina++;
            if(tempsMaquina<0||durada<tempsMaquina){
                tempsMaquina=durada;
                printf("Nou record.\n");
            }
        }
        else empats++;

        printf("\nDurada de la partida: %.0f segons\n", durada);

        // Mostrar marcador
        mostrarMarcador(numPartides,victoriesJugador,victoriesMaquina,empats,tempsJugador,tempsMaquina);

        // Preguntar si vol jugar una altra
        if (!demanaSiNo()) {break;}


    }
//    netejarPantalla();
    printf("\nGracies per jugar!\n");
    return 0;


}


