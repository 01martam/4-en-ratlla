#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "4_enratlla.h"
#include "Arbre.h"

void obtenirDataHora(char *buffer, int mida) {
    time_t ara = time(NULL);
    struct tm *info = localtime(&ara);
    strftime(buffer, mida, "%d-%m-%Y_%H-%M.txt", info);
}

/*
    char dataHora[64];
    obtenirDataHora(dataHora, sizeof(dataHora));
*/
int main(){
    //INICIALITZACIÓ
    srand(time(NULL));
    int victoriesJugador=0, victoriesMaquina=0, empats=0;
    int numPartides=0;
    double tempsJugador_millor=-1,tempsJugador_pitjor=-1;//sense record

    //FITXER de partides
    FILE *fitxerPartides=NULL;

    // Obrir el fitxer de partides (si hem dit si)
    if (demanaSiNo("Vols guardar les partides en un fitxer?")){
        fitxerPartides = fopen("Partides.txt", "a");
        if (fitxerPartides == NULL) fprintf(stderr, "Error: no s'ha pogut obrir _.txt\n");
    }

    //PARTIDES
    char guanyador=0;//primera partida o empat

    while(1){
        netejarPantalla();
        //Decidir la dificultat de la partida actual
        int dificultat= demanaDificultat();

        //Decidir qui comença la partida
        char torn=decideixTornInicial(guanyador);

        //Partida
        time_t inici=time(NULL);
        guanyador=partida(torn,fitxerPartides,dificultat);
        time_t fi=time(NULL);

        //Actualitzar MARCADOR
        double durada=(double)(fi-inici);
        numPartides++;

        if(guanyador==JUGADOR) {
            victoriesJugador++;
            if(tempsJugador_millor<0||durada<tempsJugador_millor){ //el temps més curt en guanyar el jugador
                tempsJugador_millor=durada;
                printf("Nou record del JUGADOR:%.0fs.\n",durada);
            }

            if(/*tempsJugador_pitjor<0||*/durada>tempsJugador_pitjor){ //el temps més llarg
                tempsJugador_pitjor=durada;
            }
        }

        else if(guanyador==MAQUINA) victoriesMaquina++;

        else empats++;

        printf("\nDurada de la partida: %.0f segons\n", durada);

        // Mostrar marcador
        mostrarMarcador(stdout,dificultat,numPartides,victoriesJugador,victoriesMaquina,empats,tempsJugador_millor,tempsJugador_pitjor);

        if (fitxerPartides != NULL) {
            printf("Partida guardada en Partides.txt\n");
            mostrarMarcador(fitxerPartides,dificultat,numPartides, victoriesJugador, victoriesMaquina, empats,tempsJugador_millor,tempsJugador_pitjor);
        }
        // Preguntar si vol jugar una altra
        if (!demanaSiNo("Vols jugar una altra partida?")) {break;}


    }
    if(fitxerPartides!=NULL) fclose(fitxerPartides);
    netejarPantalla();
    printf("\nGracies per jugar!\n");
    return 0;


}


