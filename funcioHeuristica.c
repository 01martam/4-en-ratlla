#include <stdio.h>
#include <stdlib.h>

#include "funcioHeuristica.h"
#include "Arbre.h"
#include "4_enratlla.h"

/**PRINCIPAL**/
//Funcio heuristica que depenen del nivell de dificultat utilitza una o una altre funcions de les de sota
double funcioHeuristica(char tauler[N][N],int dificultat, Posicio Casella){
    if(dificultat<=0){//totalment aleaotori
        return ((double)rand() / RAND_MAX) * 2.0 * 1000 - 1000; //valors entre -1000 i 1000
    }

    //Ara primer mirem si hi ha jugades guanyadores o el tauler esta ple sense guanya

    if(Casella.fila>=0){
        if (ComprovaGuanyadorDesDe(tauler,MAQUINA,Casella,NULL)) return INFINIT;
        if (ComprovaGuanyadorDesDe(tauler,JUGADOR,Casella,NULL)) return -INFINIT;
    }
    else{//Per si no tenim una ultima casella on tirem( no hauria de ser però per si acas)
        if (ComprovarGuanyador(tauler, MAQUINA)) return INFINIT;
        if (ComprovarGuanyador(tauler, JUGADOR)) return -INFINIT;
    }
    if(taulerPle(tauler)) return 0; //empat

    //Aleaotiri però evitant victories
    if(dificultat==1) return ((double)rand() / RAND_MAX) * 2.0 * 1000 - 1000; //valors entre -1000 i 1000
    // contem quantes fitxes hi ha de cada tipus
    if(dificultat==2)return comptarCadaTipus(tauler);
    //comtem les fitxes pero donantli més pes a les fitxes centrals
    if(dificultat==3) return comptarCadaTipusCentre(tauler);
    //comptem el numero de linies de 2 i 3
    if(dificultat ==4) return comptarLinies(tauler);
    //Contem fitxes pero mirant de 4 en 4
    if(dificultat==5)return sumaDelPesos(tauler,0);
    //Contem fitxes pero mirant de 4 en 4, i a més mirem 2 fitxes més
    if(dificultat==6)return sumaDelPesos(tauler,1);
    if(dificultat >=7) return SumaJugadesGuanyadores(tauler);
   return 0;
}

/**COMPTAR FITXES**/
//conta el numero de fitxes de cada tipus i calcula la resta
double comptarCadaTipus(char tauler[N][N]){
    int contM = 0, contJ = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tauler[i][j] == MAQUINA) contM++;
            else if (tauler[i][j] == JUGADOR) contJ++;
        }
    }
    return (double)(contM - contJ);
}

//conta el numero de fitxes de cada tipus i calcula la resta
// donan més pes a les fitxes més properes al centre
double comptarCadaTipusCentre(char tauler[N][N]){
    int countM = 0, countJ = 0;
    double extraM=0,extraJ=0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double aux=ABS(i - N/2) + ABS(j - N/2);//distancia de (i,j) a (N/2,N/2) ->Norma 1 de la diferencia
            aux=aux/(double)N;//normalitzar
            aux=1.0-aux;
            // la fitxa al centre del tauler és la posició(N/2,N/2)
            // la norma 1 de (i,j)-(N/2,N/2) és  abs(i - N/2) + abs(j - N/2), pot pendre valors de 0 a N
            //normalitza la distancia es dividir això pel valor maxim que pot adquirir, N
            //pero volem que el centre tingui més pes llavors restem aixo a 1
            if (tauler[i][j] == MAQUINA) {
                    countM++;
                    extraM+=aux;
            }
            else if (tauler[i][j] == JUGADOR) {
                    countJ++;
                    extraJ+=aux;
            }
        }
    }
    return (double)(countM - countJ)+(extraM-extraJ);
}


