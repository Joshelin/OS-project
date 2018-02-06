#include "../header/Alloc.h"
#include "../header/uARMtypes.h"
// variabili globali qui
static int i = 0;
static pcb_t *temp;
static bool flag = FALSE; //condizione per capire se pcb è già presente nella lista dei pcbfree
static bool start = TRUE; //condizione per vedere se pcb puntato da p è la testa

void initPcbs() {
	// Solo per lista ancora vuota.
	if (i == 0){
		pcbfree_h = &pcbFree_table[i++];
		temp = pcbfree_h ;
	}
	if (i < MAXPROC) {  
		temp->p_next = &pcbFree_table[i++];
		temp = temp->p_next;
		initPcbs();
	}
	if (i == MAXPROC){
		temp->p_next = NULL;
	}
}

// Inserisce il PCB puntato da p nella lista dei PCB liberi (pcbFree)
void freePcb(pcb_t *p){
	if(start){
		if (pcbfree_h == NULL){ //controllo se lista è vuota, se lo è, p diventa la testa.
			flag = TRUE;
			pcbfree_h = p;
		}else if(pcbfree_h == p) { //controllo su primo elemento lista, se p è già tra i liberi.
			flag = TRUE;
		}else{
			temp = pcbfree_h;
			flag = FALSE; 
			start = FALSE;
		}
	}
	if(!flag){
		if (temp->p_next!=NULL){ // la lista contiene altri elementi.
			if (temp->p_next == p){ // controllo se p è già tra i liberi.
				// se si, non fare niente e resetta start per la prossima freePcb.
				start = TRUE;
			}else {
				temp = temp->p_next;
				freePcb(p);
			}
		}else{ // la lista non ha altri pcb e non ho trovato conflitti. Resetto start e flag per prossimi freePcb.
			temp->p_next = p;
			start = TRUE;
		}
	}
}


/* Restituisce NULL se la pcbFree è vuota.
 Altrimenti rimuove un elemento dalla pcbFree, inizializza tutti i campi (NULL/0) e restituisce l’elemento rimosso.*/
pcb_t *allocPcb(){
		if (pcbfree_h == NULL){ // Non ci sono pcb liberi, torna NULL.
			return NULL;
		}else if (pcbfree_h->p_next == NULL) { // Ultimo pcb libero, la lista pcbfree diventa vuota.
			temp = pcbfree_h;
			pcbfree_h = NULL;
			temp->p_parent = NULL;
			temp->p_first_child = NULL;
			temp->p_sib = NULL;
			temp->priority = 0;
			temp->p_s.a1=0;
			temp->p_s.a2=0;
			temp->p_s.a3=0;
			temp->p_s.a4=0;
			temp->p_s.v1=0;
			temp->p_s.v2=0;
			temp->p_s.v3=0;
			temp->p_s.v4=0;
			temp->p_s.v5=0;
			temp->p_s.v6=0;
			temp->p_s.sl=0;
			temp->p_s.fp=0;
			temp->p_s.ip=0;
			temp->p_s.sp=0;
			temp->p_s.lr=0;
			temp->p_s.pc=0;
			temp->p_s.cpsr=0;
			temp->p_s.CP15_Control=0;
			temp->p_s.CP15_EntryHi=0;
			temp->p_s.CP15_Cause=0;
			temp->p_s.TOD_Hi=0;
			temp->p_s.TOD_Low=0;
			temp->p_semKey = NULL;
			return temp;
		}else{ // rimuovo la testa dalla lista dei pcbfree.
			temp = pcbfree_h;
			pcbfree_h = pcbfree_h->p_next;
			temp->p_next = NULL;
			temp->p_parent = NULL;
			temp->p_first_child = NULL;
			temp->p_sib = NULL;
			temp->priority = 0;
			temp->p_s.a1=0;
			temp->p_s.a2=0;
			temp->p_s.a3=0;
			temp->p_s.a4=0;
			temp->p_s.v1=0;
			temp->p_s.v2=0;
			temp->p_s.v3=0;
			temp->p_s.v4=0;
			temp->p_s.v5=0;
			temp->p_s.v6=0;
			temp->p_s.sl=0;
			temp->p_s.fp=0;
			temp->p_s.ip=0;
			temp->p_s.sp=0;
			temp->p_s.lr=0;
			temp->p_s.pc=0;
			temp->p_s.cpsr=0;
			temp->p_s.CP15_Control=0;
			temp->p_s.CP15_EntryHi=0;
			temp->p_s.CP15_Cause=0;
			temp->p_s.TOD_Hi=0;
			temp->p_s.TOD_Low=0;
			temp->p_semKey = NULL;
			return temp;
		}
}
