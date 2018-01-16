#include "Alloc.h"
#define DEBUG FALSE;
// variabili globali qui
int i = 0;
pcb_t *temp;
bool flag = FALSE; //condizione per capire se pcb è già presente nella lista dei pcbfree
bool start = TRUE; //condizione per vedere se pcb puntato da p è la testa

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
			temp->p_s = NULL;
			temp->priority = NULL;
			temp->p_semKey = NULL;
			return temp;
		}else{ // rimuovo la testa dalla lista dei pcbfree.
			temp = pcbfree_h;
			pcbfree_h = pcbfree_h->p_next;
			temp->p_next = NULL;
			temp->p_parent = NULL;
			temp->p_first_child = NULL;
			temp->p_sib = NULL;
			temp->p_s = NULL;
			temp->priority = NULL;
			temp->p_semKey = NULL;
			return temp;
		}
}
int main(void)
{
}