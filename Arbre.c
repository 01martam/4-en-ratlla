#include <stdio.h>
#include <stdlib.h>

//#include "1 vs 1.h"

#define N 2
#define BUIT ' '
#define MAQUINA 'X'
#define JUGADOR 'O'
#define PROFUNDITAT 3// nivells de l'arbre
#define INFINIT 1e9 // Variable de numero gran

typedef struct node {
	char tauler[N][N];
	struct node* *fills;
	int n_fills;
	double valor;
}Node;

int comptador=0;//variable global

/*FUNCIONS*/
//funció que calcular el valor de les fulles
/*int funcioHeuristica(){
    // Per editar encara
    return ++comptador;
}*/

double funcioHeuristica(char tauler[N][N]) {
    int countM = 0, countJ = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (tauler[i][j] == MAQUINA) countM++;
            else if (tauler[i][j] == JUGADOR) countJ++;
        }
    }

    return countM - countJ;
}

//Omple el tauler amb BUIT
void inicialitzarTauler(char tauler[N][N]) {// #include 1 vs 1.h
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tauler[i][j] = BUIT;
        }
    }
}

// Copia el tauler del origen (pare) al desti(fill)
void copiarTauler(char desti[N][N],char origen[N][N]){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            desti[i][j]= origen[i][j];
        }
    }
}

// Comprova si la columna és vàlida (1 = sí, 0 = no)
int columnaValida(char tauler[N][N], int col) {//#include 1 vs 1
    if (col < 0 || col >= N) return 0;
    return tauler[0][col] == BUIT; // Retorna 1 si la casella superior està buida
}


// Compta quantes columnes NO estan plenes, es a dir el numero de posibles jugades, les quals cadascuna sera un fill
int calcularNumeroDeFills(char tauler[N][N]){
//mirar nomes la fila d'adalt de tot
    int aux=0;
    for (int j = 0; j < N; j++) {
        if(columnaValida(tauler,j)) aux++; //si la columna es valida no està plena
    }
   return aux;
}

// Aquesta funció rep el número de fill, i retorna l'índex real de la columna a la matriu
int numFillAColumna(char tauler[N][N],int numfill){
    int aux=0;
    for(int j=0;j<N;j++){
        // Si la columna té espai (no està plena)
        if (columnaValida(tauler,j)) {
            if (aux == numfill) {
                return j; // Retorna la columna real de la matriu
            }
            aux++;
        }
    }

    return -1; // En cas d'error (no hauria d'arribar mai aquí si numFill és vàlid)
}

/*
void tirada(char tauler[N][N],int numfill,char fitxa){
    //1.Transformar el numfill a la columna matriu
    int col = numFillAColumna(tauler,numfill);
    if(!columnaValida(tauler,col)) return -1; //columna invalida
    //2.Calcular la fila on cau per gravetat
    for(int i=N-1;i>=0;i--){
        //3.Posar la fitxa a la fila/columna
        if(tauler[i][col]==BUIT){
            tauler[i][col]=fitxa;
            return;
        }
    }

    //1.Transformar el numFill a columna de la matriu.
	//2.Calcula la fila on cau per gravetat
	//3.Posar la fitxa a la fila/columna calculada
}
*/

