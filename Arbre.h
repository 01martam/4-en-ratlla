#ifndef ARBRE_H
#define ARBRE_H

#include "4 enratlla.h"
/*CONSTANTS propies del joc (dins de  "4 enratlla.h")*/
//#define N 8
//#define BUIT ' '
//#define MAQUINA 'X'
//#define JUGADOR 'O'

/*CONSTANTS*/
#define PROFUNDITAT 2// nivells de l'arbre
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
int calcularNumeroDeFills(char tauler[N][N]);

/*ARBRE*/
int numFillAColumna(char tauler[N][N],int numfill);

/*ARBRE DE 2 NIVELLS*/
Node *creaNode2(Node *pare, int numFill, int nivell);
void creaFills2(Node *pare, int nivell);
void creaArbre2(Node *arrel);

/*ARBRE DE +2 NIVELL*/
Node *creaNode(Node *pare, int numFill, int nivell);
void creaFills(Node *pare, int nivell);
void creaArbreRec(Node *node, int nivell);

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
int millorColumna(Node *arrel);

int tiradaMaquina(Node *arrel);
int tiradaMaquina_arbre(char tauler[N][N]);

double ferminimax(Node *p, int nivell, int *col);
int ferminimax_arbre(char tauler[N][N]);

/*PER EDITAR*/
double funcioHeuristica();


#endif // ARBRE_H_INCLUDED
