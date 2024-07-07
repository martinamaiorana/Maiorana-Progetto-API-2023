# Prova Finale di Algoritmi e Principi dell'Informatica A.A. 2022-2023

**Docente:** Matteo Pradella

**Valutazione:** 30L/30

---

## Obiettivi e Specifiche

Qui trovi la [specifica 2022-2023](documents/Specifica_2022_2023.pdf).

L'obiettivo del progetto è pianificare il percorso con il minor numero di tappe tra due stazioni di servizio su un'autostrada, rispettando le seguenti condizioni:

- Ogni stazione ha una distanza unica dall'inizio dell'autostrada.
- Ogni stazione dispone di un massimo di 512 veicoli elettrici con autonomie variabili.
- Il viaggio inizia in una stazione e termina in un'altra, passando per stazioni intermedie senza mai tornare indietro.
- Il conducente noleggia un'auto nuova in ogni stazione visitata e può raggiungere solo stazioni entro l'autonomia del veicolo noleggiato.
- Se esistono più percorsi con lo stesso numero minimo di tappe, si sceglie il percorso con le tappe più vicine all'inizio dell'autostrada.

## Scelte Progettuali

**Funzione Pianifica-Percorso:**
- Effettuo una visita in ordine dell'autostrada (BST), incrementando un contatore 'i'
  finché non arrivo sulla stazione da cui deve partire la visita.
- Creo un nuovo array 'arrayVisita' di dimensione fissa pari al numero di vertici (valore passato in input alla funzione)
  composto da elementi 'struct nodoVisita { int dist; int parent; int maxDist;}'.
- Per ogni stazione che incontro durante la visita del BST a partire dalla stazione di PARTENZA,
  riempio una nuova cella dell'Array 'arrayVisita' associando a un nuovo nodoVisita la dist= dist della stazione
  su cui si trova la visita, parent=-1 e maxDist='dist' della stazione + parcoVeicoli[0] della stazione che sto visitando.
  Così procedo con la visita in ordine, inserendo i nodiVisita finché non trovo una stazione con 'dist' > di ARRIVO (qui mi fermo: le stazioni che voglio prendere in considerazione sono già nell'array)
- Eseguo ora la visita 'in ampiezza' del nuovo array di nodiVisita per trovare il Percorso Minimo, salvarlo e stamparlo.

**Funzione Pianifica-Percorso-Inverso:**
- Effettuo una visita in post-order del BST per ottenere nell'array i nodi riordinati dal più grande (PARTENZA) al più piccolo (ARRIVO).
- Necessario il campo 'TAPPA' in ogni struct nodoVisita per eseguire correttamente la visita in ampiezza delle 'struct nodoVisita'.
- Inizio da PARTENZA. Il confronto deve sempre partire dal primo nodo con il valore di 'TAPPA' diverso dal nodo che sto visitando (CURRENT).
  Se il valore di TAPPA è diverso dal mio, allora sostituisco 'parent' e metto me stesso come parent,
  perchè vengo DOPO rispetto al nodo prima settato come 'parent' e devo qui prediligere i PARENT più vicini all'origine
  (e questa volta i valori sono visitati in ordine DECRESCENTE).


### Test e Verifica

Test forniti: 
[archivio_test_aperti_1.zip.html](documents%2FTests%2Farchivio_test_aperti_1.zip.html)]
[archivio_test_aperti_2.zip.html](documents%2FTests%2Farchivio_test_aperti_2.zip.html)
[open_extra_gen.zip.html](documents%2FTests%2Fopen_extra_gen.zip.html)
---

