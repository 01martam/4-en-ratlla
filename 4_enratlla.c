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

// Comprova si una casella és dins del tauler
//int dinsTauler(int fila, int col) {
//    return fila >= 0 && fila < N && col >= 0 && col < N;
//}

int dinsTauler(Posicio p) {
    return p.fila >= 0 && p.fila < N && p.col >= 0 && p.col < N;
}

// Comprova si el tauler està ple (1 = ple, 0 = encara hi ha lloc)
int taulerPle(char tauler[N][N]) {
    for (int j = 0; j < N; j++) {
        if (columnaValida(tauler,j)) return 0;
    }
    return 1;
}

int taulerBuit(char tauler[N][N]){
     for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tauler[i][j] != BUIT) return 0;
        }
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
            L[k].fila=iniciFil+k;
            L[k].col=iniciCol-k;

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

//Agrupa les 4 en una sola versió
int Fitxa_Linia(char tauler[N][N], char fitxa, Posicio p, int df, int dc, Posicio linia[4]) {
    int aux = 1;
    Posicio inici=p;
    // Cap endavant (df, dc)
    for (int k = 1; k < 4; k++) {
        Posicio casella={p.fila + k * df,p.col + k * dc};
//        int f = p.fila + k * df;
//        int c = p.col + k * dc;
        if (!dinsTauler(casella)) break;
        if (tauler[casella.fila][casella.col] == fitxa) aux++;
        else break;
    }

    // Cap enrere (-df, -dc)
    for (int k = 1; k < 4; k++) {
        Posicio casella={p.fila - k * df,p.col - k * dc};
//        int f = p.fila - k * df;
//        int c = p.col - k * dc;
        if (!dinsTauler(casella)) break;
        if (tauler[casella.fila][casella.col] == fitxa) {
            aux++;
            inici= casella;
        } else {
            break;
        }
    }

    if (aux >= 4) {
        if(linia!=NULL){
            // Omplir linia amb les 4 posicions
            for (int k = 0; k < 4; k++) {
                linia[k].fila = inici.fila + k * df;
                linia[k].col = inici.col + k * dc;
            }
        }
        return 1;//guanya
    }

    return 0;//no guanya
}
//horitzontal:  X(iniciFil,iniciCol) X X X
//vertical:
//    x(iniciFil,iniciCol)
//    x
//    x
//    x
//diagonal /:
//       x(iniciFil,iniciCol)
//      x
//     x
//    x
//diagonal \:
//    x(iniciFil,iniciCol)
//     x
//      x
//       x

//Funció que comprova des de la posició de l'ultima tirada si hi ha linia guanyadora.
int ComprovaGuanyadorDesDe(char tauler[N][N], char fitxa,Posicio p,Posicio L[4]) {
//    return Fitxa_Linia_Horitzontal(tauler,fitxa,p,L)||
//           Fitxa_Linia_vertical(tauler,fitxa,p,L)||
//           Fitxa_Linia_Diagonal_dret(tauler,fitxa,p,L)||
//           Fitxa_Linia_Diagonal_esq(tauler,fitxa,p,L);

//horitzontal||vertical|| diagonal(/)||diagonal(\)

    return Fitxa_Linia(tauler, fitxa, p, 0, 1, L)||Fitxa_Linia(tauler, fitxa, p, 1, 0, L)||Fitxa_Linia(tauler, fitxa, p, -1, 1, L)||Fitxa_Linia(tauler, fitxa, p, 1, 1, L);

}


