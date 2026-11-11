#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "4 enratlla.h"

/*DINS de "4 enratlla.h"
#define N 8
#define BUIT ' '
#define MAQUINA 'X'
#define JUGADOR 'O'

#define VERMELL "\033[31m"
#define BLAU    "\033[34m"
#define RESET   "\033[0m"
#define FONS_GROC "\033[43m"
#define NEGRETA   "\033[1m"

typedef struct {
    int fila;
    int col;
} Posicio;
*/

//Omple el tauler amb BUIT
void inicialitzarTauler(char tauler[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tauler[i][j] = BUIT;
        }
    }
}

// Mostra el tauler per pantalla
void dibuixarTauler(char tauler[N][N],Posicio *linia) {
    printf("======= 4 EN RATLLA (%dx%d) =======\n", N, N);
    printf("\n  1   2   3   4   5   6   7   8\n");
    for (int i = 0; i < N; i++) {
        printf("|");
        for (int j = 0; j < N; j++) {
            int destacat = 0;
                if (linia != NULL) {
                    for (int k = 0; k < 4; k++) {
                        if (linia[k].fila == i && linia[k].col == j) {
                            destacat = 1;
                            break;
                        }
                    }
                }
            if (destacat) {printf(" " FONS_GROC NEGRETA "%c" RESET " |", tauler[i][j]);}
            else if(tauler[i][j]==MAQUINA){printf(" " VERMELL "%c" RESET " |", tauler[i][j]);}
            else if(tauler[i][j]==JUGADOR){printf(" " BLAU "%c" RESET " |", tauler[i][j]);}
            else {printf(" %c |", tauler[i][j]);}
        }
        printf("\n");

    }
        for (int j = 0; j < N; j++) printf("----");
        printf("-\n");

}

// Comprova si la columna és valida (1 = sí, 0 = no)
int columnaValida(char tauler[N][N], int col) {
    if (col < 0 || col >= N) return 0;
    return tauler[0][col] == BUIT; // Retorna 1 si la casella superior està buida
    /*Equivalent
    if (tauler[0][col] == BUIT) return 1;
    else return 0;*/
}

// Comprova si el tauler està ple (1 = ple, 0 = encara hi ha lloc)
/*int taulerPle(char tauler[N][N]) {
    for (int j = 0; j < N; j++) {
        if (tauler[0][j] == BUIT) return 0;
    }
    return 1;
}*/

int taulerPle(char tauler[N][N]) {
    for (int j = 0; j < N; j++) {
        if (columnaValida(tauler,j)) return 0;
    }
    return 1;
}

/*
int taulerPle(char tauler[N][N]) {
    return calcularNumeroDeFills(tauler) == 0;
}
*/

/* DESCARTAR
void tirada(char tauler[N][N],int col,char fitxa){
    if(!columnaValida(tauler,col)) return;// columna invalida
    //2.Calcular la fila on cau per gravetat
    for(int i=N-1;i>=0;i--){
        //3.Posar la fitxa a la fila/columna
        if(tauler[i][col]==BUIT){
            tauler[i][col]=fitxa;
            return;
        }
    }
    return;//columna plena
}
*/

int tirada(char tauler[N][N],int col,char fitxa){
    if(!columnaValida(tauler,col)) return -1; //columna invalida
    //2.Calcular la fila on cau per gravetat
    for(int i=N-1;i>=0;i--){
        //3.Posar la fitxa a la fila/columna
        if(tauler[i][col]==BUIT){
            tauler[i][col]=fitxa;
            return i;
        }
    }
    return -1; //columna plena
}

//Comprova si hi ha linia horitzontal guanyadora en tot el tauler
int Linia_Horitzontal(char tauler[N][N],char fitxa){
    for (int i=N-1;i>=0;i--){//de abaix cap a dalt
        for(int j=0;j<N-3;j++){// de esquerra a dreta
            if (tauler[i][j] == fitxa && tauler[i][j+1] == fitxa &&
                tauler[i][j+2] == fitxa && tauler[i][j+3] == fitxa)
                return 1;// hi ha linia horitzontal
        }
    }
    return 0;//No hi ha linia horitzontal
}

//Comprova si hi ha linia vertical guanyadora en tot el tauler
int Linia_Vertical(char tauler[N][N],char fitxa){
    for (int j = 0; j < N ; j++) {//esquerra a dreta
        for (int i=N-1; i>=3; i--) { //baix cap adalt
            if (tauler[i][j] == fitxa && tauler[i-1][j] == fitxa &&
                tauler[i-2][j] == fitxa && tauler[i-3][j] == fitxa)
                return 1;//hi ha linia vertical
        }
    }
    return 0;//No hi ha linia vertical
}

//Comprova si hi ha linia diagonal (/) guanyadora en tot el tauler
int Linia_Diagonal_dret(char tauler[N][N],char fitxa){
    for(int i=N-1;i>=3;i--){
        for(int j=0;j<N-3 ;j++){
            if (tauler[i][j] == fitxa && tauler[i-1][j+1] == fitxa &&
                tauler[i-2][j+2] == fitxa && tauler[i-3][j+3] == fitxa)
                return 1;//hi ha linia diagonal


        }
    }
    return 0;
}

