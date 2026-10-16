# 4 en ratlla
[Practica minimax.pdf](practica_minimax.pdf)  
[slides](slides_4enratlla.pdf)

## 0.Entrega
**Data d’entrega:** Fins el 30 de Novembre (inclòs). Es valorarà si s’entrega abans. Entregar-la consisteix en enviar-la per e-mail (fitxers .c i .h, no executables), i serà la data considerada d’entrega.  
Per a validar la nota de la pràctica, és obligatori fer una entrevista personal, que serà a posteriori de 
l’entrega.  
**Entrevista personal:** l’entrevista es farà presencialment.  
**Dades del professor:** Vicenç Soler. email: Vicenc.Soler@uab.cat.  
Despatx QC-2032, 2ª planta, Escola d’Enginyeria. Per a consultes sobre la pràctica (o de l’assignatura), m’ho podeu consultar en horari de classe o enviant-me email (o per Teams). 

## 1.Algorisme del MiniMax 
L’algorisme del minimax és un mètode per a la selecció de la millor elecció d’acció en situació, o joc, on dos jugadors treballen oposadament per a aconseguir objectius diferents, actuant sobre el mateix conjunt d’informació. S’aplica específicament en la cerca d’arbres de jocs per a determinar el millor moviment pel jugador actual del joc (el que li toca moure). Fa servir el simple principi que a cada moviment, el jugador que mou escollirà el millor moviment disponible.  

Per a implementar el MiniMax es crea un arbre que crea totes les possibles tirades de joc. Tot arbre comença amb un node arrel (a cada element de l’arbre li’n direm node) que conté la informació de l’estat de la partida actual (bàsicament el tauler actual de la partida). Aquest node arrel tindrà ‘n’ fills que penjaran d’ell (seran el 1er nivell de l’arbre), corresponents a les ‘n’ possibles tirades del jugador que li toqui tirar (l’ordinador). El node arrel el considerarem a nivell 0 de l’arbre.  

Una vegada s’ha creat el 1er nivell de nodes de l’arbre (fills de l’arrel), crearem un segon nivell per a cadascun dels nodes del nivell 1. És a dir, per a cada node que havíem creat del nivell 1, hem de crear, de nou, una sèrie de nodes que pengin d’ell, corresponents a les possibles tirades de l’altre jugador (jugador humà, en aquest cas), a partir del nou estat de la partida produït per la tirada del jugador que avaluem. És a dir, a partir del node arrel, el primer fill correspon a la 1ª tirada possible del jugador que avaluem (ordinador) i, a la seva vegada, els fills d‘aquest primer node corresponen a les possibles tirades de l’altre jugador (humà) després que el primer jugador ha tirat a la 1ª columna possible.  

Així doncs, s’anirà creant l’arbre fins a una profunditat que fixarem abans de començar. Quan arribem a aquesta profunditat, aturarem aquesta generació de fills.

<img scr=https://github.com/01martam/4-en-ratlla/blob/ed0cfcb65291addd7868b8e5f678408732a706c8/Arbre.jpg\>

Els **nodes fulla** són els nodes que no tenen fills, on hem acabat la generació de l’arbre en cada branca. 
Per a cada node fulla se li calcula com està el tauler amb respecte al jugador ordinador (a qui li toca tirar ara, ja que generem l’arbre quan li toca a l’ordinador per a decidir quina firada farà). Avaluant eltauler en les fulles d’aquest arbre, avaluem l’estat projectat del joc després de la seqüència de moviments feta pels jugadors. Quant més profund es generi l’arbre del joc, més informació tenim sobre els possibles estats futurs de la partida i podem produir un millor moviment.  

Per poder fer l’avaluació de l’estat a cada node fulla es farà servir una funció que, rebent l’estat de la 
partida en aquell node, retorni un valor. La funció que assigna un valor a cada situació del tauler s’anomena funció heurística. La funció heurística donarà un valor alt si l’estat del tauler és favorable per a l’ordinador i baix si desfavorable. Quant més petit sigui el valor, pitjor per a l’ordinador és la situació; i quant més gran, millor per a l’ordinador.

L’algorisme del Minimax es basa en la creació de l’abre abans comentat i, a partir de l’estat de la partida actual, ens ha de retornar la tirada que proposa l’algorisme. En el cas de 4 en ratlla, ens dirà a quina columna del tauler posarà la fitxa l’ordinador.

### 1.1.Funcionament
Passos de l’Algorisme MiniMax:
1. Crear l’arbre basat en els nivells de profunditat que vulguis.
2. Calcular el valor dels nodes fulla fent servir la funció heurística
3. Pujar aquests valors als nodes superiors de l’arbre calculant mínims i màxims.
4. Trobar el node de Nivell 1 que té el màxim valor. Aquest node definirà la tirada que ha decidit la màquina.
5. Destruir l’arbre creat alliberant la memòria reservada.
   