// Fa caure una fitxa per gravetat a la columna corresponent
int tirada(char tauler[N][N],int col,char fitxa){//#include 1 vs 1
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

/*Arbre de nomes 2 nivells----------------------------------------------*/
//Crea un node fill a partir d’un pare
Node *creaNode2(Node *pare, int numFill, int nivell) {
	Node *p=malloc(sizeof(Node));
	//p->valor=funcioHeuristica();
	//1.Copiar Tauler
	copiarTauler(p->tauler,pare->tauler);
	//2.Tirar la fitxa a la columna que toqui
	// Determinem la fitxa segons el nivell (Nivell senars la maquina i parells el jugador)
	char fitxa = (nivell % 2 == 1) ? MAQUINA : JUGADOR;
	int col = numFillAColumna(p->tauler, numFill);
	tirada(p->tauler,col,fitxa);
	//3.Crear l'array de fills si n'ha de tenir
	if (nivell<2) {
		p->n_fills=calcularNumeroDeFills(p->tauler);
		p->fills=malloc(p->n_fills * sizeof(Node *));
		p->valor = 0;  //no cal realment pero si faig Recorrearbre podria haberhi kk si no faig minimax
	}
	else {//es una fulla
		p->n_fills=0;
		p->fills=NULL;
		p->valor=funcioHeuristica(p->tauler);//nomès en les fulles s'aplica

	}
	return p;
}

//Crea tots els fills d’un node cridant creaNode() per a cada i (columna 0..7).
//El paràmetre nivell es passa a creaNode per decidir si el fill tindrà al seu torn fills.
void creaFills2(Node *pare, int nivell) {
	for(int i=0;i<pare->n_fills;i++) {
		pare->fills[i]=creaNode2(pare,i,nivell);
	}
}

//Construeix un arbre de profunditat 2
void creaArbre2(Node *arrel) {
	creaFills2(arrel,1);	//creem el 1er nivell de l'arbre
	for(int i=0;i<arrel->n_fills;i++)
		creaFills2(arrel->fills[i],2);
}



/*Arbre de més de 2 nivells----------------------------------------------*/
Node *creaNode(Node *pare, int numFill, int nivell) {
	Node *p=malloc(sizeof(Node));
	//p->valor=funcioHeuristica();
	copiarTauler(p->tauler,pare->tauler);

	char fitxa = (nivell % 2 == 1) ? MAQUINA : JUGADOR;
	int col = numFillAColumna(p->tauler, numFill);
	tirada(p->tauler,col,fitxa);

	if (nivell<PROFUNDITAT) {
		p->n_fills=calcularNumeroDeFills(p->tauler);
		p->fills=malloc(p->n_fills * sizeof(Node *));
		p->valor=0;//posa el minimax
	}
	else {
		p->n_fills=0;
		p->fills=NULL;
		p->valor=funcioHeuristica(p->tauler);

	}
	return p;
}

//Es exactament la mateixa funcio que sense el plus pero com creo una nova creaNode la he de crear
void creaFills(Node *pare, int nivell) {
	for(int i=0;i<pare->n_fills;i++) {
		pare->fills[i]=creaNode(pare,i,nivell);
	}
}

void creaArbreRec(Node *node, int nivell) {
    if (nivell > PROFUNDITAT) return;
    creaFills(node, nivell);
    for (int i = 0; i < node->n_fills; i++)
        creaArbreRec(node->fills[i], nivell + 1);
}



//imprimeix cada fill de l'arrel i després els fills d'aquests fills.
//iteratiu (nomes 2 nivells)
void recorreArbre(Node *arrel){//apuntador per evitar fer copiar encara que no editem
    for(int i=0;i<arrel->n_fills;i++){
        printf("%.0f\n",arrel->fills[i]->valor);
        for(int j=0;j<arrel->fills[i]->n_fills;j++){
            printf("   %.0f\n",arrel->fills[i]->fills[j]->valor);
        }
    }
}

//imprimeix cada fill de l'arrel i després els fills d'aquests fills.
//Recursiu (N nivells)
void recorreArbreRecursiu(Node *p,int Nivell){
    for(int k=0;k<Nivell;k++){printf("  ");}
    printf("%.0f\n",p->valor);
    for(int i=0;i<p->n_fills;i++){// el for ja asegura que si no te fills no continui
        recorreArbreRecursiu(p->fills[i],Nivell+1);
    }
}

// Esborra NOMÉS 1 node (sense els seus fills, que s'han d'haver esborrat abans)
void esborraNode(Node *p) {
    if (p == NULL) return;
    free(p->fills);
    free(p);
}

// Esborra tots els FILLS DIRECTES d'un node (no el node, ni els néts)
// Assumeix que els fills ja no tenen descendents
void esborra1Nivell(Node *p) {
    if (p == NULL) return;
    for (int i = 0; i < p->n_fills; i++) {
        esborraNode(p->fills[i]);
    }
    free(p->fills);
    p->fills = NULL;
    p->n_fills = 0;
}

// Esborra TOTS els nodes de l'arbre
void esborraArbre(Node *arrel) {
    if (arrel == NULL) return;

    // Baixem recursivament als néts abans d'esborrar els fills
    for (int i = 0; i < arrel->n_fills; i++) {
        esborraArbre(arrel->fills[i]);
    }
    // Ara els fills ja no tenen descendents → podem esborrar el nivell
    esborra1Nivell(arrel);   // esborra tots els fills + l'array fills
}

void esborraArbreRec(Node *arrel) {
    if (arrel == NULL) return;

    // Baixem recursivament als néts abans d'esborrar els fills
    for (int i = 0; i < arrel->n_fills; i++) {
        esborraArbreRec(arrel->fills[i]);
        free(arrel->fills[i]);//allibera el fill
    }
    free(arrel->fills);//allibera l'array de fills
    //arrel->fills=NULL;
    //arrel->n_fills=0;
    //free(arrel); no fem aixo perque depen de com definim l'arrel en el main

}




/*MINIMAX-----------------------------------------------------------------------*/
////Funcio que calcular el màxim entre 2 valors
//double max(double a,double b){
//    return (a>b)?a:b;
//}
////Funcio que calcular el minim entre 2 valors
//double min(double a, double b){
//    return (a<b)?a:b;
//}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//Funcio minimax
double minimax(Node *p, int nivell) {
	//Pas 1: comprovar si estem a les fulles ( és a dir al nivell maxim de l'arbre)
    if (p->n_fills == 0) {return p->valor;}

	//Pas 2:Separar els casos en si estem fent maxim o minim
    int esMaxim = (nivell % 2 == 1);
	//Cas maxim
    if (esMaxim) {
        double aux = -INFINIT;
        for (int i = 0; i < p->n_fills; i++) {
            aux = MAX(aux, minimax(p->fills[i], nivell + 1));
        }
        p->valor = aux;
        return aux;
    }
	//Cas minim
	else {
        double aux = INFINIT;
        for (int i = 0; i < p->n_fills; i++) {
            aux = MIN(aux, minimax(p->fills[i], nivell + 1));
        }
        p->valor = aux;
        return aux;
    }
}

//Funció que calcula la columna a traves de fer minimax
int tiradaMaquina(Node *arrel) {
    double aux = -INFINIT;
    int col = -1;

    for (int i = 0; i < arrel->n_fills; i++) {
        double v = minimax(arrel->fills[i], 1);   // ← crida minimax per cada fill
        if (v > aux) {
            aux = v;
            col = numFillAColumna(arrel->tauler, i);
        }
    }

    arrel->valor = aux;
    return col;
}


double ferminimax(Node *p, int nivell, int *col) {
    //Pas 1: comprovar si estem a les fulles ( és a dir al nivell maxim de l'arbre)
    if (p->n_fills == 0) {return p->valor;}
    //Pas 2:Separar els casos en si estem fent maxim o minim
    int esMaxim = (nivell % 2 == 1);
	int columna=-1;
    //Cas maxim
    if (esMaxim) {
        double aux = -INFINIT;
        //int columna = -1;

        for (int i = 0; i < p->n_fills; i++) {
            double v = minimax(p->fills[i], nivell + 1, NULL);
            if (v > aux) {//calcular el maxim entre el valors i calcular la columna que es la jugada
                aux = v;
                if (col != NULL) {columna = numFillAColumna(p->tauler, i);}
            }
        }

        p->valor = aux;
        if (col != NULL) *col = columna;
        return aux;
    }
    //Cas minim
    else {

        double aux = INFINIT;
        //int columna = -1;

        for (int i = 0; i < p->n_fills; i++) {
            double v = minimax(p->fills[i], nivell + 1, NULL);
            if (v < aux) {//calcular el maxim entre el valors i calcular la columna que es la jugada
                aux = v;
                if (col != NULL) {columna = numFillAColumna(p->tauler, i);}
            }
        }

        p->valor = aux;
        if (col != NULL) *col = columna;
        return aux;
    }
}



/*Idea

int actualitzaMax(double v, double *aux, int *col, int i, char tauler[N][N]) {
    if (v > *aux) {
        *aux = v;
        //if (col != NULL) {
            return numFillAColumna(tauler, i);
        //}
    }
    return -1;   // no s'ha actualitzat
}
int actualitzaMin(double v, double *aux, int *col, int i, char tauler[N][N]) {
    if (v < *aux) {
        *aux = v;
       // if (col != NULL) {
            return numFillAColumna(tauler, i);
        //}
    }
    return -1;   // no s'ha actualitzat
}


double minimax(Node *p, int nivell, int *col) {
    // Cas base: fulla
    if (p->n_fills == 0) {return p->valor;}

    int esMaxim = (nivell % 2 == 1);
	int columna=-1;

    if (esMaxim) {
        // MAX
        double aux = -INFINIT;
        //int columna = -1;

        for (int i = 0; i < p->n_fills; i++) {
            double v = minimax(p->fills[i], nivell + 1, NULL);
			int n= actualitzaMax(v,&aux,col,i,p->tauler);
			if(n!=-1&&col==NUlL) columna=n;
        }

        p->valor = aux;
        if (col != NULL) *col = columna;
        return aux;
    } else {
        // MIN
        double aux = INFINIT;
        //int columna = -1;

        for (int i = 0; i < p->n_fills; i++) {
            double v = minimax(p->fills[i], nivell + 1, NULL);
            int n= actualitzaMin(v,&aux,col,i,p->tauler);
			if(n!=-1&&col==NUlL) columna=n;
        }

        p->valor = aux;
        if (col != NULL) *col = columna;
        return aux;
    }
}


*/




/*MAIN-----------------------------------------------------------*/


int main() {
	Node arrel;
	arrel.valor=0;//valor posat pel recorreArbre
	arrel.n_fills=N;
	arrel.fills=malloc(N*sizeof(Node *));
	inicialitzarTauler(arrel.tauler);

    //creaArbre(&arrel);
	creaArbreRec(&arrel,1);
	//minimax 1 crida 2 funcions
	//int col = tiradaMaquina(&arrel);       // ← una sola crida

	//crida minimax amb col
	//int col;
    //double valor = ferminimax(arrel, 1, &col);   // ← col s'omple amb la millor columna

// Ara col té la columna on la MÀQUINA ha de tirar
	//recorreArbreRecursiu(&arrel,0);
	minimax(&arrel, 0);
	recorreArbreRecursiu(&arrel,0);
	printf("%f",arrel.valor);
	esborraArbre(&arrel);
	//free(arrel) si arrel fos dinamic

	return 0;
}