/**COMPTAR LINIES**/
//Funcio que compta el nombre de linies de 2 que te un jugador(fitxa) en el tauler que es podrien completar
//Observem que tornem a estar contant les de de tres, però com que aquestes tenen 100 cops més punts,no val la pena evitar-ho
int liniesDe2(char tauler[N][N], char fitxa) {
    int i, j, linies = 0;

    //contem les horitzontals (de baix a dalt)
    //_ _ X X
    //X X _ _
    //_ X X _
    for(i = N - 1; i >= 0; i--){
        for(j = 0; j < N - 1; j++){
            if(tauler[i][j] == fitxa && tauler[i][j + 1] == fitxa){
                if ((j > 1 && tauler[i][j - 1] == BUIT && tauler[i][j - 2] == BUIT) ||
                    (j + 3 < N && tauler[i][j + 2] == BUIT && tauler[i][j + 3] == BUIT) ||
                    (j > 0 && tauler[i][j - 1] == BUIT && j + 2 < N && tauler[i][j + 2] == BUIT))
                    {linies++;}
            }
        }
    }

    //contem les verticals (de baix a dalt)
    //-
    //-
    //X
    //X
    for(j = 0; j < N; j++){
        for(i = N - 1; i > 0; i--){
            if (tauler[i][j] == fitxa && tauler[i - 1][j] == fitxa){
                if (i - 3 >= 0 && tauler[i - 2][j] == BUIT && tauler[i - 3][j] == BUIT)
                    {linies++;}
            }
        }
    }

    //contem les diagonals (\) (de baix a dalt)
    // X * * *    _ * * *    _ * * *
    // * X * *    * _ * *    * X * *
    // * * _ *    * * X *    * * X *
    // * * * _    * * * X    * * * _
    for (i = N - 1; i > 0; i--){
        for (j = 1; j < N; j++){
            if (tauler[i][j] == fitxa && tauler[i - 1][j - 1] == fitxa){
                if ((i + 2 < N && j + 2 < N && tauler[i + 1][j + 1] == BUIT && tauler[i + 2][j + 2] == BUIT) ||
                    (i - 3 >= 0 && j - 3 >= 0 && tauler[i - 2][j - 2] == BUIT && tauler[i - 3][j - 3] == BUIT) ||
                    (i + 1 < N && j + 1 < N && tauler[i + 1][j + 1] == BUIT && i - 2 >= 0 && j - 2 >= 0 && tauler[i - 2][j - 2] == BUIT))
                    {linies++;}
            }
        }
    }

    //contem les diagonals (/) (de baix a dalt)
    // * * * X    * * * _    * * * _
    // * * X *    * * _ *    * * X *
    // * _ * *    * X * *    * X * *
    // _ * * *    X * * *    _ * * *
    for (i = N - 1; i > 0; i--){
        for (j = 0; j < N - 1; j++){
            if (tauler[i][j] == fitxa && tauler[i - 1][j + 1] == fitxa){
                if ((i + 2 < N && j > 1 && tauler[i + 1][j - 1] == BUIT && tauler[i + 2][j - 2] == BUIT) ||
                    (i - 3 >= 0 && j + 3 < N && tauler[i - 2][j + 2] == BUIT && tauler[i - 3][j + 3] == BUIT) ||
                    (i + 1 < N && j > 0 && tauler[i + 1][j - 1] == BUIT && i - 2 >= 0 && j + 2 < N && tauler[i - 2][j + 2] == BUIT))
                    {linies++;}
            }
        }
    }
    return linies;
}


