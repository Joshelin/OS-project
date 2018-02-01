#include "Semaphore.h"

int i ;
semd_t *temp ;
bool flag = FALSE;

semd_t* allocSemaphore(){
	if (semdFree_h == NULL)
		return NULL ;
	else if (semdFree_h->s_next == NULL) { // Ultimo pcb libero, la lista pcbfree diventa vuota.
		temp = semdFree_h;
		semdFree_h = NULL;
		return temp;
		}else{ // rimuovo la testa dalla lista dei pcbfree.
			temp = semdFree_h;
			semdFree_h = semdFree_h->s_next ;
			return temp;
		}
	}

	int hash(int* key){
		const int a = 11;
		const int b = 1;
		long longKey = (long)key;
		longKey = ((a*longKey)+b)%MAXSEMD;
		return (int)longKey%ASHDSIZE;
	}

	bool matchKey(int* key){
		if (!flag) {  // inizializzazione.
			if (semdhash[hash(key)]->s_next == NULL){ // Se semdhash[hash(key)] non ha semafori, ovviamente non è già bloccato.
				return FALSE;
			}
			temp = semdhash[hash(key)]->s_next;
			flag = TRUE;
		}
		if (temp->s_key == key){ // se trovo un conflitto.
			flag = FALSE;
			return TRUE;
		}
		else if(temp->s_next == NULL) { // sono arrivato alla coda senza conflitti.
			flag = FALSE;
			return FALSE;
		}
		else{
			temp = temp->s_next;
			return matchKey(key);
		}
	}

	int insertBlocked(int *key, pcb_t *p){
	if(matchKey(key)){
		// Se entriamo qui, il semaforo è già bloccato, inserire valore di ritorno che vuole Davoli. Nelle specifiche non lo dice >:(.
		// return 0? TUTTI GLI ALTRI CASI non vuol dire un cazzo.
	}
	else{
		temp = allocSemaphore() ;
		if (temp == NULL)
			return -1 ;

		if (semdhash[hash(key)]->s_next == NULL){
			semdhash[hash(key)]->s_next = temp ;
			semdhash[hash(key)]->s_next->s_next = NULL ;
			semdhash[hash(key)]->s_next->s_key = key ;
			semdhash[hash(key)]->s_next->s_procQ = p ;
			p->p_semKey = key ;
			return 0 ;
		}
		else {
			temp->s_next = semdhash[hash(key)]->s_next ;
			semdhash[hash(key)]->s_next = temp ;
			semdhash[hash(key)]->s_next->s_key = key ;
			semdhash[hash(key)]->s_next->s_procQ = p ;
			p->p_semKey = key ;
			return 0 ;
		}
	}
}

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

/* Rimuove il PCB puntato da p dalla coda del semaforo su cui è  bloccato.
(La hash table deve essere aggiornata in modo coerente).*/
void outChildBlocked(pcb_t *p);


void initASL(){
	if (i == 0){
		semdFree_h = &semd_table[i++];
		temp = semdFree_h ;
	}
	if (i < MAXSEMD) {  
		temp->s_next = &semd_table[i++];
		temp = temp->s_next;
		initASL();
	}
	if (i == MAXSEMD){
		temp->s_next = NULL;
	}
}
