#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "4 enratlla.h"
#include "Arbre.h"


int main(){
    int victoriesJugador=0;
    int victoriesMaquina=0;
    int empats=0;
    int numPartides=0;
    double tempsJugador_millor=-1;//sense record
    double tempsJugador_pitjor=-1;//sense record
    double tempsMaquina_millor=-1;
    double tempsMaquina_pitjor=-1;
    while(1){
        time_t inici=time(NULL);
        char guanyador=partida(JUGADOR);
        time_t fi=time(NULL);

        double durada=(double)(fi-inici);
        numPartides++;

        if(guanyador==JUGADOR) {
            victoriesJugador++;
            if(tempsJugador_millor<0||durada<tempsJugador_millor){ //el temps més curt
                tempsJugador_millor=durada;
                printf("Nou record del JUGADOR:%.0fs.\n",durada);
            }

            if(tempsJugador_pitjor<0||durada>=tempsJugador_pitjor){ //el temps més llarg
                tempsJugador_pitjor=durada;
            }
        }

        else if(guanyador==MAQUINA) {
            victoriesMaquina++;
            if(tempsMaquina_millor<0||durada<tempsMaquina_millor){
                tempsMaquina_millor=durada;
                printf("Nou record de la MAQUINA:%.0fs.\n",durada);
            }
            if(tempsMaquina_pitjor<0||durada>=tempsMaquina_pitjor){
                tempsMaquina_pitjor=durada;
            }
        }
        else empats++;

        printf("\nDurada de la partida: %.0f segons\n", durada);

        // Mostrar marcador
        mostrarMarcador(numPartides,victoriesJugador,victoriesMaquina,empats,tempsJugador_millor,tempsMaquina_millor,tempsJugador_pitjor,tempsMaquina_pitjor);

        // Preguntar si vol jugar una altra
        if (!demanaSiNo()) {break;}


    }
//    netejarPantalla();
    printf("\nGracies per jugar!\n");
    return 0;


}


