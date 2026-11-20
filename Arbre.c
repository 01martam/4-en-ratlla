#include <stdio.h>
#include <stdlib.h>

#include "Arbre.h"
#include "4_enratlla.h"

/*DINS de "arbre.h"
#define N 8
#define BUIT ' '
#define MAQUINA 'X'
#define JUGADOR 'O'
#define PROFUNDITAT 4// nivells de l'arbre
#define INFINIT 1e9 // Variable de numero gran

typedef struct node {
	char tauler[N][N];
	struct node* *fills;
	int n_fills;
	double valor;
}Node;
*/

/**FUNCIONS**/
/**Funcions sobre el tauler**/
// Copia el tauler del origen (pare) al desti(fill)
void copiarTauler(char desti[N][N],char origen[N][N]){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            desti[i][j]= origen[i][j];
        }
    }
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

//int tirada1(char tauler[N][N],int col,char fitxa){}

/**Crear Arbre de nivell exactament 2**/
//Crea un node fill a partir d’un pare
Node *creaNode2(Node *pare, int numFill, int nivell, int dificultat) {
	Node *p=malloc(sizeof(Node));
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
		p->valor=funcioHeuristica(p->tauler,dificultat);//nomès en les fulles s'aplica

	}
	return p;
}

//Crea tots els fills d’un node cridant creaNode() per a cada i (columna 0..7).
//El paràmetre nivell es passa a creaNode per decidir si el fill tindrà al seu torn fills.
void creaFills2(Node *pare, int nivell,int dificultat) {
	for(int i=0;i<pare->n_fills;i++) {
		pare->fills[i]=creaNode2(pare,i,nivell,dificultat);
	}
}

//Construeix un arbre de profunditat 2
void creaArbre2(Node *arrel,int dificultat) {
	creaFills2(arrel,1,dificultat);	//creem el 1er nivell de l'arbre
	for(int i=0;i<arrel->n_fills;i++)
		creaFills2(arrel->fills[i],2,dificultat);
}


/**Crear Arbre de nivell mallor o igual a 2**/
//Crea un node fill a partir d’un pare
Node *creaNode(Node *pare, int numFill, int nivell,int dificultat) {
	Node *p=malloc(sizeof(Node));
	//1.Copiar Tauler
	copiarTauler(p->tauler,pare->tauler);
    //2.Tirar la fitxa a la columna que toqui
	// Determinem la fitxa segons el nivell (Nivell senars la maquina i parells el jugador)
	char fitxa = (nivell % 2 == 1) ? MAQUINA : JUGADOR;
	int col = numFillAColumna(p->tauler, numFill);
	tirada(p->tauler,col,fitxa);
    //3.Crear l'array de fills si n'ha de tenir
	if (nivell<PROFUNDITAT) {
		p->n_fills=calcularNumeroDeFills(p->tauler);

		if (p->n_fills>0){
            p->fills=malloc(p->n_fills * sizeof(Node *));
            p->valor=0;
		}else{
            p->fills=NULL;
            p->valor=funcioHeuristica(p->tauler, dificultat);


		}
	}
	else {
		p->n_fills=0;
		p->fills=NULL;
		p->valor=funcioHeuristica(p->tauler, dificultat);

	}
	return p;
}

//Crea tots els fills d’un node cridant creaNode() per a cada i (columna 0..7).
//El paràmetre nivell es passa a creaNode per decidir si el fill tindrà al seu torn fills.
void creaFills(Node *pare, int nivell, int dificultat) {
	for(int i=0;i<pare->n_fills;i++) {
		pare->fills[i]=creaNode(pare,i,nivell,dificultat);
	}
}

//Construeix un arbre de profunditat PROFUNDITAT de forma recursiva
void creaArbreRec(Node *node, int nivell, int dificultat) {
    if (nivell >= PROFUNDITAT) return;
    creaFills(node, nivell+1,dificultat);
    for (int i = 0; i < node->n_fills; i++)
        creaArbreRec(node->fills[i], nivell + 1,dificultat);
}

