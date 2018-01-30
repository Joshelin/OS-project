#include "../PCB.h"

// Inserisce il PCB puntato da p come figlio del PCB puntato da parent.
void insertChild(pcb_t *parent, pcb_t *p);

/* Rimuove il primo figlio del PCB puntato da p.
Se p non ha figli, restituisce NULL.*/
pcb_t* removeChild(pcb_t *p);

/* Rimuove il PCB puntato da p dalla lista dei figli del padre.
Se il PCB puntato da p non ha un padre, restituisce NULL.
Altrimenti restituisce l’elemento rimosso (cioé p).
A differenza della removeChild, p può trovarsi in una posizione arbitraria (ossia non è necessariamente il primo figlio del padre).*/
pcb_t* outChild(pcb_t* p);