//Comprova si hi ha linia diagonal (\) guanyadora en tot el tauler
int Linia_Diagonal_esq(char tauler[N][N],char fitxa){
    for(int i=N-1;i>=3;i--){
        for(int j=N-1;j>=3 ;j--){
            if (tauler[i][j] == fitxa && tauler[i-1][j-1] == fitxa &&
                tauler[i-2][j-2] == fitxa && tauler[i-3][j-3] == fitxa)
                return 1;//hi ha linia diagonal

        }
    }
    return 0;
}

//Mira si hi algu jugador que ha guanyat
int ComprovarGuanyador(char tauler[N][N],char fitxa){
    return Linia_Horitzontal(tauler,fitxa)||Linia_Vertical(tauler,fitxa)||Linia_Diagonal_dret(tauler,fitxa)||Linia_Diagonal_esq(tauler,fitxa);
}

////////
//Comprova si hi ha linia horitzontal guanyadora al voltant de l'ultima fitxa tirada
int Fitxa_Linia_Horitzontal(char tauler[N][N], char fitxa,Posicio p,Posicio L[4]){
    int aux = 1,iniciCol=p.col;
    //Cap a l'esquerra
    for (int j = p.col-1; j >= 0 && tauler[p.fila][j] == fitxa; j--) {aux++;iniciCol=j;}
    //iniciCol la columna més a l'esquerra X(fila,iniciCol) X X X
    //Cap a la dreta
    for (int j = p.col+1; j < N  && tauler[p.fila][j] == fitxa; j++) aux++;
    if(aux>=4){
        for(int k=0;k<4;k++){
            L[k].fila=p.fila;
            L[k].col=iniciCol+k;
        }
        return 1;
    }
    return 0;



    //return (aux >= 4);
    /*Equivalent
    if (aux >= 4) return 1;
    else return 0;*/
}

//Comprova si hi ha linia vertical guanyadora al voltant de l'ultima fitxa tirada
int Fitxa_Linia_vertical(char tauler[N][N], char fitxa,Posicio p,Posicio L[4]){
    int aux = 1,iniciFil=p.fila;
    //cap amunt
    for (int i = p.fila-1; i >= 0 && tauler[i][p.col] == fitxa; i--) {aux++;iniciFil=i;}
    //iniciFil es la fila més amunt
//    x(iniciFil,col)
//    x
//    x
//    x
    // Cap avall
    for (int i = p.fila+1; i < N  && tauler[i][p.col] == fitxa; i++) aux++;
    if(aux>=4){
        for(int k=0;k<4;k++){
            L[k].col=p.col;
            L[k].fila=iniciFil+k;
        }
        return 1;
    }
    return 0;
    //return (aux >= 4);
    /*Equivalent
    if (aux >= 4) return 1;
    else return 0;*/
}

//Comprova si hi ha linia diagonal (/) guanyadora al voltant de l'ultima fitxa tirada
int Fitxa_Linia_Diagonal_dret(char tauler[N][N], char fitxa,Posicio p, Posicio L[4]){
    // Diagonal (/)
    int aux = 1,iniciFil=p.fila,iniciCol=p.col;
    // Cap amunt-dreta
    for (int i = p.fila-1, j = p.col+1; i >= 0 && j < N && tauler[i][j] == fitxa; i--, j++){ aux++;iniciFil=i;iniciCol=j;}
    //iniciFil es la fila més amunt i iniciCol la columna més a la dreta
//       x(iniciFil,iniciCol)
//      x
//     x
//    x
    // Cap avall-esquerra
    for (int i = p.fila+1, j = p.col-1; i < N && j >= 0 && tauler[i][j] == fitxa; i++, j--) aux++;
    if(aux>=4){
        for(int k=0;k<4;k++){
            L[k].col=iniciCol-k;
            L[k].fila=iniciFil+k;
        }
        return 1;
    }
    return 0;


    //return (aux >= 4);
    /*Equivalent
    if (aux >= 4) return 1;
    else return 0;*/
}

//Comprova si hi ha linia diagonal (\) guanyadora al voltant de l'ultima fitxa tirada
int Fitxa_Linia_Diagonal_esq(char tauler[N][N], char fitxa,Posicio p,Posicio L[4]){
    // Diagonal (\)
    int aux = 1,iniciFil=p.fila,iniciCol=p.col;
    // Cap amunt-esquerra
    for (int i = p.fila-1, j = p.col-1; i >= 0 && j >= 0 && tauler[i][j] == fitxa; i--, j--) {aux++;iniciFil=i;iniciCol=j;}
    //iniciFil es la fila més amunt i iniciCol la columna més a l'esquerra
//    x(iniciFil,iniciCol)
//     x
//      x
//       x
    // Cap avall-dreta
    for (int i = p.fila+1, j = p.col+1; i < N && j < N && tauler[i][j] == fitxa; i++, j++) aux++;
    if(aux>=4){
        for(int k=0;k<4;k++){
            L[k].col=iniciCol+k;
            L[k].fila=iniciFil+k;
        }
        return 1;
    }
    return 0;
    //return (aux >= 4);
    /*Equivalent
    if (aux >= 4) return 1;
    else return 0;*/
}

