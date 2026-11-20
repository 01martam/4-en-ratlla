#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "4_enratlla.h"
#include "Arbre.h"







int main(){
    //INICIALITZACIÓ
    srand(time(NULL));
    const char *nomsDificultat[]=NOMS_DIFICULTAT;
    int victoriesJugador=0, victoriesMaquina=0;  //nombre de partides guanyades en general
    int numPartides=0;
    //empats= numPartides-victoriesJugador-victoriesMaquina
    int ratxaJugador=0,ratxaMaximaJugador=0;// nombre de partides guanyades seguides
    double tempsJugador_millor=-1,tempsJugador_pitjor=-1;//sense record

    char guanyador=0;//primera partida o empat , els altres valors son MAQUINA o JUGADOR
    int dificultat=5;// dificultat per defecte


    //FITXER de partides
    FILE *fitxerPartides=NULL;
    int guardar=0;

//    int guardar=demanaSiNo("Vols guardar un historial de les partides en un fitxer?");
//    // Obrir el fitxer de partides (si hem dit si)
//    if (guardar){
//        fitxerPartides = fopen("Partides.txt", "a");
//        if (fitxerPartides == NULL) fprintf(stderr, "Error: no s'ha pogut obrir _.txt\n");
//    }

    while(1){
        //netejarPantalla();
        int opcio=Menu(dificultat,guardar);
        netejarPantalla();

        if(opcio==1){
            while(1){
                netejarPantalla();
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
                    ratxaJugador++;
                    ratxaMaximaJugador = MAX(ratxaMaximaJugador, ratxaJugador);
                    if(tempsJugador_millor<0||durada<tempsJugador_millor){ //el temps més curt en guanyar el jugador
                        tempsJugador_millor=durada;
                        printf("Nou record del JUGADOR:%.0fs.\n",durada);
                    }
        
                    if(/*tempsJugador_pitjor<0||*/durada>tempsJugador_pitjor){ //el temps més llarg
                        tempsJugador_pitjor=durada;
                    }
                }
        
                else if(guanyador==MAQUINA) {
                        victoriesMaquina++;
                        ratxaJugador=0;
                }
                //empats
                else {ratxaJugador=0;}
                
                printf("\nDurada de la partida: %.0f segons\n", durada);
        
                // Mostrar marcador
                printf("Dificultat MAQUINA Actual: %d (%s)\n\n", dificultat,nomsDificultat[dificultat]);
                mostrarMarcador(stdout,numPartides,victoriesJugador,victoriesMaquina,tempsJugador_millor,tempsJugador_pitjor,ratxaJugador,ratxaMaximaJugador);
        
                if (fitxerPartides != NULL) {
                    fprintf(fitxerPartides,"Dificultat MAQUINA Actual: %d (%s)\n\n", dificultat,nomsDificultat[dificultat]);
                    printf("Partida guardada en Partides.txt\n");
                    mostrarMarcador(fitxerPartides,numPartides, victoriesJugador, victoriesMaquina,tempsJugador_millor,tempsJugador_pitjor,ratxaJugador,ratxaMaximaJugador);
        
                }
                // Preguntar si vol jugar una altra
                if (!demanaSiNo("Vols jugar una altra partida?")) {break;}
        
        
            }

            netejarPantalla();

        }
        else if(opcio==2){
            //netejarPantalla();
            mostrarMarcador(stdout, numPartides, victoriesJugador,
                            victoriesMaquina, tempsJugador_millor, tempsJugador_pitjor,
                            ratxaJugador, ratxaMaximaJugador);
             printf(" Dificultat MAQUINA Actual: %d (%s)\n\n", dificultat,nomsDificultat[dificultat]);

        }
        else if (opcio==3){
            //netejarPantalla();
            printf("\n=== INSTRUCCIONS ===\n");
            printf("El joc consisteix a aconseguir 4 fitxes seguides\n en linia horitzontal, vertical o diagonal.\n\n");
            printf("- Les teves fitxes son " BLAU "%c" RESET ".\n",JUGADOR);
            printf("- Les de la MAQUINA son " VERMELL "%c" RESET ".\n",MAQUINA);
            printf("- Per tirar, escriu un numero de columna (1-8).\n");
            printf("- La fitxa caura per gravetat.\n\n");
        }
        else if(opcio==4){
            //netejarPantalla();
            dificultat=demanaDificultat(dificultat);
            netejarPantalla();
        }
        else if (opcio==5){
            guardar=configurarFitxer(&fitxerPartides);
        }
        else if(opcio==6){
            if (fitxerPartides != NULL) fclose(fitxerPartides);
            netejarPantalla();
            printf("\nGracies per jugar!\n");
            return 0;
        }

    }
}




