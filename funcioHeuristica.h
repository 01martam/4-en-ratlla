#ifndef FUNCIOHEURISTICA_H_INCLUDED
#define FUNCIOHEURISTICA_H_INCLUDED

#include "4_enratlla.h"

#define NOMS_DIFICULTAT {"Aleatoria", \
                         "Aleatoria + victories", \
                         "Comptar fitxes", \
                         "Comptar fitxes resp Centre", \
                         "Comptar linies",\
                         "4 simples", \
                         "4 simples +2",\
                         "Comptar guanyadors"\
                         }
#define ABS(a) ((a)>=0? (a):-(a))
/*FUNCIO HEURISTICA PRINCIPAL*/
double funcioHeuristica(char tauler[N][N],int dificultat, Posicio Casella);

/*COMPTAR FITXES*/
double comptarCadaTipus(char tauler[N][N]);
double comptarCadaTipusCentre(char tauler[N][N]);

/*COMPTAR LINIES*/
int liniesDe2(char tauler[N][N], char fitxa);
int liniesDe3(char tauler[N][N], char fitxa);
double comptarLinies(char  tauler[N][N]);

/*FINESTRES DE 4 FITXES*/
double avalua(char tauler[N][N], int fila, int col, int df, int dc, int modo);
double sumaDelPesos(char tauler[N][N],int modo);

/*AMENACES GUANYADORES*/
int comptaJugadesGuanyadores(char tauler[N][N], char fitxa);
double SumaJugadesGuanyadores(char tauler[N][N]);

#endif
