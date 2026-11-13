#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "4_enratlla.h"
#include "Arbre.h"


int main(){
    srand(123);
    int victoriesJugador=0;
    int victoriesMaquina=0;
    int empats=0;
    int numPartides=0;
    double tempsJugador_millor=-1;//sense record
    double tempsJugador_pitjor=-1;//sense record
    double tempsMaquina_millor=-1;
    double tempsMaquina_pitjor=-1;



    // Obrir el fitxer de partides (afegint)
    FILE *fitxerPartides = fopen("partides.txt", "a");
    if (fitxerPartides == NULL) {
        fprintf(stderr, "Error: no s'ha pogut obrir partides.txt\n");
        return 1;
    }


    char guanyador=0;//primera partida o empat

    while(1){
        //decidir qui comença la partida
        char torn=decideixTornInicial(guanyador);

        time_t inici=time(NULL);
        guanyador=partida(torn,fitxerPartides);
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
        mostrarMarcador(stdout,numPartides,victoriesJugador,victoriesMaquina,empats,tempsJugador_millor,tempsMaquina_millor,tempsJugador_pitjor,tempsMaquina_pitjor);

        if (fitxerPartides != NULL) {
            mostrarMarcador(fitxerPartides, numPartides, victoriesJugador, victoriesMaquina, empats,tempsJugador_millor, tempsMaquina_millor,tempsJugador_pitjor, tempsMaquina_pitjor);
        }
        // Preguntar si vol jugar una altra
        if (!demanaSiNo()) {break;}


    }
    fclose(fitxerPartides);
//    netejarPantalla();
    printf("\nGracies per jugar!\n");
    return 0;


}


