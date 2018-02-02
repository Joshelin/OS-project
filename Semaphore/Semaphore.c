#include "Semaphore.h"

int i ;
semd_t *semdTemp ;
pcb_t *pcbTemp;
bool flag = FALSE;

semd_t* allocSemaphore(){
	if (semdFree_h == NULL)
		return NULL ;
	else if (semdFree_h->s_next == NULL) { // Ultimo semaforo libero, la lista semdFree diventa vuota.
		semdTemp = semdFree_h;
		semdFree_h = NULL;
		return semdTemp;
	}
	else{ // rimuovo la testa dalla lista dei semdFree.
		semdTemp = semdFree_h;
		semdFree_h = semdFree_h->s_next ;
		return semdTemp;
	}
}
// NB: freeSemaphore viene invocata SOLO quando è sicuro che il semaforo può esser rimosso.
void freeSemaphore(int *key){
	if(!flag){
		if(semdhash[hash(key)]->s_next->s_key == key){ // se il semaforo da liberare è il primo della lista in 'semdhash[hash(key)]' lo aggiungo in testa a semdFree e lo rimuovo da ASHT
			semdTemp = semdhash[hash(key)]->s_next;
			semdhash[hash(key)]->s_next = semdhash[hash(key)]->s_next->s_next;
			semdTemp->s_next = semdFree_h;
			semdFree_h = semdTemp;
			return;
		}
		semdTemp = semdhash[hash(key)]->s_next;
		flag = TRUE;
	}
// temp è un altro puntatore (locale), necessario per inserire il semaforo in semdFree. semdTemp è usato per scorrere la lista in semdhash[hash(key)];
	if(semdTemp->s_next->s_key == key){ // se ho trovato il semaforo da liberare, lo aggiungo in testa a semdFree e lo rimuovo da ASHT.
		semd_t *temp = semdTemp->s_next; 
		semdTemp->s_next = semdTemp->s_next->s_next;
		temp->s_next = semdFree_h;
		semdFree_h = temp;
		flag = FALSE;
	}
	else{
		semdTemp = semdTemp->s_next;
		freeSemaphore(key);
	}
}

int hash(int* key){ // Calcolo a caso che ritorni un valore tra 0 e ASHDSIZE.
	const int a = 11;
	const int b = 1;
	long longKey = (long)key;
	longKey = ((a*longKey)+b)%MAXSEMD;
	return (int)longKey%ASHDSIZE;
}

semd_t* matchKey(int* key){
	if (!flag) {  // inizializzazione.
		if (semdhash[hash(key)]->s_next == NULL){ // Se semdhash[hash(key)] non ha semafori, ovviamente non è già bloccato.
			return FALSE;
		}
		semdTemp = semdhash[hash(key)]->s_next;
		flag = TRUE;
	}
	if (semdTemp->s_key == key){ // ho trovato il semaforo con chiave key, lo ritorno.
		flag = FALSE;
		return semdTemp;
	}
	else if(semdTemp->s_next == NULL) { // sono arrivato alla coda senza trovare il semaforo con chiave key.
		flag = FALSE;
		return FALSE;
	}
	else{
		semdTemp = semdTemp->s_next;
		return matchKey(key);
	}
}

void enqueuePcb(semd_t *semaforo, pcb_t *p){
	if (!flag){
		pcbTemp = semaforo->s_procQ;
		flag = TRUE;
	}
	if (pcbTemp->p_next == NULL){ //trovato ultimo pcb in coda su questo semaforo.
		pcbTemp->p_next = p;
		p->p_semKey = semaforo->s_key;
	}
	else{
		pcbTemp = pcbTemp->p_next;
		enqueuePcb(semaforo, p);
	}
}

int insertBlocked(int *key, pcb_t *p){
	if(semdTemp = matchKey(key)){ // se eiste già un semaforo con chiave 'key', aggiungo il pcb in coda a tale semaforo.
		enqueuePcb(semdTemp, p);
		return 0;
	}
	else{
		semdTemp = allocSemaphore() ; // il semaforo con 'key' non esiste, procedo a crearlo e aggiungere il suo primo pcb.
		if (semdTemp == NULL)
			return -1 ;

		if (semdhash[hash(key)]->s_next == NULL){
			semdhash[hash(key)]->s_next = semdTemp ;
			semdhash[hash(key)]->s_next->s_next = NULL ;
			semdhash[hash(key)]->s_next->s_key = key ;
			semdhash[hash(key)]->s_next->s_procQ = p ;
			p->p_semKey = key ;
			return 0 ;
		}
		else {
			semdTemp->s_next = semdhash[hash(key)]->s_next ;
			semdhash[hash(key)]->s_next = semdTemp ;
			semdhash[hash(key)]->s_next->s_key = key ;
			semdhash[hash(key)]->s_next->s_procQ = p ;
			p->p_semKey = key ;
			return 0 ;
		}
	}
}

/* Restituisce il puntatore al pcb del primo processo bloccato sul semaforo, senza deaccordarlo.
Se il semaforo non esiste restituisce NULL.*/
pcb_t *headBlocked(int *key){
	if(semdTemp = matchKey(key)){
		return semdTemp->s_procQ;
	}else{
		return NULL;
	}
}

/* Ritorna il primo PCB dalla coda dei processi bloccati (s_ProcQ) associata al SEMD della ASHT con chiave key.
Se tale descrittore non esiste nella ASHT, restituisce NULL.
Altrimenti, restituisce l’elemento  rimosso.
Se la coda dei processi bloccati per il semaforo diventa vuota, rimuove il descrittore corrispondente dalla  ASHT e lo inserisce nella coda dei descrittori liberi (semdFree).*/
pcb_t* removeBlocked(int *key){
	if(semdTemp = matchKey(key)){ //trovo il semaforo con chiave 'key', se esiste, ritorno il primo pcb in coda, se il pcb ritornato è l'ultimo, libero il semaforo aggiungedolo a 'semdFree'.
		pcbTemp = semdTemp->s_procQ;
		semdTemp->s_procQ = pcbTemp->p_next;
		if(semdTemp->s_procQ == NULL){
			freeSemaphore(key);
		}
		return pcbTemp;
	}else{
		return NULL;
	}
}

// Richiama la funzione fun per ogni processo bloccato sul semaforo identificato da key.
void forallBlocked(int *key, void (*fun)(pcb_t *pcb, void *), void *arg){
	if(!flag){
		semdTemp = matchKey(key);
		if (semdTemp == NULL)
			return;
		pcbTemp = semdTemp->s_procQ;
		flag = TRUE;
	}
	fun(pcbTemp,arg);
	if(pcbTemp->p_next != NULL){
		pcbTemp = pcbTemp->p_next;
		forallBlocked(key,fun,arg);
	}
	flag = FALSE;
}

/* Rimuove il PCB puntato da p dalla coda del semaforo su cui è  bloccato.
(La hash table deve essere aggiornata in modo coerente).*/
void outChildBlocked(pcb_t *p);


void initASL(){
	if (i == 0){
		semdFree_h = &semd_table[i++];
		semdTemp = semdFree_h ;
	}
	if (i < MAXSEMD) {  
		semdTemp->s_next = &semd_table[i++];
		semdTemp = semdTemp->s_next;
		initASL();
	}
	if (i == MAXSEMD){
		semdTemp->s_next = NULL;
	}
}