//Funcio que compta el nombre de linies de 3 un jugador (fitxa) en el  tauler que es podrien completar
int liniesDe3(char tauler[N][N], char fitxa) {
    int i, j, linies = 0;

    //contem les horitzontals (de baix a dalt)
    //_ X X X
    //X X X _
    //X X _ X
    //X _ X X
    for(i = N - 1; i >= 0; i--){
        for(j = 0; j < N - 2; j++){
            if(tauler[i][j] == fitxa && tauler[i][j + 1] == fitxa && tauler[i][j + 2] == fitxa) {
                if ((j > 0 && tauler[i][j - 1] == BUIT) || (j + 3 < N && tauler[i][j + 3] == BUIT))
                    {linies++;}
            }
            else if (j + 3 < N && tauler[i][j] == fitxa && tauler[i][j + 1] == fitxa && tauler[i][j + 2] == BUIT && tauler[i][j + 3] == fitxa)
                {linies++;}
            else if (j + 3 < N && tauler[i][j] == fitxa && tauler[i][j + 1] == BUIT && tauler[i][j + 2] == fitxa && tauler[i][j + 3] == fitxa)
                {linies++;}
        }
    }

    //contem les verticals (de baix cap a dalt)
    //_
    //X
    //X
    //X
    for(j = 0; j < N; j++){
        for(i = N - 1; i > 1; i--){
            if (tauler[i][j] == fitxa && tauler[i - 1][j] == fitxa && tauler[i - 2][j] == fitxa){
                if (i - 3 >= 0 && tauler[i - 3][j] == BUIT)
                    {linies++;}
            }
        }
    }

    //contem les diagonals (\) (de baix a dalt cap a l'esquerra)
    //X * * *  _ * * *  X * * *  X * * *
    //* X * *  * X * *  * _ * *  * X * *
    //* * X *  * * X *  * * X *  * * _ *
    //* * * _  * * * X  * * * X  * * * X
    for (i = N - 1; i > 1; i--){
        for (j = 2; j < N; j++){
            if (tauler[i][j] == fitxa && tauler[i - 1][j - 1] == fitxa && tauler[i - 2][j - 2] == fitxa) {
                if ((i + 1 < N && j + 1 < N && tauler[i + 1][j + 1] == BUIT) ||
                    (i - 3 >= 0 && j - 3 >= 0 && tauler[i - 3][j - 3] == BUIT))
                    {linies++;}
            }
            else if (i - 3 >= 0 && j - 3 >= 0 && tauler[i][j] == fitxa && tauler[i - 1][j - 1] == fitxa && tauler[i - 2][j - 2] == BUIT && tauler[i - 3][j - 3] == fitxa)
                {linies++;}
            else if (i - 3 >= 0 && j - 3 >= 0 && tauler[i][j] == fitxa && tauler[i - 1][j - 1] == BUIT && tauler[i - 2][j - 2] == fitxa && tauler[i - 3][j - 3] == fitxa)
                {linies++;}
        }
    }

    //contem les diagonals (/) (de baix a dalt cap a la dreta)
    //* * * X  * * * _  * * * X  * * * X
    //* * X *  * * X *  * * _ *  * * X *
    //* X * *  * X * *  * X * *  * _ * *
    //_ * * *  X * * *  X * * *  X * * *
    for (i = N - 1; i > 1; i--){
        for (j = 0; j < N - 2; j++){
            if (tauler[i][j] == fitxa && tauler[i - 1][j + 1] == fitxa && tauler[i - 2][j + 2] == fitxa) {
                if ((i + 1 < N && j - 1 >= 0 && tauler[i + 1][j - 1] == BUIT) ||
                    (i - 3 >= 0 && j + 3 < N && tauler[i - 3][j + 3] == BUIT))
                    {linies++;}
            }
            else if (i - 3 >= 0 && j + 3 < N && tauler[i][j] == fitxa && tauler[i - 1][j + 1] == fitxa && tauler[i - 2][j + 2] == BUIT && tauler[i - 3][j + 3] == fitxa)
                {linies++;}
            else if (i - 3 >= 0 && j + 3 < N && tauler[i][j] == fitxa && tauler[i - 1][j + 1] == BUIT && tauler[i - 2][j + 2] == fitxa && tauler[i - 3][j + 3] == fitxa)
                {linies++;}
        }
    }

    return linies;
}


