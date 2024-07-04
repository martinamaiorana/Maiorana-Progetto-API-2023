# Prova Finale di Algoritmi e Principi dell'Informatica A.A. 2022-2023

**Docente:** Matteo Pradella

**Valutazione:** 30L/30

---

## Obiettivi e Specifiche

L'obiettivo di questo progetto è stato sviluppare un algoritmo efficiente per risolvere il problema della pianificazione di un percorso tra stazioni di servizio su un'autostrada, utilizzando tecniche di programmazione avanzate studiate durante il corso. Le specifiche del progetto includevano:

Implementazione di un algoritmo con complessità temporale O(n log n).
Utilizzo di strutture dati appropriate per ottimizzare le prestazioni.
Scrittura di test unitari per verificare la correttezza dell'algoritmo.
Documentazione del codice e delle scelte progettuali.

## Scelte Progettuali

### Strutture Dati Utilizzate

Per risolvere il problema della pianificazione del percorso, ho scelto di utilizzare una struttura dati a grafo, dove i nodi rappresentano le stazioni di servizio e gli archi rappresentano i percorsi possibili tra di esse. Ho usato un heap binario per gestire le operazioni di inserimento e estrazione dei nodi, garantendo una complessità temporale efficiente per la ricerca del percorso minimo.

### Algoritmi Implementati

Ho implementato l'algoritmo di Dijkstra per trovare il percorso con il minor numero di tappe tra due stazioni. Questo algoritmo è stato scelto perché è in grado di gestire grafi con pesi non negativi e di trovare il percorso più breve in termini di numero di nodi attraversati. Inoltre, ho adattato l'algoritmo per gestire il caso in cui ci siano più percorsi con lo stesso numero minimo di tappe, scegliendo il percorso che predilige le tappe con distanza più breve dall'inizio dell'autostrada.

### Ottimizzazioni

Per ridurre l'overhead di memoria, ho implementato una versione dell'algoritmo di Dijkstra che utilizza un heap binario, riducendo il tempo di inserimento ed estrazione dei nodi. Ho anche ottimizzato l'algoritmo per terminare non appena viene trovato il percorso ottimale, evitando di esplorare ulteriormente il grafo.

### Test e Verifica

Test forniti: 

---

### Come Eseguire il Progetto

```sh
git clone https://github.com/username/repository.git
cd repository
make
./run_project
