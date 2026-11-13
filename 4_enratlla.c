#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "4_enratlla.h"
#include "Arbre.h"

/*//DINS de "4 enratlla.h"
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

/**Sobre el tauler**/
//Omple el tauler amb BUIT
void inicialitzarTauler(char tauler[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tauler[i][j] = BUIT;
        }
    }
}

// Mostra el tauler per pantalla amb colors i senyala la linia guanyadora
void dibuixarTauler(char tauler[N][N],Posicio linia[4]) {
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

//Mostra al fitxer el tauler simple sense colors
void escriureTaulerFitxer(FILE *fitxer, char tauler[N][N]) {
    fprintf(fitxer, "\n  1   2   3   4   5   6   7   8\n");
    for (int i = 0; i < N; i++) {
        fprintf(fitxer, "|");
        for (int j = 0; j < N; j++) {
            fprintf(fitxer," %c |", tauler[i][j]);
        }
        fprintf(fitxer, "\n");
    }
    for (int j = 0; j < N; j++) fprintf(fitxer,"----");
    fprintf(fitxer,"-\n");
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


/**Revisar mirant tot el tauler si hem guanyat**/
//Comprova si hi ha linia horitzontal guanyadora en tot el tauler
int Linia_Horitzontal(char tauler[N][N],char fitxa){
    for (int i=N-1;i>=0;i--){//de abaix cap a dalt
        for(int j=0;j<=N-4;j++){// de esquerra a dreta
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
        for(int j=0;j<=N-4 ;j++){
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
    return Linia_Horitzontal(tauler,fitxa)||
           Linia_Vertical(tauler,fitxa)||
           Linia_Diagonal_dret(tauler,fitxa)||
           Linia_Diagonal_esq(tauler,fitxa);
}

/**Revisar al voltant de l'última fitxa si hem guanyat**/
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
    // Cap amunt-dreta
    for (int i = p.fila-1, j = p.col-1; i >= 0 && j >= 0 && tauler[i][j] == fitxa; i--, j--) {aux++;iniciFil=i;iniciCol=j;}
    //iniciFil es la fila més amunt i iniciCol la columna més a l'esquerra
//    x(iniciFil,iniciCol)
//     x
//      x
//       x
    // Cap avall-avall
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

//Agrupa les 4 en una sola versió
int Fitxa_Linia(char tauler[N][N], char fitxa, Posicio p, int df, int dc, Posicio linia[4]) {
    int count = 1;
    int iniciFil = p.fila, iniciCol = p.col;

    // Cap endavant (df, dc)
    for (int k = 1; k < 4; k++) {
        int f = p.fila + k * df;
        int c = p.col + k * dc;
        if (f < 0 || f >= N || c < 0 || c >= N) break;
        if (tauler[f][c] == fitxa) count++;
        else break;
    }

    // Cap enrere (-df, -dc)
    for (int k = 1; k < 4; k++) {
        int f = p.fila - k * df;
        int c = p.col - k * dc;
        if (f < 0 || f >= N || c < 0 || c >= N) break;
        if (tauler[f][c] == fitxa) {
            count++;
            iniciFil = f;
            iniciCol = c;
        } else {
            break;
        }
    }

    if (count >= 4) {
        // Omplir linia amb les 4 posicions
        for (int k = 0; k < 4; k++) {
            linia[k].fila = iniciFil + k * df;
            linia[k].col = iniciCol + k * dc;
        }
        return 1;
    }

    return 0;
}


//Si en alguna direcció hi ha 4 o més fitxes seguides (comptant la fitxa nova), hi ha guanyador.
int ComprovaGuanyadorDesDe(char tauler[N][N], char fitxa,Posicio p,Posicio L[4]) {
//    return Fitxa_Linia_Horitzontal(tauler,fitxa,p,L)||
//           Fitxa_Linia_vertical(tauler,fitxa,p,L)||
//           Fitxa_Linia_Diagonal_dret(tauler,fitxa,p,L)||
//           Fitxa_Linia_Diagonal_esq(tauler,fitxa,p,L);
    return Fitxa_Linia(tauler, fitxa, p, 0, 1, L) ||    // horitzontal
           Fitxa_Linia(tauler, fitxa, p, 1, 0, L) ||    // vertical
           Fitxa_Linia(tauler, fitxa, p, -1, 1, L) ||   // diagonal /
           Fitxa_Linia(tauler, fitxa, p, 1, 1, L);      // diagonal \

}


/**Demanar al usuari**/
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


/**Neteja la pantalla**/
//Funcio que neteja la pantalla de la consola
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

/**Qui comença**/
char decideixTornInicial(char guanyadorAnterior) {
    char tornInicial;
    if (guanyadorAnterior == JUGADOR || guanyadorAnterior == MAQUINA) {
        tornInicial = guanyadorAnterior;   // comença el guanyador anterior
    } else {
        // Empat: triar aleatori
        tornInicial = (rand() % 2 == 0) ? JUGADOR : MAQUINA;
    }

    return tornInicial;
}

/**4 en ratlla**/
//Funció que reprodueix una partida del 4 en ratlla
char partida(char torn,FILE *fitxer){
    char tauler[N][N];
    int guanyat = 0;//FALSE
    Posicio linia[4];

    inicialitzarTauler(tauler);

    if(fitxer!=NULL){
        fprintf(fitxer,"\n===PARTIDA===");
        escriureTaulerFitxer(fitxer, tauler);

    }

    while (!guanyat && !taulerPle(tauler)) {
        netejarPantalla();
        dibuixarTauler(tauler,NULL);

        Posicio p;

        if (torn==JUGADOR){//torn humà
            printf("Jugador('%c') ", torn);
            p.col=demanaColumna(tauler);
            p.fila=tirada(tauler,p.col,JUGADOR);
            guanyat=ComprovaGuanyadorDesDe(tauler,JUGADOR,p,linia);

            if(fitxer!=NULL){
                fprintf(fitxer, "Torn JUGADOR ('%c'):columna %d (fila %d)\n",JUGADOR, p.col + 1, p.fila);
                escriureTaulerFitxer(fitxer, tauler);
            }
        }else{//torn maquina (ara demoment es el 2nd jugador
            //printf("Maquina ('%c') ", torn);
            //p.col=demanaColumna(tauler);
            //p.col=tiradaMaquina_arbre(tauler);
            p.col=ferminimax_arbre(tauler);
            p.fila=tirada(tauler,p.col,MAQUINA);
            guanyat=ComprovaGuanyadorDesDe(tauler,MAQUINA,p,linia);
            if(fitxer!=NULL){
                fprintf(fitxer, "Torn MAQUINA ('%c'): columna %d (fila %d)\n",MAQUINA, p.col + 1, p.fila);
                escriureTaulerFitxer(fitxer, tauler);
            }
        }
        if (!guanyat) {//canvi de jugador
            torn = (torn == JUGADOR) ? MAQUINA : JUGADOR;
        }
    }

    netejarPantalla();

    if (fitxer != NULL) {
        if (guanyat) {
            fprintf(fitxer,"\n%s ('%c') ha guanyat!\n", ((torn == JUGADOR) ? "El JUGADOR" : "La MAQUINA"), torn);
        } else {
            fprintf(fitxer, "\nEMPAT\n\n");
        }
    }

    if (guanyat) {
        dibuixarTauler(tauler,linia);
        printf("\n%s ('%c') ha guanyat!\n", ((torn == JUGADOR) ? "El JUGADOR" : "La MAQUINA"), torn);
        return torn;
    } else {
        dibuixarTauler(tauler,NULL);
        printf("\nEmpat! El tauler esta ple.\n");
        return 0;
    }
}


/**Mostrar resultats**/
void mostrarMarcador(FILE *sortida,int numPartides, int victoriesJugador, int victoriesMaquina, int empats,double recordJugador_MILLOR,double recordMaquina_MILLOR,double recordJugador_PITJOR,double recordMaquina_PITJOR) {
    fprintf(sortida,"\n+---------------------------------------+\n");
    fprintf(sortida," Partides jugades:     %d                \n", numPartides);
    fprintf(sortida," Victories JUGADOR:    %d                \n", victoriesJugador);
    fprintf(sortida," Victories MAQUINA:    %d                \n", victoriesMaquina);
    fprintf(sortida," Empats:               %d                \n", empats);
    fprintf(sortida,"+----------------JUGADOR----------------+\n");
    fprintf(sortida," Millor temps:         %.0fs             \n", (recordJugador_MILLOR<0)?0.0:recordJugador_MILLOR);
    fprintf(sortida," Pitjor temps:         %.0fs             \n", (recordJugador_PITJOR<0)?0.0:recordJugador_PITJOR);
    fprintf(sortida,"+----------------MAQUINA----------------+\n");
    fprintf(sortida," Millor temps:         %.0fs             \n", (recordMaquina_MILLOR<0)?0.0:recordMaquina_MILLOR);
    fprintf(sortida," Pitjor temps:         %.0fs             \n", (recordMaquina_PITJOR<0)?0.0:recordMaquina_PITJOR);
    fprintf(sortida,"+---------------------------------------+\n\n");

}