/**Recorre l'arbre**/
//imprimeix cada fill de l'arrel i després els fills d'aquests fills de forma iterativa (nomes 2 nivells)
void recorreArbre(Node *arrel){//apuntador per evitar fer copiar encara que no editem
    for(int i=0;i<arrel->n_fills;i++){
        printf("%.0f\n",arrel->fills[i]->valor);
        for(int j=0;j<arrel->fills[i]->n_fills;j++){
            printf("   %.0f\n",arrel->fills[i]->fills[j]->valor);
        }
    }
}

//imprimeix cada fill de l'arrel i després els fills d'aquests fills de forma recursiva (N nivells).
void recorreArbreRecursiu(Node *p,int Nivell){
    for(int k=0;k<Nivell;k++){printf("  ");}
    printf("%.0f\n",p->valor);
    for(int i=0;i<p->n_fills;i++){// el for ja asegura que si no te fills no continui
        recorreArbreRecursiu(p->fills[i],Nivell+1);
    }
}

/**Esborra arbre**/
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
    //free(arrel); no fem aixo perque depen de com definim l'arrel en el main

    //alliberem tots els fills de l'arrel i descendents i l'array fills de cadascun, però no alliberem l'arrel
}




/**MINIMAX**/
//Funcio d'algoritme minimax
double minimax(Node *p, int nivell) {
	//Pas 1: comprovar si estem a les fulles ( és a dir al nivell maxim de l'arbre)
    if (p->n_fills == 0) {return p->valor;}

	//Pas 2:Separar els casos en si estem fent maxim o minim
    int esMaxim = (nivell % 2 == 0);
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
    return p->valor;
}