Amb més detall:
1. Decideix els nivells de profunditat de l’arbre (usualment 2, 4 o 6 ).
2. Crea l’Arbre.
2.1. Crea el node arrel
2.2. Anar creant l’arbre nivell a nivell:
2.2.1.A partir d’un node pare, es crearan els fills. 
2.2.2.A cada node nou: creem el node (malloc), copiem el tauler del pare i tirem la fitxa a la columna que toqui. Si el node no és fulla, calculem quants fills tindrà i creem l’array de nodes fills.
3. Avalua a través d’una funció heurística l’estat dels taulers del nivell de sota del tot (nodes fulla). Aquesta funció retornarà un valor que s’assignarà al node.
3.1. Si l’ordinador guanya, el valor del node ha de ser molt gran (aquesta opció ha de ser l’escollida).
3.2. Si el jugador humà guanya, posa-hi un valor molt baix (aquesta no ha de ser escollida).
4. Després, ves pujant per l’arbre i posant valor als pares seguint l’algorisme MiniMax:
4.1. Assigna el valor mínim dels fills quan el pare està en un nivell senar.
4.2. Assigna el valor màxim dels fills quan el pare està en un nivell parell.
5. Ves aplicant valors cap a amunt seguint la regla 4 fins que el node Arrel tingui valor assignat.
6. La columna on tirar és la columna corresponent al fill amb valor màxim (el valor que té l’arrel).
7. Finalment, destrueix l’arbre (fent anar la funció ‘free’) per alliberar la memòria reservada amb els malloc’s del punt 2. Per alliberar-la completament has de fer tants free’s com malloc’s hagis fet.

## 2.Implementació 
La implementació s’haurà de fer mitjançant la creació de l’arbre del MiniMax, tal i com es descriu més a baix. Cada node de l’arbre serà del tipus Node descrit a continuació: 
```
#define N 8 
typedef struct node { 
struct node **fills; //array dinàmic de pointers a Node
int n_fills; //tamany de l’array dinàmic
char tauler[N][N]; 
double valor; //el valor del node (F. Heurística)
 } Node;
```
Per poder implementar l’algorisme amb una mica més de facilitat, ho farem sobre un joc en concret: el 4 en ratlla. Per a aquest joc farem servir un tauler NxN, on N és una constant definida pel 
programador.  

Per què fills és un array dinàmic? L’algorisme del MiniMax es basa en desenvolupar totes les possibles tirades de l’adversari donat un estat de la partida donat. Però no sempre poden ser les mateixes tirades. Si tenim un tauler NxN, en aquest joc només es poden produir, com a màxim, N tirades possibles. Però n’hi pot haver menys, ja que és possible que una columna sigui plena. En aquest cas, el node en qüestió només podrà tenir N-1 fills. Per tant, la funció que ha de decidir quants fills tindrà un node, haurà de mirar primer al taules quants fills pot tenir i actuar en conseqüència. 

Es donen implementades les següents funcions:
- Funcions per crear un arbre:
  Node *creaNode(int num_fills) //crea 1 node
  void crea1Nivell(Node *) //crea els fills d’1 node
  void creaArbre(Node *) //crea 2 nivells d‘arbre, a partir d’un node 
- Funcions per veure el contingut d’un arbre:
  void recorreArbre(Node *) //Es mostrarà la versió recursiva. Podeu fer-la servir per entendre la recursivitat en aquesta pràctica. Aquesta funció recorre tot l’arbre (és una funció exemple de com recórrer tot l’arbre que no té cap incidència en la pràctica final: no cal presentar-la).

Falten per fer altres funcions, com les que permeten esborrar l’arbre i fer anar el minimax: 
- Funcions que permeten esborrar. Es proposen aquestes 3 funcions simplement per seguir la mateixa estructura que la creació de l’arbre, però es pot reduir la seva quantitat si es creu necessari (hi ha llibertat en aquest sentit):
  void esborraNode(Node *) //esborra 1 node de l’arbre
  void esborra1Nivell(Node *) //esborra tots els fills d’1 node, però no aquest
  void esborraArbre(Node *) //esborra tots els nodes de l’arbre, passant-li l’arrel
- Funció (recursiva) per decidir a quina columna tirar:
  int ferMinimax(Node *) //retorna la columna escollida per a la millor jugada 
- moltes altres... (copiar el tauler als nodes fills aplicant el moviment desitjat, etc.)

