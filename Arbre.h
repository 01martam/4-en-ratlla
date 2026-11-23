#ifndef ARBRE_H
#define ARBRE_H

#include "4_enratlla.h"
#include "funcioHeuristica.h"
/*CONSTANTS propies del joc (dins de  "4 enratlla.h")*/
//#define N 8
//#define BUIT ' '
//#define MAQUINA 'X'
//#define JUGADOR 'O'

/*CONSTANTS*/
#define PROFUNDITAT 4// nivells de l'arbre
#define INFINIT 1e9 // Variable de numero gran

/*ESTRUCTURA*/
typedef struct node {
	char tauler[N][N];
	struct node* *fills;
	int n_fills;
	double valor;
}Node;
/*MAXIM I MINIM*/
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))


/*SOBRE EL TAULER*/
void copiarTauler(char desti[N][N],char origen[N][N]);
void CopiarTaulerInvertit(char desti[N][N],char origen[N][N]);
int calcularNumeroDeFills(char tauler[N][N]);
int numFillAColumna(char tauler[N][N],int numfill);

/*ARBRE DE 2 NIVELLS*/
Node *creaNode2(Node *pare, int numFill, int nivell, int dificultat);
void creaFills2(Node *pare, int nivell, int dificultat);
void creaArbre2(Node *arrel, int dificultat);

/*ARBRE DE +2 NIVELL*/
Node *creaNode(Node *pare, int numFill, int nivell, int dificultat);
void creaFills(Node *pare, int nivell, int dificultat);
void creaArbreRec(Node *node, int nivell, int dificultat);

/*RECORRE L'ARBRE*/
void recorreArbre(Node *arrel);//Nomes serveix per nivell 2
void recorreArbreRecursiu(Node *p,int Nivell);

/*BORRA ARBRE*/ //PER EDITAR
void esborraNode(Node *p);
void esborra1Nivell(Node *p);
void esborraArbre(Node *arrel);
void esborraArbreRec(Node *arrel);

/*MINIMAX*/
double minimax(Node *p, int nivell);

int tiradaMaquina(Node *arrel);
int tiradaMaquina_arbre(char tauler[N][N],int dificultat);

double ferminimax(Node *p, int nivell, int *col);
int ferminimax_arbre(char tauler[N][N],int dificultat);


//
///*FUNCIÓ HEURISTICA*/
//double funcioHeuristica(char tauler[N][N],int dificultat, Posicio Casella);
//double contarCadaTipus(char tauler[N][N]);
//double contarCadaTipusCentre(char tauler[N][N]);
//double avalua(char tauler[N][N], int fila, int col, int df, int dc, int modo);
//double sumaDelPesos(char tauler[N][N],int modo);
//
//int comptaJugadesGuanyadores(char tauler[N][N], char fitxa);
//double SumaJugadesGuanyadores(char tauler[N][N]);
//
//int filesDe2(char tauler[N][N], char fitxa);
//int filesDe3(char tauler[N][N], char fitxa);
//char victoria_tsencer(char  tauler[N][N]);
//double comptarLinies(char  tauler[N][N]);

#endif
