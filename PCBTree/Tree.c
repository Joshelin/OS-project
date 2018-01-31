#include "Tree.h"

pcb_t *temp ;
bool init = FALSE ;

void insertChild(pcb_t *parent, pcb_t *p){

	if (parent->p_first_child == NULL){ //Parent non ha figli.
		parent->p_first_child = p ;
		p->p_parent = parent ;
		p->p_sib = NULL ;
	}
	else {
		
		if (!init){ // Inizializzazione temp
			temp = parent->p_first_child ;
			init = TRUE ;
		}
		if (temp->p_sib == NULL){ //"Iterazione ricorsiva" per i fratelli fino alla coda.
			temp->p_sib = p ;
			p->p_parent = parent ;
			p->p_sib = NULL ;
			init = FALSE;
		}
		else {
			temp = temp->p_sib ;
			insertChild(parent,p);
		}
	}
}

pcb_t* removeChild(pcb_t *p){
	if (p->p_first_child == NULL)
		return NULL ;
	else {
		pcb_t *tmp = p->p_first_child ;
		p->p_first_child = p->p_first_child->p_sib ;
		tmp->p_sib = NULL ;
		tmp->p_parent = NULL ;
		return tmp ;	
	}
}

pcb_t* outChild(pcb_t* p){
	
	if (p->p_parent == NULL){
		return NULL ;
	}
	if ((p->p_parent->p_first_child == p) && !init){ // p è il first_child && !init per eseguire solo 1 volta.
		p->p_parent->p_first_child = p->p_sib ;
		p->p_parent = NULL ;
		p->p_sib = NULL ;
		return p ;
	}
	if (!init){
		temp = p->p_parent->p_first_child ;
		init = TRUE ;
	}
	if (temp->p_sib == p){ // "Iterazione ricorsiva" fino al match.
		temp->p_sib = p->p_sib ;
		p->p_parent = NULL ;
		p->p_sib = NULL ;
		init = FALSE ;
		return p ;
	}
	else {
		temp = temp->p_sib ;
		return outChild(p);
	}
}