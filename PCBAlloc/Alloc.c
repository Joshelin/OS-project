#include "Alloc.h"
// variabili globali qui
int i = 0;
pcb_t *temp ; 
bool flag = false ; //condizione per capire se pcb è già presente nella lista dei pcbfree
bool start = true ; //condizione per vedere se pcb puntato da p è la testa

void initPcbs() {
	// Solo per lista ancora vuota.
	if (i == 0){
		pcbfree_h = &pcbFree_table[i++];
		temp = pcbfree_h ;
		temp->p_next = &pcbFree_table[i++];
		temp = temp->p_next;
	}
	if (i < MAXPROC) {  
		temp->p_next = &pcbFree_table[i++];
		temp = temp->p_next;
		initPcbs();
	}
}

// Inserisce il PCB puntato da p nella lista dei PCB liberi (pcbFree)
void freePcb(pcb_t *p){

	if (pcbfree_h == 0){ //controllo che lista sia vuota
		start = false ;
		flag = true ;
		pcbfree_h = p ;
	}
	if (start){ //controllo su primo elemento lista, se coincide con p
		if (p == pcbfree_h){
			flag = true ;
		}
		temp = pcbfree_h ;
		start = false ;
	}
	if((!flag) && (temp->p_next == 0)){
		if (temp->p_next == p)
			flag = true ; 
		temp = temp->p_next ;
		freePcb(p);
	}
	if((temp->p_next == 0) && (!flag)){
		temp->p_next = p ;

	}
	flag = false ;
	start = true ;

}

/* Restituisce NULL se la pcbFree è vuota.
 Altrimenti rimuove un elemento dalla pcbFree, inizializza tutti i campi (NULL/0) e restituisce l’elemento rimosso.*/
pcb_t *allocPcb(){
	if (pcbfree_h == 0){
		return 0 ;
	}
	else if (pcbfree_h->p_next == 0){
		temp = pcbfree_h ;
		pcbfree_h = 0 ;
		//inizializza campi a null
		return temp ;
	}
	else {
		temp = pcbfree_h ;
		pcbfree_h = pcbfree_h->p_next ;
		//inizializza campi a null
		return temp ;
	}

}
