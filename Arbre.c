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

/**Funcions sobre el tauler**/
// Copia el tauler del origen (pare) al desti(fill)
void copiarTauler(char desti[N][N],char origen[N][N]){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            desti[i][j]= origen[i][j];
        }
    }
}
// Copia el tauler del origen (pare) al desti(fill), però intercanviant el paper de la maquina i el jugador
void CopiarTaulerInvertit(char desti[N][N],char origen[N][N]){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            if(origen[i][j]==JUGADOR)desti[i][j]=MAQUINA;
            else if(origen[i][j]==MAQUINA)desti[i][j]=JUGADOR;
            else desti[i][j]=BUIT;
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

// Aquesta funció rep el número de fill (0 a nfills-1), i retorna l'índex real de la columna a la matriu
int numFillAColumna(char tauler[N][N],int numfill){
    if(numfill<0){
        fprintf(stderr,"ERROR, num de fills no es correcte");
        return -1;
    }
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


/**Crear Arbre de nivell exactament 2**/
//Crea un node fill a partir d’un pare
Node *creaNode2(Node *pare, int numFill, int nivell, int dificultat) {
	Node *p=malloc(sizeof(Node));
	//1.Copiar Tauler
	copiarTauler(p->tauler,pare->tauler);
	//2.Tirar la fitxa a la columna que toqui
	// Determinem la fitxa segons el nivell (Nivell senars la maquina i parells el jugador)
	char fitxa = (nivell % 2 == 1) ? MAQUINA : JUGADOR;
	Posicio casella;
	casella.col = numFillAColumna(p->tauler, numFill);
	casella.fila=tirada(p->tauler,casella.col,fitxa);
	//3.Crear l'array de fills si n'ha de tenir
	if (nivell<2) {
		p->n_fills=calcularNumeroDeFills(p->tauler);
		p->fills=malloc(p->n_fills * sizeof(Node *));
		p->valor = 0;  //no cal realment pero si faig Recorrearbre podria haberhi kk si no faig minimax
	}
	else {//es una fulla
		p->n_fills=0;
		p->fills=NULL;
		p->valor=funcioHeuristica(p->tauler,dificultat,casella);//nomès en les fulles s'aplica

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
	if (p==NULL){fprintf(stderr, "Error reservant memoria\n");return NULL;}

	//1.Copiar Tauler
	copiarTauler(p->tauler,pare->tauler);

    //2.Tirar la fitxa a la columna que toqui
	// Determinem la fitxa segons el nivell (Nivell senars la maquina i parells el jugador)
	char fitxa = (nivell % 2 == 1) ? MAQUINA : JUGADOR;
	Posicio casella;//={0,0};
	casella.col = numFillAColumna(p->tauler, numFill);//pasa de numfill a una columna del tauler
	casella.fila=tirada(p->tauler,casella.col,fitxa);
    //printf("Casella(%c)=(%i,%i)\n",fitxa,casella.fila,casella.col);
    //3.Crear l'array de fills si n'ha de tenir
    //Es terminal si  ha guanyat o el tauler esta ple
    int terminal =
        ComprovaGuanyadorDesDe(p->tauler,fitxa,casella,NULL) ||
        taulerPle(p->tauler);

    //Si no es fulla
	if (nivell<PROFUNDITAT&&!terminal) {
		p->n_fills=calcularNumeroDeFills(p->tauler);
        p->fills=malloc(p->n_fills * sizeof(Node *));

        if (p->fills == NULL) {
            fprintf(stderr, "Error reservant memoria\n");
            free(p);
            return NULL;
        }
        p->valor=0;// No cal pero es per tindre net el arbre, i
	}
	else {
		p->n_fills=0;
		p->fills=NULL;
		p->valor=funcioHeuristica(p->tauler, dificultat,casella);

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
    for(int k=0;k<Nivell;k++){printf("     ");}
    printf("%.0f\n",p->valor);
    for(int i=0;i<p->n_fills;i++){// el for ja asegura que si no te fills no continui
        recorreArbreRecursiu(p->fills[i],Nivell+1);
    }
}

/**Esborra arbre**/
// Esborra NOMÉS 1 node (sense els seus fills, que s'han d'haver esborrat abans)
void esborraNode(Node *p) {
    if (p == NULL) return;
    if(p->fills!=NULL) free(p->fills);
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
    double aux;
	//Pas 2:Separar els casos en si estem fent maxim o minim
	//Cas maxim
    if (nivell % 2 == 0) {
        aux = -INFINIT;
        for (int i = 0; i < p->n_fills; i++) {
            aux = MAX(aux, minimax(p->fills[i], nivell + 1));
        }
//        p->valor = aux;
//        return aux;
    }
	//Cas minim
	else {
        aux = INFINIT;
        for (int i = 0; i < p->n_fills; i++) {
            aux = MIN(aux, minimax(p->fills[i], nivell + 1));
        }
//        p->valor = aux;
//        return aux;
    }
    p->valor = aux;
    return p->valor;
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


//double ferminimax(Node *p, int nivell,int *col) {
//	//Pas 1: comprovar si estem a les fulles ( és a dir al nivell maxim de l'arbre)
//    if (p->n_fills == 0) {return p->valor;}
//    double aux;
//    int millor=0;
//	//Pas 2:Separar els casos en si estem fent maxim o minim
//	//Cas maxim
//    if (nivell % 2 == 0) {
//        aux = -INFINIT;
//        for (int i = 0; i < p->n_fills; i++) {
//            double v= ferminimax(p->fills[i], nivell + 1,col);
//            if(v>aux){
//                aux=v;
//                millor=i;
//            }
//            printf("Fill %d(%i) -> valor minimax: %.2f\n", i,numFillAColumna(p->tauler,i) ,v);
//        }
////        p->valor = aux;
////        return aux;
//    }
//	//Cas minim
//	else {
//        aux = INFINIT;
//        for (int i = 0; i < p->n_fills; i++) {
//            double v=ferminimax(p->fills[i], nivell + 1,col);
//            if(v<aux){
//                aux=v;
//                millor=i;
//            }
//            printf("Fill %d(%i) -> valor minimax: %.2f\n", i,numFillAColumna(p->tauler,i) ,v);
//        }
////        p->valor = aux;
////        return aux;
//    }
//
//    if(nivell==0&&col!=NULL) *col=numFillAColumna(p->tauler,millor);
//    p->valor = aux;
//    return aux;
//
//}



//Funcio que far el minimax i alhora calcula la columna que necesita
double ferminimax(Node *p, int nivell, int *col) {
    //Pas 1: comprovar si estem a les fulles ( és a dir al nivell maxim de l'arbre)
    if (p->n_fills == 0) {return p->valor;}
    //Pas 2:Separar els casos en si estem fent maxim o minim
    int columna=-1;

    //Cas maxim
    if (nivell % 2 ==0) {
        double aux = -INFINIT;
        for (int i = 0; i < p->n_fills; i++) {
            double v = ferminimax(p->fills[i], nivell + 1, NULL);
            if (v > aux) {//calcular el maxim entre el valors i calcular la columna que es la jugada
                aux = v;
                if (col != NULL) {columna = numFillAColumna(p->tauler, i);}
            }
            //printf("Fill %d(%i) -> valor minimax: %.2f\n", i,columna ,v);
        }
        p->valor = aux;
        if (col != NULL) *col = columna;
        return aux;
    }
    //Cas minim
    else {
        double aux = INFINIT;
        for (int i = 0; i < p->n_fills; i++) {
            double v = ferminimax(p->fills[i], nivell + 1, NULL);
            if (v < aux) {//calcular el minim entre el valors i calcular la columna que es la jugada
                aux = v;
                if (col != NULL) {columna = numFillAColumna(p->tauler, i);}
            }
            //printf("Fill %d(%i) -> valor minimax: %.2f\n", i,columna ,v);
        }
        p->valor = aux;
        if (col != NULL) *col = columna;
        return aux;
    }
}

//Funcio que crea l'arbre per despres fer el minimax i alhora calcula la columna que necesita
int ferminimax_arbre(char tauler[N][N], int dificultat) {
    int col=-1;
    Node *arrel = malloc(sizeof(Node));
    if(arrel==NULL) return col;
    copiarTauler(arrel->tauler, tauler);
    arrel->n_fills = calcularNumeroDeFills(arrel->tauler);
    arrel->fills = malloc(arrel->n_fills * sizeof(Node *));
    arrel->valor = 0;

    creaArbreRec(arrel, 0, dificultat);

    ferminimax(arrel, 0, &col);   // <- nivell 0 (arrel), col s'omple
    esborraArbreRec(arrel);
    free(arrel);

    return col;
}


/**FUNCIÓ HEURISTICA**///potser posarlo en un altre.c?

double funcioHeuristica(char tauler[N][N],int dificultat, Posicio Casella){
    if(dificultat<=0){//totalment aleaotori
        return ((double)rand() / RAND_MAX) * 2.0 * 1000 - 1000; //valors entre -1000 i 1000
    }

    //Ara primer mirem si hi ha jugades guanyadores o el tauler esta ple sense guanya

    if(Casella.fila>=0){
        if (ComprovaGuanyadorDesDe(tauler,MAQUINA,Casella,NULL)) return INFINIT;
        if (ComprovaGuanyadorDesDe(tauler,JUGADOR,Casella,NULL)) return -INFINIT;
    }
    else{//Per si no tenim una ultima casella on tirem
        if (ComprovarGuanyador(tauler, MAQUINA)) return INFINIT;
        if (ComprovarGuanyador(tauler, JUGADOR)) return -INFINIT;
    }
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
//    if(dificultat >=6) return SumaJugadesGuanyadores(tauler);
   return 0;
}

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


/*Funcions a mirar per si utilitzo o no*/
//Funcio que compta les jugades guanyadores
int comptaJugadesGuanyadores(char tauler[N][N], char fitxa){
    int total = 0;

    for (int col = 0; col < N; col++) {
        if (!columnaValida(tauler, col)) continue;

        char copia[N][N];
        copiarTauler(copia, tauler);
        int fila=tirada(copia, col, fitxa);

        if (ComprovarGuanyador(copia, fitxa)) {
            total++;
        }
    }

    return total;
}

//Funcio que suma totes les jugades guanyadores de cadascu
double SumaJugadesGuanyadores(char tauler[N][N]){

    int guanyadorsM=comptaJugadesGuanyadores(tauler,MAQUINA);
    int guanyadorsJ=comptaJugadesGuanyadores(tauler,JUGADOR);
    double v=0;

    //Amenaces immediates
    v+=guanyadorsM* 100000;
    v-=guanyadorsJ* 120000;
   // Dues jugades guanyadores simultanies solen ser decisives
    if (guanyadorsM >= 2) v += 300000;
    if (guanyadorsJ >= 2) v -= 350000;

    // Patrons generals
    v += sumaDelPesos(tauler, 1);

    // Control del centre
    v+= 5.0 * contarCadaTipusCentre(tauler);

    return v;
}




/*retorna el nombre de linies de 2 que hi ha del caracter c en el  tauler que es podrien completar
notem que tornem a estar contant les de de tres, però com que aquestes tenen 100 cops més punts,
no val la pena evitar-ho*/
int filesDe2(char tauler[N][N], char c) {
    int i, j, files = 0;

    //contem les horitzontals
    for(i = 0; i < N; i++)
        for(j = 0; j < N - 1; j++)
            if( tauler[i][j] == c &&  tauler[i][j + 1] == c)
                if ((j > 1 &&  tauler[i][j - 1] == BUIT &&  tauler[i][j - 2] == BUIT) ||
                    (j + 3 < N &&  tauler[i][j + 2] == BUIT &&  tauler[i][j + 2] == BUIT) ||
                    (j > 0 &&  tauler[i][j - 1] == BUIT && j + 2 < N &&  tauler[i][j + 2] == BUIT))
                    files++;
    //contem les veritcals
    for(j = 0; j < N; j++)
        for(i = 0; i < N - 1; i++)
            if ( tauler[i][j] == c &&  tauler[i + 1][j] == c)
                if (i + 3 < N &&  tauler[i + 2][j] == BUIT &&  tauler[i + 3][j] == BUIT)
                    files++;
    //contem les diagonals de dalt a l'esquerra a baix a la dreta (negatives)
    for (i = 0; i < N - 1; i++)
        for (j = 0; j < N - 1; j++)
            if ( tauler[i][j] == c &&  tauler[i + 1][j + 1] == c)
                if ((i > 0 && j > 0 &&  tauler[i - 1][j - 1] == BUIT) ||
                    (i + 2 < N && j + 2 < N &&  tauler[i + 2][j + 2] == BUIT))
                    files++;
    //contem les diagonals de dalt a la dreta a baix a l'esquerra (positives)
    for (i = 0; i < N - 1; i++)
        for (j = 1; j < N; j++)
            if ( tauler[i][j] == c &&  tauler[i + 1][j - 1] == c)
                if ((i > 0 && j + 1 < N &&  tauler[i - 1][j + 1] == BUIT) ||
                    (i + 2 < N && j - 1 > 0 &&  tauler[i + 2][j - 2] == BUIT))
                    files++;
    return files;
}

/*retorna el nombre de linies de 3 que hi ha del caracter c en el  tauler que es podrien completar*/
int filesDe3(char  tauler[N][N], char c) {
    int i, j, files = 0;

    //contem les horitzontals
    for(i = 0; i < N; i++)
        for(j = 0; j < N - 2; j++)
            if( tauler[i][j] == c &&  tauler[i][j + 1] == c &&  tauler[i][j + 2] == c) {
                if ((j > 0 &&  tauler[i][j - 1] == BUIT) || (j + 3 < N &&  tauler[i][j + 3] == BUIT))
                    files++;
            }
            else if (j + 3 < N &&  tauler[i][j] == c &&  tauler[i][j + 1] == c &&  tauler[i][j + 2] == BUIT &&  tauler[i][j + 3] == c)
                files++;
            else if (j + 3 < N &&  tauler[i][j] == c &&  tauler[i][j + 1] == BUIT &&  tauler[i][j + 2] == c &&  tauler[i][j + 3] == c)
                files++;
    //contem les veritcals
    for(j = 0; j < N; j++)
        for(i = 0; i < N - 2; i++)
            if ( tauler[i][j] == c &&  tauler[i + 1][j] == c &&  tauler[i + 2][j] == c) {
                if (i + 3 < N &&  tauler[i + 3][j] == BUIT)
                    files++;
            }
    //contem les diagonals de dalt a l'esquerra a baix a la dreta (negatives)
    for (i = 0; i < N - 2; i++)
        for (j = 0; j < N - 2; j++)
            if ( tauler[i][j] == c &&  tauler[i + 1][j + 1] == c &&  tauler[i + 2][j + 2] == c) {
                if ((i > 0 && j > 0 &&  tauler[i - 1][j - 1] == BUIT) ||
                    (i + 3 < N && j + 3 < N &&  tauler[i + 3][j + 3] == BUIT))
                    files++;
            }
            else if (i + 3 < N && j + 3 < N &&  tauler[i][j] == c &&  tauler[i + 1][j + 1] == c &&  tauler[i + 2][j + 2] == BUIT &&  tauler[i + 3][j + 3] == c)
                files++;
            else if (i + 3 < N && j + 3 < N &&  tauler[i][j] == c &&  tauler[i + 1][j + 1] == BUIT &&  tauler[i + 2][j + 2] == c &&  tauler[i + 3][j + 3] == c)
                files++;
    //contem les diagonals de dalt a la dreta a baix a l'esquerra (positives)
    for (i = 0; i < N - 2; i++)
        for (j = 2; j < N; j++)
            if ( tauler[i][j] == c &&  tauler[i + 1][j - 1] == c &&  tauler[i + 2][j - 2] == c) {
                if ((i > 0 && j + 1 < N &&  tauler[i - 1][j + 1] == BUIT) ||
                    (i + 3 < N && j - 2 > 0 &&  tauler[i + 3][j - 3] == BUIT))
                    files++;
            }
            else if (i + 3 < N && j - 2 > 0 &&  tauler[i][j] == c &&  tauler[i + 1][j - 1] == c &&  tauler[i + 2][j - 2] == BUIT &&  tauler[i + 3][j - 3] == c)
                files++;
            else if (i + 3 < N && j - 2 > 0 &&  tauler[i][j] == c &&  tauler[i + 1][j - 1] == BUIT &&  tauler[i + 2][j - 2] == c &&  tauler[i + 3][j - 3] == c)
                files++;
    return files;
}

/*mira si hi ha linea horitzontal o veritcal o una diagonal inclinada positiva o
negativa dels caracters en tot el  tauler i de llargada 4, retorna el char que té
4 en ratlla o, en el seu defecte, un espai.
diferència amb victoria: comprova totes les peces del tauler*/
char victoria_tsencer(char  tauler[N][N]) {
    int i, j;

    //comprovem les horitzontals
    for(i = 0; i < N; i++)
        for(j = 0; j < N - 3; j++)
            if ( tauler[i][j] != BUIT &&  tauler[i][j + 1] ==  tauler[i][j] &&
                 tauler[i][j + 2] ==  tauler[i][j] &&  tauler[i][j + 3] ==  tauler[i][j])
                return  tauler[i][j];
    //comprovem les verticals
    for(j = 0; j < N; j++)
        for(i = 0; i < N - 3; i++)
            if ( tauler[i][j] != BUIT &&  tauler[i + 1][j] ==  tauler[i][j] &&
                 tauler[i + 2][j] ==  tauler[i][j] &&  tauler[i + 3][j] ==  tauler[i][j])
                return  tauler[i][j];
    //comprovem les diagonals de dalt a l'esquerra a baix a la dreta (negatives)
    for (i = 0; i < N - 3; i++)
        for (j = 0; j < N - 3; j++)
            if ( tauler[i][j] != BUIT &&  tauler[i + 1][j + 1] ==  tauler[i][j] &&
                 tauler[i + 2][j + 2] ==  tauler[i][j] &&  tauler[i + 3][j + 3] ==  tauler[i][j])
                return  tauler[i][j];
    //comprovem les diagonals de dalt a la dreta a baix a l'esquerra (positives)
    for (i = 0; i < N - 3; i++)
        for (j = 3; j < N; j++)
            if ( tauler[i][j] != BUIT &&  tauler[i + 1][j - 1] ==  tauler[i][j] &&
                 tauler[i + 2][j - 2] ==  tauler[i][j] &&  tauler[i + 3][j - 3] ==  tauler[i][j])
                return  tauler[i][j];
    return BUIT;
}

/*si hi ha victoria del programa retorna 100.000, si hi ha victoria de la persona retorna -100.000
altrament, conta quantes files té cada jugador de dos i de 3 que es puguin completar i retorna la
diferència entre les de 3 del programa i la persona multiplicades per 100 més la resta entre les de
dos del programa i de la persona*/
int f_heuristica(char  tauler[N][N]) {
    char    vict;
    int     max3, min3, max2, min2;

    vict = victoria_tsencer( tauler);
    if(vict == JUGADOR)
        return -100000;
    if(vict == MAQUINA)
        return 100000;
    min3 = filesDe3( tauler, JUGADOR);
    max3 = filesDe3( tauler, MAQUINA);
    min2 = filesDe2( tauler,JUGADOR);
    max2 = filesDe2( tauler, MAQUINA);
    return 100*(max3 - min3) + max2 - min2;
}











/**Funció per veure que rep la funcio heuristica**/
void recorreTaulersRecursiu(Node *p,int Nivell) {
    printf("\nNivell %i:%.0f",Nivell,p->valor);
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
    {'X', 'X', 'X', ' ', ' ', ' ', 'X', 'X'},   // fila 0
    {'X', 'O', 'X', ' ', ' ', ' ', 'O', 'O'},   // fila 1
    {'O', 'O', 'O', ' ', ' ', ' ', 'X', 'X'},   // fila 2
    {'X', 'O', 'O', 'O', ' ', ' ', 'O', 'O'},   // fila 3
    {'O', 'X', 'X', 'O', ' ', ' ', 'X', 'X'},   // fila 4
    {'X', 'X', 'O', 'X', 'X', 'O', 'X', 'X'},   // fila 5
    {'O', 'X', 'X', 'O', 'O', 'X', 'O', 'O'},   // fila 6
    {'X', 'O', 'O', 'X', 'O', 'O', 'X', 'X'}    // fila 7
    };

    char tauler2[8][8] = {
    {' ', ' ', 'X', 'X', ' ', ' ', 'O', 'O'},
    {' ', ' ', 'X', 'O', ' ', ' ', 'O', 'X'},
    {' ', ' ', 'O', 'O', ' ', ' ', 'X', 'X'},
    {' ', ' ', 'X', 'O', ' ', ' ', 'O', 'O'},
    {' ', ' ', 'O', 'X', ' ', ' ', 'X', 'O'},
    {' ', ' ', 'X', 'X', ' ', ' ', 'O', 'X'},
    {' ', ' ', 'O', 'O', ' ', ' ', 'X', 'O'},
    {' ', ' ', 'X', 'O', ' ', ' ', 'O', 'X'}
    };

    char tauler3[8][8] = {
    {'X', ' ', ' ', ' ', 'O', ' ', ' ', 'X'},
    {'O', ' ', ' ', ' ', 'X', ' ', ' ', 'O'},
    {'X', ' ', ' ', ' ', 'O', ' ', ' ', 'X'},
    {'O', ' ', ' ', ' ', 'X', ' ', ' ', 'O'},
    {'X', ' ', ' ', ' ', 'O', ' ', ' ', 'X'},
    {'O', ' ', ' ', ' ', 'X', ' ', ' ', 'O'},
    {'X', ' ', ' ', ' ', 'O', ' ', ' ', 'X'},
    {'O', ' ', ' ', ' ', 'X', ' ', ' ', 'O'}
    };

    char tauler4[8][8] = {
    {'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O'},
    {'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X'},
    {'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O'},
    {'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X'},
    {'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O'},
    {'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X'},
    {'X', 'O', 'X', 'O', 'X', 'O', 'X', 'O'},
    {'O', 'X', 'O', 'X', 'O', 'X', 'O', 'X'}
    };

    char tauler5[8][8] = {
    {' ', 'X', 'O', ' ', 'X', 'O', ' ', 'X'},
    {'O', 'X', ' ', 'O', 'X', ' ', 'O', 'X'},
    {'X', ' ', 'O', 'X', ' ', 'O', 'X', ' '},
    {' ', 'O', 'X', ' ', 'O', 'X', ' ', 'O'},
    {'O', 'X', ' ', 'O', 'X', ' ', 'O', 'X'},
    {'X', ' ', 'O', 'X', ' ', 'O', 'X', ' '},
    {' ', 'O', 'X', ' ', 'O', 'X', ' ', 'O'},
    {'O', 'X', ' ', 'O', 'X', ' ', 'O', 'X'}
    };

    char tauler6[8][8] = {
    {'O', 'O', 'O', 'O', ' ', ' ', ' ', ' '},
    {'X', 'X', 'X', ' ', ' ', ' ', ' ', ' '},
    {'O', 'O', ' ', ' ', ' ', ' ', ' ', ' '},
    {'X', 'X', 'X', 'X', ' ', ' ', ' ', ' '},
    {'O', 'O', 'O', ' ', ' ', ' ', ' ', ' '},
    {'X', 'X', ' ', ' ', ' ', ' ', ' ', ' '},
    {'O', 'O', 'O', 'O', 'O', ' ', ' ', ' '},
    {'X', 'X', 'X', ' ', ' ', ' ', ' ', ' '}
    };



	Node arrel;
    inicialitzarTauler(arrel.tauler);
    copiarTauler(arrel.tauler,tauler);

	arrel.valor=0;//valor posat pel recorreArbre
	arrel.n_fills=calcularNumeroDeFills(arrel.tauler);
	arrel.fills=malloc(arrel.n_fills*sizeof(Node *));
    printf("numero de fills=%i\n",arrel.n_fills);
    printf("numero de la primera columna lliure(0-7)=%i\n",numFillAColumna(tauler,0));


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
    //p.col =tiradaMaquina(&arrel);   // ← col s'omple amb la millor columna
    p.fila=tirada(tauler,p.col,MAQUINA);
    printf("Col=%i (0-7)\n",p.col);
    printf("fil=%i (0-7)\n",p.fila);
    //printf("valor=%.0f\n\n\n",valor);
    dibuixarTauler(tauler,NULL);


// Ara col té la columna on la MÀQUINA ha de tirar
	recorreArbreRecursiu(&arrel,0);
	//minimax(&arrel, 0);
	//recorreArbreRecursiu(&arrel,0);

	freopen("PPP.txt", "w", stdout);
    recorreTaulersRecursiu(&arrel,0);
    fclose(stdout);
    printf("\n\narrel.valor=%.0f\n\n\n",arrel.valor);
	esborraArbreRec(&arrel);
	//free(arrel) si arrel fos dinamic

	return 0;
}
