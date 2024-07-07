#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 512

typedef struct stazione {
    int dist;
    int parcoVeicoli[MAX];
    int numCar;
    struct stazione* left;
    struct stazione* right;
} stazione;

typedef stazione* autostrada;

typedef struct nodoVisita {
    int dist;
    int parent;
    int tappa;
    int maxDist;
} nodoVisita;

stazione* creaStazione(int dist){
    stazione* nuovaStazione = (stazione*)malloc(sizeof(stazione));
    nuovaStazione->dist=dist;
    nuovaStazione->numCar=0;
    nuovaStazione->left=NULL;
    nuovaStazione->right=NULL;
    return nuovaStazione;
}


void maxHeapify(int* heap, int n, int i) // 'heap' è l'array che rappresenta l'heap, n = dimensione heap, i = posizione nodo da correggere
{ 
    int riordina = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (right < n && heap[right] > heap[riordina]){
        riordina = right;
    }
    if (left < n && heap[left] > heap[riordina]){
        riordina = left;
    }
    if (riordina != i) {
        int temp = heap[i];
        heap[i] = heap[riordina];
        heap[riordina] = temp;

        maxHeapify(heap, n, riordina); // Chiamata ricorsiva di maxHeapify sul sottoalbero
    }
}

void inserisciCar(stazione* S,int autonomia)
{   
    if(S==NULL){
        return;
    }
    int i;
    i=S->numCar;
    S->parcoVeicoli[i]= autonomia;
    S->numCar++;
    //Correggo il MaxHeap:
    while (i > 0 && S->parcoVeicoli[i] > S->parcoVeicoli[(i - 1) / 2]) {
        int temp = S->parcoVeicoli[i];
        S->parcoVeicoli[i] = S->parcoVeicoli[(i - 1) / 2];
        S->parcoVeicoli[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}
 
void rimuoviCar(stazione* S, int autonomia)
{
    int cerca = -1; // Indice della posizione del veicolo da rimuovere
    int i=0;
    // Trova la posizione del veicolo con l'autonomia specificata
    
    while(i < S->numCar && cerca== -1){
        
        if (S->parcoVeicoli[i] == autonomia) {
            cerca = i;
        }
        i++;
    }
    if (cerca == -1) {
        printf("non rottamata\n");
        return;
    }
    // Rimuovi il veicolo trovato e correggi il MaxHeap
    S->numCar--;
    S->parcoVeicoli[cerca] = S->parcoVeicoli[S->numCar];
    maxHeapify(S->parcoVeicoli, S->numCar, cerca);
    printf("rottamata\n");
    
}

stazione* inserisciStazione(autostrada bst, int key, int* inserita)
    {
        stazione* corrente = bst; // puntatore per attraversare il BST
        stazione* parent = NULL; // mantengo un puntatore al genitore
 
        while (corrente != NULL && corrente->dist!=key) 
        {
            parent = corrente;
            if (key < corrente->dist)
                corrente = corrente->left;
            else
                corrente = corrente->right;
        }
        if(corrente==NULL)
        { //sono arrivato con corrente su NULL: creo la nuovaStazione e la attacco al precedente:
            stazione* nuovaStazione= creaStazione(key); // creo la Stazione da inserire
        
            if (key < parent->dist){
                parent->left = nuovaStazione;
                printf("aggiunta\n");
                *inserita=1;
                return parent->left; // ritorno il puntatore alla stazione inserita
            }
            // altrimenti assegno il nuovo nodo come figlio destro
            if (key > parent->dist){
                parent->right = nuovaStazione;
                printf("aggiunta\n");
                *inserita=1;
                return parent->right;
            }
            // Ritorno il puntatore alla nuovaStazione inserita
            return parent;
        }
        
        // Finisco qui nel caso in cui abbia corrente->dist==distanza (cioè la stazione è già presente)
        printf("non aggiunta\n");
        return NULL;
          
    }
 
// Funzione per cercare una stazione nel BST
stazione* cercaStazione(autostrada cur, int distanza) 
{
    if (cur == NULL) {
        return NULL;
    }

    if (distanza < cur->dist) {
        return cercaStazione(cur->left, distanza);
    } else if (distanza > cur->dist) {
        return cercaStazione(cur->right, distanza);
    } else {
        return cur;
    }
}

// Funzione per trovare il nodo con la distanza minima nel BST destro (cioè il SUCCESSORE di un nodo)
stazione* cercaSucc(autostrada A) {
    while (A->left != NULL) {
        A = A->left;
    }
    return A;
}

// Funzione per demolire una stazione dal BST
stazione* demolisciStazione(autostrada A, int daEliminare)
{
    stazione* parent = NULL;
    stazione* current= A;
    while (current != NULL && current->dist != daEliminare) {
        parent = current;
        if (daEliminare < current->dist) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (current == NULL) {
        printf("non demolita\n");
        return NULL; // Il nodo da eliminare non è presente nel BST
    }
    // Caso 1: Il nodo da eliminare ha 0 o 1 figlio
    if (current->left == NULL) {
        stazione* temp = current->right;
        free(current);
        if (parent == NULL) {
            return temp; // Nodo radice eliminato
        }
        if (parent->left == current) {
            parent->left = temp;
        } else {
            parent->right = temp;
        }
    } else if (current->right == NULL) {
        stazione* temp = current->left;
        free(current);
        if (parent == NULL) {
            return temp; // Nodo radice eliminato
        }
        if (parent->left == current) {
            parent->left = temp;
        } else {
            parent->right = temp;
        }
    } else {
        // Caso 2: Il nodo da eliminare ha 2 figli
        stazione* successore = cercaSucc(current->right);
        current->dist = successore->dist; //sposto il successore nella stazione da eliminare ed elimino 
        for(int i=0; i<successore->numCar; i++){
            current->parcoVeicoli[i]=successore->parcoVeicoli[i];
        }
        current->numCar=successore->numCar;
        current->right = demolisciStazione(current->right, successore->dist); 
    }

    return A;
}



void aggiungiAuto(int distanza,int autono, autostrada A)
{
    if(A==NULL){
        printf("non aggiunta\n");
        return;
    }
    stazione* cur = cercaStazione(A, distanza);
    if(cur==NULL){
        printf("non aggiunta\n");
        return;
    } 
    if (cur->dist == distanza){
        inserisciCar(cur, autono); 
        printf("aggiunta\n");
    }
}
 
void rottamaAuto(int distanza,int autono, autostrada A)
{
    stazione* cur= A;
    cur = cercaStazione(cur, distanza);
    if(cur==NULL){
        printf("non rottamata\n");
        return;
    } 
    if (cur->dist == distanza){
        rimuoviCar(cur, autono);
    }
}

// Visita in Ordine del BST per ricavare l'Array ORDINATO di nodiLista necessario per effettuare la Visita
void creaArrayVisita(autostrada A,struct nodoVisita* arrayDaRiempire, int partenzaDist, int arrivoDist, int* indice)
{   
    if (A == NULL) {
        return;
    }

    // Visita il sottoalbero sinistro
    creaArrayVisita(A->left, arrayDaRiempire, partenzaDist, arrivoDist, indice);

    if(A->dist>=partenzaDist && A->dist<=arrivoDist){ //NON aggiungo proprio le stazioni che NON mi servono per la visita (0 auto OR autonomiaMax=0)
        // Inserisci il valore del nodo corrente nell'array
        
        (arrayDaRiempire[(*indice)]).dist = A->dist;
        (arrayDaRiempire[(*indice)]).parent = -1;  // Valore iniziale per parent
        if(A->numCar==0){ //caso particolare: 0 auto (assumo autonomia=0)
            (arrayDaRiempire[(*indice)]).maxDist = A->dist;
        } else {
            (arrayDaRiempire[(*indice)]).maxDist =A->dist + A->parcoVeicoli[0] ; // Valore iniziale per maxDist
        }
        (*indice)++;
    }
    if(A->dist==arrivoDist){
        return;
    }

    // Visita il sottoalbero destro
    creaArrayVisita(A->right, arrayDaRiempire, partenzaDist, arrivoDist, indice);
}

//in PianificaPercorso chiamo la funzione che riempie l'array su cui fare poi la visita
//ALLA FINE DEALLOCA L'ARRAY VISITA!
void pianificaPercorso(autostrada A, int partenzaDist, int arrivoDist, int vertici)
{
    int i=0;
    struct nodoVisita* arrayVisita= (nodoVisita*)malloc(vertici * sizeof(nodoVisita)); // alloco array di elementi 'struct nodoVisita'
    creaArrayVisita(A, arrayVisita, partenzaDist, arrivoDist, &i); // ritorna l'array di nodiVisita ORDINATO
    int arrivato=0;
    
    //VISITA
    struct nodoVisita* currentNode= &(arrayVisita[0]); // scorro le celle dall'inizio dell'array
    struct nodoVisita* confrontoNode= &(arrayVisita[1]); // considero i nodi 'adiacenti' a currentNode
    currentNode->tappa=1;
    confrontoNode->tappa=2;
    //CASI PARTICOLARI:
    if(currentNode->maxDist==currentNode->dist||confrontoNode->dist>currentNode->maxDist){ // cioè se autonomia più alta è =0 oppure non ha neanche un adiacente
        printf("nessun percorso\n");
        free(arrayVisita);
        return;
    }
    
    while(currentNode->dist<arrivoDist && currentNode->dist<confrontoNode->dist && arrivato==0) //se CurrentNode raggiunge ARRIVO, questo non è stato trovato tra gli adiacenti di nessun altro nodo: nessun percorso 
    {
        
        if(currentNode->maxDist!=currentNode->dist){ //cioè se quella stazione ha autonomia>0, altrimenti vado al nodo successivo (quella non ha adiacenti da valutare)
            while(confrontoNode->dist <= currentNode->maxDist && confrontoNode->dist<arrivoDist&&arrivato==0){  
            
            confrontoNode->parent=currentNode->dist;
            confrontoNode->tappa= (currentNode->tappa)+1;
            
            confrontoNode++; 
            }
        }
        if(confrontoNode->dist==arrivoDist){ //se il nodo 'confronto' è = ARRIVO, esiste un percorso
                arrivato=1;
            while(arrivoDist>currentNode->maxDist){
                currentNode++; //così arrivo correttamente sul PRIMO nodo Parent di Arrivo
            }
        } 
        //così se trovo ARRIVATO, current mi rimane sul PARENT di Arrivato
        if(arrivato==0){
            currentNode++;
        }

    }
    if(arrivato==1){
        //Alloco array INVERSO per stampa e STAMPO
        //'tappa' conta la lunghezza del percorso 
        int* stampa= (int*)malloc((currentNode->tappa)*sizeof(int)); //controlla il j+1
        int p=0;
        stampa[p]= arrivoDist;
        p++;
        stampa[p]= currentNode->dist; //current, uscito dal while, punta al Parent di Arrivo
        p++;
        int cerca=(currentNode)->parent; //primo valore da cercare: il parent del parent di Arrivo, cioè di currentNode
       
        currentNode--;
    
        while(cerca!=partenzaDist)
        {
            while((currentNode)->dist!=cerca){
                currentNode--;
            }
            stampa[p]=(currentNode)->dist;
            p++;
            cerca=(currentNode)->parent; //ora CURR punta al primo adiacente di Partenza
        }
        p--;
        printf("%d ", partenzaDist);
        int f;
        for(f=p; f>0; f--){ //arrivo fino al valore dopo ArrivoDist (lo stampo dopo a parte:)
            printf("%d ", stampa[f]);
        }
        printf("%d\n", arrivoDist);
        free(stampa);
    } else{
        printf("nessun percorso\n");
    }
    free(arrayVisita); 
}


void creaArrayVisitaInverso(autostrada A, struct nodoVisita* arrayDaRiempire, int partenzaDist, int arrivoDist, int* indice) 
{
    if (A == NULL) {
        return;
    }
    // Visita il sottoalbero destro
    creaArrayVisitaInverso(A->right, arrayDaRiempire, partenzaDist, arrivoDist, indice);

    if (A->dist <= partenzaDist && A->dist >= arrivoDist){ 
        // Inserisci il valore del nodo corrente nell'array
        (arrayDaRiempire[(*indice)]).dist = A->dist;
        (arrayDaRiempire[(*indice)]).tappa = -1;
        (arrayDaRiempire[(*indice)]).parent = -1;  // Valore iniziale per parent
        if(A->numCar==0){ //caso particolare: 0 auto (assumo autonomia=0)
            (arrayDaRiempire[(*indice)]).maxDist = A->dist;
        } else {
            if(A->parcoVeicoli[0] > A->dist ) //verrebbe MaxDist negativo: settandolo a 0 assumo che arriva fino alla radice)
            {
                (arrayDaRiempire[(*indice)]).maxDist =0;
            } else {
                (arrayDaRiempire[(*indice)]).maxDist =A->dist - A->parcoVeicoli[0] ; 
            }
        }
        (*indice)++;
    
        if (A->dist == arrivoDist) {
            return;
        }
    }
    // Visita il sottoalbero sinistro
    creaArrayVisitaInverso(A->left, arrayDaRiempire, partenzaDist, arrivoDist, indice);
}


 void pianificaPercorsoInverso(autostrada A, int partenzaDist, int arrivoDist, int vertici)
 {
    int i=0;
    struct nodoVisita* arrayVisita= (nodoVisita*)malloc(vertici * sizeof(nodoVisita)); // alloco array di elementi struct nodoVisita
    creaArrayVisitaInverso(A, arrayVisita, partenzaDist, arrivoDist, &i); //mi ritorna l'array di nodiVisita ORDINATO
    int arrivato=0;
    
    //VISITA
    struct nodoVisita* currentNode= &(arrayVisita[0]); 
    struct nodoVisita* confrontoNode= &(arrayVisita[1]);
    currentNode->tappa=1;
    confrontoNode->tappa=2;
    //CASI PARTICOLARI (auto con Auton=0 OR 0 auto OR 0 stazioni raggiungibili):
    if(currentNode->maxDist==currentNode->dist||confrontoNode->dist<currentNode->maxDist){ //cioè autonomia + alta è =0 oppure non ha neanche 1 adiacente
        printf("nessun percorso\n");
        free(arrayVisita);
        return;
    }
    
    struct nodoVisita* Parent=NULL;
    
    while(currentNode->dist>arrivoDist && currentNode->dist>confrontoNode->dist && arrivato==0) //se currentNode arriva su ARRIVO, non c'è percorso (non ho trovato arrivo tra gli ADIACENTI di un'altra stazione)
    {
        if(currentNode->maxDist!=currentNode->dist){ //cioè se quella stazione ha autonomia>0, altrimenti vado al nodo successivo (quella non ha adiacenti da valutare)
            while(confrontoNode->dist >= currentNode->maxDist && confrontoNode->dist>arrivoDist && arrivato==0){ //TOLGO confrontoNode!=NULL &&//mi sa che non serve && confrontoNode->dist<=arrivoDist (per forza perchè l'array arriva solo fino a quel valore)
                if(confrontoNode->parent==-1 || confrontoNode->tappa == (currentNode->tappa)+1){
                    confrontoNode->parent=currentNode->dist;
                    confrontoNode->tappa= (currentNode->tappa)+1;
                }
                confrontoNode++; 
            }
            if(confrontoNode->dist==arrivoDist){ //se il nodo 'confronto' è= ARRIVO, esiste un percorso
                arrivato=1;
                while(arrivoDist<currentNode->maxDist){
                currentNode++; //così arrivo correttamente sul PRIMO nodo Parent di Arrivo
            }   
                Parent=currentNode;
                int temp=currentNode->tappa; //salvo la tappa del 1° valore con cui posso raggiungere arrivo
                currentNode++;
                
                while(currentNode->tappa==temp && currentNode->dist > arrivoDist){ //finchè i successivi hanno la stessa tappa, se possono raggiungere loro ARRIVO, allora devo prediligerli (hanno valore inferiore) 
                    if(arrivoDist >= currentNode->maxDist)
                    {
                        Parent= currentNode;
                    }
                    currentNode++;
                }
            }
        }
        if(arrivato==0){ //se trovo ARRIVO mi serve mantenere CURR e CONFRONTO dove si sono fermati
            currentNode++;
            if(currentNode->tappa==-1){ //se arrivo su un nodo che non è figlio di nessuno, non ci sono percorsi che possono portarmi ad ARRIVO
                break;
            }
            confrontoNode=currentNode+1;
            
        }
    } 
    if(arrivato==1){
        //Alloco array INVERSO per stampa e STAMPO
        //'tappa' conta quanto è lungo il percorso
        int* stampa= (int*)malloc((Parent->tappa)*sizeof(int)); 
        int p=0;
        stampa[p]= arrivoDist;
        p++;
        stampa[p]= Parent->dist; //current, uscito dal while, punta al Parent di Arrivo
        p++;
        int cerca=(Parent)->parent; //primo valore da cercare: il Parent del Parent di Arrivo
        Parent--;
    
        while(cerca!=partenzaDist)
        {
            while((Parent)->dist!=cerca){
                Parent--;
            }
            stampa[p]=(Parent)->dist;
            p++;
            cerca=(Parent)->parent; //ora CURR punta al primo adiacente di Partenza
        }
        p--;
        printf("%d ", partenzaDist);
        int f;
        for(f=p; f>0; f--){ //arrivo fino al valore dopo ArrivoDist (lo stampo dopo a parte:)
            printf("%d ", stampa[f]);
        }
        printf("%d\n", arrivoDist);
        free(stampa);
    } else{
        printf("nessun percorso\n");
    }
    //FREE:
    free(arrayVisita); 
}

int main () {

    
    char istruzione[50]; // memorizza la prima parola che leggo sulla riga
    stazione* staz=NULL;
    int numeroAuto;
    int distanza;
    int autono;
    int i=0;
    int v=0;
    int Partenza;
    int Arrivo;
    autostrada A = NULL;
    int aggiunta=0;
 
    while(scanf("%s", istruzione)!= EOF) {
        if (strcmp (istruzione, "aggiungi-stazione") == 0 ) {
            if ((scanf("%d",&distanza))==1 && (scanf("%d", &numeroAuto))== 1){
               aggiunta=0;
               if(A==NULL){ //se BST è ancora vuoto creo il primo nodo (radice) e faccio puntare A a questo
                    staz=creaStazione(distanza);
                    A=staz;
                    printf("aggiunta\n");
                    aggiunta=1;
               } else{ //altrimenti mando in ingresso il BST finora formato per aggiungervi una nuovaStazione
                staz=inserisciStazione(A, distanza, &aggiunta);
                
               }
               if(aggiunta==1)
               {    
                    v++;
                    if(numeroAuto>0){
                    for(i=0;i<numeroAuto;i++){
                        if(scanf("%d", &autono)==1){
                        inserisciCar(staz,autono);
                    }
                    }   
                }
                staz->numCar=numeroAuto;
               }
                
            }  
        }
        if (strcmp (istruzione, "demolisci-stazione") == 0 ) {
            if(scanf("%d", &distanza)==1){
                if(A->dist==distanza){ //se la stazione da demolire è la radice del BST, allora salverò il puntatore alla nuova radice del BST in A
                    staz =demolisciStazione(A,distanza); //in 'demolisci-stazione' DEALLOCA il nodo
                    A=staz;
                    printf("demolita\n");
                    v--;
                   
               } else{ //altrimenti mando in ingresso il BST finora formato per demolire una stazione (ma il valore di ritorno non sarà il puntatore alla radice del mio BST)
                staz=demolisciStazione(A,distanza); //A staz viene restituiita la radice del sottoalbero in cui è stata demolita
                if(staz!=NULL){//ha demolito correttamente la stazione 
                    printf("demolita\n");
                    v--;
                }
            }
            
        }
        }
        if (strcmp (istruzione, "aggiungi-auto") == 0 ) {
            if((scanf( "%d", &distanza))==1 && (scanf( "%d", &autono))==1){
                if(autono>0){ //quella con autonomia 0 non mi serve per il pianifica percorso (serve anche un controllo nel pianifica percorso)
                aggiungiAuto (distanza,autono, A);
                } 
            }
        }
 
        if (strcmp (istruzione, "rottama-auto") == 0 ) {
           if((scanf("%d", &distanza))==1 && (scanf("%d", &autono))==1){
            rottamaAuto (distanza,autono, A);
            }
        }
       
        if (strcmp (istruzione, "pianifica-percorso") == 0 ) {
            
           
            if((scanf("%d", &Partenza))==1 && (scanf("%d", &Arrivo))==1){
           
                if (Partenza == Arrivo)
                {
                    printf("%d\n",Partenza);

                }
                if (Partenza<Arrivo)
                {
                    pianificaPercorso (A, Partenza, Arrivo, v);
                }
                if (Partenza>Arrivo) 
                {
                    pianificaPercorsoInverso (A, Partenza, Arrivo, v);
                }
            }
        }
 

    }
        return 0;
}