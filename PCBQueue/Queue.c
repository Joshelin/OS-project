#include "Queue.h"

pcb_t *temp ;

void insertProcQ(pcb_t **head, pcb_t *p){ 
	if (*head == NULL) { //lista vuota
		*head = p;
	}
	else if ((*head)->p_next == NULL){ //lista con un solo elemento 
		if ((*head)->priority <= p->priority){ //-> ha precedenza su *, quindi uso parentesi 
			p->p_next = *head;
			*head = p;
		}
		else {
			(*head)->p_next = p ;			
		}
	}
	else {
		if ((*head)->priority <= p->priority){
			p->p_next = *head;
			*head = p;
		}
		else if ((*head)->p_next->priority <= p->priority){ //controllo nel caso in cui x e y siano due priorità consecutive, e z sia la priorità da inserire in mezzo. Perciò devo mantenere puntatore a x
			p->p_next = (*head)->p_next ;
			(*head)->p_next = p ;
		}
		else { 
			insertProcQ(&((*head)->p_next),p);
		}

	}
		
}

pcb_t* headProcQ(pcb_t *head){
	return head ;
}

pcb_t* removeProcQ(pcb_t **head){
	if (*head == NULL)
		return NULL ;
	else {
		temp = *head ;
		*head = (*head)->p_next ;
		temp->p_next = NULL ;
		temp->priority = 0 ;
	}
}

pcb_t* outProcQ(pcb_t **head, pcb_t *p){
	if (*head == NULL)
		return NULL ;
	if (*head == p){ //si dovrebbe usare removeProcQ, in ogni caso entra solo se p è la testa 
		*head = (*head)->p_next ;
		p->priority = 0 ;
		p->p_next = NULL ;
		return p ;
	}
	else {
		if ((*head)->p_next == p){
			(*head)->p_next = p->p_next ;
			p->priority = 0 ;
			p->p_next = NULL ;
			return p ;
		}
		else 
			return outProcQ(&((*head)->p_next),p) ;
	}
}