//Si en alguna direcció hi ha 4 o més fitxes seguides (comptant la fitxa nova), hi ha guanyador.
int ComprovaGuanyadorDesDe(char tauler[N][N], char fitxa,Posicio p,Posicio L[4]) {
    return Fitxa_Linia_Horitzontal(tauler,fitxa,p,L)||Fitxa_Linia_vertical(tauler,fitxa,p,L)||Fitxa_Linia_Diagonal_dret(tauler,fitxa,p,L)||Fitxa_Linia_Diagonal_esq(tauler,fitxa,p,L);
}


// Compta quantes columnes NO estan plenes, es a dir el numero de posibles jugades, les quals cadascuna sera un fill
/*int calcularNumeroDeFills(char tauler[N][N]){//# INCLUDE ARBRE
//mirar nomes la fila d'adalt de tot
    int aux=0;
    for (int j = 0; j < N; j++) {
        if(columnaValida(tauler,j)) aux++; //si la columna es valida no està plena
    }
   return aux;
}
*/

//Funció que demana al usuari que esculli una columna
int demanaColumna(char tauler[N][N]) {
    int col;
    while (1) {
        printf("Tria columna (1-%d): ", N);
        if (scanf("%d", &col) != 1) {
            while (getchar() != '\n');
            printf("Entrada no valida.(Han de ser numeros) ");
            continue;
        }
        col--;   // 1-N → 0-(N-1)

        if(0>col||col>=N) {
            printf("Columna no valida. ");
            continue;
        }

        if (tauler[0][col] != BUIT) {//Columna plena
            printf("Columna plena. ");
            continue;
        }


//        if (!columnaValida(tauler, col)) {
//            printf("Columna no valida o plena.\n");
//            continue;
//        }
        return col;
    }
}

//Funció que demana al usuari si vol jugar mes partides o no
int demanaSiNo() {
    char resposta;
    while (1) {
        printf("Vols jugar una altra partida? (s/n): ");
        if (scanf(" %c", &resposta) != 1) {   // ← espai abans de %c ignora espais/salts
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');             // neteja la resta de la línia

        if (resposta == 's' || resposta == 'S') return 1;
        if (resposta == 'n' || resposta == 'N') return 0;
        printf("Resposta no valida. Introdueix 's' o 'n'.\n");
    }
}

//Funcio que netejs la pantalla de la consola
void netejarPantalla2() {
    printf("\033[2J\033[H");
}

void netejarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}



//Funció que reprodueix una partida del 4 en ratlla
char partida(char torn){
    char tauler[N][N];
    int guanyat = 0;//FALSE
    Posicio linia[4];

    inicialitzarTauler(tauler);

    while (!guanyat && !taulerPle(tauler)) {
        netejarPantalla();
        dibuixarTauler(tauler,NULL);

        if (torn==JUGADOR){//torn humà
            printf("Jugador('%c') ", torn);
            Posicio p;
            p.col=demanaColumna(tauler);
            p.fila=tirada(tauler,p.col,JUGADOR);
            guanyat=ComprovaGuanyadorDesDe(tauler,JUGADOR,p,linia);
        }else{//torn maquina (ara demoment es el 2nd jugador
            printf("Maquina ('%c') ", torn);
            Posicio p;
            p.col=tiradaMaquina_arbre(tauler);
            //p.col=ferminimax_arbre(tauler);
            p.fila=tirada(tauler,p.col,MAQUINA);
            guanyat=ComprovaGuanyadorDesDe(tauler,MAQUINA,p,linia);
        }
        if (!guanyat) {//canvi de jugador
            torn = (torn == JUGADOR) ? MAQUINA : JUGADOR;
        }
    }

    netejarPantalla();

    if (guanyat) {
        dibuixarTauler(tauler,linia);
        printf("\nEnhorabona! El Jugador %s ('%c') ha guanyat!\n", ((torn == JUGADOR) ? "JUGADOR" : "MAQUINA"), torn);
        return torn;
    } else {
        dibuixarTauler(tauler,NULL);
        printf("\nEmpat! El tauler esta ple.\n");
        return 0;
    }

    return 0;
}

void mostrarMarcador(int numPartides, int victoriesJugador, int victoriesMaquina, int empats,double recordJugador,double recordMaquina) {
    printf("\n+---------------------------------------+\n");
    printf(" Partides jugades:     %d                \n", numPartides);
    printf(" Victories JUGADOR:    %d                \n", victoriesJugador);
    printf(" Victories MAQUINA:    %d                \n", victoriesMaquina);
    printf(" Empats:               %d                \n", empats);
    printf(" Record JUGADOR:       %.0fs             \n", (recordJugador<0)?0.0:recordJugador);
    printf(" Record MAQUINA:       %.0fs             \n", (recordMaquina<0)?0.0:recordMaquina);
    printf("+---------------------------------------+\n\n");

}




int main(){
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

