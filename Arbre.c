//Classe 6
/*Com crear un arbre*/
#include <stdio.h>
#include <stdlib.h>
#define N 8
typedef struct node {
    char tauler[N][N];//char ocupa 1 byte
    struct node* *fills; //array dinàmic de pointers a Node
    //struct node *fills[N] no es exactament lo mateix?
    int n_fills; //tamany de l’array dinàmic
    double valor; //el valor del node (F. Heurística) //pots ficar int
    //espot ficar més coses si preguntes al profe
 } Node;

// Copia el tauler del pare al fill
void copiarTauler(char desti[N][N],char origen[N][N]){
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            desti[i][j]= origen[i][j]
        }
    }
}

// Compta quantes columnes NO estan plenes
int calcularNumeroDeFills(char tauler[N][N]){
//mirar nomes la fila d'adalt de tot
    int aux=0;
    for (int j = 0; j < N; j++) {
        if (tauler[0][j] == ' ') aux++;//si la columna no està plena
    }
    return aux;
}

// Aquesta funció rep el tauler i el número de fill, i retorna l'índex real de la columna a la matriu
int numFillAColumna(char tauler[N][N],int numfill){
    int aux=0;
    for(int j=0;j<N;j++){
        // Si la columna té espai (no està plena)
        if (tauler[0][j] == ' ') {
            if (aux == numFill) {
                return j; // Retorna la columna real de la matriu
            }
            aux++;
        }
    }

    return -1; // En cas d'error (no hauria d'arribar mai aquí si numFill és vàlid)
}





// Fa caure una fitxa per gravetat a la columna corresponent
void Tirada(char tauler[N][N],int numfill,char fitxa){
    //1.Transformar el numfill a la columna matriu
    int col = numFillAColumna(tauler,numfill);
    if(col==-1){return;}
    //2.Calcular la fila on cau per gravetat
    for(int i=N-1;i>=0;i--){
        //3.Posar la fitxa a la fila/columna
        if(tauler[i][col]==' '){
            tauler[i][col]=fitxa;
            return;
        }
    }
}
//mira entremitg si algu ja a guanyat


Node *creaNode(Node*pare,int numfill,int Nivell) {//crea 1 node
    Node *p=malloc(sizeof(Node));

    //1.copiar el tauler
    copiarTauler(p->tauler,pare->tauler);//(desti,origen);

    // Determinem la fitxa segons el nivell (Nivell 1 = 'X'(IA), Nivell 2 = 'O'(Jugador))
    char fitxa = (Nivell == 1) ? 'X' : 'O';

    //2.Tirar la fitxa a la columna que toqui
    Tirada(p->tauler,numfill,fitxa);

    //3.Crear l'array de fills si n'ha de tenir
    if(Nivell<2){
        p->n_fills=calcularNumeroDeFills(p->tauler);
        p->fills=malloc(p->n_fills*sizeof(Node *));
    }else{
        p->n_fills=0;
        p->fills=NULL;
    }
    return p;
}

void crearFills(Node *pare,int Nivell){
    for (int i=0;i<pare->n_fills;i++){
        pare->fills[i]=crearNode(pare,i,Nivell);
    }
}

//crea 2 nivells d‘arbre, a partir d’un node
void creaArbre(Node *arrel){
    creaFills(arrel,1);//creem el 1er nivell de l'arbre
    for(int i=0;i<arrel->n_fills;i++){
        creaFills(arrel->fills[i],2); //creem el 2n nivell de l'arbre
    }
}


// Funció indispensable per evitar fugues de memòria (Memory Leaks)
//La funció utilitza la recursivitat per esborrar l'arbre de baix cap amunt (de les fulles cap a l'arrel):
void alliberarArbre(Node *node) {
    // 1. Cas base: Si el node no existeix, no fem res
    if (node == NULL) return;

    // 2. Primer alliberem TOTS els fills d'aquest node
    for (int i = 0; i < node->n_fills; i++) {
        alliberarArbre(node->fills[i]);// Crida recursiva fins arribar a les fulles
    }
    // 3. Un cop els fills estan alliberats, alliberem l'array de punters del node
    if (node->fills != NULL) {
        free(node->fills);
    }
    // 4. Finalment, alliberem el propi node
    free(node);
}

void inicialitzarTauler(char tauler[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tauler[i][j] = ' ';
        }
    }
}

void main(){
    Node arrel;
    // Inicialitzem el tauler de l'arrel buit
    inicialitzarTauler(arrel.tauler);

    arrel.n_fills=N;
    arrel.fills=malloc(N*sizeof(Node *));
    crearArbre(&arrel);

    printf("Arbre creat correctament amb %d fills a l'arrel.\n", arrel.n_fills);

    // Alliberem la memòria reservada dinàmicament
    for (int i = 0; i < arrel.n_fills; i++) {
        alliberarArbre(arrel.fills[i]);
    }
    free(arrel.fills);


}
