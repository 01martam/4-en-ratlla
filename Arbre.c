//Classe 7: 02/10/26

#include <stdio.h>
#include <stdlib.h>

#define N 5
#define BUIT ' '
#define MAQUINA 1
#define JUGADOR 2

typedef struct node {
	char tauler[N][N];
	struct node* *fills;
	int n_fills;
	double valor;
}Node;
int comptador=0;//variable global

/*FUNCIONS*/
//Omple el tauler amb BUIT
void inicialitzarTauler(char tauler[N][N]) {
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

// Compta quantes columnes NO estan plenes, es a dir el numero de posibles jugades, les quals cadascuna sera un fill
int calcularNumeroDeFills(char tauler[N][N]){
//mirar nomes la fila d'adalt de tot
    int aux=0;
    for (int j = 0; j < N; j++) {
        if (tauler[0][j] == BUIT) aux++;//si la columna no està plena
    }
   return aux;
   return N;

}

// Aquesta funció rep el tauler i el número de fill, i retorna l'índex real de la columna a la matriu
int numFillAColumna(char tauler[N][N],int numfill){
    int aux=0;
    for(int j=0;j<N;j++){
        // Si la columna té espai (no està plena)
        if (tauler[0][j] == BUIT) {
            if (aux == numfill) {
                return j; // Retorna la columna real de la matriu
            }
            aux++;
        }
    }

    return -1; // En cas d'error (no hauria d'arribar mai aquí si numFill és vàlid)
}

// Fa caure una fitxa per gravetat a la columna corresponent
/*void Tirada(char tauler[N][N],int numfill,char fitxa){
    //1.Transformar el numfill a la columna matriu
    int col = numFillAColumna(tauler,numfill);
    if(col==-1){return;}
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

void tirada(char tauler[N][N],int numfill){}

//Crea un node fill a partir d’un pare
Node *creaNode(Node *pare, int numFill, int nivell) {
	Node *p=malloc(sizeof(Node));
	p->valor=++comptador;// summa i despres asigna. lo que seria la funcio heuristica
	//1.Copiar Tauler
	copiarTauler(p->tauler,pare->tauler);
	//2.Tirar la fitxa a la columna que toqui
    // Determinem la fitxa segons el nivell (Nivell 1 = 'X'(IA), Nivell 2 = 'O'(Jugador))
    //char fitxa = (nivell == 1) ? MAQUINA : JUGADOR;
	tirada(p->tauler,numFill);
	//3.Crear l'array de fills si n'ha de tenir
	if (nivell<2) {
		p->n_fills=calcularNumeroDeFills(p->tauler);
		p->fills=malloc(p->n_fills * sizeof(Node *));
	}
	else {
		p->n_fills=0;
		p->fills=NULL;
	}
	return p;
}

//Crea tots els fills d’un node cridant creaNode() per a cada i (columna 0..7).
//El paràmetre nivell es passa a creaNode per decidir si el fill tindrà al seu torn fills.
void creaFills(Node *pare, int nivell) {
	for(int i=0;i<pare->n_fills;i++) {
		pare->fills[i]=creaNode(pare,i,nivell);
	}
}

//Construeix un arbre de profunditat 2
void creaArbre(Node *arrel) {
	creaFills(arrel,1);	//creem el 1er nivell de l'arbre
	for(int i=0;i<arrel->n_fills;i++)
		creaFills(arrel->fills[i],2);
}

//imprimeix cada fill de l'arrel i després els fills d'aquests fills.
//iteratiu
void recorreArbre(Node *arrel){//apuntador per evitar fer copiar encara que no editem
    for(int i=0;i<arrel->n_fills;i++){
        printf("%.0f\n",arrel->fills[i]->valor);
        for(int j=0;j<arrel->fills[i]->n_fills;j++){
            printf("   %.0f\n",arrel->fills[i]->fills[j]->valor);
        }
    }
}

//imprimeix cada fill de l'arrel i després els fills d'aquests fills.
//Recursiu
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
    p->fills = NULL;
    p->n_fills = 0;
    free(p);
}

// Esborra tots els FILLS DIRECTES d'un node (no el node, ni els néts)
// Assumeix que els fills ja no tenen descendents
void esborra1Nivell(Node *p) {
    if (p == NULL) return;
    for (int i = 0; i < p->n_fills; i++) {
        esborraNode(p->fills[i]);
        p->fills[i] = NULL;
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




/*MAIN*/


int main() {
	Node arrel;
	arrel.valor=0;//valor posat pel recorreArbre
	arrel.n_fills=N;
	arrel.fills=malloc(N*sizeof(Node *));
	inicialitzarTauler(arrel.tauler);


	creaArbre(&arrel);
	recorreArbreRecursiu(&arrel,0);
	esborraArbre(&arrel);
	//free(arrel) si arrel fos dinamic

	return 0;
}
