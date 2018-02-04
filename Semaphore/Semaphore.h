

#ifndef SEMAPHORE_H
#define SEMAPHORE_H 

#include "../PCB.h"
#include "../PCBTree/Tree.h"

/* Ad  ogni  semaforo  è  associato  un  descrittore (SEMD)  con  la  struttura  seguente.
s_key è l'indirizzo della variabile intera che contiene il valore del semaforo.
L'indirizzo di s_key serve come identificatore del semaforo.*/
typedef struct semd_t {
	struct semd_t *s_next;
	int *s_key;
	struct pcb_t *s_procQ;
} semd_t;

/* Active Semaphore Hash Table (ASHT)*/

//array di SEMD con dimensione massima MAXSEMD.
semd_t semd_table[MAXSEMD];

/* Testa della lista  dei SEMD  liberi  o  inutilizzati.
I SEMD attivi sono gestiti tramite una Hash Table di dimensione ASHDSIZE*/
semd_t *semdFree_h;

// hash table, (ogni elemento punta alla lista di collisione per il valore di hash corrispondente all’indice)
semd_t *semdhash[ASHDSIZE];

/* Funzioni */

//PRIVATE

//alloca semaforo dalla lista dei semafori liberi
semd_t* allocSemaphore();

//libera il semaforo inserendolo nella lista dei semafori liberi
void freeSemaphore(int *key);

//cerca chiave nelle liste di trabocco se c'è già
semd_t* matchKey(int* key);

//inserisce il pcb nella coda s_procQ del semaforo.
void enqueuePcb(semd_t *semaforo, pcb_t *p);


//PUBLIC

/* Viene inserito il PCB puntato da  p nella coda dei processi  bloccati  associata  al semaforo con chiave key.
Se il semaforo corrispondente non è  presente nella ASHT, alloca un nuovo SEMD dalla lista di quelli liberi e lo inserisce nella  ASHT, settando i campi in maniera opportuna.
Se non è possibile allocare un nuovo SEMD perché la  lista  di quelli liberi e’ vuota, restituisce -1.
In  tutti  gli altri  casi, restituisce 0.*/
int insertBlocked(int *key, pcb_t *p);

/* Restituisce il puntatore al pcb del primo processo bloccato sul semaforo, senza deaccordarlo.
Se il semaforo non esiste restituisce NULL.*/
pcb_t *headBlocked(int *key);

/* Ritorna il primo PCB dalla coda dei processi bloccati (s_ProcQ) associata al SEMD della ASHT con chiave key.
Se tale descrittore non esiste nella ASHT, restituisce NULL.
Altrimenti, restituisce l’elemento  rimosso.
Se la coda dei processi bloccati per il semaforo diventa vuota, rimuove il descrittore corrispondente dalla  ASHT e lo inserisce nella coda dei descrittori liberi (semdFree).*/
pcb_t* removeBlocked(int *key);

// Richiama la funzione fun per ogni processo bloccato sul semaforo identificato da key.
void forallBlocked(int *key, void (*fun)(pcb_t *pcb, void *), void *arg);

// Rimuove il PCB puntato da p dalla coda del semaforo su cui è bloccato.
void outPcbBlocked(pcb_t *p);

/* Rimuove il PCB puntato da p dalla coda del semaforo su cui è bloccato. <--- NEL MAIN VUOLE CHE VENGANO RIMOSSI TUTTI I FIGLI, NIPOTI, ETC... DI P.
(La hash table deve essere aggiornata in modo coerente).*/
void outChildBlocked(pcb_t *p);

/* Inizializza la lista dei semdFree in modo da contenere tutti gli elementi della semdTable.
Questo metodo viene invocato una volta sola durante l’inizializzazione della struttura dati.*/
void initASL();

	#endif