//funcio que conta quantes files té cada jugador de 2 i de 3 que es puguin completar i retorna ladiferència entre les de 3 de la maquina i el jugador  multiplicades per 100 més la resta entre les de 2 de la maquina i del jugador
double comptarLinies(char  tauler[N][N]) {
    int     max3, min3, max2, min2;

    //if(ComprovarGuanyador(tauler, JUGADOR))return -INFINIT;
    //if(ComprovarGuanyador(tauler, JUGADOR))return INFINIT;
    //no cal per que ja ho comprovem quan fem la crida de funcio heuristica abans d'arribar aquesta opcio
    min3 = liniesDe3( tauler, JUGADOR);
    max3 = liniesDe3( tauler, MAQUINA);
    min2 = liniesDe2( tauler,JUGADOR);
    max2 = liniesDe2( tauler, MAQUINA);
    return (double) 100*(max3 - min3) + max2 - min2;
}

/**FINESTRES DE 4 FITXES**/
//Agafem les fitxes en posició  (fila, col), (fila+df, col+dc), (fila+2df, col+2dc), (fila+3df, col+3dc).
//Avalua una finestre de 4 caselles en una direcció i li assigna un pes segons la quantitat de fitxes de cada tipus.
//si el mode es diferent de 0, a més mirem els extrems (fila-df, col-df) i (fila+4df, col+4dc)
double avalua(char tauler[N][N], int fila, int col, int df, int dc, int modo) {
    int countM = 0, countJ = 0;

    // Comptar fitxes
    for (int k = 0; k < 4; k++) {
        Posicio c={fila + k * df,col + k * dc};
        //int f = fila + k * df;
        //int c = col + k * dc;
        if (!dinsTauler(c)) return 0;
        if (tauler[c.fila][c.col] == MAQUINA) countM++;
        else if (tauler[c.fila][c.col] == JUGADOR) countJ++;
    }

    // Si hi ha fitxes dels dos, no és útil
    if (countM > 0 && countJ > 0) return 0;

    // Mode simple (avalua1)
    if (modo == 0) {
        if (countM == 4) return 1000000;
        if (countM == 3) return 100;
        if (countM == 2) return 10;
        if (countM == 1) return 1;

        if (countJ == 4) return -1000000;
        if (countJ == 3) return -100;
        if (countJ == 2) return -10;
        if (countJ == 1) return -1;

        return 0;
    }
    else{
        // Mode avançat (avalua2)
        // Comprovar extrems
        int extrems = 0;
        Posicio inici={fila - df,col - dc},fi={fila + 4 * df, col + 4 * dc};
        //int f_inici = fila - df, c_inici = col - dc;
        //int f_fi = fila + 4 * df, c_fi = col + 4 * dc;

        if (dinsTauler(inici) && tauler[inici.fila][inici.col] == BUIT) extrems++;
        if (dinsTauler(fi) && tauler[fi.fila][fi.col] == BUIT) extrems++;

        if (countM == 4) return 1000000;
        if (countM == 3) {
            if (extrems >= 2) return 10000;
            if (extrems == 1) return 1000;
            return 10;
        }
        if (countM == 2) {
            if (extrems >= 2) return 100;
            if (extrems == 1) return 10;
            return 1;
        }
        if (countM == 1) return 1;

        if (countJ == 4) return -1000000;
        if (countJ == 3) {
            if (extrems >= 2) return -10000;
            if (extrems == 1) return -1000;
            return -10;
        }
        if (countJ == 2) {
            if (extrems >= 2) return -100;
            if (extrems == 1) return -10;
            return -1;
        }
        if (countJ == 1) return -1;

        return 0;
    }
}

