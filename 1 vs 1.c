#include <stdlib.h>
#include <stdio.h>


#define N 8
#define BUIT ' '
#define MAQUINA 'X'
#define JUGADOR 'O'

#define VERMELL "\033[31m"
#define BLAU    "\033[34m"
#define RESET   "\033[0m"


//Omple el tauler amb BUIT
void inicialitzarTauler2(char tauler[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tauler[i][j] = BUIT;
        }
    }
}

// Mostra el tauler per pantalla
void dibuixarTauler3(char tauler[N][N]) {
    printf("\n  1   2   3   4   5   6   7   8\n");
    for (int i = 0; i < N; i++) {
        printf("|");
        for (int j = 0; j < N; j++) {
            if(tauler[i][j]==MAQUINA){printf(" " VERMELL "%c" RESET " |", tauler[i][j]);}
            else if(tauler[i][j]==JUGADOR){printf(" " BLAU "%c" RESET " |", tauler[i][j]);}
            else {printf(" %c |", tauler[i][j]);}
        }
        for (int j = 0; j < N; j++) printf("    ");
        printf("\n");

    }
        for (int j = 0; j < N; j++) printf("----");
        printf("-\n");
}

void tirada3(char tauler[N][N],int col,char fitxa){
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
int Fitxa_Linia_Horitzontal(char tauler[N][N], char fitxa, int fila, int col){
    int aux = 1;
    //Cap a l'esquerra
    for (int j = col-1; j >= 0 && tauler[fila][j] == fitxa; j--) aux++;
    //Cap a la dreta
    for (int j = col+1; j < N  && tauler[fila][j] == fitxa; j++) aux++;
    return (aux >= 4);
    /*Equivalent
    if (aux >= 4) return 1;
    else return 0;*/
}

//Comprova si hi ha linia vertical guanyadora al voltant de l'ultima fitxa tirada
int Fitxa_Linia_vertical(char tauler[N][N], char fitxa, int fila, int col){
    int aux = 1;
    //cap amunt
    for (int i = fila-1; i >= 0 && tauler[i][col] == fitxa; i--) aux++;
    // Cap avall
    for (int i = fila+1; i < N  && tauler[i][col] == fitxa; i++) aux++;
    return (aux >= 4);
    /*Equivalent
    if (aux >= 4) return 1;
    else return 0;*/
}

//Comprova si hi ha linia diagonal (/) guanyadora al voltant de l'ultima fitxa tirada
int Fitxa_Linia_Diagonal_dret(char tauler[N][N], char fitxa, int fila, int col){
    // Diagonal (/)
    int aux = 1;
    // Cap amunt-dreta
    for (int i = fila-1, j = col+1; i >= 0 && j < N && tauler[i][j] == fitxa; i--, j++) aux++;
    // Cap avall-esquerra
    for (int i = fila+1, j = col-1; i < N && j >= 0 && tauler[i][j] == fitxa; i++, j--) aux++;
    return (aux >= 4);
    /*Equivalent
    if (aux >= 4) return 1;
    else return 0;*/
}

//Comprova si hi ha linia diagonal (\) guanyadora al voltant de l'ultima fitxa tirada
int Fitxa_Linia_Diagonal_esq(char tauler[N][N], char fitxa, int fila, int col){
    // Diagonal (\)
    int aux = 1;
    // Cap amunt-esquerra
    for (int i = fila-1, j = col-1; i >= 0 && j >= 0 && tauler[i][j] == fitxa; i--, j--) aux++;
    // Cap avall-dreta
    for (int i = fila+1, j = col+1; i < N && j < N && tauler[i][j] == fitxa; i++, j++) aux++;
    return (aux >= 4);
    /*Equivalent
    if (aux >= 4) return 1;
    else return 0;*/
}

//Si en alguna direcció hi ha 4 o més fitxes seguides (comptant la fitxa nova), hi ha guanyador.
int ComprovaGuanyadorDesDe(char tauler[N][N], char fitxa, int fila, int col) {
    return Fitxa_Linia_Horitzontal(tauler,fitxa,fila,col)||Fitxa_Linia_vertical(tauler,fitxa,fila,col)||Fitxa_Linia_Diagonal_dret(tauler,fitxa,fila,col)||Fitxa_Linia_Diagonal_esq(tauler,fitxa,fila,col);
}

int Tirada_ultima_poscicio(char tauler[N][N],int col,char fitxa){
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


// Comprova si la columna és vàlida (1 = sí, 0 = no)
int columnaValida3(char tauler[N][N], int col) {
    if (col < 0 || col >= N) return 0;
    return tauler[0][col] == BUIT; // Retorna 1 si la casella superior està buida
    /*Equivalent
    if (tauler[0][col] == BUIT) return 1;
    else return 0;*/
}

// Compta quantes columnes NO estan plenes, es a dir el numero de posibles jugades, les quals cadascuna sera un fill
int calcularNumeroDeFills3(char tauler[N][N]){
//mirar nomes la fila d'adalt de tot
    int aux=0;
    for (int j = 0; j < N; j++) {
        if(columnaValida(tauler,j)) aux++; //si la columna es valida no està plena
    }
   return aux;
}

// Comprova si el tauler està ple (1 = ple, 0 = encara hi ha lloc)
/*int taulerPle(char tauler[N][N]) {
    for (int j = 0; j < N; j++) {
        if (tauler[0][j] == BUIT) return 0;
    }
    return 1;
}*/

int taulerPle3(char tauler[N][N]) {
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





int partida(int jugadorActual){
    char tauler[N][N];
    char fitxaActual;
    int col;
    int guanyat = 0;//TRUE

    inicialitzarTauler(tauler);

    printf("=== BENVINGUT AL 4 EN RATLLA (%dx%d) ===\n", N, N);

    while (!guanyat && !taulerPle(tauler)) {
        dibuixarTauler(tauler);
        fitxaActual = (jugadorActual == 1) ? JUGADOR : MAQUINA;

        printf("Jugador %d ('%c'), tria una columna (1-%d): ", jugadorActual, fitxaActual, N);
        if (scanf("%d", &col) != 1) {
            // Neteja del buffer d'entrada
            while (getchar() != '\n');
            printf("Entrada no vàlida. Introdueix un número.\n");
            continue;
        }

        col--; // Adaptem l'entrada humana (1-N) a índex C (0 a N-1)

        if (!columnaValida(tauler, col)) {
            printf("Columna no vàlida o plena. Torna-ho a provar.\n");
            continue;
        }

        tirada(tauler, col, fitxaActual);
        guanyat = comprovarGuanyador(tauler, fitxaActual);

        if (!guanyat) {
            // Canvi de jugador
            jugadorActual = (jugadorActual == 1) ? 2 : 1;
        }
    }

    dibuixarTauler(tauler);

    if (guanyat) {
        printf("\nEnhorabona! El Jugador %d ('%c') ha guanyat!\n", jugadorActual, fitxaActual);
    } else {
        printf("\nEmpat! El tauler està ple.\n");
    }

    return 0;

}
int main(){
    int jugadorActual = 1;//qui comença
    printf("=== BENVINGUT AL 4 EN RATLLA (%dx%d) ===\n", N, N);
    partida(jugadorActual);
    return 0;



}