//funció que recorre l'arbre per trobar el node que s'ha obtingut el minimax
int millorColumna(Node *arrel) {

    int nFill = 0;

    for (int i = 1; i < arrel->n_fills; i++) {
        if (arrel->fills[i]->valor > arrel->fills[nFill]->valor) {
            nFill = i;
        }
    }

    return numFillAColumna(arrel->tauler, nFill);
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

//Funció que crea l'arbre i que calcula la columna a traves de fer minimax
int tiradaMaquina_arbre(char tauler[N][N],int dificultat) {
    //crear l'arrel
    Node *arrel = malloc(sizeof(Node));
    copiarTauler(arrel->tauler, tauler);
    arrel->n_fills = calcularNumeroDeFills(arrel->tauler);
    arrel->fills = malloc(arrel->n_fills * sizeof(Node *));
    arrel->valor = 0;

    //crear l'arbre
    creaArbreRec(arrel, 0,dificultat);

    //Recorrem els fills per triar el millor
    double aux = -INFINIT;
    int col = -1;

    for (int i = 0; i < arrel->n_fills; i++) {
        double v = minimax(arrel->fills[i], 1);   // ← crida minimax per cada fill
        if (v > aux) {
            aux = v;
            col = numFillAColumna(arrel->tauler, i);
        }
    }

    //arrel->valor = aux; no cal realment

    //esborrem l'arbre
    esborraArbreRec(arrel);
    free(arrel);

    return col;
}

//Funcio que far el minimax i alhora calcula la columna que necesita
double ferminimax(Node *p, int nivell, int *col) {
    //Pas 1: comprovar si estem a les fulles ( és a dir al nivell maxim de l'arbre)
    if (p->n_fills == 0) {return p->valor;}
    //Pas 2:Separar els casos en si estem fent maxim o minim
    int columna=-1;
    int esMaxim = (nivell % 2 == 0);

    //Cas maxim
    if (esMaxim) {
        double aux = -INFINIT;
        //int columna = -1;

        for (int i = 0; i < p->n_fills; i++) {
            double v = ferminimax(p->fills[i], nivell + 1, NULL);
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
            double v = ferminimax(p->fills[i], nivell + 1, NULL);
            if (v < aux) {//calcular el minim entre el valors i calcular la columna que es la jugada
                aux = v;
                if (col != NULL) {columna = numFillAColumna(p->tauler, i);}
            }
        }

        p->valor = aux;
        if (col != NULL) *col = columna;
        return aux;
    }
}

//Funcio que crea l'arbre per despres fer el minimax i alhora calcula la columna que necesita
int ferminimax_arbre(char tauler[N][N], int dificultat) {
    Node *arrel = malloc(sizeof(Node));
    copiarTauler(arrel->tauler, tauler);
    arrel->n_fills = calcularNumeroDeFills(arrel->tauler);
    arrel->fills = malloc(arrel->n_fills * sizeof(Node *));
    arrel->valor = 0;

    creaArbreRec(arrel, 0, dificultat);

    int col;
    ferminimax(arrel, 0, &col);   // ← nivell 0 (arrel), col s'omple
    //ferminimax_poda(arrel, 0, &col,-INFINIT,+INFINIT);
    esborraArbreRec(arrel);
    free(arrel);

    return col;
}


/**FUNCIÓ HEURISTICA**/
//conta el numero de fitxes de cada tipus i calcula la resta
double contarCadaTipus(char tauler[N][N]){
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
double contarCadaTipusCentre(char tauler[N][N]){
    int countM = 0, countJ = 0;
    double extraM=0,extraJ=0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double aux=abs(i - N/2) + abs(j - N/2);//distancia de (i,j) a (N/2,N/2) ->Norma 1 de la diferencia
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


//Agafem les fitxes en posició  (fila, col), (fila+df, col+dc), (fila+2df, col+2dc), (fila+3df, col+3dc).
//Avalua una finestre de 4 caselles en una direcció i li assigna un pes segons la quantitat de fitxes de cada tipus.
//si el mode es diferent de 0, a més mirem els extrems (fila-df, col-df) i (fila+4df, col+4dc)
double avalua(char tauler[N][N], int fila, int col, int df, int dc, int modo) {
    int countM = 0, countJ = 0;

    // Comptar fitxes
    for (int k = 0; k < 4; k++) {
        int f = fila + k * df;
        int c = col + k * dc;
        if (!dinsTauler(f, c)) return 0;
        if (tauler[f][c] == MAQUINA) countM++;
        else if (tauler[f][c] == JUGADOR) countJ++;
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
        int f_inici = fila - df, c_inici = col - dc;
        int f_fi = fila + 4 * df, c_fi = col + 4 * dc;

        if (dinsTauler(f_inici, c_inici) && tauler[f_inici][c_inici] == BUIT) extrems++;
        if (dinsTauler(f_fi, c_fi) && tauler[f_fi][c_fi] == BUIT) extrems++;

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


double funcioHeuristica(char tauler[N][N],int dificultat){
    if(dificultat<=0){//totalment aleaotori
        return ((double)rand() / RAND_MAX) * 2.0 * 1000 - 1000; //valors entre -1000 i 1000
    }

    //Ara primer mirem si hi ha jugades guanyadores o el tauler esta ple sense guanya
    if (ComprovarGuanyador(tauler, MAQUINA)) return INFINIT;
    if (ComprovarGuanyador(tauler, JUGADOR)) return -INFINIT;
    if(taulerPle(tauler)) return 0; //empat
    //Aleaotiri però evitant victories
    if(dificultat==1) return ((double)rand() / RAND_MAX) * 2.0 * 1000 - 1000; //valors entre -1000 i 1000
    // contem quantes fitxes hi ha de cada tipus
    if(dificultat==2)return contarCadaTipus(tauler);

    if(dificultat==3) return contarCadaTipusCentre(tauler);
    //Contem fitxes pero mirant de 4 en 4
    if(dificultat==4)return sumaDelPesos(tauler,0);
    //Contem fitxes pero mirant de 4 en 4, i a més mirem 2 fitxes més
    if(dificultat>=5)return sumaDelPesos(tauler,1);

}

/*
//Agafem les fitxes en posició  (fila, col), (fila+df, col+dc), (fila+2df, col+2dc), (fila+3df, col+3dc).
//Avalua una finestre de 4 caselles en una direcció i li assigna un pes segons la quantitat de fitxes de cada tipus.
//double avalua1(char tauler[N][N], int fila, int col, int df, int dc) {
//    int countM = 0, countJ = 0;
//
//    // Comptar fitxes de la MÀQUINA i del JUGADOR a la finestra
//    for (int k = 0; k < 4; k++) {
//        int f = fila + k * df;
//        int c = col + k * dc;
//        if(!dinsTauler(f,c)) return 0;
//        if (tauler[f][c] == MAQUINA) countM++;
//        else if (tauler[f][c] == JUGADOR) countJ++;
//    }
//
//    // Si hi ha fitxes dels dos jugadors, la finestra no és útil, ja que no podrà haber-hi 4 del mateix tipus.
//    if (countM > 0 && countJ > 0) return 0;
//
//    // Només fitxes de la MÀQUINA (valors positius)
//    if (countM == 4) return 1000000;   // victòria
//    if (countM == 3) return 100;       // amenaça (3 en ratlla)
//    if (countM == 2) return 10;        // 2 en ratlla
//    if (countM == 1) return 1;         // 1 fitxa
//
//    // Només fitxes del JUGADOR (valors negatius)
//    if (countJ == 4) return -1000000;  // derrota
//    if (countJ == 3) return -100;      // amenaça del JUGADOR
//    if (countJ == 2) return -10;       // 2 en ratlla del JUGADOR
//    if (countJ == 1) return -1;        // 1 fitxa del JUGADOR
//
//    return 0;   // finestra buida
//}
//
//
////considera també els extrems (fila-df, col-df) i (fila+4df, col+4dc) a més de les 4 fitxes
//double avalua2(char tauler[N][N], int fila, int col, int df, int dc) {
//    int countM = 0, countJ = 0;
//
//    // Comptar fitxes
//    for (int k = 0; k < 4; k++) {
//        int f = fila + k * df;
//        int c = col + k * dc;
//        if(!dinsTauler(f,c)) return 0;
//        if (tauler[f][c] == MAQUINA) countM++;
//        else if (tauler[f][c] == JUGADOR) countJ++;
//    }
//
//    // Si hi ha fitxes dels dos, no és útil
//    if (countM > 0 && countJ > 0) return 0;
//
//    // Comprovar extrems (fila-df, col-df) i (fila+4df, col+4dc)
//    int extrems = 0;
//    int f_inici = fila - df, c_inici = col - dc;
//    int f_fi = fila + 4 * df, c_fi = col + 4 * dc;
//
//    if (dinsTauler(f_inici,c_inici) &&tauler[f_inici][c_inici] == BUIT) extrems++;
//    if (dinsTauler(f_fi,c_fi)&&tauler[f_fi][c_fi] == BUIT) extrems++;
//
//    // Valors segons fitxes i extrems oberts
//    if (countM == 4) return 1000000;
//    if (countM == 3) {
//        if (extrems >= 2) return 10000;   // 3 obert (amenaça doble)
//        if (extrems == 1) return 1000;    // 3 obert (amenaça simple)
//        return 10;                               // 3 tancat (no es pot completar)
//    }
//    if (countM == 2) {
//        if (extrems >= 2) return 100;
//        if (extrems == 1) return 10;
//        return 1;
//    }
//    if (countM == 1) return 1;
//
//    if (countJ == 4) return -1000000;
//    if (countJ == 3) {
//        if (extrems >= 2) return -10000;
//        if (extrems == 1) return -1000;
//        return -10;
//    }
//    if (countJ == 2) {
//        if (extrems >= 2) return -100;
//        if (extrems == 1) return -10;
//        return -1;
//    }
//    if (countJ == 1) return -1;
//
//    return 0;
//}
*/


/**Funció per veure que rep la funcio heuristica**/
void recorreTaulersRecursiu(Node *p,int Nivell) {
    printf("Nivell %i:%.0f\n",Nivell,p->valor);
    escriureTaulerFitxer(stdout,p->tauler);
    //dibuixarTauler(p->tauler, NULL);

    for (int i = 0; i < p->n_fills; i++){
        recorreTaulersRecursiu(p->fills[i],Nivell+1);
    }
}





/*MAIN-----------------------------------------------------------*/


int main1() {
    srand(123);
    int dificultat =0;

    char tauler[N][N] = {
    {'O', 'X', ' ', ' ', ' ', ' ', 'X', 'X'},   // fila 0
    {'X', 'O', ' ', ' ', ' ', ' ', 'O', 'O'},   // fila 1
    {'O', 'O', ' ', ' ', ' ', ' ', 'X', 'X'},   // fila 2
    {'X', 'O', ' ', ' ', ' ', ' ', 'O', 'O'},   // fila 3
    {'O', 'X', ' ', ' ', ' ', ' ', 'X', 'X'},   // fila 4
    {'X', 'X', ' ', 'O', ' ', ' ', 'X', 'X'},   // fila 5
    {'O', 'X', ' ', 'X', 'O', 'X', 'O', 'O'},   // fila 6
    {'X', 'O', ' ', 'X', 'O', 'O', 'X', 'X'}    // fila 7
    };

//char tauler[N][N] = {
//    {'O', 'X', 'O', 'X', ' ', 'X', 'O', 'X'},
//    {'O', 'X', 'X', 'O', ' ', 'O', 'X', 'O'},
//    {'O', 'X', 'O', 'X', ' ', 'X', 'O', 'X'},
//    {'X', 'O', 'X', 'O', ' ', 'O', 'X', 'O'},
//    {'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X'},
//    {'X', 'X', 'O', 'O', 'X', 'O', 'X', 'O'},
//    {'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X'},
//    {'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O'}
//};



	Node arrel;
    inicialitzarTauler(arrel.tauler);
    copiarTauler(arrel.tauler,tauler);

	arrel.valor=0;//valor posat pel recorreArbre
	arrel.n_fills=calcularNumeroDeFills(arrel.tauler);
	arrel.fills=malloc(arrel.n_fills*sizeof(Node *));
    printf("numero de fills=%i\n",arrel.n_fills);
    printf("numero de columna=%i\n",numFillAColumna(tauler,0));


    //creaArbre(&arrel);
	creaArbreRec(&arrel,0,dificultat);
	//minimax 1 crida 2 funcions

	//int col = tiradaMaquina_arbre(arrel.tauler);       // ← una sola crida

	//int col = tiradaMaquina(&arrel);       // ← una sola crida
    //minimax(&arrel,0);
	//crida minimax amb col
	Posicio p;
	p.col=-1;
	dibuixarTauler(tauler,NULL);
    double valor = ferminimax(&arrel,0, &p.col);   // ← col s'omple amb la millor columna
    p.fila=tirada(tauler,p.col,MAQUINA);
    printf("Col=%i (0-7)\n",p.col);
    printf("fil=%i (0-7)\n",p.fila);
    printf("valor=%.0f\n\n\n",valor);
    dibuixarTauler(tauler,NULL);


// Ara col té la columna on la MÀQUINA ha de tirar
	recorreArbreRecursiu(&arrel,0);
	//minimax(&arrel, 0);
	//recorreArbreRecursiu(&arrel,0);

	freopen("Probeta.txt", "w", stdout);
    recorreTaulersRecursiu(&arrel,0);
    fclose(stdout);
    printf("\n\narrel.valor=%.0f\n\n\n",arrel.valor);
	esborraArbreRec(&arrel);
	//free(arrel) si arrel fos dinamic

	return 0;
}
