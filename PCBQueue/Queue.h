#include "../PCB.h"

/* Inserisce l’elemento puntato da p nella coda dei processi puntata da head.
L’inserimento deve avvenire tenendo conto della priorità di ciascun pcb (campo  p- ‐>priority).
La coda dei processi deve essere ordinata in base alla priorità dei PCB, in ordine decrescente (i.e. l'elemento di testa è l'elemento con la prioprità più alta).*/
void insertProcQ(pcb_t **head, pcb_t *p);

/* Restituisce l’elemento di testa della coda dei processi da head, SENZA RIMUOVERLO.
Ritorna NULL se la coda non ha elementi.*/
pcb_t* headProcQ(pcb_t *head);

/* Rimuove il primo elemento dalla coda dei processi puntata da head.
Ritorna NULL se la coda è  vuota.
Altrimenti ritorna il  puntatore  all’elemento rimosso dalla lista.*/
pcb_t* removeProcQ(pcb_t **head);

/* Rimuove il PCB puntato da p dalla coda dei processi puntata da head.
Se p non è presente nella coda, restituisce NULL.
(NOTA: p può trovarsi in una posizione arbitraria all’interno della coda).*/
pcb_t* outProcQ(pcb_t **head, pcb_t *p);

// Richiama la funzione fun per ogni elemento della lista puntata da head.
void forallProcQ(pcb_t *head, void (*fun)(pcb_t *pcb, void *), void *arg);