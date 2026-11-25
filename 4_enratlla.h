#ifndef ENRATLLA_H
#define ENRATLLA_H

/*CONSTANTS*/
#define N 8
#define BUIT ' '
#define MAQUINA 'X'
#define JUGADOR 'O'

#define VERMELL "\033[31m"
#define BLAU    "\033[34m"
#define RESET   "\033[0m"
#define FONS_GROC "\033[43m"
#define NEGRETA   "\033[1m"

/*ESTRUCTURA*/
typedef struct posicio {
    int fila;
    int col;
} Posicio;

/*SOBRE EL TAULER*/
void inicialitzarTauler(char tauler[N][N]);
void dibuixarTauler(char tauler[N][N],Posicio linia[4]);
void escriureTaulerFitxer(FILE *fitxer, char tauler[N][N]);
int columnaValida(char tauler[N][N], int col);
int taulerPle(char tauler[N][N]);
int dinsTauler(int fila, int col);
int tirada(char tauler[N][N],int col,char fitxa);

/*REVISAR GUANYADOR, REVISANT TOT EL TAULER*/
int Linia_Horitzontal(char tauler[N][N],char fitxa);
int Linia_Vertical(char tauler[N][N],char fitxa);
int Linia_Diagonal_dret(char tauler[N][N],char fitxa);
int Linia_Diagonal_esq(char tauler[N][N],char fitxa);
int ComprovarGuanyador(char tauler[N][N],char fitxa);

/*REVISAR GUANYADOR DESDE LA POSICIÓ DE L'ÚLTIMA FITXA*/
int Fitxa_Linia_Horitzontal(char tauler[N][N], char fitxa,Posicio p,Posicio L[4]);
int Fitxa_Linia_vertical(char tauler[N][N], char fitxa,Posicio p,Posicio L[4]);
int Fitxa_Linia_Diagonal_dret(char tauler[N][N], char fitxa,Posicio p, Posicio L[4]);
int Fitxa_Linia_Diagonal_esq(char tauler[N][N], char fitxa,Posicio p,Posicio L[4]);
int ComprovaGuanyadorDesDe(char tauler[N][N], char fitxa,Posicio p,Posicio L[4]);

int Fitxa_Linia(char tauler[N][N], char fitxa, Posicio p, int df, int dc, Posicio linia[4]);

/*DEMANAR AL USUARI*/
int demanaColumna(char tauler[N][N]);
int demanaDificultat();
int demanaSiNo(char *pregunta);

/*NETEJAR LA PANTALLA*/
void netejarPantalla();

/*MARCADOR*/
void mostrarMarcador(FILE *sortida,int dificultat,int numPartides, int victoriesJugador, int victoriesMaquina, int empats,double recordJugador_MILLOR,double recordJugador_PITJOR);

/*4 EN RATLLA*/
char decideixTornInicial(char guanyadorAnterior);
char partida(char torn,FILE *fitxer,int dificultat);
























#endif // 1_VS_1_H_INCLUDED