El que hem implementat amb aquestes funcions és l’algorisme del MiniMax. És a dir, hem intentat implementar un algorisme sense tenir en compte el joc al qual l’aplicarem. Però això no ha estat del tot possible, ja que depenem del joc en molts aspectes, com ara decidir quants fills tindrà un node, etc.  
És per això que, basant-nos en el que hem fet fins ara (dividir les tasques en funcions independents), fem el mateix amb les tasques encomanades. Per exemple, la tasca de decidir quants fills ha de tenir un node s’hauria d’implementar en una funció a banda, no només per independitzar la tasca de la resta del codi, sinó també ja que és possible que necessitem cridar-la des de més d’un lloc del codi.  

Això sí, per a cada joc diferent (4-ratlla, dames, etc.) canviem el contingut de la funció però la capçalera no canvia, ja que passant-li un node (o un tauler) en tenim prou. Podria ser de la següent manera: 
`int numFills(Node *) `

Si necessiteu més funcions, ho heu de decidir vosaltres. Heu de decidir les capçaleres també. 

La idea principal en aquesta pràctica és crear funcions per a cada tasca individual. Per tant, creeu-ne tantes com cregueu necessari (teniu llibertat). Així, el codi serà més intel·ligible, escalable (ampliable) i us serà més senzill tant trobar errors com poder modificar alguna funció (arreglar-la, millorar-la, etc.) sense que això afecti la resta de funcions. Això s’aconsegueix no fent anar variables globals i que cada funció definida rebi per paràmetre tot el necessari per poder treballar (com hem fet fins ara a classe i com habitualment trobem a les funcions matemàtiques).

## 3.El joc 
El joc del 4 en ratlla consisteix en un tauler N x N caselles, on cada fitxa es col·loca en una columna i es deixa caure fins que troba una fitxa o el fons. 

<img scr=https://github.com/01martam/4-en-ratlla/blob/ed0cfcb65291addd7868b8e5f678408732a706c8/Joc.jpg\>

Podem guardar a la matriu els valors que vulguem (com ho vam fer al laberint):
- 0=casella buida, 
- 1=casella ocupada per l’ordinador, 
- 2=casella ocupada pel jugador contrari(l’humà). 
O també guardar-hi directament el char que volem mostrar al tauler. Feu-ho com us vingui millor.

La funció que avalua una jugada la podeu implementar pel vostre compte. Aquesta funció s’anomena funció heurística.
 
Per a implementar la funció heurística del MiniMax, un consell és fer servir valors positius per a jugades favorables i negatius per a desfavorables (favorables i desfavorables a l’ordinador, que és qui ha de decidir la tirada creant l’arbre), i és important ressaltar quan es guanya o es perd una partida (valor molt positiu o negatiu, 1e6 i -1e6, per exemple). L’algorisme ha d’escollir la opció de guanyar quan pugui fer-ho i de no perdre quan hi hagi aquesta possibilitat, segons la situació del tauler. 

## 4.Què s’ha de fer i entregar? 
La pràctica consisteix en implementar el joc del 4 en ratlla fent que una persona jugui contra la màquina. Quan sigui el torn de la màquina, aquesta decidirà a quina columna tira implementantl’algorisme del MiniMax. Per a aquesta decisió, a partir d’un node inicial, muntarà l’arbre de n nivells, calcularà la millor jugada, farà la tirada al tauler i després esborrarà l’arbre que havia creat (tot això a cada torn que li toqui tirar a la màquina).

És obligatori que el MiniMax s’implementi amb recursivitat.

S’havia comentat que la funció que calcula el valor d’una jugada s’anomena funció heurística, i li heu de definir la seva capçalera. El valor assignat a cada node fulla (el que hauria de retornar la funció heurística) pot ser un valor aleatori (funcions rand i srand), però molt millor és fer que aquest valor vingui d’un algorisme més “intel·ligent”. Un consell és que, mentre feu la pràctica, feu una funció que us escrigui per pantalla els valors dels nodes fulla; així es veurà si realment funciona o no el MiniMax que va pujant els valors des dels nodes fulla cap a amunt. De fet, aquesta funció ja us servirà a vosaltres mateixos per a futures comprovacions del que fa el vostre programa (la funció recorreNode() que se us proporcionarà, ja fa aquesta tasca).

Heu de tenir molta cura en dividir bé les funcions i no deixar les coses a mitges, és a dir, separeu bé el que és l’algorisme genèric del que és el joc. Crideu a funcions a mesura que les necessiteu i no hi fiqueu codi al mig que molesti. Feu-ho sempre via crida a funcions intentant no fer servir variables globals.

