#ifndef 1_VS_1_H
#define 1_VS_1_H

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
typedef struct {
    int fila;
    int col;
} Posicio;

/*SOBRE EL TAULER*/
void inicialitzarTauler(char tauler[N][N]);
void dibuixarTauler(char tauler[N][N],Posicio *linia);
int columnaValida(char tauler[N][N], int col);
int taulerPle(char tauler[N][N]);

//void tirada(char tauler[N][N],int col,char fitxa);
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

/*DEMANAR AL USUARI*/
int demanaColumna(char tauler[N][N]);
int demanaSiNo();

/*NETEJAR LA PANTALLA*/
void netejarPantalla();
void netejarPantalla2();

/*MARCADOR*/
void mostrarMarcador(int numPartides, int victoriesJugador, int victoriesMaquina, int empats,double recordJugador,double recordMaquina);

/*4 EN RATLLA*/
char partida(char torn);























#endif // 1_VS_1_H_INCLUDED
