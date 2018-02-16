#include "../header/Semaphore.h"

static int i = 0;
static semd_t *semdTemp;
static pcb_t *pcbTemp;
static pcb_t *pcbSib;
static pcb_t *pcbParent ;
static bool init = FALSE;

// Calcolo a caso che ritorni un valore tra 0 e ASHDSIZE.
static int hash(int* key){ 
	const int a = 11;
	const int b = 1;
	long longKey = (long)key;
	longKey = ((a*longKey)+b)%MAXSEMD;
	return (int)longKey%ASHDSIZE;
}

// Alloca un semaforo dalla semdFree e lo mette nella ASHT
static semd_t* allocSemaphore(){
	if (semdFree_h == NULL)
		return NULL ;
	else if (semdFree_h->s_next == NULL) { // Ultimo semaforo libero, la lista semdFree diventa vuota.
		semdTemp = semdFree_h;
		semdFree_h = NULL;
		semdTemp->s_key = NULL;
		return semdTemp;
	}
	else{ // rimuovo la testa dalla lista dei semdFree.
		semdTemp = semdFree_h;
		semdFree_h = semdFree_h->s_next ;
		semdTemp->s_next = NULL;
		semdTemp->s_key = NULL;
		return semdTemp;
	}
}
/* Rimuovi il semaforo dalla ASHT, aggiornando la ASHT, e lo inserisce nella semdFree.
NB: freeSemaphore viene invocata SOLO quando è sicuro (Coda pcb vuota) che il semaforo può esser rimosso.*/
static void freeSemaphore(int *key){
	if(!init){
		if(semdhash[hash(key)]->s_key == key){ // se il semaforo da liberare è il primo della lista in 'semdhash[hash(key)]' lo aggiungo in testa a semdFree e lo rimuovo da ASHT
			semdTemp = semdhash[hash(key)];
		semdhash[hash(key)] = semdhash[hash(key)]->s_next;
		semdTemp->s_next = semdFree_h;
		semdFree_h = semdTemp;
		return;
	}
	semdTemp = semdhash[hash(key)];
	init = TRUE;
}
// temp è un altro puntatore (locale), necessario per inserire il semaforo in semdFree. semdTemp è usato per scorrere la lista in semdhash[hash(key)];
	if(semdTemp->s_next->s_key == key){ // se ho trovato il semaforo da liberare, lo aggiungo in testa a semdFree e lo rimuovo da ASHT.
		semd_t *temp = semdTemp->s_next; 
		semdTemp->s_next = semdTemp->s_next->s_next;
		temp->s_next = semdFree_h;
		semdFree_h = temp;
		init = FALSE;
	}
	else{
		semdTemp = semdTemp->s_next;
		freeSemaphore(key);
	}
}

// Ritorno puntatore al semaforo con s_key = key.
static semd_t* matchKey(int* key){

    if (!init) {  // inizializzazione.
		if (semdhash[hash(key)] == NULL){ // Se semdhash[hash(key)] non ha semafori, ovviamente non è già bloccato.
			return NULL;
		}
		semdTemp = semdhash[hash(key)];
		init = TRUE;
	}
	if (semdTemp->s_key == key){ // ho trovato il semaforo con chiave key, lo ritorno.
		init = FALSE;
		return semdTemp;
	}
	else if(semdTemp->s_next == NULL) { // sono arrivato alla coda senza trovare il semaforo con chiave key.
		init = FALSE;
		return NULL;
	}
	else{
		semdTemp = semdTemp->s_next;
		return matchKey(key);
	}
}

static void outPcbBlocked(pcb_t *p){
	if(p == NULL)
		return;
	semdTemp = matchKey(p->p_semKey);
	pcb_t* tmp = outProcQ(&(semdTemp->s_procQ),p) ;
	tmp->p_semKey = NULL;
	if(semdTemp->s_procQ == NULL) // se il p che ho tolto era l'ultimo del semaforo, libero il semaforo.
		freeSemaphore(semdTemp->s_key);
}