Feu servir constants (sempre que les necessiteu). Per exemple, les dimensions del tauler són NxN o els valors que guardes a la matriu del tauler per saber si una cel·la està buida o hi ha la fitxa d’algun jugador. En aquest cas, tenim l’avantatge de si fem servir sempre la constant N al llarg de tot l’algorisme, només canviant el valor de N, canviem les dimensions del tauler sense tocar cap línia més del programa. També podeu fer servir altres constants com s’ha comentat abans.  

La pràctica s’ha de presentar dividida en dos (o més) fitxers .c, amb els seus corresponents fitxers .h de capçaleres, que podrien tenir el següent nom i contingut: 
```
minimax.h -> estructura del MiniMax i capçalera de les funcions del MiniMax.
minimax.c -> codi de les funcions del MiniMax.
4enratlla.h -> capçalera de les funcions del joc.
4enratlla.c -> funció main i funcions pròpies del joc.
```
Podeu fer servir aquests noms o d’altres i afegir d’altres fitxers si ho creieu necessari.
Podeu implementar tantes millores com cregueu necessàries. Es valorarà qualsevol cosa que implementeu.
**Consells**
Alguns consells per a implementar la pràctica: 
- [ ] Si teniu dificultats en on i com començar la pràctica, inicialment feu un joc del 4 en ratlla on juguin 2 jugadors humans, sense implementar el MiniMax. Fixeu-vos bé si hi ha algun tros de codi duplicat que pugueu ficar en una sola funció cridant-la vàries vegades. S’ha d’intentar evitar el codi duplicat.
- [ ] Una vegada l’aplicació funcioni per a 2 jugadors humans, implementeu una funció que 
decideixi la columna on tirarà l’ordinador en el seu torn. Aquesta funció ha d’implementar el 
MiniMax: a partir d’un node inicial (o directament del tauler actual) ha de retornar un enter
que indica la columna on es tirarà. Cada vegada que li toqui tirar a l’ordinador, prendrà la 
decisió d’on tirar depenent del resultat de l’algorisme Minimax. Per tant, cada vegada s’haurà 
de:
 - [ ] Crear l’arbre del Minimax
 - [ ] Calcular, fent servir l’algorisme de Minimax, quina és la tirada a fer.
 - [ ] Destruir l’arbre creat.
- [ ] Els valors dels nodes fulla han de ser calculats via la funció heurística, que pot donar els valor aleatòriament o d’una altra manera més treballada.
- [ ] Dividir en funcions i fer servir constants quan calguin. No barrejar el codi i separar-ho bé. Intentar fer servir noms de variables i funcions intel·ligibles.
- [ ] Implementar-ho en els 4 fitxers descrits abans. Si voleu implementar més fitxers, cap problema.
- [ ] Fer arbres de profunditat > 2. En el tipus de jugades que es planteja fins ara, l’ordinador, al preveure quin futur tindrà la seva jugada, només veu la propera tirada que farà el jugador humà (si fem arbre de 2 nivells).S’haurà de poder decidir la profunditat de l’arbre, podent preveure més d’una jugada en el futur. La profunditat la podeu implementar com una constant definida, per a què sempre busqui la mateixa profunditat. S’ha d’implementar fent servir recursivitat.
- [ ] Afegeix els paràmetres que necessitis a les funcions definides pel professor. El professor ha donat una pauta de com fer l’arbre, però és possible que algunes funcions requereixin d’informació addicional a passar per paràmetre. Teniu llibertat per a aplicar els canvis que necessiteu.
- [ ] El tauler es guardarà en una matriu que haureu de revisar, per exemple, per saber si algun dels 2 jugadors guanya. Eviteu sortir-vos de la matriu quan la recorreu per evitar errors (per exemple, si la matriu és 8x8 no aneu a consultar què hi ha a la columna 9, ja que no existeix). C us deixarà fer-ho, però no té cap sentit.
- [ ] Tota la memòria que es reserva, s’ha d’alliberar. És a dir, s’han de fer el mateix nombre de 
malloc’s que de free’s, quan es construeix i es destrueix l’arbre.
## 5. Millores
A continuació es proposen una sèrie de millores. No tenen perquè ser només aquestes, i vosaltres en 
podeu proposar d’altres. Totes les millores que s’implementin pujaran nota:
- Implementar una funció heurística que faci que costi guanyar la màquina. Que jugui bé es valorarà bastant bé.
- Es poden fer millores a l’algorisme del MiniMax per poder jugar millor. Suposo que això ho anireu veient quan l’implementeu i el proveu. Consulteu les que se us ocorrin amb el professor si teniu dubtes.
- Altres... Podeu afegir menús, opcions, guardar partides, rècords, etc.
  
Es valorarà qualsevol cosa que implementeu.

