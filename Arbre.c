#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "funcioHeuristica.h"
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

//Funció que crea de manera aleatoria un tauler del joc
void inicialitzaTaulerRandom(char tauler[N][N]) {
    inicialitzarTauler(tauler);//omplim tot buit

    for (int j=0;j<N;j++) {
        for (int i=N-1;i>=0;i--) {// de baix a adalt
            double random = (double)rand()/RAND_MAX;
            (tauler)[i][j]=BUIT;
            if (random < 0.25) (tauler)[i][j]=JUGADOR; //jugador
            else if (random <0.5) (tauler)[i][j]=MAQUINA; //màquina
            else break;

        }
    }
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


//Funció que imprimeix a un document el tauler que arriba a cada node de l'arbre (per veure que rep la funcio heuristica)
void recorreTaulersRecursiu(Node *p,int Nivell) {
    printf("\nNivell %i:%.0f",Nivell,p->valor);
    escriureTaulerFitxer(stdout,p->tauler);
    //dibuixarTauler(p->tauler, NULL);

    for (int i = 0; i < p->n_fills; i++){
        recorreTaulersRecursiu(p->fills[i],Nivell+1);
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
    int millor = 0;

    for (int i = 0; i < arrel->n_fills; i++) {
        double v = minimax(arrel->fills[i], 1);   // ← crida minimax per cada fill
        if (v > aux) {
            aux = v;
            millor = i;
        }
    }

    arrel->valor = aux;
    return numFillAColumna(arrel->tauler, millor);
}

//Funció que crea l'arbre i que calcula la columna a traves de fer minimax
int tiradaMaquina_arbre(char tauler[N][N],int dificultat) {
    //crear l'arrel
    Node *arrel = malloc(sizeof(Node));
    if (arrel==NULL) return -1;
    copiarTauler(arrel->tauler, tauler);
    arrel->n_fills = calcularNumeroDeFills(arrel->tauler);

    if(arrel->n_fills==0){
        free(arrel);
        return -1;
    }
    arrel->fills = malloc(arrel->n_fills * sizeof(Node *));
    arrel->valor = 0;

    //crear l'arbre
    creaArbreRec(arrel, 0,dificultat);

    //Recorrem els fills per triar el millor
    double aux = -INFINIT;
    int millor=0;

    for (int i = 0; i < arrel->n_fills; i++) {
        double v = minimax(arrel->fills[i], 1);   // ← crida minimax per cada fill
        if (v > aux) {
            aux = v;
           millor=i;
        }
    }

    //arrel->valor = aux; no cal realment
    int col = numFillAColumna(arrel->tauler, millor);
    //esborrem l'arbre
    esborraArbreRec(arrel);
    free(arrel);

    return col;
}


//double ferminimax(Node *p, int nivell,int *col) {
//	//Pas 1: comprovar si estem a les fulles ( és a dir al nivell maxim de l'arbre)
//    if (p->n_fills == 0) {return p->valor;}
//    double aux;
//    int millor=0;// inicialitzem amb la primera columna posible
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
    int columna=numFillAColumna(p->tauler, 0);// inicialitzem amb la primera columna posible
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
    if(arrel==NULL) return col; // l'arrel es buida
    copiarTauler(arrel->tauler, tauler);
    arrel->n_fills = calcularNumeroDeFills(arrel->tauler);
    if(arrel->n_fills==0) {//En cas que l'arrel no te fills
        free(arrel);
        return col;}
    arrel->fills = malloc(arrel->n_fills * sizeof(Node *));
    arrel->valor = 0;

    creaArbreRec(arrel, 0, dificultat);

    ferminimax(arrel, 0, &col);   // <- nivell 0 (arrel), col s'omple
    esborraArbreRec(arrel);
    free(arrel);

    return col;
}


/*PROVES-----------------------------------------------------------*/

int main1() {
    srand(time(NULL));
    int dificultat =7;
    char tauler1[N][N] = {
    {'X', 'X', 'X', ' ', ' ', ' ', 'X', 'X'},   // fila 0
    {'X', 'O', 'X', ' ', ' ', ' ', 'O', 'O'},   // fila 1
    {'O', 'O', 'O', ' ', ' ', ' ', 'X', 'X'},   // fila 2
    {'X', 'O', 'O', ' ', ' ', 'O', 'O', 'O'},   // fila 3
    {'O', 'X', 'X', 'O', 'O', 'O', 'X', 'X'},   // fila 4
    {'X', 'X', 'O', 'O', 'X', 'O', 'X', 'X'},   // fila 5
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
    {'X', 'X', 'X', 'O', 'O', 'X', 'O', 'X'}
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

    char tauler[8][8];
    inicialitzaTaulerRandom(tauler);

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


#define NUM_PROVES 5

int main2() {
    srand(time(NULL));
    const char *nomsDificultat[] = NOMS_DIFICULTAT; // els noms de les dificultats
    int numDificultats = sizeof(nomsDificultat) / sizeof(nomsDificultat[0]);// el nombre de dificultats

    for (int prova = 1; prova <= NUM_PROVES; prova++) {
        char taulerBase[N][N];

        // Genera tauler
        inicialitzaTaulerRandom(taulerBase);

        printf("\n=================================================================\n");
        printf("                    PROVA ALEATORIA #%d / %d\n", prova, NUM_PROVES);
        printf("\n=================================================================\n");
        dibuixarTauler(taulerBase, NULL);

        // Probamos todas las dificultades sobre este mismo tablero aleatorio
        for (int dificultat = 0; dificultat < numDificultats; dificultat++) {
            printf("\n------------------------------------------------------\n");
            printf("[Prova #%d] Dificultat %d: %s\n", prova, dificultat, nomsDificultat[dificultat]);
            printf("------------------------------------------------------\n");

            // Copia el tauler base per no modificarlo amb les diferents dificultats
            char taulerProva[N][N];
            copiarTauler(taulerProva, taulerBase);

            // 2. Inici de l'arrel
            Node arrel;
            copiarTauler(arrel.tauler, taulerProva);
            arrel.valor = 0;// per no tindre valors basura al crear l'arbre
            arrel.n_fills = calcularNumeroDeFills(arrel.tauler);

            if (arrel.n_fills == 0) {
                printf("Tauler ple, passant a la seguent prova.\n");
                break;
            }

            arrel.fills = malloc(arrel.n_fills * sizeof(Node *));
            if (arrel.fills == NULL) {
                fprintf(stderr, "Error reservant memoria a l'arrel\n");
                return 1;
            }

            // 3. Creem l'arbre i i fem minimax
            creaArbreRec(&arrel, 0, dificultat);

            Posicio p;
            p.col = -1;
            double valor = ferminimax(&arrel, 0, &p.col);

            //Mirem si dona error per que la columna no es valida
            if (p.col < 0 || !columnaValida(taulerProva, p.col)) {
                fprintf(stderr, "ALERTA: Columna invalida (%d)!\n", p.col);
            } else {
                p.fila = tirada(taulerProva, p.col, MAQUINA);
                printf("Columna triada (1-8): %d | Fila: %d | Valor minimax: %.2f\n",
                       p.col + 1, p.fila, valor);
                dibuixarTauler(taulerProva, NULL);
            }

            // 4. Esborrem l'arbre
            esborraArbreRec(&arrel);
        }
    }

    return 0;
}