//Versio anterior
int main2(){
    //INICIALITZACIÓ
    srand(time(NULL));
    const char *nomsDificultat[]=NOMS_DIFICULTAT;
    int victoriesJugador=0, victoriesMaquina=0;  //nombre de partides guanyades en general
    int numPartides=0;
    //empats= numPartides-victoriesJugador-victoriesMaquina
    int ratxaJugador=0,ratxaMaximaJugador=0;// nombre de partides guanyades seguides
    double tempsJugador_millor=-1,tempsJugador_pitjor=-1;//sense record

    char guanyador=0;//primera partida o empat
    int dificultat=4;// dificultat per defecte

    //FITXER de partides
    FILE *fitxerPartides=NULL;

    // Obrir el fitxer de partides (si hem dit si)
    if (demanaSiNo("Vols guardar un historial de les partides en un fitxer?")){
        fitxerPartides = fopen("Partides.txt", "a");
        if (fitxerPartides == NULL) fprintf(stderr, "Error: no s'ha pogut obrir _.txt\n");
    }

    //PARTIDES ->Jugar()

    while(1){
        netejarPantalla();
        //Decidir la dificultat de la partida actual
        dificultat= demanaDificultat(dificultat);

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
            ratxaJugador++;
            ratxaMaximaJugador = MAX(ratxaMaximaJugador, ratxaJugador);
            if(tempsJugador_millor<0||durada<tempsJugador_millor){ //el temps més curt en guanyar el jugador
                tempsJugador_millor=durada;
                printf("Nou record del JUGADOR:%.0fs.\n",durada);
            }

            if(/*tempsJugador_pitjor<0||*/durada>tempsJugador_pitjor){ //el temps més llarg
                tempsJugador_pitjor=durada;
            }
        }

        else if(guanyador==MAQUINA) {
                victoriesMaquina++;
                ratxaJugador=0;
        }

        else {
            //empats
            ratxaJugador=0;
        }



        printf("\nDurada de la partida: %.0f segons\n", durada);

        // Mostrar marcador
        printf("Dificultat MAQUINA Actual: %d (%s)\n\n", dificultat,nomsDificultat[dificultat]);
        mostrarMarcador(stdout,numPartides,victoriesJugador,victoriesMaquina,tempsJugador_millor,tempsJugador_pitjor,ratxaJugador,ratxaMaximaJugador);

        if (fitxerPartides != NULL) {
            fprintf(fitxerPartides,"Dificultat MAQUINA Actual: %d (%s)\n\n", dificultat,nomsDificultat[dificultat]);
            printf("Partida guardada en Partides.txt\n");
            mostrarMarcador(fitxerPartides,numPartides, victoriesJugador, victoriesMaquina,tempsJugador_millor,tempsJugador_pitjor,ratxaJugador,ratxaMaximaJugador);

        }
        // Preguntar si vol jugar una altra
        if (!demanaSiNo("Vols jugar una altra partida?")) {break;}


    }
    //PARTIDES FI

    if(fitxerPartides!=NULL) fclose(fitxerPartides);
    netejarPantalla();
    printf("\nGracies per jugar!\n");
    return 0;


}