/**Demanar al usuari**/
//Funció que demana a l'usuari la resposta d'una pregunta del tipus si o no
int demanaSiNo(char *pregunta) {
    char resposta;
    while (1) {
        printf("%s (s/n): ",pregunta);
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

//Funcio que demana al usuari un valor entre min a max
int demanaNumero(int min, int max) {
    int num;
    while (1) {
        printf("Tria (%d-%d): ", min,max);
        if (scanf("%d", &num) != 1) {
            while (getchar() != '\n');
            printf("Entrada no vAlida.(han de ser numeros)\n");
            continue;
        }
        while (getchar() != '\n');
        if (num >= min && num <= max) return num;
        printf("Opcio no valida. Ha de ser %d-%d.\n", min, max);
    }
}

//Funció que demana al usuari que esculli una columna
int demanaColumna(char tauler[N][N]) {
    int col;
    while (1) {
//        printf("Tria columna (1-%d): ", N);
//        if (scanf("%d", &col) != 1) {
//            while (getchar() != '\n');
//            printf("Entrada no valida.(Han de ser numeros)");
//            continue;
//        }
//        col--;   // 1-N → 0-(N-1)
//
//        if(col<0||col>=N) {
//            printf("Columna no valida. ");
//            continue;
//        }
        col=demanaNumero(1,N);
        col--;

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

//Funció que demana a l'usuari quin tipus de funció heuristica vol usar(dificultat de la maquina).
int demanaDificultat(int dificultatActual){
    printf("\n========= CANVIAR DIFICULTAT =========\n");
    printf("Dificultat actual: %d\n", dificultatActual);
//    int dificultat;
    const char *nomsDificultat[]=NOMS_DIFICULTAT;
    int mida= sizeof(nomsDificultat)/sizeof(nomsDificultat[0]);
    for(int i =0;i<mida;i++) printf("  %i - %s\n",i,nomsDificultat[i]);
    printf("======================================\n");
    return demanaNumero(0,mida-1);
//    while (1) {
//        printf("Tria dificultat: ");
//        if (scanf("%d", &dificultat) != 1) {
//            while (getchar() != '\n');
//            printf("Entrada no valida.(Han de ser numeros)\n");
//            continue;
//        }
//        if (dificultat >= 0 && dificultat <= 5) return dificultat;
//        printf("Dificultat no valida. Ha de ser 0-5.\n");
//    }
}

//Funció que enseña el menu d'opcions i permet escollir
int Menu(int dificultat,int guardar,int asistent){

    printf("\n===================== MENU PRINCIPAL =====================\n");
    printf("1. Jugar partida\n");
    printf("2. Veure records\n");
    printf("3. Veure instruccions\n");
    printf("4. Canviar dificultat (%i)\n",dificultat);
    printf("5. Guardar partides %s \n",(guardar)?"Activat":"Desactivat");
    printf("6. Assistent de tir %s \n",(asistent)?"Activat":"Desactivat");
    printf("7. Sortir");
    printf("\n==========================================================\n");
    return demanaNumero(1,7);

//    int opcio;
//    while (1) {
//        printf("Tria una opcio: ");
//        if (scanf("%d", &opcio) != 1) {
//            while (getchar() != '\n');
//            printf("Entrada no valida.(Han de ser numeros)\n");
//            continue;
//        }
//        if (opcio >= 1 && opcio <= 6) return opcio;
//        printf("Opcio no valida. Ha de ser 1-6.\n");
//    }
}

//Funcio que obre o tanca un fitxer segons la resposta de l'usuari
int configurarFitxer(FILE **fitxerPartides) {
    if (*fitxerPartides == NULL) {
        // No hi ha fitxer obert → preguntar si vol obrir-lo
        if (demanaSiNo("Vols guardar un historial de les partides en un fitxer?")) {
            *fitxerPartides = fopen("Partides.txt", "a");
            if (*fitxerPartides == NULL) {
                fprintf(stderr, "Error: no s'ha pogut obrir Partides.txt\n");
                return 0;
            } else {
                printf("Les partides es guardaran a Partides.txt.\n");
                return 1;
            }
        }
        return 0;
    } else {
        // Ja hi ha un fitxer obert → preguntar si vol tancar-lo
        if (demanaSiNo("Vols deixar de guardar l'historial de partides?")) {
            fclose(*fitxerPartides);
            *fitxerPartides = NULL;
            printf("Ja no guardem les partides.\n");
            return 0;
        }
        return 1;
    }
}


/**Neteja la pantalla**/
//Funcio que neteja la pantalla de la consola
void netejarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/**4 en ratlla**/
void instruccions(){
    const char *nomsDificultat[]=NOMS_DIFICULTAT;
    printf("\n=================================================== INSTRUCCIONS =======================================================\n");
    printf("\nEl joc consisteix a aconseguir 4 fitxes seguides en linia horitzontal, vertical o diagonal.\n\n");
    printf("- Les teves fitxes son " BLAU "%c" RESET ".\n",JUGADOR);
    printf("- Les de la MAQUINA son " VERMELL "%c" RESET ".\n",MAQUINA);
    printf("- Per tirar, escriu un numero de columna (1-8).\n");
    printf("- La fitxa caura per gravetat.\n\n");
    printf("\n=================================================== DIFICULTAT =========================================================\n");
    int mida=sizeof(nomsDificultat)/sizeof(nomsDificultat[0]);

    //"Aleatoria" //"Aleatoria + victories" //"Comptar fitxes" //"Comptar fitxes resp Centre"//"Comptar linies" //"4 simples" //"4 simples +2" // "Comptar guanyadors"
    char*Explicacio[]={
                "Els valors s'escull de forma totalment aleatoria entre valors de -1000 i 1000",
                "Com aleatoria,pero abans revisa el tauler per tal de veure si hi ha linia guanyadora",
                "Revisem el tauler per tal de veure si hi ha linia guanyadora i despres comptem el \n      nombre de fitxes de cada tipus del tauler i calcular la diferencia.",
                "Revisem el tauler per tal de veure si hi ha linia guanyadora i despres com 'Comptar \n      fitxes', pero li afegim mes pes a les fitxes del centre.",
                "Revisem el tauler per tal de veure si hi ha linia guanyadora i despres \n      comptem quants tipus de linia hi ha de cada tipus i li asignem un pes,i despres farem la seva suma.",
                "Revisem el tauler per tal de veure si hi ha linia guanyadora i despres anem revisant \n      cada 4 fitxes i comptem el nombre de que hi ha de cada tipus i li asignem un pes, que farem la seva suma.",
                "Revisem el tauler per tal de veure si hi ha linia guanyadora i despres a mes de revisar \n      4 fitxes com el '4simples' revisem 2 fitxes mes dels laterals.",
                "Revisem el tauler per tal de veure si hi ha linia guanyadora i despres\n      Comptem quant tirs guanyadors que hi ha de cada tipus i li asignem un pes,i despres farem la seva suma."
    };
    printf("\nExplicacio del les estrategies que segueix la màquina segons el nivell de dificultat.\n");
    for(int i =0;i<mida;i++) printf("  %i - %s:%s\n",i,nomsDificultat[i],Explicacio[i]);
    printf("\n Prem ENTER\n");
    getchar();
}

//Funció que decideix qui comença la partida següent segons qui guanya l'anterior, o aleatori si es empat
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


//Funció que reprodueix una partida del 4 en ratlla
char partida(char torn,FILE *fitxer,int dificultat,int asistent){
    char tauler[N][N];
    int guanyat = 0;//FALSE
    Posicio linia[4];// Vector on es guardarà la linia guanyadora.

    inicialitzarTauler(tauler);

    if(fitxer!=NULL){
        fprintf(fitxer,"\n======= PARTIDA =======");
        escriureTaulerFitxer(fitxer, tauler);

    }

    while (!guanyat && !taulerPle(tauler)) {
        netejarPantalla();
        dibuixarTauler(tauler,NULL);

        Posicio p;

        if (torn==JUGADOR){//torn humà
            if(asistent&&!taulerBuit(tauler)){
                char Copia[N][N];
                CopiarTaulerInvertit(Copia,tauler);
                int pista=ferminimax_arbre(Copia,dificultat);
                if(pista<0){fprintf(stderr,"Error en la tria de columna del minimax\n");}
                else{printf("PISTA: Tira a la columna %i\n",pista+1);}
            }
            printf("Jugador('%c') ", torn);
            p.col=demanaColumna(tauler);
            p.fila=tirada(tauler,p.col,JUGADOR);
            guanyat=ComprovaGuanyadorDesDe(tauler,JUGADOR,p,linia);

            if(fitxer!=NULL){
                fprintf(fitxer, "\nTorn JUGADOR ('%c'):columna %d (fila %d)",JUGADOR, p.col + 1, p.fila);
                escriureTaulerFitxer(fitxer, tauler);
            }
        }else{//torn maquina (o el 2nd jugador)
            //printf("Jugador ('%c') ", torn);
            //p.col=demanaColumna(tauler); //canviar el minimax per aixo i ja tindras partida de 2 jugadors, encara que no amb el marcador actualitzat
            //p.col=tiradaMaquina_arbre(tauler,dificultat); no va bé
            p.col=ferminimax_arbre(tauler,dificultat);
            if(p.col<0){fprintf(stderr,"Error en la tria de columna del minimax\n");}
            p.fila=tirada(tauler,p.col,MAQUINA);
            guanyat=ComprovaGuanyadorDesDe(tauler,MAQUINA,p,linia);
            if(fitxer!=NULL){
                fprintf(fitxer, "\nTorn MAQUINA ('%c'): columna %d (fila %d)",MAQUINA, p.col + 1, p.fila);
                escriureTaulerFitxer(fitxer, tauler);
            }
        }
        //canvi de jugador
        if (!guanyat) {torn = (torn == JUGADOR) ? MAQUINA : JUGADOR;}
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


//Funció que mostra el numero de partides i quantes ha guanyat cadascú, la dificultat de la funció heurística, i el millor i pitjor temps del jugador quan guanya
void mostrarMarcador(FILE *sortida,int numPartides, int victoriesJugador, int victoriesMaquina,
                     double recordJugador_MILLOR,double recordJugador_PITJOR,int ratxaJugador,int ratxaMaximaJugador) {

    char* aux=(ratxaJugador==1)?"a":"es";
    char* aux2=(ratxaMaximaJugador==1)?"a":"es";

    fprintf(sortida,"\n======================== RECORDS =========================\n");
    fprintf(sortida," Partides jugades:     %d                \n", numPartides);
    fprintf(sortida," Victories JUGADOR:    %d                \n", victoriesJugador);
    fprintf(sortida," Victories MAQUINA:    %d                \n", victoriesMaquina);
    fprintf(sortida," Empats:               %d                \n", numPartides - victoriesJugador - victoriesMaquina);
    fprintf(sortida,"----------------------------------------------------------\n");
    fprintf(sortida," Millor temps:         %.0fs             \n", (recordJugador_MILLOR<0)?0.0:recordJugador_MILLOR);
    fprintf(sortida," Pitjor temps:         %.0fs             \n", (recordJugador_PITJOR<0)?0.0:recordJugador_PITJOR);
    fprintf(sortida," Ratio Victories:      %.1f%%             \n", (numPartides > 0)? (100.0 * victoriesJugador / numPartides) : 0.0);
    fprintf(sortida," Ratxa actual:         %d victori%s seguid%s\n", ratxaJugador,aux,aux);
    fprintf(sortida," Maxima Ratxa:         %d victori%s seguid%s", ratxaMaximaJugador,aux2,aux2);
    fprintf(sortida,"\n==========================================================\n");

}




