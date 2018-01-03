#include "../PCB.h"

// pcbFree:  lista  dei  PCB  che  sono  liberi o  inutilizzati.
pcb_t *pcbfree_h; //  testa  della  lista pcbFree.  
pcb_t pcbFree_table[MAXPROC]; // array  di PCB  con dimensione MAXPROC.

/* Inizializza la pcbFree in modo da contenere tutti gli elementi della pcbFree_table.
Questo metodo deve essere chiamato una volta sola in fase di inizializzazione della struttura dati.*/
void initPcbs();

// Inserisce il PCB puntato da p nella lista dei PCB liberi (pcbFree)
void freePcb(pcb_t *p);

/* Restituisce NULL se la pcbFree è vuota.
 Altrimenti rimuove un elemento dalla pcbFree, inizializza tutti i campi (NULL/0) e restituisce l’elemento rimosso.*/
pcb_t *allocPcb();