//La suma dels pesos de avalua
double sumaDelPesos(char tauler[N][N],int modo) {
    double valor = 0;

    // ─── Horitzontals ───
    for (int i = 0; i < N; i++){
        for (int j = 0; j <= N - 4; j++){
            valor += avalua(tauler, i, j, 0, 1,modo);
        }
    }

    // ─── Verticals ───
    for (int i = 0; i <= N - 4; i++){
        for (int j = 0; j < N; j++){
            valor += avalua(tauler, i, j, 1, 0,modo);
        }
    }

    // ─── Diagonal / ───
    for (int i = 3; i < N; i++){
        for (int j = 0; j <= N - 4; j++){
            valor += avalua(tauler, i, j, -1, 1,modo);
        }
    }

    // ─── Diagonal \ ───
    for (int i = 0; i <= N - 4; i++){
        for (int j = 0; j <= N - 4; j++){
            valor += avalua(tauler, i, j, 1, 1,modo);
        }
    }

    return valor;
}

/**COMPTAR AMENACES GUANYADORES**/
/*Funcions a mirar per si utilitzo o no*/
//Funcio que agafa un tauler i far les diverses tirades posibles en una copia i mira quantes de elles son guanyadores pel jugador 'fitxa'
int comptaJugadesGuanyadores(char tauler[N][N], char fitxa){
    int total = 0;
    Posicio p;
    for (p.col = 0; p.col < N; p.col++) {
        if (!columnaValida(tauler, p.col)) continue;
        char copia[N][N];
        copiarTauler(copia, tauler);
        p.fila=tirada(copia, p.col, fitxa);
        if (ComprovaGuanyadorDesDe(copia,fitxa,p,NULL)) {total++;}
    }
    return total;
}

//Funcio que suma totes les jugades guanyadores de cadascu
double SumaJugadesGuanyadores(char tauler[N][N]){

    int guanyadorsM=comptaJugadesGuanyadores(tauler,MAQUINA);//nombre de jugades guanyadores de la maquina
    int guanyadorsJ=comptaJugadesGuanyadores(tauler,JUGADOR);//nombre de jugades guanyadores del jugador
    double v=0;
    //donem més pes(negatiu) al jugador per notar el pes de que el jugador vagi a guanya per la maquina
    //Amenaces immediates
    v+=guanyadorsM* 100000;
    v-=guanyadorsJ* 120000;
   // Dues jugades guanyadores simultanies solen ser decisives
    if (guanyadorsM >= 2) v += 300000;
    if (guanyadorsJ >= 2) v -= 350000;

    // Patrons generals
    v += sumaDelPesos(tauler, 1);

    // Control del centre
    v+= 5.0 * comptarCadaTipusCentre(tauler);

    return v;
}

/*Proves---------*/

int main3(){
    char t1[8][8] = {
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'X', ' ', ' ', 'O', 'O', ' ', ' ', ' '}
    };

    char t2[8][8] = {
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'O', 'O', 'X', ' ', ' ', ' ', ' ', ' '},
    {'O', 'X', 'O', ' ', ' ', ' ', ' ', ' '},
    {'X', 'O', 'X', ' ', ' ', ' ', ' ', ' '}
    };

    char t3[8][8] = {
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', 'X', 'O', 'O'},
    {' ', ' ', ' ', ' ', ' ', 'O', 'X', 'O'},
    {' ', ' ', ' ', ' ', ' ', 'X', 'O', 'X'}
    };

    char t[8][8] = {
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'X'},
    {' ', ' ', ' ', ' ', ' ', ' ', 'X', 'O'},
    {' ', ' ', ' ', ' ', ' ', ' ', 'O', 'X'},
    {'X', 'X', ' ', 'X', 'X', 'O', 'X', 'O'}
    };
    char t5[8][8] = {
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'O', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'X', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'X', 'O', ' ', ' ', ' ', 'O', 'X', 'X'}
    };
    dibuixarTauler(t,NULL);
    printf("#Linia 2 Jugador= %i \n",liniesDe2(t,JUGADOR));
    printf("#Linia 2 Maquina=%i\n",liniesDe2(t,MAQUINA));
    printf("#Linia 3 Jugador= %i\n",liniesDe3(t,JUGADOR));
    printf("#Linia 3 Maquina=%i\n",liniesDe3(t,MAQUINA));


    return 0;
}