int insertBlocked(int *key, pcb_t *p){
	if((semdTemp = matchKey(key)) != NULL){ // se eiste già un semaforo con chiave 'key', aggiungo il pcb in coda a tale semaforo.
		p->p_semKey = key;
		insertProcQ(&(semdTemp->s_procQ),p);
	return 0;
}
else{
		semdTemp = allocSemaphore() ; // il semaforo con 'key' non esiste, procedo a crearlo e aggiungere il suo primo pcb.
		if (semdTemp == NULL)
			return -1 ;

		if (semdhash[hash(key)] == NULL){
			semdhash[hash(key)] = semdTemp ;
			semdhash[hash(key)]->s_next = NULL ;
			semdhash[hash(key)]->s_key = key ;
			p->p_semKey = key ;
			insertProcQ(&(semdTemp->s_procQ),p);
			return 0 ;
		}
		else {
			semdTemp->s_next = semdhash[hash(key)] ;
			semdhash[hash(key)] = semdTemp ;
			semdhash[hash(key)]->s_key = key ;
			p->p_semKey = key ;
			insertProcQ(&(semdhash[hash(key)]->s_procQ),p);
			return 0 ;
		}
	}
}

/* Restituisce il puntatore al pcb del primo processo bloccato sul semaforo, senza deaccordarlo.
Se il semaforo non esiste restituisce NULL.*/
pcb_t *headBlocked(int *key){
	if((semdTemp = matchKey(key)) != NULL){
		return headProcQ(semdTemp->s_procQ);
	}else{
		return NULL;
	}
}

/* Ritorna il primo PCB dalla coda dei processi bloccati (s_ProcQ) associata al SEMD della ASHT con chiave key.
Se tale descrittore non esiste nella ASHT, restituisce NULL.
Altrimenti, restituisce l’elemento  rimosso.
Se la coda dei processi bloccati per il semaforo diventa vuota, rimuove il descrittore corrispondente dalla  ASHT e lo inserisce nella coda dei descrittori liberi (semdFree).*/
pcb_t* removeBlocked(int *key){
	if((semdTemp = matchKey(key)) != NULL){ //trovo il semaforo con chiave 'key', se esiste, ritorno il primo pcb in coda, se il pcb ritornato è l'ultimo, libero il semaforo aggiungedolo a 'semdFree'.
		pcbTemp = removeProcQ(&(semdTemp->s_procQ));
		if(semdTemp->s_procQ == NULL){
			freeSemaphore(key);
		}
		pcbTemp->p_semKey = NULL;
		return pcbTemp;
	}
	else{
		return NULL;
	}
}

// Richiama la funzione fun per ogni processo bloccato sul semaforo identificato da key.
void forallBlocked(int *key, void (*fun)(pcb_t *pcb, void *), void *arg){
	semdTemp = matchKey(key);
	forallProcQ(semdTemp->s_procQ,fun,arg);
}

/* Rimuove il PCB puntato da p dalla coda del semaforo su cui è  bloccato.
(La hash table deve essere aggiornata in modo coerente).*/
void outChildBlocked(pcb_t *p){
	if(!init){
		pcbParent = p; // Salvo chi è il parent per poi poter fare outChild nel caso sia figlio di qualcuno.
		init = TRUE;
	}
	if(p->p_first_child != NULL){ // Scendo fino all'ultimo figlio, Es. Se parto dal nonno, comincio a togliere dai semafori e dall'albero i nipoti, poi i fratelli dei nipoti, poi il genitore e così via.
		outChildBlocked(p->p_first_child);
}
	if(p == pcbParent){ // Entro solo se ho già tolto tutti e rimane solo il parent. NB: Il parent originale potrebbe essere figlio di qualcuno o avere dei fratelli.
		outPcbBlocked(outChild(pcbParent));
		init = FALSE;
	}
	else{
		pcbSib = p->p_sib; // Salvo il fratello del p che sto togliendo perchè removeChild, giustamente, toglie i puntatori ai fratelli.
		outPcbBlocked(removeChild(p->p_parent)); 
		if(pcbSib!=NULL){
			outChildBlocked(pcbSib); // Ripeto sul fratello che a questo punto è il first_child del parent attuale.
		}
	}
}

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
		i = 0 ;
	}
}
