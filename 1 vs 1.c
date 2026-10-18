#include <stdlib.h>
#include <stdio.h>


#define N 8
#define BUIT ' '
#define MAQUINA 'O'
#define JUGADOR 'X'

//Omple el tauler amb BUIT
void inicialitzarTauler(char tauler[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tauler[i][j] = BUIT;
        }
    }
}


// Mostra el tauler per pantalla
void dibuixarTauler(char tauler[N][N]) {
    //printf("\n  1   2   3   4   5   6   7   8\n");
    printf("\n ");
    for (int j = 0; j < N; j++) {
        printf(" %d  ", j);
    }
    printf("\n");
    for (int i = 0; i < N; i++) {
        printf("|");
        for (int j = 0; j < N; j++) {
            printf(" %c |", tauler[i][j]);
        }
        //printf("\n ");
        //for (int j = 0; j < N; j++) printf("----");
        //printf("\n");
        printf("\n---------------------------------\n");
    }
}

// Col·loca la fitxa fent que caigui fins a la posició més baixa
int tirada(char tauler[N][N],int numfill,char fitxa){
    //1.Transformar el numfill a la columna matriu
    int col = numfill;
    //int col = numFillAColumna(tauler,numfill);
    if (col < 0 || col >= N) return 0;
    //2.Calcular la fila on cau per gravetat
    for(int i=N-1;i>=0;i--){
        //3.Posar la fitxa a la fila/columna
        if(tauler[i][col]==BUIT){
            tauler[i][col]=fitxa;
            return 1;//tot va bé
        }
    }
    return 0;

    //1.Transformar el numFill a columna de la matriu.
	//2.Calcula la fila on cau per gravetat
	//3.Posar la fitxa a la fila/columna calculada
}



// Comprova si la columna és vàlida (1 = sí, 0 = no)
int columnaValida(char tauler[N][N], int col) {
    if (col < 0 || col >= N) return 0;
    return tauler[0][col] == BUIT; // Retorna 1 si la casella superior està buida
}



// Comprova si hi ha 4 fitxetes iguals (1 = hi ha victòria, 0 = no)
int comprovarVictoria(char tauler[N][N], char fitxa) {
    // 1. Horitzontal
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= N - 4; j++) {
            if (tauler[i][j] == fitxa && tauler[i][j+1] == fitxa &&
                tauler[i][j+2] == fitxa && tauler[i][j+3] == fitxa)
                return 1;
        }
    }

    // 2. Vertical
    for (int i = 0; i <= N - 4; i++) {
        for (int j = 0; j < N; j++) {
            if (tauler[i][j] == fitxa && tauler[i+1][j] == fitxa &&
                tauler[i+2][j] == fitxa && tauler[i+3][j] == fitxa)
                return 1;
        }
    }

    // 3. Diagonal cap a la dreta (/)
    for (int i = 3; i < N; i++) {
        for (int j = 0; j <= N - 4; j++) {
            if (tauler[i][j] == fitxa && tauler[i-1][j+1] == fitxa &&
                tauler[i-2][j+2] == fitxa && tauler[i-3][j+3] == fitxa)
                return 1;
        }
    }

    // 4. Diagonal cap a l'esquerra (\)
    for (int i = 0; i <= N - 4; i++) {
        for (int j = 0; j <= N - 4; j++) {
            if (tauler[i][j] == fitxa && tauler[i+1][j+1] == fitxa &&
                tauler[i+2][j+2] == fitxa && tauler[i+3][j+3] == fitxa)
                return 1;
        }
    }

    return 0;
}

// Comprova si el tauler està ple (1 = ple, 0 = encara hi ha lloc)
int taulerPle(char tauler[N][N]) {
    for (int j = 0; j < N; j++) {
        if (tauler[0][j] == BUIT) return 0;
    }
    return 1;
}


int main(){
    char tauler[N][N];
    int jugadorActual = 1;
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
        guanyat = comprovarVictoria(tauler, fitxaActual);

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

